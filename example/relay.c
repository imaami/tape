#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "relay.h"

bool
relay_init (relay_t    *obj,
            const char *fifo_path)
{
	if (sizeof(char *) != sizeof(uintptr_t)) {
		fputs("unsupported pointer size: sizeof(char *) != sizeof(uintptr_t)\n", stderr);
		abort(); // Precaution, unlikely to actually happen
	}

	if (!obj || !fifo_path || !*fifo_path) {
		fprintf(stderr, "%s: invalid arguments\n", __func__);
		return false;
	}

	memset((void *)obj, 0, sizeof(*obj));

	if (!fifo_init(&obj->fifo, fifo_path)) {
		return false;
	}

	if (!buf_init(&obj->buf)) {
		fifo_fini(&obj->fifo);
		return false;
	}

	return true;
}

void
relay_fini (relay_t *obj)
{
	fifo_fini(&obj->fifo);
	buf_fini(&obj->buf);

	memset((void *)obj, 0, sizeof(*obj));
}

__attribute__((__always_inline__))
static inline bool
relay_serialize (relay_t  *obj,
                 int       argc,
                 char    **argv);

__attribute__((__always_inline__))
static inline void
relay_deserialize (relay_t   *obj,
                   int       *pargc,
                   char    ***pargv);

bool
relay_send (relay_t  *obj,
            int       argc,
            char    **argv)
{
	bool success = relay_serialize(obj, argc, argv) &&
	               fifo_write(&obj->fifo, &obj->buf);
	buf_release(&obj->buf);
	return success;
}

bool
relay_recv (relay_t   *obj,
            int       *pargc,
            char    ***pargv)
{
	buf_release(&obj->buf);

	if (!fifo_read(&obj->fifo, &obj->buf)) {
		return false;
	}

	relay_deserialize(obj, pargc, pargv);
	return true;
}

/** Serialize argc + argv for sending through the FIFO */
__attribute__((__always_inline__))
static inline bool
relay_serialize (relay_t  *obj,
                 int       argc,
                 char    **argv)
{
	if (argc < 1 || argv == NULL) {
		return false;
	}

	int i;
	size_t argv_size, total_size, size;
	char **dest_argv;
	uint8_t *buf_ptr;

	/* Preallocate space for the destination pointer array */
	argv_size = (size_t)(argc + 1) * sizeof(char *);
	buf_release(&obj->buf);
	buf_reserve(&obj->buf, argv_size);
	if (buf_get_free_space(&obj->buf) < argv_size) {
		return false;
	}

	/* Get the dest argv pointer */
	dest_argv = (char **)buf_get_data_ptr(&obj->buf);
	total_size = argv_size;

	/* First pass over input argv */
	for (i = 0; i < argc; ++i) {
		if (!argv[i]) {
			return false;
		}
		size = strlen(argv[i]) + 1u;
		dest_argv[i] = (char *)(uintptr_t)size;
		total_size += size;
	}

	/* Terminate dest argv */
	dest_argv[argc] = NULL;

	/* Now we know how much we need for everything */
	buf_reserve(&obj->buf, total_size);
	if (buf_get_free_space(&obj->buf) < total_size) {
		return false;
	}

	/* buf_reserve() may realloc(), so re-fetch the pointer */
	buf_ptr = buf_get_data_ptr(&obj->buf);
	dest_argv = (char **)buf_ptr;

	/* Move write pointer to beginning of string data */
	buf_consume(&obj->buf, argv_size);

	/* Copy the strings */
	for (i = 0; i < argc; ++i) {
		uint8_t *dest = buf_get_write_ptr(&obj->buf);
		size = (size_t)(uintptr_t)dest_argv[i];
		dest_argv[i] = (char *)(uintptr_t)(dest - buf_ptr);
		memcpy((void *)dest, (const void **)argv[i], size);
		buf_consume(&obj->buf, size);
	}

	return true;
}

__attribute__((__always_inline__))
static inline void
relay_deserialize (relay_t   *obj,
                   int       *pargc,
                   char    ***pargv)
{
	int argc = 0;
	char **argv = NULL;

	do {
		size_t total_size;
		uint8_t *buf_ptr;
		char **ptr;

		total_size = buf_get_used_space(&obj->buf);

		if (total_size < sizeof(char *)) {
			break;
		}

		buf_ptr = buf_get_data_ptr(&obj->buf);
		ptr = (char **)buf_ptr;

		if (!*ptr) {
			break;
		}

		for (size_t i = 0; i < total_size; ) {
			char **ptr = (char **)&buf_ptr[i];
			if (!*ptr) {
				argc = (int)(i / sizeof(char *));
				break;
			}
			i += sizeof(char *);
			//uintptr_t pos = (uintptr_t)*ptr;
			//if (pos <= i || pos >= total_size)
		}

	} while (0);

	*pargc = argc;
	*pargv = argv;
}
