#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

#define PAGE_ALIGN(x) ((x + 4096) & ~(4096 - 1))

void *map_file(char *filename, size_t *size);

#endif /* !UTILS_H */
