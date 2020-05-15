#include "gdt.h"
#include <string.h>

struct gdtr {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));

#define NULL_SEG (0)
#define KERN_CODE_SEG (1)
#define KERN_DATA_SEG (2)

#define SET_BASE(entry, x)                                                     \
	do {                                                                   \
		entry->base_low = (x)&0xffffff;                                \
		entry->base_high = ((x) >> 24) & 0xff;                         \
	} while (0)

#define SET_LIMIT(entry, x)                                                    \
	do {                                                                   \
		entry->seg_limit_low = (x)&0xffff;                             \
		entry->seg_limit_high = ((x) >> 16) & 0xf;                     \
	} while (0)

#define PRIVILEGED (0)
#define UNPRIVILEGED (3)

#define MAKE_GDT_SELECTOR(idx, rpl) ((idx) << 3 | ((rpl)&0x3))

#define GRANULARITY_1B (0)
#define GRANULARITY_4KB (1)

#define DESC_SYSTEM_SEGMENT (0)
#define DESC_CODE_DATA_SEGMENT (1)

#define SEG_RO (0)
#define SEG_RWA (3)
#define SEG_RXA (11)
#define SEG_RX (10)

#define SEGMENT_PRESENT (1)
#define SEGMENT_NOT_PRESENT (0)

static void setup_deg_desc(struct seg_desc *desc, uint32_t base, uint32_t limit,
			   uint8_t granularity, uint8_t privilege,
			   uint8_t seg_present, uint8_t desc_type,
			   uint8_t seg_type)
{
	desc->l = 0;
	desc->avlbl = 0;
	SET_BASE(desc, base);
	desc->def_op_size = 1;
	desc->dpl = privilege;
	desc->granularity = granularity;
	SET_LIMIT(desc, limit);
	desc->seg_present = seg_present;
	desc->desc_type = desc_type;
	desc->seg_type = seg_type;
}

void gdt_boot_descriptors(struct vm *vm)
{
	struct seg_desc *gdt = vm->pmem;
	struct seg_desc *kern_code = gdt + KERN_CODE_SEG;
	struct seg_desc *kern_data = gdt + KERN_DATA_SEG;

	memset(gdt + NULL_SEG, 0, sizeof(struct seg_desc));

	setup_deg_desc(kern_code, 0, -1, GRANULARITY_4KB, PRIVILEGED,
		       SEGMENT_PRESENT, DESC_CODE_DATA_SEGMENT, SEG_RXA);

	setup_deg_desc(kern_data, 0, -1, GRANULARITY_4KB, PRIVILEGED,
		       SEGMENT_PRESENT, DESC_CODE_DATA_SEGMENT, SEG_RWA);

	struct kvm_dtable gdtr = { .base = 0,
				   .limit = sizeof(struct seg_desc) * 3 - 1 };

	// KVM basically needs segment descriptors instead of segment selectors
	struct kvm_segment cs = {
		.base = 0,
		.limit = -1,
		.selector = MAKE_GDT_SELECTOR(KERN_CODE_SEG, PRIVILEGED),
		.type = SEG_RXA,
		.present = 1,
		.dpl = PRIVILEGED,
		.db = 1,
		.s = DESC_CODE_DATA_SEGMENT,
		.l = 0,
		.g = GRANULARITY_4KB,
		.avl = 0,
		.unusable = 0,
	};

	struct kvm_segment ds = {
		.base = 0,
		.limit = -1,
		.selector = MAKE_GDT_SELECTOR(KERN_DATA_SEG, PRIVILEGED),
		.type = SEG_RWA,
		.present = 1,
		.dpl = PRIVILEGED,
		.db = 1,
		.s = DESC_CODE_DATA_SEGMENT,
		.l = 0,
		.g = GRANULARITY_4KB,
		.avl = 0,
		.unusable = 0,
	};

	struct kvm_sregs sregs = vcpu_get_sregs(&vm->vcpu);
	sregs.gdt = gdtr;
	sregs.cs = cs;
	sregs.ds = ds;
	sregs.fs = ds;
	sregs.gs = ds;
	sregs.es = ds;
	sregs.ss = ds;

	vcpu_set_sregs(&vm->vcpu, sregs);
}
