#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "module.h"
#include "module_list.h"

static void usage (char *argv0);

int
main (int    argc,
      char **argv)
{
	int cmd_argc;
	char **cmd_argv;
	const module_t *module;

	module = module_list_find(argv[1]);

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
	char *prog;

	prog = strrchr((const char *)argv0, '/');
	if (prog) {
		prog++;
	} else {
		prog = argv0;
	}

	printf("\nusage: %s COMMAND [OPTIONS]...\n", prog);

	module_list_help();
}
