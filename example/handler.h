#ifndef TAPE__EXAMPLE__HANDLER_H__INCLUDED_
#define TAPE__EXAMPLE__HANDLER_H__INCLUDED_

#include <stdbool.h>

#include "list.h"

#define for_each_handler(list, node) list_for_each(list, node, handler_t, hook)

typedef struct handler_s handler_t;

struct handler_s
{
	list_head_t  hook;
	list_head_t  cmd_list;
	unsigned     name_len;
	unsigned     num_args;
	char        *name;
	char        *args[];
};

extern handler_t *
handler_new (char  *name,
             int    argc,
             char **argv);

extern void
handler_delete (handler_t *obj);

extern bool
handler_add_command (handler_t  *obj,
                     char       *name,
                     int         argc,
                     char      **argv);

extern void *
handler_get_command (handler_t *obj,
                     char      *name);

extern bool
handler_del_command (handler_t *obj,
                     char      *name);

#endif /* TAPE__EXAMPLE__HANDLER_H__INCLUDED_ */
