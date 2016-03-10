#ifndef __core_H__
#define __core_H__

#include <stc15f2k60s2.h>

#define TASK_MAX 5 		//可运行的最多任务数
#define SP_DEEP  30		//任务堆栈的深度

#define NUM_PTR_NOPC 13	//不带PC的寄存器所需空间长度



#define ENTER_CRITICAL do{EA = 0; critical_counter ++;}while(0)
#define EXIT_CRITICAL  do{EA = 1; critical_counter --;}while(0)

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

/* 外部变量声明 */
/* 串口接收数据缓冲区，长度为256个字节 */
extern xdata unsigned char reve_buff[256];
/* 统计进入临界栈的次数 */
extern xdata unsigned int critical_counter;
/* 正在运行任务的id，初始化为0，默认最开始运行0号任务 */
extern xdata unsigned char current_id;
/* 申请任务控制模块结构体数组，存储所有任务的控制结构体 */
extern xdata struct task_pcb task_pcb_buf[TASK_MAX];

/* 外部函数声明 */
extern void sp_test(void);
extern unsigned char  task_create(unsigned char priority, unsigned int task_address, unsigned char xdata *task_msg);
extern void task_switch(void);
extern void OS_start(void);
extern void OS_init(void);
extern void OS_delay(unsigned int ms);
extern void task_delay(unsigned char delay);

#endif