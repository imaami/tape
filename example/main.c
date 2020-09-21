#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include "module.h"
#include "grid/grid.h"
#include "wait/wait.h"

static const module_t *const modules[] = {
	&grid,
	&wait
};

int
main (int    argc,
      char **argv)
{
	return module_invoke_by_name(modules, argv[1], argc - 1, argv + 1);
}
