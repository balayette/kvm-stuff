#ifndef VM_H
#define VM_H

#include "vcpu.h"

struct vm {
	int vm_fd;
	void *pmem;
	size_t pmem_sz;
	struct vcpu vcpu;
};

void vm_create_cpu(struct vm *vm);

void vm_allocate_pmem(struct vm *vm, size_t size);

void vm_write_to_pmem(struct vm *vm, void *s, size_t n, size_t offt);

void vm_set_pmem(struct vm *vm, size_t guest_phys_addr, size_t size);

#endif /* !VM_H */
