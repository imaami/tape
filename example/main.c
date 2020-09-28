#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "module.h"
#include "dispatch/dispatch.h"
#include "grid/grid.h"
#include "wait/wait.h"

static MODULE_LIST( modules,
	&dispatch,
	&grid,
	&wait
);

static void usage (char *argv0);

int
main (int    argc,
      char **argv)
{
	int cmd_argc;
	char **cmd_argv;
	const module_t *module;

	module = module_list_find(modules, argv[1]);

	if (!module) {
		usage(argv[0]);
		return EXIT_FAILURE;
	}

	cmd_argc = argc - 1;
	cmd_argv = argv + 1;

	return module_invoke(module, cmd_argc, cmd_argv);
}

static void
usage (char *argv0)
{
	int max;
	char *prog;
	module_iter_t iter;
	const module_t *ptr;

	prog = strrchr((const char *)argv0, '/');
	if (prog) {
		prog++;
	} else {
		prog = argv0;
	}

	max = 0;

	for_each_module (ptr, &iter, modules) {
		int n = (int)strlen(ptr->name);
		if (n > max) {
			max = n;
		}
	}

	printf("\nusage: %s COMMAND [OPTIONS]...\n", prog);

	for_each_module (ptr, &iter, modules) {
		printf("\n %-*s [OPTIONS]...\t%s\n\n", max, ptr->name, ptr->what);
		if (ptr->help) {
			ptr->help("   ");
		}
	}

	putchar('\n');
}
