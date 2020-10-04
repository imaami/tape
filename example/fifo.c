#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "module.h"
#include "module_list.h"
#include "fifo.h"

bool
fifo_init (fifo_t     *obj,
           const char *path)
{
	errno = 0;
	obj->path = strdup(path);
	obj->stream = NULL;

	if (!obj->path) {
		fprintf(stderr, "%s: strdup(\"%s\"): %s\n", __func__, path, strerror(errno));
		return false;
	}

	return true;
}

void
fifo_fini (fifo_t *obj)
{
	if (obj->path) {
		free(obj->path);
		obj->path = NULL;
	}

	if (obj->stream) {
		(void)fclose(obj->stream);
		obj->stream = NULL;
		errno = 0;
	}
}

bool
fifo_mkfifo (fifo_t *obj)
{
	errno = 0;

	if (mkfifo(obj->path, 0666) != 0) {
		fprintf(stderr, "%s: %s\n", __func__, strerror(errno));
		return false;
	}

	return true;
}

static bool
fifo_open_for_reading (fifo_t *obj)
{
	errno = 0;
	obj->stream = fopen(obj->path, "rbe");

	if (!obj->stream) {
		fprintf(stderr, "%s: fopen(\"%s\", \"rbe\"): %s\n", __func__, obj->path, strerror(errno));
		return false;
	}

	return true;
}

static bool
fifo_open_for_writing (fifo_t *obj)
{
	int fd;
	struct stat sb;

	/* In our design only the dispatcher is allowed to create its input
	 * FIFO. Unfortunately fopen() in write-only mode tries to create a
	 * new file if the target doesn't exist, and calling stat() followed
	 * by fopen() is inherently racy. As a workaround we call open() and
	 * then get the stream pointer with fdopen().
	 */
	errno = 0;
	fd = open(obj->path, O_WRONLY|O_CLOEXEC);

	if (fd < 0) {
		fprintf(stderr, "%s: open(\"%s\", O_WRONLY|O_CLOEXEC): %s\n", __func__, obj->path, strerror(errno));
		return false;
	}

	do {
		if (fstat(fd, &sb) != 0) {
			fprintf(stderr, "%s: fstat: %s\n", __func__, strerror(errno));
			break;
		}

		if ((sb.st_mode & S_IFMT) != S_IFIFO) {
			fprintf(stderr, "%s: \"%s\" is not a FIFO\n", __func__, obj->path);
			break;
		}

		obj->stream = fdopen(fd, "w");

		if (!obj->stream) {
			fprintf(stderr, "%s: fdopen: %s\n", __func__, strerror(errno));
			break;
		}

		return true;
	} while (0);

	(void)close(fd);
	return false;
}

static void
fifo_close (fifo_t *obj)
{
	(void)fclose(obj->stream);
	obj->stream = NULL;
	errno = 0;
}

bool
fifo_read (fifo_t *obj,
           buf_t  *buf)
{
	if (!fifo_open_for_reading(obj)) {
		return false;
	}

	size_t free_space, bytes_read;

	do {
		buf_reserve(buf, 4096);
		free_space = buf_get_free_space(buf) - 1;
		bytes_read = fread((void *)buf_get_write_ptr(buf),
		                   1u, free_space, obj->stream);
		buf_consume(buf, bytes_read);
	} while (bytes_read == free_space);

	bool success = !ferror(obj->stream);

	if (success) {
		// null-terminate
		*(buf_get_write_ptr(buf)) = (uint8_t)'\0';
		buf_consume(buf, 1);

	} else {
		fprintf(stderr, "%s: fread failed\n", __func__);
		buf_release(buf);
	}

	fifo_close(obj);

	return success;
}

bool
fifo_write (fifo_t *obj,
            buf_t  *buf)
{
	if (!fifo_open_for_writing(obj)) {
		return false;
	}

	const uint8_t *ptr = (const uint8_t *)buf_get_data_ptr(buf);
	size_t len = buf->used, i = 0;

	while (i < len) {
		size_t n = fwrite((const void *)&ptr[i],
		                  1u, len - i, obj->stream);
		if (n == 0) {
			break;
		}
		i += n;
	}

	if (ferror(obj->stream)) {
		fprintf(stderr, "%s: fwrite failed\n", __func__);
	}

	fifo_close(obj);

	return i >= len;
}
