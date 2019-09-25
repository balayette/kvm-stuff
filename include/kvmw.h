#ifndef KVMW_H
#define KVMW_H

#include "vm.h"

struct kvmw {
	int kvm_fd;
	struct vm vm;
};

struct kvmw kvmw_open(void);

void kvmw_close(struct kvmw *w);

int kvmw_check_cap(struct kvmw *w, int cap);

void kvmw_create_vm(struct kvmw *w);

int kvmw_get_kvm_run_sz(struct kvmw *w);

#endif /* !KVMW_H */
