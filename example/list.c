#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "list.h"

list_node_t *
list_node_new (size_t sizeof_node)
{
	errno = 0;
	list_node_t *node = calloc(sizeof_node, 1);
	if (!node) {
		int err = errno;
		fprintf(stderr, "%s: calloc(%zu, 1): %s\n", __func__, sizeof_node, strerror(err));
		return NULL;
	}
	list_init(&node->head, &node->head, &node->head);
	return node;
}

void
list_node_delete (list_node_t *node)
{
	list_del(&node->head);
	list_init(&node->head, NULL, NULL);
	free(node);
	node = NULL;
}
