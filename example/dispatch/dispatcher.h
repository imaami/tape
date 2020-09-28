#ifndef TAPE__EXAMPLE__DISPATCH__DISPATCHER_H__INCLUDED_
#define TAPE__EXAMPLE__DISPATCH__DISPATCHER_H__INCLUDED_

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "buf.h"

typedef struct dispatcher dispatcher_t;

struct dispatcher
{
	struct {
		char *path;
		FILE *stream;
	} fifo;

	buf_t buf;
};

extern bool
dispatcher_init (dispatcher_t *obj,
                 const char   *fifo);

extern int
dispatcher_run (dispatcher_t *obj);

__attribute__((__always_inline__, __unused__))
static inline buf_t *
dispatcher_get_buf (dispatcher_t *obj)
{
	return &obj->buf;
}

#endif /* TAPE__EXAMPLE__DISPATCH__DISPATCHER_H__INCLUDED_ */
