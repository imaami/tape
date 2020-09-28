#ifndef TAPE__EXAMPLE__MODULE_ITER_H__INCLUDED_
#define TAPE__EXAMPLE__MODULE_ITER_H__INCLUDED_

#include "module_types.h"

/** Iterate a module descriptor pointer array. */
#define for_each_module(ptr, iter, list) \
	for (module_iter_init(iter, list); (ptr = module_iter_get(iter)); module_iter_next(iter))

/** Module iterator. */
struct module_iter
{
	module_list_t *ptr;
};

/** Module iterator initializer. */
__attribute__((__always_inline__, __unused__))
static inline void
module_iter_init (module_iter_t *iter,
                  module_list_t *list)
{
	iter->ptr = list;
}

/** Returns the module pointer at current iterator position. */
__attribute__((__always_inline__, __unused__))
static inline const module_t *
module_iter_get (module_iter_t *iter)
{
	return *iter->ptr;
}

/** Advances the iterator to the next position. */
__attribute__((__always_inline__, __unused__))
static inline void
module_iter_next (module_iter_t *iter)
{
	iter->ptr++;
}

#endif /* TAPE__EXAMPLE__MODULE_ITER_H__INCLUDED_ */
