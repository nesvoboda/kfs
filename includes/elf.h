#ifndef ELF_H
#define ELF_H

#include "types.h"

/* The symbol table for a.out. */
struct multiboot_aout_symbol_table {
	u32int tabsize;
	u32int strsize;
	u32int addr;
	u32int reserved;
};
typedef struct multiboot_aout_symbol_table multiboot_aout_symbol_table_t;

/* The section header table for ELF. */
struct multiboot_elf_section_header_table {
	u32int num;
	u32int size;
	u32int addr;
	u32int shndx;
};
typedef struct multiboot_elf_section_header_table multiboot_elf_section_header_table_t;

typedef struct mb_info {
	/* Multiboot info version number */
	u32int flags;

	/* Available memory from BIOS */
	u32int mem_lower;
	u32int mem_upper;

	/* "root" partition */
	u32int boot_device;

	/* Kernel command line */
	u32int cmdline;

	/* Boot-Module list */
	u32int mods_count;
	u32int mods_addr;

	union {
		multiboot_aout_symbol_table_t aout_sym;
		multiboot_elf_section_header_table_t elf_sec;
	} u;

	/* Memory Mapping buffer */
	u32int mmap_length;
	u32int mmap_addr;

	/* Drive Info buffer */
	u32int drives_length;
	u32int drives_addr;

	/* ROM configuration table */
	u32int config_table;

	/* Boot Loader Name */
	u32int boot_loader_name;

	/* APM table */
	u32int apm_table;

	/* Video */
	u32int vbe_control_info;
	u32int vbe_mode_info;
	u16int vbe_mode;
	u16int vbe_interface_seg;
	u16int vbe_interface_off;
	u16int vbe_interface_len;

	u64int framebuffer_addr;
	u32int framebuffer_pitch;
	u32int framebuffer_width;
	u32int framebuffer_height;
	u8int framebuffer_bpp;
#define MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED 0
#define MULTIBOOT_FRAMEBUFFER_TYPE_RGB 1
#define MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT 2
	u8int framebuffer_type;
	union {
		struct
		{
			u32int framebuffer_palette_addr;
			u16int framebuffer_palette_num_colors;
		};
		struct
		{
			u8int framebuffer_red_field_position;
			u8int framebuffer_red_mask_size;
			u8int framebuffer_green_field_position;
			u8int framebuffer_green_mask_size;
			u8int framebuffer_blue_field_position;
			u8int framebuffer_blue_mask_size;
		};
	};
} multiboot_info_t;

typedef u32int Elf32_Off;
typedef u32int Elf32_Addr;

typedef struct {
	u32int sh_name;
	u32int sh_type;
	u32int sh_flags;
	Elf32_Addr sh_addr;
	Elf32_Off sh_offset;
	u32int sh_size;
	u32int sh_link;
	u32int sh_info;
	u32int sh_addralign;
	u32int sh_entsize;
} Elf32_Shdr;

extern u32int multiboot_magic;
extern u32int multiboot_info;

#define SHT_NULL 0
#define SHT_PROGBITS 1
#define SHT_SYMTAB 2
#define SHT_STRTAB 3
#define SHT_RELA 4
#define SHT_HASH 5
#define SHT_DYNAMIC 6
#define SHT_NOTE 7
#define SHT_NOBITS 8
#define SHT_REL 9
#define SHT_SHLIB 10
#define SHT_DYNSYM 11
#define SHT_NUM 12
#define SHT_LOPROC 0x70000000
#define SHT_HIPROC 0x7fffffff
#define SHT_LOUSER 0x80000000
#define SHT_HIUSER 0xffffffff

char *addr2name(u32int *addr);

typedef struct {
	u32int st_name;
	Elf32_Addr st_value;
	u32int st_size;
	unsigned char st_info;
	unsigned char st_other;
	u16int st_shndx;
} Elf32_Sym;

#endif