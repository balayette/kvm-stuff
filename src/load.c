#include <elf.h>
#include <string.h>
#include <link.h>
#include "assert.h"

typedef Elf32_Ehdr elf_header;
typedef Elf32_Phdr prog_header;

size_t load_elf(uint8_t *file, uint8_t *mem)
{
	elf_header *ehdr = (elf_header *)file;
	ASSERT(!memcmp(ehdr->e_ident, "\177ELF", 4), "Wrong ELF magic");

	prog_header *phdr = (prog_header *)(file + ehdr->e_phoff);

	for (size_t i = 0; i < ehdr->e_phnum; i++) {
		prog_header *p = phdr + i;
		if (p->p_type != PT_LOAD)
			continue;

		INFO("Segment %zu => [%p, %p]\n", i, mem + p->p_vaddr,
		     mem + p->p_vaddr + p->p_memsz);
		memcpy(mem + p->p_vaddr, file + p->p_offset, p->p_filesz);
		memset(mem + p->p_vaddr + p->p_filesz, 0,
		       p->p_memsz - p->p_filesz);
	}

	return ehdr->e_entry;
}
