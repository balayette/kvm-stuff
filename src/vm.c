#include <sys/ioctl.h>
#include <sys/mman.h>
#include <string.h>
#include <stdio.h>
#include "assert.h"
#include "vm.h"
#include "log.h"

void vm_create_cpu(struct vm *vm)
{
	vm->vcpu.cpu_fd = ioctl(vm->vm_fd, KVM_CREATE_VCPU, 0);
	ASSERT(vm->vcpu.cpu_fd != -1, "ioctl(KVM_CREATE_VCPU) failed");

	INFO("cpu_fd: %d\n", vm->vcpu.cpu_fd);
}

void vm_allocate_pmem(struct vm *vm, size_t size)
{
	vm->pmem = mmap(NULL, size, PROT_READ | PROT_WRITE,
			MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	ASSERT(vm->pmem != MAP_FAILED, "mmap failed");

	vm->pmem_sz = size;

	INFO("pmem: %p\n", vm->pmem);
}

void vm_write_to_pmem(struct vm *vm, void *s, size_t n, size_t offt)
{
	memcpy((char *)vm->pmem + offt, s, n);
}

void vm_set_pmem(struct vm *vm, size_t guest_phys_addr, size_t size)
{
	int ret = ioctl(vm->vm_fd, KVM_SET_USER_MEMORY_REGION,
			&(struct kvm_userspace_memory_region){
				.slot = 0, /* XXX */
				.guest_phys_addr = guest_phys_addr,
				.memory_size = size,
				.userspace_addr = (size_t)vm->pmem });
	ASSERT(ret != -1, "ioctl(KVM_SET_USER_MEMORY_REGION) failed");
}
