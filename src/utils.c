#include "utils.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include "assert.h"

void *map_file(char *filename, size_t *size)
{
	int fd = open(filename, O_RDONLY);
	ASSERT(fd != -1, "Couldn't open the file.");

	struct stat buf;
	ASSERT(fstat(fd, &buf) != -1, "Couldn't stat the file.");

	*size = PAGE_ALIGN(buf.st_size);

	void *base =
		mmap(NULL, *size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	ASSERT(base != MAP_FAILED, "Couldn't map the file.");

	close(fd);

	return base;
}
