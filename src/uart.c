#include "uart.h"
#include "log.h"
#include <linux/kvm.h>

static int uart_driver_in(struct uart_state *uart, struct io_request *io,
			  void *data)
{
	(void)uart;

	if (io->port == COM1 + 5) {
		/* we're always "empty" */
		*(char *)data = 0x20;
	}

	return 0;
}

static int uart_driver_out(struct uart_state *uart, struct io_request *io,
			   void *data)
{
	if (io->port == COM1 + 3) {
		int value = (*(char *)data & 0x80) >> 7;
		INFO("DLAB=%d\n", value);
		uart->dlab = value;

		return 0;
	}

	if (io->port == COM1 && !uart->dlab)
		printf("%c", *(char *)data);

	return 0;
}

int uart_driver(struct uart_state *uart, struct io_request *io, void *data)
{
	if (io->direction == KVM_EXIT_IO_IN)
		return uart_driver_in(uart, io, data);
	return uart_driver_out(uart, io, data);
}
