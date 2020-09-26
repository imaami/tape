#ifndef TAPE__EXAMPLE__LIST_H__INCLUDED_
#define TAPE__EXAMPLE__LIST_H__INCLUDED_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#define list_for_each(list, node, type, member) for (\
	list_head_t *next = (list) ? (list)->next : NULL;\
	({\
		bool ok = (next) && (next != (list));\
		if (ok) {\
			node = list_node(next, type, member);\
			next = next->next;\
		} else {\
			node = NULL;\
		}\
		ok;\
	});\
)

#define LIST_ITER_INITIALIZER(list) {(list),(list)->next}

#define list_iter_next(iter, type, member) ({\
	list_iter_t *iter_ = (iter);\
	type *node_ = NULL;\
	if ((iter_->next) && (iter_->next != iter_->list)) {\
		node_ = list_node(iter_->next, type, member);\
		iter_->next = iter_->next->next;\
	}\
	node_;\
})

#define container_of(ptr, type, member) \
	((type *) ((char *) (ptr) - offsetof(type, member)))

#define list_node(ptr, type, member) container_of(ptr, type, member)

typedef struct list_head_s list_head_t;
typedef struct list_node_s list_node_t;
typedef struct list_iter_s list_iter_t;

extern list_node_t *
list_node_new (size_t sizeof_node);

extern void
list_node_delete (list_node_t *node);

struct list_head_s
{
	list_head_t *next;
	list_head_t *prev;
};

struct list_node_s
{
	list_head_t head;
	uint8_t     data[];
};

struct list_iter_s
{
	list_head_t *list;
	list_head_t *next;
};

__attribute__((__always_inline__))
static inline void
list_init (list_head_t *head,
           list_head_t *next,
           list_head_t *prev)
{
	head->next = next;
	head->prev = prev;
}

__attribute__((__always_inline__, __unused__))
static inline void
list_add (list_head_t *list,
          list_head_t *node)
{
	list_init(node, list, list->prev);
	list->prev->next = node;
	list->prev = node;
}

__attribute__((__always_inline__, __unused__))
static inline void
list_del (list_head_t *node)
{
	node->prev->next = node->next;
	node->next->prev = node->prev;
}

__attribute__((__always_inline__, __unused__))
static inline bool
list_empty (list_head_t *list)
{
	return list->next == list;
}

#endif /* TAPE__EXAMPLE__LIST_H__INCLUDED_ */
