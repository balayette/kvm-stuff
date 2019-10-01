#ifndef PORTIO_H
#define PORTIO_H

#include "uart.h"
#include "io.h"
#include <linux/kvm.h>

struct portio {
	struct uart_state uart;
};

int portio_driver(struct portio *portio, struct io_request *io, void *data);

#endif /* !PORTIO_H */
