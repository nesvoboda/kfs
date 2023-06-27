#include "elf.h"

#define ELF_ENABLED 1 << 5

int is_elf_available()
{
	// If multiboot magic is incorrect, we don't have reliable multiboot info
	if (multiboot_magic != 0x2BADB002) {
		return 0;
	}
	multiboot_info_t* mb_info = (multiboot_info_t*)multiboot_info;
	// If ELF is not provided to us by Multiboot, we can't resolve addresses to
	// names
	if (mb_info->flags & ELF_ENABLED == 0) {
		return 0;
	}
	return 1;
}

Elf32_Shdr* find_symtab(Elf32_Shdr* shdrs)
{
	multiboot_info_t* mb_info = (multiboot_info_t*)multiboot_info;

	for (int i = 0; i < mb_info->u.elf_sec.size; i++) {
		if (shdrs[i].sh_type == SHT_SYMTAB) {
			return &(shdrs[i]);
		}
	}

	// Symtab not found
	return NULL;
}

u32int find_name_index(Elf32_Shdr* symtab, u32int addr)
{
	Elf32_Sym* symbols = symtab->sh_addr;

	for (int i = 0; i < symtab->sh_size; i++) {
		if (symbols[i].st_value == addr) {
			return symbols[i].st_name;
		}
	}
	return 0;
}

char* find_name(Elf32_Shdr* shdrs, u32int name_index)
{
	multiboot_info_t* mb_info = (multiboot_info_t*)multiboot_info;

	for (int i = 0; i < mb_info->u.elf_sec.size; i++) {
		if (shdrs[i].sh_type == SHT_STRTAB) {
			// string table too small to possibly contain the name in question
			if (shdrs[i].sh_size < name_index) {
				continue;
			}
			return (char*)shdrs[i].sh_addr + name_index;
		}
	}
	// name not found in string tables
	return NULL;
}

char* addr2name(u32int* addr)
{
	if (!is_elf_available()) {
		return NULL;
	}

	multiboot_info_t* mb_info = (multiboot_info_t*)multiboot_info;

	Elf32_Shdr* symtab = find_symtab(mb_info->u.elf_sec.addr);

	// No symtab header loaded
	if (symtab == NULL) {
		return NULL;
	}

	u32int name_index_in_strtab = find_name_index(symtab, addr);

	// All string tables have NULL as string index 0, so if we return 0,
	// that means that we didn't have a symbol entry for this function
	if (name_index_in_strtab == 0) {
		return NULL;
	}

	char* name = find_name(mb_info->u.elf_sec.addr, name_index_in_strtab);

	return name;
}
