#define BUILTIN_MODULE

#include "run.h"
#include "module_priv.h"

#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

static int
callback (int    argc __attribute__((__unused__)),
          char **argv)
{
#if 0
	fprintf(stderr, "%s:%d: argc=%d, argv={\"%s\"", __FILE__, __LINE__, argc, argv[0]);
	for (int i = 1; i < argc; ++i) {
		fprintf(stderr, ", \"%s\"", argv[i]);
	}
	fprintf(stderr, "}\n");
#endif
	pid_t pid;

	errno = 0;
	pid = fork();

	if (pid == -1) {
		fprintf(stderr, "fork: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}

	if (pid == 0) {
		fprintf(stderr, "%s:%d: execv(\"%s\", {\"%s\"", __FILE__, __LINE__, argv[1], argv[1]);
		for (int i = 2; i < argc; ++i) {
			fprintf(stderr, ", \"%s\"", argv[i]);
		}
		fprintf(stderr, "})\n");
		errno = 0;
		if (execv(argv[1], argv+1) == -1) {
			int err = errno;
			fprintf(stderr, "%s:%d: execv(\"%s\", {\"%s\"", __FILE__, __LINE__, argv[1], argv[1]);
			for (int i = 2; i < argc; ++i) {
				fprintf(stderr, ", \"%s\"", argv[i]);
			}
			fprintf(stderr, "}): %s\n", strerror(err));
			exit(err);
		}
	}

	return EXIT_SUCCESS;
}

MODULE(
	run,
	"Run an external command.",
	callback
);
