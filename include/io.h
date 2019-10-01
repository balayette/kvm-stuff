#ifndef IO_H
#define IO_H

#include <stdint.h>

struct io_request {
	uint8_t direction;
	uint8_t size;
	uint16_t port;
	uint32_t count;
	uint64_t data_offset;
};

#endif /* !IO_H */
