#ifndef TAPE__EXAMPLE__MODULE_LIST_H__INCLUDED_
#define TAPE__EXAMPLE__MODULE_LIST_H__INCLUDED_

#include <stdlib.h>

#include "module_types.h"
#include "module_flag.h"

/** Define a module descriptor pointer array. */
#define MODULE_LIST(name, ...) module_list_t name[] = { __VA_ARGS__, NULL }

/** Returns a module descriptor pointer matching the given name and
 *  flags, or NULL if not found. */
extern const module_t *
module_list_find_cond (const char    *name,
                       module_flag_t  mask,
                       module_flag_t  cond);

/** Prints a description and help text for each module matching the
 * given flags. */
extern void
module_list_help_cond (module_flag_t mask,
                       module_flag_t cond);

__attribute__((__always_inline__, __unused__))
static inline const module_t *
module_list_find (const char *name)
{
	return module_list_find_cond(name, MODULE_FLAG_BUILTIN, MODULE_FLAG_NONE);
}

__attribute__((__always_inline__, __unused__))
static inline void
module_list_help (void)
{
	module_list_help_cond(MODULE_FLAG_BUILTIN, MODULE_FLAG_NONE);
}

__attribute__((__always_inline__, __unused__))
static inline const module_t *
module_list_find_builtin (const char *name)
{
	return module_list_find_cond(name, MODULE_FLAG_BUILTIN, MODULE_FLAG_BUILTIN);
}

#endif /* TAPE__EXAMPLE__MODULE_LIST_H__INCLUDED_ */
