#include <stdio.h>
#include "log.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "assert.h"
#include "kvmw.h"

struct kvmw kvmw_open(void)
{
	struct kvmw ret;

	ret.kvm_fd = open("/dev/kvm", O_RDWR | O_CLOEXEC);
	ASSERT(ret.kvm_fd != -1, "open(\"/dev/kvm\") failed");
	INFO("kvm_fd: %d\n", ret.kvm_fd);

	int api_version = ioctl(ret.kvm_fd, KVM_GET_API_VERSION, NULL);
	ASSERT(api_version != -1, "ioctl(KVM_GET_API_VERSION) failed");
	ASSERT(api_version == KVM_API_VERSION, "API version mismatch");

	SUCCESS("KVM API version: %d\n", api_version);

	return ret;
}

void kvmw_close(struct kvmw *w)
{
	close(w->kvm_fd);
}

int kvmw_check_cap(struct kvmw *w, int cap)
{
	int ok = ioctl(w->kvm_fd, KVM_CHECK_EXTENSION, cap);
	ASSERT(ok != -1, "ioctl(KVM_CHECK_EXTENSION) failed");

	return ok != 0;
}

void kvmw_create_vm(struct kvmw *w)
{
	w->vm.vm_fd = ioctl(w->kvm_fd, KVM_CREATE_VM, 0);
	ASSERT(w->vm.vm_fd != -1, "ioctl(KVM_CREATE_VM) failed");

	INFO("vm_fd: %d\n", w->vm.vm_fd);
}

int kvmw_get_kvm_run_sz(struct kvmw *w)
{
	int ret = ioctl(w->kvm_fd, KVM_GET_VCPU_MMAP_SIZE, NULL);
	ASSERT(ret != -1, "ioctl(KLM_GET_VCPU_MMAP_SIZE) failed");

	return ret;
}
