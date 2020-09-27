#include "dispatch.h"
#include <stdio.h>
#include <stdlib.h>

#include "module_priv.h"
#include "dispatcher.h"

static dispatcher_t dispatcher;

static int
callback (int    argc,
          char **argv)
{
	if (argc < 2) {
		return 1;
	}

	const char *fifo = NULL;

	if (argv[1][0] == '-') {
		const char *arg = NULL;
		tape_option_tag_t tag;

		if (argv[1][1] == '-') {
			arg = module_option_tag_from_string(&tag, (const char *)&argv[1][2]);
			if (arg != NULL && *arg == '\0') {
				arg = (const char *)argv[2];
			}
		}

		if (arg != NULL && tag == Fifo) {
			fifo = arg;
		}
	}

	if (!fifo) {
		fprintf(stderr, "%s: invalid argument: %s\n", argv[0], argv[1]);
		return 1;
	}

	if (!dispatcher_init(&dispatcher, fifo)) {
		fprintf(stderr, "%s: dispatcher init failed\n", argv[0]);
		return 1;
	}

	return dispatcher_run(&dispatcher);
}

const module_t dispatch = {
	"dispatch",
	"Start command dispatcher.",
	callback, module_help
};
