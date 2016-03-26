#ifndef	__structs_H__
#define __structs_H__

struct list_head
{
	struct list_head *prev;
	struct list_head *next;
};

/* 列出来当前list_head所在的结构体，返回该结构体 */
#define mlist_entry(list_head, type, member_name) \
			(type *)((unsigned int)list_head - (unsigned int)(&(((type*)(0))->member_name)))
/* 列出来所有的双向链表中的list_head结构体，从head开始往下寻找 */
#define mlist_each_entry(pos, head) \
			for(pos = (head)->next; pos != head; pos = pos->next)

extern void mlist_add_tail(struct list_head *new, struct list_head *head);

#endif