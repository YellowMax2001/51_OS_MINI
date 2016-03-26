#ifndef __core_H__
#define __core_H__

#include <stc15f2k60s2.h>
#include "common.h"

#define TASK_MAX 5 			//可运行的最多任务数
#define SP_DEEP  30			//任务堆栈的深度
#define PUBLIC_SP_DEEP 30	//任务的公共堆栈空间深度

#define NUM_PTR_NOPC 13	//不带PC的寄存器所需空间长度

#define ENTER_CRITICAL do{EA = 0; critical_counter ++;}while(0)	//进入临界状态，动态切换任务，动态创建任务的时候需要
#define EXIT_CRITICAL  do{EA = 1; critical_counter --;}while(0)	//退出临界状态。
#define SUSPEND_TASK(taskid)   do{task_pcb_buf[taskid].suspend = 1;}while(0)	//挂起任务
#define DESUSPEND_TASK(taskid) do{task_pcb_buf[taskid].suspend = 0;}while(0)	//任务解挂起

/* 任务进程管理 */
struct task_pcb
{
	unsigned char *id;		//任务的id，也就是任务的名字
	unsigned char new_circle;	//任务新周期的开始，也就是上次退出任务之后又一次进入任务的标志
	unsigned char priority;	//任务的优先级
	unsigned char suspend;	//任务挂起标志
	unsigned char delay;	//任务延时标志
	unsigned char sp;		//任务的堆栈指针
	unsigned long run_time;	//任务运行的时间
	unsigned char *msg;//任务的消息
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
extern xdata unsigned char cmd_split[10][20];
/* 统计进入临界栈的次数 */
extern xdata unsigned int critical_counter;
/* 正在运行任务的id，初始化为0，默认最开始运行0号任务 */
extern xdata unsigned char current_id;
/* 申请任务控制模块结构体数组，存储所有任务的控制结构体 */
extern xdata struct task_pcb task_pcb_buf[TASK_MAX];
/* 任务运行标志，值反映了有多少任务正在运行 */
extern xdata unsigned long task_run_flag;
/* 当前系统正在运行的任务数 */
extern xdata unsigned char task_running;
/* 从开机到现在任务运行时间，单位10ms */
extern xdata unsigned long task_runtime;

/* 外部函数声明 */
extern void sp_test(void);
extern unsigned char task_kill(unsigned char pid);
extern unsigned char task_create(unsigned char priority, unsigned int task_address, unsigned char xdata *task_msg, unsigned char *name);
extern void task_switch(void);
extern void OS_start(void);
extern void OS_init(void);
extern void OS_delay(unsigned int ms);
extern void task_delay(unsigned char delay);
extern unsigned char get_task_newcircle_flag(void);

#endif