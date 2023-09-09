#include "oarray.h"

void oarray_create(oarray_t* array, u32int max_size, predicate* pr)
{
	array->data = KALLOCATE(max_size * sizeof(type_t));
	array->size = max_size;
	array->p = pr;
	for (int i = 0; i < max_size; i++) {
		array->data[i] = NULL;
	}
	array->len = 0;
}

int find_position(oarray_t* array, type_t item)
{
	int i = 0;
	for (i = 0; i < array->len; i++) {
		int less = array->p(item, array->data[i]);
		if (less != 0) {
			return i;
		}
	}
	return i;
}

void oarray_insert(oarray_t* array, type_t item)
{
	int position = find_position(array, item);
	if (position > array->size) {
		PANIC();
	}

	if (position >= array->len) {
		array->data[array->len] = item;
		array->len += 1;
		return;
	}

	type_t tmp = array->data[position];
	for (int i = position + 1; i < array->len; i++) {
		tmp = array->data[i];
		array->data[i] = array->data[i - 1];
	}
	array->data[array->len] = tmp;
	array->data[position] = item;
	array->len += 1;
}

void oarray_delete(oarray_t* array, size_t index)
{
	for (int i = index; i < array->len - 1; i++) {
		array->data[i] = array->data[i + 1];
	}
	array->len -= 1;
	return;
}

type_t oarray_retrieve(oarray_t* array, size_t index)
{
	return array->data[index];
}

int lessthan(type_t a, type_t b)
{
	return a < b ? 1 : 0;
}
