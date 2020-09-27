#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "buf.h"

#define BUF_GROW 8192u

#define buf_check(obj_) buf_check_((obj), __func__)

static void
buf_check_ (buf_t      *obj,
            const char *caller);

bool
buf_init (buf_t *obj)
{
	if (!obj) {
		fprintf(stderr, "%s: invalid arguments\n", __func__);
		return false;
	}

	memset((void *)obj, 0, sizeof(*obj));
	obj->data = NULL;

	return true;
}

void
buf_fini (buf_t *obj)
{
	buf_check(obj);

	if (obj->data) {
		if (obj->used > 0) {
			memset((void *)obj->data, 0, obj->used);
		}
		free(obj->data);
		obj->data = NULL;
	}

	obj->size = 0;
	obj->used = 0;
}

__attribute__((__always_inline__))
static inline size_t
buf_get_free_space_ (buf_t *obj)
{
	return obj->size - obj->used;
}

size_t
buf_get_free_space (buf_t *obj)
{
	buf_check(obj);
	return buf_get_free_space_(obj);
}

void
buf_reserve (buf_t  *obj,
             size_t  need)
{
	if (need < 1) {
		fprintf(stderr, "%s: invalid arguments\n", __func__);
		return;
	}

	buf_check(obj);
	size_t have = buf_get_free_space_(obj);

	if (need > have) do {
		uint8_t *data = obj->data;
		size_t size = ((obj->used + need) + (BUF_GROW - 1u)) & ~(BUF_GROW - 1u);

		if (size < 1 || size <= obj->used) {
			break;
		}

		errno = 0;
		data = (uint8_t *)realloc((void *)data, size);
		if (!data) {
			fprintf(stderr, "%s: realloc(..., %zu): %s\n", __func__, size, strerror(errno));
			break;
		}

		have = size - obj->used;
		memset((void *)&data[obj->used], 0, have);

		obj->data = data;
		obj->size = size;
	} while (0);
}

void
buf_release (buf_t *obj)
{
	buf_check(obj);
	if (obj->used > 0) {
		memset((void *)obj->data, 0, obj->used);
		obj->used = 0;
	}
}

static void
buf_check_ (buf_t      *obj,
            const char *caller)
{
	const char *reason = NULL;

	if (obj == NULL) {
		reason = " == NULL";

	} else if (obj->size < obj->used) {
		reason = "->size < obj->used";

	} else if (obj->size > 0) {
		if (obj->data == NULL) {
			reason = "->size > 0 && obj->data == NULL";
		}

	} else if (obj->data != NULL) {
		reason = "->size <= 0 && obj->data != NULL";
	}

	if (reason) {
		fprintf(stderr, "%s: panic: obj%s\n", caller, reason);
		abort();
	}
}
