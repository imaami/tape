#include "handler.h"

handler_t *
handler_new (char  *name,
             int    argc,
             char **argv)
{
	// TODO
	return NULL;
}

void
handler_delete (handler_t *obj)
{
	// TODO
}

bool
handler_add_command (handler_t  *obj,
                     char       *name,
                     int         argc,
                     char      **argv)
{
	// TODO
	return false;
}

void *
handler_get_command (handler_t *obj,
                     char      *name)
{
	// TODO
	return NULL;
}

bool
handler_del_command (handler_t *obj,
                     char      *name)
{
	// TODO
	return false;
}
