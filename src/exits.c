#include "exits.h"
#include "log.h"
#include <linux/kvm.h>

static char *kvm_exit_codes[] = {
	"KVM_EXIT_UNKNOWN",
	"KVM_EXIT_EXCEPTION",
	"KVM_EXIT_IO",
	"KVM_EXIT_HYPERCALL",
	"KVM_EXIT_DEBUG",
	"KVM_EXIT_HLT",
	"KVM_EXIT_MMIO",
	"KVM_EXIT_IRQ_WINDOW_OPEN",
	"KVM_EXIT_SHUTDOWN",
	"KVM_EXIT_FAIL_ENTRY",
	"KVM_EXIT_INTR",
	"KVM_EXIT_SET_TPR",
	"KVM_EXIT_TPR_ACCESS",
	"KVM_EXIT_S390_SIEIC",
	"KVM_EXIT_S390_RESET",
	"KVM_EXIT_DCR",
	"KVM_EXIT_NMI",
	"KVM_EXIT_INTERNAL_ERROR",
	"KVM_EXIT_OSI",
	"KVM_EXIT_PAPR_HCALL",
	"KVM_EXIT_S390_UCONTROL",
	"KVM_EXIT_WATCHDOG",
	"KVM_EXIT_S390_TSCH",
	"KVM_EXIT_EPR",
	"KVM_EXIT_SYSTEM_EVENT",
	"KVM_EXIT_S390_STSI",
	"KVM_EXIT_IOAPIC_EOI",
	"KVM_EXIT_HYPERV",
};

static int unhandled_vm_exit(struct vm *vm)
{
	(void)vm;

	FAIL("Unhandled VM exit.\n");

	return 1;
}

static int hlt_exit()
{
	INFO("Reached hlt, exiting.\n");

	return 1;
}

static int io_exit(struct vm *vm)
{
	return vm_portio_handler(vm);
}

static int (*vm_exit_handlers[])(struct vm *vm) = {
	[KVM_EXIT_UNKNOWN] = unhandled_vm_exit,
	[KVM_EXIT_EXCEPTION] = unhandled_vm_exit,
	[KVM_EXIT_IO] = io_exit,
	[KVM_EXIT_HYPERCALL] = unhandled_vm_exit,
	[KVM_EXIT_DEBUG] = unhandled_vm_exit,
	[KVM_EXIT_HLT] = hlt_exit,
	[KVM_EXIT_MMIO] = unhandled_vm_exit,
	[KVM_EXIT_IRQ_WINDOW_OPEN] = unhandled_vm_exit,
	[KVM_EXIT_SHUTDOWN] = unhandled_vm_exit,
	[KVM_EXIT_FAIL_ENTRY] = unhandled_vm_exit,
	[KVM_EXIT_INTR] = unhandled_vm_exit,
	[KVM_EXIT_SET_TPR] = unhandled_vm_exit,
	[KVM_EXIT_TPR_ACCESS] = unhandled_vm_exit,
	[KVM_EXIT_S390_SIEIC] = unhandled_vm_exit,
	[KVM_EXIT_S390_RESET] = unhandled_vm_exit,
	[KVM_EXIT_DCR] = unhandled_vm_exit,
	[KVM_EXIT_NMI] = unhandled_vm_exit,
	[KVM_EXIT_INTERNAL_ERROR] = unhandled_vm_exit,
	[KVM_EXIT_OSI] = unhandled_vm_exit,
	[KVM_EXIT_PAPR_HCALL] = unhandled_vm_exit,
	[KVM_EXIT_S390_UCONTROL] = unhandled_vm_exit,
	[KVM_EXIT_WATCHDOG] = unhandled_vm_exit,
	[KVM_EXIT_S390_TSCH] = unhandled_vm_exit,
	[KVM_EXIT_EPR] = unhandled_vm_exit,
	[KVM_EXIT_SYSTEM_EVENT] = unhandled_vm_exit,
	[KVM_EXIT_S390_STSI] = unhandled_vm_exit,
	[KVM_EXIT_IOAPIC_EOI] = unhandled_vm_exit,
	[KVM_EXIT_HYPERV] = unhandled_vm_exit,
};

int handle_vm_exit(struct vm *vm)
{
	int reason = vm->vcpu.run->exit_reason;

	INFO("VM_EXIT: %s (%d)\n", kvm_exit_codes[reason], reason);
	return vm_exit_handlers[reason](vm);
}
