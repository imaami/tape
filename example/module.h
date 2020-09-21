#ifndef TAPE__EXAMPLE__MODULE_H__INCLUDED_
#define TAPE__EXAMPLE__MODULE_H__INCLUDED_

/** Opaque module descriptor type. */
typedef struct module module_t;

/** Module invocation callback type. */
typedef int callback_t(int argc, char **argv);

/** Module descriptor. */
struct module
{
	const char *name;
	callback_t *func;
};

/** Returns an opaque module descriptor pointer, or NULL if not found. */
extern const module_t *
module_list_find_by_name (const module_t *const *list,
                          const char            *name);

/** Invoke a module with a set of command-line arguments. */
extern int
module_invoke (const module_t  *module,
               int              argc,
               char           **argv);

/** Find the named module and invoke it with a set of command-line arguments. */
extern int
module_invoke_by_name (const module_t *const  *list,
                       const char             *name,
                       int                     argc,
                       char                  **argv);

#endif /* TAPE__EXAMPLE__MODULE_H__INCLUDED_ */
