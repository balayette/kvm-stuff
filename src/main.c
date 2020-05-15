#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "assert.h"
#include <stdio.h>
#include "gdt.h"
#include "utils.h"
#include "log.h"
#include "exits.h"
#include "kvmw.h"
#include "load.h"

#define X86_MEM_SIZE (4ULL * 1024 * 1024 * 1024)

void check_caps(struct kvmw *w)
{
	ASSERT(kvmw_check_cap(w, KVM_CAP_USER_MEMORY),
	       "KVM_CAP_USER_MEMORY missing.");
}

int usage(char *name)
{
	fprintf(stderr, "usage: %s binary\n", name);
	fprintf(stderr, "binary will be run in KVM.\n");
	return 1;
}

int main(int argc, char *argv[])
{
	if (argc != 2)
		return usage(argv[0]);

	struct kvmw wr = kvmw_open();
	check_caps(&wr);

	kvmw_create_vm(&wr);

	vm_create_cpu(&wr.vm);

	size_t code_sz;
	void *code = map_file(argv[1], &code_sz);

	vm_allocate_pmem(&wr.vm, X86_MEM_SIZE);
	size_t entry = load_elf(code, wr.vm.pmem);
	INFO("ELF entry point 0x%lx\n", entry);

	vm_set_pmem(&wr.vm, 0, X86_MEM_SIZE);

	vcpu_map_kvm_run(&wr.vm.vcpu, kvmw_get_kvm_run_sz(&wr));

	struct kvm_sregs sregs = vcpu_get_sregs(&wr.vm.vcpu);
	sregs.cs.base = 0;
	sregs.cs.selector = 0;
	sregs.cr0 |= 1; // Protected mode
	vcpu_set_sregs(&wr.vm.vcpu, sregs);

	struct kvm_regs init_regs = { 0 };
	init_regs.rip = entry;
	init_regs.rflags = 0x2;

	vcpu_set_regs(&wr.vm.vcpu, init_regs);

	gdt_boot_descriptors(&wr.vm);

	struct vcpu *v = &wr.vm.vcpu;

	for (;;) {
		ioctl(v->cpu_fd, KVM_RUN, NULL);
		if (handle_vm_exit(&wr.vm))
			break;
	}

	kvmw_close(&wr);
}
