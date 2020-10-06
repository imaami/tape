#ifndef TAPE__EXAMPLE__BUF_H__INCLUDED_
#define TAPE__EXAMPLE__BUF_H__INCLUDED_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct buf buf_t;

struct buf
{
	uint8_t *data;
	size_t   size;
	size_t   used;
};

extern bool
buf_init (buf_t *obj);

extern void
buf_fini (buf_t *obj);

extern size_t
buf_get_free_space (buf_t *obj);

extern void
buf_reserve (buf_t  *obj,
             size_t  need);

extern void
buf_release (buf_t *obj);

__attribute__((__always_inline__, __unused__))
static inline uint8_t *
buf_get_data_ptr (buf_t *obj)
{
	return obj->data;
}

__attribute__((__always_inline__, __unused__))
static inline size_t
buf_get_used_space (buf_t *obj)
{
	return obj->used;
}

__attribute__((__always_inline__, __unused__))
static inline uint8_t *
buf_get_write_ptr (buf_t *obj)
{
	return obj->data + obj->used;
}

__attribute__((__always_inline__, __unused__))
static inline void
buf_consume (buf_t  *obj,
             size_t  size)
{
	obj->used += size;
}

#endif /* TAPE__EXAMPLE__BUF_H__INCLUDED_ */
