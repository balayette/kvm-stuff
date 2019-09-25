#include <sys/mman.h>
#include <sys/ioctl.h>
#include "vcpu.h"
#include "assert.h"

void vcpu_map_kvm_run(struct vcpu *v, int kvm_run_sz)
{
	v->run_sz = kvm_run_sz;

	v->run = mmap(NULL, v->run_sz, PROT_READ | PROT_WRITE, MAP_SHARED,
		      v->cpu_fd, 0);
	ASSERT(v->run != MAP_FAILED, "mmap(kvm_run) failed");
}

struct kvm_sregs vcpu_get_sregs(struct vcpu *v)
{
	struct kvm_sregs regs;

	int ret = ioctl(v->cpu_fd, KVM_GET_SREGS, &regs);
	ASSERT(ret != -1, "ioctl(KVM_GET_SREGS) failed");

	return regs;
}

void vcpu_set_sregs(struct vcpu *v, struct kvm_sregs regs)
{
	int ret = ioctl(v->cpu_fd, KVM_SET_SREGS, &regs);
	ASSERT(ret != -1, "ioctl(KVM_SET_SREGS) failed");
}

struct kvm_regs vcpu_get_regs(struct vcpu *v)
{
	struct kvm_regs regs;

	int ret = ioctl(v->cpu_fd, KVM_GET_REGS, &regs);
	ASSERT(ret != -1, "ioctl(KVM_GET_REGS) failed");

	return regs;
}

void vcpu_set_regs(struct vcpu *v, struct kvm_regs regs)
{
	int ret = ioctl(v->cpu_fd, KVM_SET_REGS, &regs);
	ASSERT(ret != -1, "ioctl(KVM_SET_REGS) failed");
}
