#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dispatch/dispatch.h"
#include "die/die.h"
#include "wait/wait.h"

#include "module_list.h"
#include "module_iter.h"

static MODULE_LIST( modules,
	&dispatch,
	&die,
	&wait
);

const module_t *
module_list_find (const char *name)
{
	if (!name || !*name) {
		return NULL;
	}

	module_iter_t  iter;
	const module_t *ptr;

	for_each_module (ptr, &iter, modules) {
		if (!strcmp(name, ptr->name)) {
			break;
		}
	}

	return ptr;
}

void
module_list_help (void)
{
	int max;
	module_iter_t iter;
	const module_t *ptr;

	max = 0;

	for_each_module (ptr, &iter, modules) {
		int n = (int)strlen(ptr->name);
		if (n > max) {
			max = n;
		}
	}

	for_each_module (ptr, &iter, modules) {
		printf("\n %-*s [OPTIONS]...\t%s\n\n", max, ptr->name, ptr->what);
		if (ptr->help) {
			ptr->help("   ");
		}
	}

	putchar('\n');
}
