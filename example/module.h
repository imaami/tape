#ifndef TAPE__EXAMPLE__MODULE_H__INCLUDED_
#define TAPE__EXAMPLE__MODULE_H__INCLUDED_

/** Define a module descriptor pointer array. */
#define MODULE_LIST(name, ...) module_list_t name[] = { __VA_ARGS__, NULL }

/** Iterate a module descriptor pointer array. */
#define for_each_module(ptr, iter, list) \
	for (module_iter_init(iter, list); (ptr = module_iter_get(iter)); module_iter_next(iter))

/** Module descriptor type. */
typedef struct module module_t;

/** Module iterator type. */
typedef struct module_iter module_iter_t;

/** Helper type for referencing a module descriptor pointer array. */
typedef const module_t *const module_list_t;

/** Module invocation callback type. */
typedef int exec_cb_t(int argc, char **argv);

/** Module help printout callback type. */
typedef void help_cb_t(const char *line_prefix);

/** Module descriptor. */
struct module
{
	const char *name;
	const char *what;
	exec_cb_t  *exec;
	help_cb_t  *help;
};

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

/** Returns a module descriptor pointer, or NULL if not found. */
extern const module_t *
module_list_find_by_name (module_list_t *list,
                          const char    *name);

/** Invoke a module with a set of command-line arguments. */
extern int
module_invoke (const module_t  *module,
               int              argc,
               char           **argv);

#endif /* TAPE__EXAMPLE__MODULE_H__INCLUDED_ */
