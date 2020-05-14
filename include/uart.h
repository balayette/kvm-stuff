#ifndef UART_STATE_H
#define UART_STATE_H

#include "io.h"

#define COM1 0x3F8

#define UART_THR 0
#define UART_RBR 0
#define UART_DLL 0
#define UART_IER 1
#define UART_DLH 1
#define UART_IIR 2
#define UART_FCR 2
#define UART_LCR 3
#define UART_MCR 4
#define UART_LSR 5
#define UART_MSR 6
#define UART_SR 7

#define DLAB 0x80

/*
Base Address	DLAB	I/O Access	Abbrv.	Register Name
 +0	        0	Write	        THR	Transmitter Holding Buffer
 +0	        0	Read	        RBR	Receiver Buffer
 +0	        1	Read/Write	DLL	Divisor Latch Low Byte
 +1	        0	Read/Write	IER	Interrupt Enable Register
 +1	        1	Read/Write	DLH	Divisor Latch High Byte
 +2	        x	Read	        IIR	Interrupt Identification Register
 +2	        x	Write	        FCR	FIFO Control Register
 +3	        x	Read/Write	LCR	Line Control Register
 +4	        x	Read/Write	MCR	Modem Control Register
 +5	        x	Read	        LSR	Line Status Register
 +6	        x	Read	        MSR	Modem Status Register
 +7	        x	Read/Write	SR	Scratch Register
*/

struct uart_state {
	uint8_t thr;
	uint8_t rbr;
	uint8_t dll;
	uint8_t ier;
	uint8_t dlh;
	uint8_t iir;
	uint8_t fcr;
	uint8_t lcr;
	uint8_t mcr;
	uint8_t lsr;
	uint8_t msr;
	uint8_t sr;
};

int uart_driver(struct uart_state *uart, struct io_request *io, void *data);

#endif /* !UART_STATE_H */

