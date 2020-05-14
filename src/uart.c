#include "uart.h"
#include "log.h"
#include "assert.h"
#include <linux/kvm.h>

#define SERIAL_OUT(val)                                                        \
	do {                                                                   \
		printf("%c", (val));                                           \
		fflush(stdout);                                                \
	} while (0)

static uint8_t uart_read(struct uart_state *uart, int port)
{
	switch (port) {
	/* case UART_DLL: */
	case UART_RBR:
		if (uart->lcr & DLAB)
			return uart->dll;
		/* Clear data ready bit */
		uart->lsr &= ~1;
		return uart->rbr;
	/* case UART_IER: */
	case UART_DLH:
		if (uart->lcr & DLAB)
			return uart->dlh;
		return uart->ier;
	case UART_IIR:
		return uart->iir;
	case UART_LCR:
		return uart->lcr;
	case UART_MCR:
		return uart->mcr;
	case UART_LSR:
		return uart->lsr | 0x60;
	case UART_MSR:
		return uart->msr;
	case UART_SR:
		return uart->sr;
	}

	ASSERT(0, "uart_read incorrect port.");
}

static void uart_driver_in(struct uart_state *uart, struct io_request *io,
			   uint8_t *data)
{
	int port = io->port - COM1;
	uint8_t ret = uart_read(uart, port);

	INFO("uart_read(%p, %hhd) = 0x%hhx\n", (void *)uart, port, ret);
	*data = ret;
}

static void uart_write(struct uart_state *uart, int port, uint8_t value)
{
	switch (port) {
	/* case UART_THR: */
	case UART_DLL:
		if (uart->lcr & DLAB)
			uart->dll = value;
		else {
			uart->thr = value;
			SERIAL_OUT(value);
		}
		break;
	/* case UART_IER: */
	case UART_DLH:
		if (uart->lcr & DLAB)
			uart->dll = value;
		uart->ier = value;
		break;
	case UART_FCR:
		uart->fcr = value;
		break;
	case UART_LCR:
		uart->lcr = value;
		break;
	case UART_MCR:
		uart->mcr = value;
		break;
	case UART_SR:
		uart->sr = value;
		break;
	default:
		ASSERT(0, "uart_write incorrect port.");
	}
}

static void uart_driver_out(struct uart_state *uart, struct io_request *io,
			    uint8_t *data)
{
	int port = io->port - COM1;
	uint8_t value = *data;
	INFO("uart_write(%p, %hhd, %hhd)\n", (void *)uart, port, value);
	uart_write(uart, port, value);
}

int uart_driver(struct uart_state *uart, struct io_request *io, void *data)
{
	if (io->direction == KVM_EXIT_IO_IN)
		uart_driver_in(uart, io, data);
	else
		uart_driver_out(uart, io, data);

	return 0;
}
