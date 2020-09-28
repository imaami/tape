#include "grid.h"
#include "module_priv.h"

static int
callback (int    argc __attribute__((__unused__)),
          char **argv __attribute__((__unused__)))
{
	puts("<grid>");
	return 0;
}

MODULE(
	grid,
	"Draw an ASCII grid.",
	callback
);
