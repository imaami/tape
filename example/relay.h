#ifndef TAPE__EXAMPLE__RELAY_H__INCLUDED_
#define TAPE__EXAMPLE__RELAY_H__INCLUDED_

#include <stdbool.h>

#include "fifo.h"
#include "buf.h"

typedef struct relay relay_t;

struct relay
{
	fifo_t fifo;
	buf_t  buf;
};

extern bool
relay_init (relay_t    *obj,
            const char *fifo_path);

extern void
relay_fini (relay_t *obj);

extern bool
relay_send (relay_t  *obj,
            int       argc,
            char    **argv);

extern bool
relay_recv (relay_t   *obj,
            int       *pargc,
            char    ***pargv);

__attribute__((__always_inline__, __unused__))
static inline bool
relay_mkfifo (relay_t *obj)
{
	return fifo_mkfifo(&obj->fifo);
}

#endif /* TAPE__EXAMPLE__RELAY_H__INCLUDED_ */
