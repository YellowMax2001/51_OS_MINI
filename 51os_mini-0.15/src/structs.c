#include "structs.h"

static void _list_add_tail(struct list_head *new, struct list_head *prev, struct list_head *next)
{
	new->next = next;
	new->prev = prev;
	prev->next = new;
	next->prev = new;
}

void mlist_add_tail(struct list_head *new, struct list_head *head)
{
	_list_add_tail(new, head->prev, head);
}