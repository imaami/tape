#include "wait.h"
#include <stdio.h>
#include <stdlib.h>

#include "module_priv.h"

static int
callback (int    argc __attribute__((__unused__)),
          char **argv __attribute__((__unused__)))
{
	puts("<wait>");
	return 0;
}

MODULE(
	wait,
	"Sleep for a specified amount of time.",
	callback
);
