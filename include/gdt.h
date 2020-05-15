#ifndef GDT_H
#define GDT_H

#include <stdint.h>
#include "vm.h"

struct seg_desc {
	uint16_t seg_limit_low;
	uint32_t base_low : 24;

	uint8_t seg_type : 4;
	uint8_t desc_type : 1;
	uint8_t dpl : 2;
	uint8_t seg_present : 1;
	uint8_t seg_limit_high : 4;
	uint8_t avlbl : 1;
	uint8_t l : 1; // useless?
	uint8_t def_op_size : 1;
	uint8_t granularity : 1;
	uint8_t base_high : 8;
} __attribute__((packed));

void gdt_boot_descriptors(struct vm *vm);

#endif /* !GDT_H */
