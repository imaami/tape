#include "module.h"
#include <stdlib.h>
#include <string.h>

const module_t *
module_list_find_by_name (const module_t *const *list,
                          const char            *name)
{
	if (!list || !name) {
		return NULL;
	}

	const module_t *ptr;
	for (; (ptr = *list) && strcmp(name, ptr->name); ++list) {}

	return ptr;
}

int
module_invoke (const module_t  *module,
               int              argc,
               char           **argv)
{
	if (!module) {
		return EXIT_FAILURE;
	}

	return module->func(argc, argv);
}

int
module_invoke_by_name (const module_t *const  *list,
                       const char             *name,
                       int                     argc,
                       char                  **argv)
{
	return module_invoke(module_list_find_by_name(list, name), argc, argv);
}
