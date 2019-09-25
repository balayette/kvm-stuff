#ifndef CPU_H
#define CPU_H

#include <stddef.h>
#include <linux/kvm.h>

struct vcpu {
	int cpu_fd;
	struct kvm_run *run;
	size_t run_sz;
};

void vcpu_map_kvm_run(struct vcpu *v, int kvm_run_sz);

struct kvm_sregs vcpu_get_sregs(struct vcpu *v);

void vcpu_set_sregs(struct vcpu *v, struct kvm_sregs regs);

struct kvm_regs vcpu_get_regs(struct vcpu *v);

void vcpu_set_regs(struct vcpu *v, struct kvm_regs regs);

#endif /* !CPU_H */
