#ifndef TAPE__EXAMPLE__MODULE_LIST_H__INCLUDED_
#define TAPE__EXAMPLE__MODULE_LIST_H__INCLUDED_

#include <stdlib.h>

#include "module_types.h"

/** Define a module descriptor pointer array. */
#define MODULE_LIST(name, ...) module_list_t name[] = { __VA_ARGS__, NULL }

/** Returns a module descriptor pointer, or NULL if not found. */
extern const module_t *
module_list_find (const char *name);

/** Prints a help text for all modules and their command-line options. */
extern void
module_list_help (void);

#endif /* TAPE__EXAMPLE__MODULE_LIST_H__INCLUDED_ */
