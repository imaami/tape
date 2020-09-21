#include "wait.h"
#include <stdio.h>
#include <stdlib.h>

static int
callback (int    argc __attribute__((__unused__)),
          char **argv __attribute__((__unused__)))
{
	puts("<wait>");
	return 0;
}

const module_t wait = { "wait", callback };
