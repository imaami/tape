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

#endif /* TAPE__EXAMPLE__DISPATCH__DISPATCHER_H__INCLUDED_ */
