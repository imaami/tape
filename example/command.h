#ifndef TAPE__EXAMPLE__COMMAND_H__INCLUDED_
#define TAPE__EXAMPLE__COMMAND_H__INCLUDED_

#include <stdbool.h>

#include "list.h"

#define for_each_command(list, node) list_for_each(list, node, command_t, hook)

typedef struct command_s command_t;

struct command_s
{
};

#endif /* TAPE__EXAMPLE__COMMAND_H__INCLUDED_ */
