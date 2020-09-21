#include "module.h"
#include <stdlib.h>
#include <string.h>

const module_t *
module_list_find_by_name (module_list_t *list,
                          const char    *name)
{
	if (!list || !name) {
		return NULL;
	}

	module_iter_t  iter;
	const module_t *ptr;

	for_each_module (ptr, &iter, list) {
		if (!strcmp(name, ptr->name)) {
			break;
		}
	}

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

	return module->exec(argc, argv);
}
