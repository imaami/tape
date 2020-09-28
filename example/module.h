#ifndef TAPE__EXAMPLE__MODULE_H__INCLUDED_
#define TAPE__EXAMPLE__MODULE_H__INCLUDED_

#include "module_types.h"

/** Declare a module descriptor. */
#define DECLARE_MODULE(name) extern const module_t name

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

/** Invoke a module with a set of command-line arguments. */
extern int
module_invoke (const module_t  *module,
               int              argc,
               char           **argv);

#endif /* TAPE__EXAMPLE__MODULE_H__INCLUDED_ */
