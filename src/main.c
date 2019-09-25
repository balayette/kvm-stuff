#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "assert.h"
#include <stdio.h>
#include "log.h"
#include "kvmw.h"

#define PAGE_ALIGN(x) ((x + 4096) & ~(4096 - 1))

char *kvm_exit_codes[] = {
	"KVM_EXIT_UNKNOWN0",
	"KVM_EXIT_EXCEPTION1",
	"KVM_EXIT_IO2",
	"KVM_EXIT_HYPERCALL3",
	"KVM_EXIT_DEBUG4",
	"KVM_EXIT_HLT5",
	"KVM_EXIT_MMIO6",
	"KVM_EXIT_IRQ_WINDOW_OPEN7",
	"KVM_EXIT_SHUTDOWN8",
	"KVM_EXIT_FAIL_ENTRY9",
	"KVM_EXIT_INTR10",
	"KVM_EXIT_SET_TPR11",
	"KVM_EXIT_TPR_ACCESS12",
	"KVM_EXIT_S390_SIEIC13",
	"KVM_EXIT_S390_RESET14",
	"KVM_EXIT_DCR15",
	"KVM_EXIT_NMI16",
	"KVM_EXIT_INTERNAL_ERROR17",
	"KVM_EXIT_OSI18",
	"KVM_EXIT_PAPR_HCALL",
	"KVM_EXIT_S390_UCONTROL",
	"KVM_EXIT_WATCHDOG21",
	"KVM_EXIT_S390_TSCH22",
	"KVM_EXIT_EPR23",
	"KVM_EXIT_SYSTEM_EVENT24",
	"KVM_EXIT_S390_STSI25",
	"KVM_EXIT_IOAPIC_EOI26",
	"KVM_EXIT_HYPERV27",
};

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

void *map_file(char *filename, size_t *size)
{
	int fd = open(filename, O_RDONLY);
	ASSERT(fd != -1, "Couldn't open the binary.");

	struct stat buf;
	ASSERT(fstat(fd, &buf) != -1, "Couldn't stat the binary.");

	*size = PAGE_ALIGN(buf.st_size);

	void *base =
		mmap(NULL, *size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	ASSERT(base != MAP_FAILED, "Couldn't map the binary.");

	close(fd);

	return base;
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

	vm_allocate_pmem(&wr.vm, code_sz);
	vm_write_to_pmem(&wr.vm, code, code_sz, 0);
	vm_set_pmem(&wr.vm, 0, code_sz);

	vcpu_map_kvm_run(&wr.vm.vcpu, kvmw_get_kvm_run_sz(&wr));

	struct kvm_sregs sregs = vcpu_get_sregs(&wr.vm.vcpu);
	sregs.cs.base = 0;
	sregs.cs.selector = 0;
	vcpu_set_sregs(&wr.vm.vcpu, sregs);

	vcpu_set_regs(&wr.vm.vcpu, (struct kvm_regs){ .rip = 0x0,
						      .rax = 0x2,
						      .rbx = 0x2,
						      .rflags = 0x2 });

	struct vcpu *v = &wr.vm.vcpu;

	for (;;) {
		ioctl(v->cpu_fd, KVM_RUN, NULL);
		int reason = v->run->exit_reason;
		INFO("VM_EXIT: %s (%d)\n", kvm_exit_codes[reason], reason);
		if (reason == KVM_EXIT_HLT) {
			INFO("Reached hlt, goodbye!\n");
			break;
		}
	}

	kvmw_close(&wr);
}
