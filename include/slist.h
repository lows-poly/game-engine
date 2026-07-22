#ifndef SLIST_H
#define SLIST_H

#include <stdbool.h>

#include "container_of.h"

#define slist_entry( ptr, type, member ) \
	container_of( ptr, type, member )

#define slist_for_each( list, pos ) \
	for ( pos = (list)->head; pos != NULL; pos = pos->next )

#define slist_for_each_safe( list, pos, n ) \
	for ( pos = (list)->head, n = pos ? pos->next : NULL; \
		pos != NULL; \
		pos = n, n = pos ? pos->next : NULL )

struct slist_node {
	struct node *next;
};

// Entire singly-linked list
struct slist {
	struct slist_node *head;
	struct slist_node *tail;
};

static inline void slist_init( struct slist *list )
{
	list->head = NULL;
	list->tail = NULL;
}

static inline bool slist_empty( struct slist *list )
{
	return list->head == NULL;
}

/*
 * OBJECT-FIRST
 * Add a new node onto the FRONT of the list
 */
static inline void slist_add( struct slist *list, struct slist_node *node )
{
	node->next = list->head;
	list->head = node;

	if ( !list->tail )
		list->tail = node;
}
static inline void slist_add_tail( struct slist *list, struct slist_node *node )
{
	node->next = NULL;

	if ( list->tail )
		list->tail->next = node;
	else
		list->head = node;

	list->tail = node;
}

static inline struct slist_node *slist_del_first( struct slist *list )
{
	struct slist_node *first = list->head;

	if ( !first )
		return NULL;

	list->head = first->next;

	if ( !list->head )
		list->tail = NULL;

	first->next = NULL;

	return first;
}

static inline bool slit_del( struct slist *list, struct slist_node *node )
{
	struct slist_node *entry = list->head;
	struct slist_node *prev = NULL;

	bool out = false;

	while ( entry ) {
		if ( entry == node ) {
			if ( prev )
				prev->next = entry->next;
			else
				list->head = entry->next;

			if ( entry == list->tail )
				list->tail = prev;

			entry->next = NULL;
			out = true;
			break;
		}

		prev = entry;
		entry = entry->next;
	}

	return out;
}

#endif
