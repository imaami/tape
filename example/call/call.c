#include "call.h"
#include <stdio.h>
#include <stdlib.h>

#include "module_priv.h"

static int
callback (int    argc __attribute__((__unused__)),
          char **argv __attribute__((__unused__)))
{
	return EXIT_SUCCESS;
}

MODULE(
	call,
	"Call a command.",
	callback
);
