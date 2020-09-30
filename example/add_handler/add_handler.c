#include "add_handler.h"
#include "module_priv.h"

static int
callback (int    argc __attribute__((__unused__)),
          char **argv __attribute__((__unused__)))
{
	return EXIT_SUCCESS;
}

MODULE(
	add_handler,
	"Register a new handler.",
	callback
);
