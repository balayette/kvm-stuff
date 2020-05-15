#ifndef ASSERT_H
#define ASSERT_H

#include <stdio.h>
#include <stdlib.h>
#include "log.h"

#define ASSERT(x, msg)                                                         \
	do {                                                                   \
		if (!(x)) {                                                    \
			FAIL("assertion '%s' failed: %s\n", #x, msg);           \
			exit(1);                                               \
		}                                                              \
	} while (0)

#endif /* !ASSERT_H */
