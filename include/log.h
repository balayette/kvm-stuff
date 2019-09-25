#ifndef LOG_H
#define LOG_H

#include <stdio.h>

#ifndef _NO_LOG

#define INFO(...)                                                              \
	do {                                                                   \
		fprintf(stderr, "[-] ");                                       \
		_LOG(__VA_ARGS__);                                             \
	} while (0)

#define INFO_NV(...)                                                           \
	do {                                                                   \
		fprintf(stderr, "[-] ");                                       \
		_LOG_NV(__VA_ARGS__);                                          \
	} while (0)

#define SUCCESS(...)                                                           \
	do {                                                                   \
		fprintf(stderr, "[+] ");                                       \
		_LOG(__VA_ARGS__);                                             \
	} while (0)

#define SUCCESS_NV(...)                                                        \
	do {                                                                   \
		fprintf(stderr, "[+] ");                                       \
		_LOG_NV(__VA_ARGS__);                                          \
	} while (0)

#define FAIL(...)                                                              \
	do {                                                                   \
		fprintf(stderr, "[x] ");                                       \
		_LOG(__VA_ARGS__);                                             \
	} while (0)

#define FAIL_NV(...)                                                           \
	do {                                                                   \
		fprintf(stderr, "[x] ");                                       \
		_LOG_NV(__VA_ARGS__);                                          \
	} while (0)

#define _LOG(...)                                                              \
	do {                                                                   \
		fprintf(stderr, "%s:%d | ", __FILE__, __LINE__);               \
		fprintf(stderr, __VA_ARGS__);                                  \
	} while (0)

#define _LOG_NV(...)                                                           \
	do {                                                                   \
		fprintf(stderr, __VA_ARGS__);                                  \
	} while (0);
#else

#define INFO(...)
#define INFO_NV(...)
#define SUCCESS(...)
#define SUCCESS_NV(...)
#define FAIL(...)
#define FAIL_NV(...)

#endif /* _NO_LOG */

#endif /* !LOG_H */
