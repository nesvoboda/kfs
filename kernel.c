#include "screen.h"
#include "gdt.h"
#include "idt.h"
#include "cursor.h"
#include "screen_handler.h"
#include "ft_printf.h"
#include "stack.h"
#include "shell.h"
                                        
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) 
{
	return fg | bg << 4;
}

const char * intro_text[] = {
"                                                                  ",
"    ____  __    ___   _____ __  __   ____  _____    __ __ ___     ",
"   / __ )/ /   /   | / ___// / / /  / __ \\/ ___/   / // /|__ \\  ",
"  / __  / /   / /| | \\__ \\/ /_/ /  / / / /\\__ \\   / // /___/ /",
" / /_/ / /___/ ___ |___/ / __  /  / /_/ /___/ /  /__  __/ __/     ",
"/_____/_____/_/  |_/____/_/ /_/   \\____//____/     /_/ /____/    ",
"                                                                  ",
"                                                                  ",
};

/* The symbol table for a.out. */
struct multiboot_aout_symbol_table
{
  uint32_t tabsize;
  uint32_t strsize;
  uint32_t addr;
  uint32_t reserved;
};
typedef struct multiboot_aout_symbol_table multiboot_aout_symbol_table_t;

/* The section header table for ELF. */
struct multiboot_elf_section_header_table
{
  uint32_t num;
  uint32_t size;
  uint32_t addr;
  uint32_t shndx;
};
typedef struct multiboot_elf_section_header_table multiboot_elf_section_header_table_t;

struct multiboot_info
{
  /* Multiboot info version number */
  uint32_t flags;

  /* Available memory from BIOS */
  uint32_t mem_lower;
  uint32_t mem_upper;

  /* "root" partition */
  uint32_t boot_device;

  /* Kernel command line */
  uint32_t cmdline;

  /* Boot-Module list */
  uint32_t mods_count;
  uint32_t mods_addr;

  union
  {
    multiboot_aout_symbol_table_t aout_sym;
    multiboot_elf_section_header_table_t elf_sec;
  } u;

  /* Memory Mapping buffer */
  uint32_t mmap_length;
  uint32_t mmap_addr;

  /* Drive Info buffer */
  uint32_t drives_length;
  uint32_t drives_addr;

  /* ROM configuration table */
  uint32_t config_table;

  /* Boot Loader Name */
  uint32_t boot_loader_name;

  /* APM table */
  uint32_t apm_table;

  /* Video */
  uint32_t vbe_control_info;
  uint32_t vbe_mode_info;
  uint16_t vbe_mode;
  uint16_t vbe_interface_seg;
  uint16_t vbe_interface_off;
  uint16_t vbe_interface_len;

  uint64_t framebuffer_addr;
  uint32_t framebuffer_pitch;
  uint32_t framebuffer_width;
  uint32_t framebuffer_height;
  uint8_t framebuffer_bpp;
#define MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED 0
#define MULTIBOOT_FRAMEBUFFER_TYPE_RGB     1
#define MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT     2
  uint8_t framebuffer_type;
  union
  {
    struct
    {
      uint32_t framebuffer_palette_addr;
      uint16_t framebuffer_palette_num_colors;
    };
    struct
    {
      uint8_t framebuffer_red_field_position;
      uint8_t framebuffer_red_mask_size;
      uint8_t framebuffer_green_field_position;
      uint8_t framebuffer_green_mask_size;
      uint8_t framebuffer_blue_field_position;
      uint8_t framebuffer_blue_mask_size;
    };
  };
};

typedef uint32_t Elf32_Off;
typedef uint32_t Elf32_Addr;

typedef struct {
               uint32_t   sh_name;
               uint32_t   sh_type;
               uint32_t   sh_flags;
               Elf32_Addr sh_addr;
               Elf32_Off  sh_offset;
               uint32_t   sh_size;
               uint32_t   sh_link;
               uint32_t   sh_info;
               uint32_t   sh_addralign;
               uint32_t   sh_entsize;
           } Elf32_Shdr;

extern uint32_t multiboot_magic;
extern uint32_t multiboot_info;

#define SHT_NULL	0
#define SHT_PROGBITS	1
#define SHT_SYMTAB	2
#define SHT_STRTAB	3
#define SHT_RELA	4
#define SHT_HASH	5
#define SHT_DYNAMIC	6
#define SHT_NOTE	7
#define SHT_NOBITS	8
#define SHT_REL		9
#define SHT_SHLIB	10
#define SHT_DYNSYM	11
#define SHT_NUM		12
#define SHT_LOPROC	0x70000000
#define SHT_HIPROC	0x7fffffff
#define SHT_LOUSER	0x80000000
#define SHT_HIUSER	0xffffffff

 typedef struct {
               uint32_t      st_name;
               Elf32_Addr    st_value;
               uint32_t      st_size;
               unsigned char st_info;
               unsigned char st_other;
               uint16_t      st_shndx;
           } Elf32_Sym;

void get_multiboot_info() {
	// int multiboot_magic;
	// int multiboot_info;
	// 1050c8
	// 1176e0
	// 104a24

	if (multiboot_magic == 0x2BADB002) {
		// printk(INFO, "Eax has the correct value");
		uint32_t *flags = multiboot_info;
		uint32_t elf_enabled = 1 << 5;
		// printk(INFO, "Flags: 0x%x", *flags);
		// printk(INFO, "Elf is enabled: %d", (*flags) & elf_enabled);

		struct multiboot_info *decoded_info = (struct multiboot_info *)multiboot_info;
		// printk(INFO, "Number of ELF sections: %d", decoded_info->u.elf_sec.num);

		Elf32_Shdr *elf_shdrs = (Elf32_Shdr *)decoded_info->u.elf_sec.addr;
		u32int string_we_are_looking_for = 42;
		for (int i =0; i < decoded_info->u.elf_sec.num; i++)
		{
			if (elf_shdrs[i].sh_type == SHT_SYMTAB || elf_shdrs[i].sh_type == SHT_STRTAB)
				// printk(INFO, "Header type: %d and index: %d", elf_shdrs[i].sh_type, i);


			if (elf_shdrs[i].sh_type == SHT_SYMTAB) {
				Elf32_Sym *symtab = elf_shdrs[i].sh_addr;

				for (int k =0; k < elf_shdrs[i].sh_size; k++)
				{
					if (symtab[k].st_value == &get_multiboot_info) {
						printk(INFO, "st_shndx: %d", symtab[k].st_shndx);
						string_we_are_looking_for = symtab[k].st_name;
					}
				}
			}
		}
		printk(INFO, "String we're looking for is index %d", string_we_are_looking_for);

		for (int i = 0; i < decoded_info->u.elf_sec.num; i++) {
			if (elf_shdrs[i].sh_type == SHT_STRTAB && elf_shdrs[i].sh_addr == 0x1176e0) {
				u32int str_addr = elf_shdrs[i].sh_addr + string_we_are_looking_for;
				printk(INFO, "String is %s", (char *) str_addr);



			}
		}

	} else {
		printk(ERROR, "EAX instead has %x", multiboot_magic);
	}
	
}


void kernel_main(void) 
{	
	
	gdt_install();
	init_idt();

	/* Initialize terminal interface */
	terminal_initialize();
	init_timer(50);
	disable_cursor();

	//ASCII ART

	for (int y = 0; y < VGA_HEIGHT; y++) {
		for (int x = 0; x < VGA_WIDTH; x++) {
			terminal_putentryat(' ', vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLUE), x, y);
		}
	}

	terminal_setcolor(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLUE));

    int i;
	int n_array = 8;
    for (i = 0; i < n_array; i++) {
		terminal_writestring_pos(intro_text[i],7, i+6);
    }
	
	sleep(2*50);
	init_logs();
	// Needed for screen handler
	init_screen();
	refresh_screen(0);
	enable_cursor(0, 15);
	printk(INFO, "System initialized");
	get_multiboot_info();
	sleep(2*50);
	// printk(ERROR, "Test error");
	// printk(ERROR, "Test %xerror", 15);

	// TraceStackTrace(16);

	shell();
}


