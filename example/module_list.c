#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dispatch/dispatch.h"
#include "add_handler/add_handler.h"
#include "add_command/add_command.h"
#include "call/call.h"
#include "del_command/del_command.h"
#include "del_handler/del_handler.h"
#include "die/die.h"
#include "run/run.h"

#include "module_list.h"
#include "module_iter.h"

static MODULE_LIST( modules,
	&dispatch,
	&add_handler,
	&add_command,
	&call,
	&del_command,
	&del_handler,
	&die,
	&run
);

const module_t *
module_list_find_cond (const char    *name,
                       module_flag_t  mask,
                       module_flag_t  cond)
{
	if (!name || !*name) {
		return NULL;
	}

	module_iter_t  iter;
	const module_t *ptr;

	for_each_module (ptr, &iter, modules) {
		if ((ptr->flags & mask) == cond
		    && !strcmp(name, ptr->name)) {
			break;
		}
	}

	return ptr;
}

void
module_list_help_cond (module_flag_t mask,
                       module_flag_t cond)
{
	int max;
	module_iter_t iter;
	const module_t *ptr;

	max = 0;

	for_each_module (ptr, &iter, modules) {
		if ((ptr->flags & mask) == cond) {
			int n = (int)strlen(ptr->name);
			if (n > max) {
				max = n;
			}
		}
	}

	if (max == 0) {
		return;
	}

	for_each_module (ptr, &iter, modules) {
		if ((ptr->flags & mask) == cond) {
			printf("\n %-*s [OPTIONS]...\t%s\n\n", max, ptr->name, ptr->what);
			if (ptr->help) {
				ptr->help("   ");
			}
		}
	}

	putchar('\n');
}
