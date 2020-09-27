#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "dispatcher.h"

#define BUF_GROW 8192u

static void
dispatcher_fini (dispatcher_t *obj)
{
	if (obj->fifo.path) {
		free(obj->fifo.path);
	}

	if (obj->fifo.stream) {
		(void)fclose(obj->fifo.stream);
		errno = 0;
	}

	buf_fini(&obj->buf);

	memset((void *)obj, 0, sizeof(*obj));
}

bool
dispatcher_init (dispatcher_t *obj,
                 const char   *fifo)
{
	if (!obj || !fifo || !*fifo) {
		fprintf(stderr, "%s: invalid arguments\n", __func__);
		return false;
	}

	memset((void *)obj, 0, sizeof(*obj));

	errno = 0;
	obj->fifo.path = strdup(fifo);

	if (!obj->fifo.path) {
		fprintf(stderr, "%s: strdup(\"%s\"): %s\n", __func__, fifo, strerror(errno));
		return false;
	}

	obj->fifo.stream = NULL;
	buf_init(&obj->buf);

	return true;
}

static bool
dispatcher_read_stream (dispatcher_t *obj)
{
	errno = 0;

	if (!(obj->fifo.stream = fopen(obj->fifo.path, "rbe"))) {
		fprintf(stderr, "%s: fopen(\"%s\", \"rbe\"): %s\n", __func__, obj->fifo.path, strerror(errno));
		return false;
	}

	for (;;) {
		buf_reserve(&obj->buf, 1024);

		uint8_t *ptr = buf_get_write_ptr(&obj->buf);
		size_t space = buf_get_free_space(&obj->buf) - 1;
		size_t count = fread((void *)ptr, 1u, space, obj->fifo.stream);

		buf_consume(&obj->buf, count);

		if (count != space) {
			ptr = buf_get_write_ptr(&obj->buf);
			*(char *)ptr = '\0';
			break;
		}
	}

	bool success = !ferror(obj->fifo.stream);
	if (!success) {
		fprintf(stderr, "%s: fread failed\n", __func__);
	}

	(void)fclose(obj->fifo.stream);
	obj->fifo.stream = NULL;
	errno = 0;

	return success;
}

int
dispatcher_run (dispatcher_t *obj)
{
	int err = EXIT_SUCCESS;

	if (!obj) {
		fprintf(stderr, "%s: invalid arguments\n", __func__);
		return EXIT_FAILURE;
	}

	if (!obj->fifo.path) {
		fprintf(stderr, "%s: module is not initialized\n", __func__);
		return EXIT_FAILURE;
	}

	do {
		errno = 0;

		if (mkfifo(obj->fifo.path, 0666) != 0) {
			err = errno;
			fprintf(stderr, "%s: mkfifo(\"%s\", 0666): %s\n", __func__, obj->fifo.path, strerror(err));
			break;
		}

		while (dispatcher_read_stream(obj)) {
			const char *str = (const char *)&obj->buf.data[0];
			const char *end = str + obj->buf.used;
			int count = 0;
			for (const char *ptr = str;; ptr++) {
				if (ptr >= end) {
					if (ptr > str) {
						printf("[%d] \"%s\"\n", count, str);
						count++;
					}
					break;
				}
				if (*ptr == '\0') {
					printf("[%d] \"%s\"\n", count, str);
					count++;
					str = ptr + 1;
				}
			}
			printf("=> %d arguments\n\n", count);
			buf_release(&obj->buf);
		}

	} while (0);

	dispatcher_fini(obj);

	return err;
}
