#include "del_command.h"
#include "module_priv.h"

static int
callback (int    argc __attribute__((__unused__)),
          char **argv __attribute__((__unused__)))
{
	return EXIT_SUCCESS;
}

MODULE(
	del_command,
	"Remove a registered command.",
	callback
);
