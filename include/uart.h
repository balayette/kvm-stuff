#ifndef UART_STATE_H
#define UART_STATE_H

#include "io.h"

#define COM1 0x3F8

struct uart_state {
	int dlab;
};

int uart_driver(struct uart_state *uart, struct io_request *io, void *data);

#endif /* !UART_STATE_H */

