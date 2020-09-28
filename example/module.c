#include "module.h"
#include <stdlib.h>

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
