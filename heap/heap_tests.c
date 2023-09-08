#include "heap.h"
#include "test.h"

#define HEAP_INDEX_SIZE   0x20000
#define true_size(x) (x + sizeof(header_t) + sizeof(footer_t))

int header_predicate(void *a, void *b) {
    header_t *header_a = (header_t *) a;
    header_t *header_b = (header_t *)b;

    return header_a->size < header_b->size;
}

_heap_t *heap_create(u32int size, int is_kernel, int is_readonly) {
    _heap_t *h = KALLOCATE(sizeof(_heap_t));

    // There should be place too
    h->data = KALLOCATE(size);
    h->size = size;
    h->is_kernel = is_kernel;
    h->is_readonly = is_readonly;

    oarray_create(&h->holes, HEAP_INDEX_SIZE, &header_predicate);

    // Add one giant hole

    header_t *hole = (header_t *)h->data;
    hole->is_hole = 1;
    hole->size = true_size(size);

    // Footer?
    footer_t *footer = (footer_t *)(((void *)hole)+sizeof(header_t)+size);
    footer->to_header = hole;

    oarray_insert(&h->holes, hole);
    
    return h;
}

header_t *find_smallest_hole(_heap_t *h, u32int size) {
    u32int true_requested_size = size + sizeof(header_t) + sizeof(footer_t);
    
    for (int i = 0; i < h->holes.len; i++) {
        header_t *hole = (header_t *)oarray_retrieve(&h->holes, i);
        if (hole->size >= true_requested_size) {
            return hole;
        }
    }
    return NULL;
}

void remove_hole(_heap_t *h, header_t *hole) {
    for (int i = 0; i < h->holes.len; i++) {
        header_t *cur = (header_t *)oarray_retrieve(&h->holes, i);
        if (cur == hole) {
            oarray_delete(&h->holes, i);
            return;
        }
    }
}

void deallocate(_heap_t *h, void *addr)
{
    header_t *head = addr - sizeof(header_t);
    header_t *next_head = (void *)head + true_size(head->size);
    footer_t *prev_footer = ((footer_t *)((void *)head - sizeof(footer_t)));
    head->is_hole = 1;
    int next = 0;
    int prev = 0;
    if (next_head && next_head->is_hole)
    {
        head->size += next_head->size;
        footer_t *next_footer = (footer_t *)((void *)next_head + next_head->size + sizeof(header_t));
        next_footer->to_header = head;
        // [header][block][footer][header][block][footer]
        // [header] 
        remove_hole(h, next_head);
        next = 1;
    }
    if (prev_footer > h->data)
    {
        header_t *prev_head = prev_footer->to_header;
        if (prev_head && prev_head->is_hole)
        {
            prev_head->size += head->size;
            footer_t *current_footer = (footer_t *)(head + head->size + sizeof(header_t));
            current_footer->to_header = prev_head;
            remove_hole(h, head);
            prev = 1;
        }
    }
    if (!prev)
    {
        oarray_insert(&h->holes, head);
    }
    // oarray_insert(&h->holes, new_hole);

    // find header
    // assign as a hole
    // remove footer
    // add to array of holes
    // 
}


void *allocate(_heap_t *h, u32int size) {
    header_t *hole = find_smallest_hole(h, size);

    if (hole == NULL) {
        // we don't have any more space
        PANIC()
    }

    hole->is_hole = 0;

    // Add footer 
    footer_t *f = (footer_t *)((void *)hole + size);
    f->to_header = hole;

    // The hole should move to the end
    header_t *new_hole = (void *) hole + true_size(size);
    
    // [h          one block          f] size
    // [h one block f][h second block f]
    // new hole size = old_size - header - footer - requested_size
    // It has a smaller size
    new_hole->size = hole->size - true_size(size);
    hole->size = size;
    new_hole->is_hole = 1;
    footer_t *new_f = (footer_t *)((void *)new_hole + new_hole->size + sizeof(header_t));
    new_f->size =new_hole->size;
    new_f->to_header = new_hole;
    remove_hole(h, hole);
    oarray_insert(&h->holes, new_hole);

    return (void*)(hole) + sizeof(header_t);
}

void test_heap_simple() {
    _heap_t *h = heap_create(500, 0, 0);

    header_t *res = allocate(h, 10);

    // [heap_start][first block header][first block][first block footer]
    ASSERT_EQ((void*)h->data + sizeof(header_t), res);

    // holes state
    // there should be one hole for the rest of the space
    ASSERT_EQ(1, h->holes.len);

    header_t *hole = oarray_retrieve(&h->holes, 0);
    ASSERT_EQ(1, hole->is_hole);
    ASSERT_EQ(true_size(500) - sizeof(header_t) - sizeof(footer_t) - 10, hole->size);

    void *res2 = allocate(h, 10);

    ASSERT_EQ(h->data + sizeof(header_t) + 
        10 + sizeof(footer_t) + sizeof(header_t), res2);
    
}

void test_heap_free() {
    _heap_t *h = heap_create(500, 0, 0);

    header_t *res = allocate(h, 10);

    void *tmp = res;

    deallocate(h, res);

    void *res2 = allocate(h, 10);

    // Memory is reused
    ASSERT_EQ(tmp, res2);  
}

void heap_tests() {
    test_heap_simple();
    test_heap_free();
}
