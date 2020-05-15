#include "portio.h"
#include "log.h"
#include "uart.h"

int portio_driver(struct portio *portio, struct io_request *io, void *data)
{
	if (io->direction == KVM_EXIT_IO_IN)
		INFO("portio IN\n");
	else
		INFO("portio OUT\n");
	INFO("%d bytes on port 0x%x (count: %d, offset: %ld)\n", io->size,
	     io->port, io->count, io->data_offset);

	if (io->port >= COM1 && io->port <= COM1 + 7)
		return uart_driver(&portio->uart, io, data);
        else
                return 1;

	return 0;
}
