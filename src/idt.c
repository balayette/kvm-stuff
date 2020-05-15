#include "idt.h"

void idt_boot_descriptors(struct vm *vm)
{
	struct kvm_dtable idtr = { .limit = 0, .base = 0 };

	struct kvm_sregs sregs = vcpu_get_sregs(&vm->vcpu);
	sregs.idt = idtr;

	vcpu_set_sregs(&vm->vcpu, sregs);
}
