#ifndef __core_H__
#define __core_H__

#include <stc15f2k60s2.h>

#define TASK_MAX 5 		//可运行的最多任务数
#define SP_DEEP  27		//任务堆栈的深度

#define NUM_PTR_NOPC 13	//不带PC的寄存器所需空间长度

/* 任务进程管理 */
struct task_pcb
{
	unsigned char *id;		//任务的id，也就是任务的名字
	unsigned char priority;	//任务的优先级
	unsigned char suspend;	//任务挂起标志
	unsigned char delay;	//任务延时标志
	unsigned char sp;		//任务的堆栈指针
	unsigned char xdata *msg;//任务的消息
};

/* 任务链表，将任务写入一个链表然后进行轮询 */
struct task_list
{
	struct task_pcd *task_pcb;	//任务进程
	struct task_list *next;	//列表下一项
};
//struct task_list *tasks_list;	//定义任务链表，在主函数里面进行初始化


extern void sp_test(void);
extern void task_create(unsigned char priority, unsigned int task_address, unsigned char task_msg);
extern void task_switch(void);
extern void OS_start(void);
extern void OS_init(void);

#endif