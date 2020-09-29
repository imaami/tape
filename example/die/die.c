#define BUILTIN_MODULE

#include "die.h"
#include "module_priv.h"

static int
callback (int    argc __attribute__((__unused__)),
          char **argv __attribute__((__unused__)))
{
	exit(0);
	return 0;
}

MODULE(
	die,
	"Die.",
	callback
);
