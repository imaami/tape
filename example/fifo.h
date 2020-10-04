/**
 * \brief Communication through a named pipe.
 *
 * This is used to relay commands to the dispatcher.
 */
#ifndef TAPE__EXAMPLE__FIFO_H__INCLUDED_
#define TAPE__EXAMPLE__FIFO_H__INCLUDED_

#include <stdio.h>
#include <stdbool.h>

#include "buf.h"

typedef struct fifo fifo_t;

struct fifo
{
	char *path;
	FILE *stream;
};

/** \brief Initialize the object structure. */
extern bool
fifo_init (fifo_t     *obj,
           const char *path);

/** \brief Free any memory held by the object.
 * Does not delete the named pipe file from disk.
 */
extern void
fifo_fini (fifo_t *obj);

/** \brief Create a named pipe with the path given to \ref fifo_init().
 * Fails if the path already exists.
 */
extern bool
fifo_mkfifo (fifo_t *obj);

/** \brief Read from the named pipe. */
extern bool
fifo_read (fifo_t *obj,
           buf_t  *buf);

/** \brief Write to the named pipe. */
extern bool
fifo_write (fifo_t *obj,
            buf_t  *buf);

#endif /* TAPE__EXAMPLE__FIFO_H__INCLUDED_ */
