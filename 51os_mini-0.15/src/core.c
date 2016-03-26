#include "core.h"

/* 申请任务控制模块结构体数组，存储所有任务的控制结构体 */
struct task_pcb xdata task_pcb_buf[TASK_MAX];

#ifdef XDATA_SP_BUFFER
/* 任务堆栈数组（伪堆栈，实际堆栈只能放在idata里面） */
volatile unsigned char xdata task_sp[TASK_MAX][SP_DEEP];
/* 任务堆栈空间，所有任务共用同一个堆栈空间 */
volatile unsigned char idata task_sp_public[PUBLIC_SP_DEEP];
unsigned char idata task_sp_bottom;	//栈空间的底部
#endif
#ifdef IDATA_SP_BUFFER
/* 任务堆栈数组 */
volatile unsigned char idata task_sp[TASK_MAX][SP_DEEP];
#endif

/* 当前系统正在运行的任务数 */
xdata unsigned char task_running = 0;
/* 正在运行任务的id，初始化为0，默认最开始运行0号任务 */
xdata unsigned char current_id = 0;
/* 任务运行标志，值反映了有多少任务正在运行 */
xdata unsigned long task_run_flag = 0;
/* 统计进入临界栈的次数 */
xdata unsigned int critical_counter = 0;
/* 从开机到现在任务运行时间，单位10ms */
xdata unsigned long task_runtime = 0;
/* 串口接收数据缓冲区，长度为256个字节 */
xdata unsigned char reve_buff[256];
xdata unsigned char cmd_split[10][20];
/* 设置系统运行标志，1为正在运行，0为未开始运行。初始化为0 */
bit task_start_flag = 0;

//void pop_ptr(void)
//{
//	#pragma asm
//		POP 7
//		POP 6
//		POP 5
//		POP 4
//		POP 3
//		POP 2
//		POP 1
//		POP 0
//		POP PSW
//		POP DPH
//		POP DPL
//		POP B
//		POP ACC
//	#pragma endasm
//}

/* 系统延时,单位ms */
void OS_delay(unsigned int ms)
{
	ms = ms / 10;	
	task_delay(ms);

	while(task_pcb_buf[current_id].delay);
}

/* 系统开机初始化 */
void OS_init(void)
{
	unsigned int i = 0;

	current_id = 0;	 		//当前运行任务号为0
	task_run_flag = 0;		//当前无任务运行
	task_runtime = 0; 		//当前运行时间为0
	task_start_flag = 0;	//系统未开始运行
	
	/* 任务运行时间全部清零 */
	for(i = 0; i < TASK_MAX; i ++)
	{
		task_pcb_buf[i].run_time = 0;
		task_pcb_buf[i].delay  = 0;
	}	
}

/* 系统开始任务调度 */
void OS_start(void)
{
#ifdef XDATA_SP_BUFFER
	task_sp_bottom = task_sp_public; 	//将程序实际运行的栈空间起始地址传给栈底暂存变量

	task_sp_public[0] = task_sp[current_id][0];	 //开始进入任务切换中断服务函数的时候把下一步该运行的函数的地址
	task_sp_public[1] = task_sp[current_id][1];	 //高八位与低八位存到栈空间中

	SP = task_sp_public;	//把栈空间起始地址赋值给SP指针
	SP = SP + 1;   			//SP指针加一，指向函数地址的高八位，开始运行（51的栈生长方向是朝着地址增加的方向生长的）
	TR1 = 1;										  //开始计时
	ET1 = 1;										  //使能定时器1
	EA  = 1;										  //开全局中断
#endif
#ifdef IDATA_SP_BUFFER
	task_pcb_buf[current_id].sp -= NUM_PTR_NOPC;

	SP = task_pcb_buf[current_id].sp;
	TR1 = 1;										  //开始计时
	ET1 = 1;										  //使能定时器1
	EA  = 1;										  //开全局中断
#endif
	
}

/* 杀掉一个任务 */
unsigned char task_kill(unsigned char pid)
{
	ENTER_CRITICAL;		//进入临界栈，以免在执行的过程中被中断
	if(task_run_flag & (1 << pid))
	{
		task_run_flag &= ~(1 << pid);	

	}
	else
	{
		goto error;
	}
	task_running --;	//当前系统正在运行的任务数减1
	EXIT_CRITICAL;		//退出临界栈，任务继续运行
	return 0;	
error:		
	EXIT_CRITICAL;		//退出临界栈，任务继续运行
	return 1;
}

/* 任务切换，用在定时器里面进行任务的切换 （暂时弃用）*/
void task_switch(void)
{
	EA= 0;	//任务切换的时候不允许被中断
	/* 保存寄存器值，寄存器值入栈 */
//	push_ptr();
	/* 把转换之前任务的栈指针保存在数组里面 */
	task_pcb_buf[current_id].sp = SP;
//	if(tasks_list

	/* 寄存器值出栈，恢复寄存器值 */
//	pop_ptr();
	EA = 1;	//任务切换完成，重新开启中断
}

/* 创建任务 
 * priority 任务优先级
 * task_address 任务地址
 * task_msg 任务消息指针
 * name 任务的名字
 */
unsigned char task_create(unsigned char priority, unsigned int task_address, unsigned char xdata *task_msg, unsigned char *name)
{
	static xdata unsigned char i = 0;
	static xdata unsigned char stack_deep = 0;
	static unsigned char stack_p;

	ENTER_CRITICAL;	//动态创建任务的时候不允许被中断，此时应该进入临界栈

	for(i = 0; i < TASK_MAX; i ++)
	{
		if(task_run_flag & (1 << i))
			;
		else
		{
#ifdef XDATA_SP_BUFFER
			/* 初始化清空堆栈区 */
			for(stack_deep = 0; stack_deep < SP_DEEP; stack_deep ++)
				task_sp[i][stack_deep] = 0;
			
			task_pcb_buf[i].sp = 14;	//初始化压入栈中数据的长度为14
			task_sp[i][0] = task_address % 256;	  //把任务地址低8位入栈
			task_sp[i][1] = task_address >> 8;	  //把任务地址高8位入栈
			
			task_pcb_buf[i].priority = priority;	//设置任务的优先级
			task_pcb_buf[i].msg		 = task_msg; 	//设置任务的消息指针
			task_pcb_buf[i].id       = name;
			
			task_run_flag |= (1 << i);		//把此位设置为有任务
#endif
#ifdef IDATA_SP_BUFFER
			stack_p = (unsigned char)task_sp[i];	//把堆栈指针的地址赋值堆栈地址暂存区
			/* 初始化清空堆栈区 */
			for(stack_deep = 0; stack_deep < SP_DEEP; stack_deep ++)
				((unsigned char idata *)stack_p)[stack_deep] = 0;
			
			task_pcb_buf[i].sp = stack_p;	//把堆栈指针赋值给任务结构体
			((unsigned char idata *)stack_p)[0] = task_address % 256;	  //把任务地址低8位入栈
			task_pcb_buf[i].sp ++;
			((unsigned char idata *)stack_p)[1] = task_address >> 8;	  //把任务地址高8位入栈
			
			task_pcb_buf[i].sp += NUM_PTR_NOPC;		//将任务堆栈指针值设为任务切换时的状态
			task_pcb_buf[i].priority = priority;	//设置任务的优先级
			task_pcb_buf[i].msg		 = task_msg; 	//设置任务的消息指针
			task_pcb_buf[i].id       = name;
			
			task_run_flag |= (1 << i);		//把此位设置为有任务
#endif
			
			task_running ++;	//当前系统正在运行的任务数加1
			EXIT_CRITICAL;	//任务创建完毕，退出临界栈

			return 0;	//任务创建成功，返回
		}
	}

	EXIT_CRITICAL;		//任务创建失败，也要退出临界栈
	return 1;			//资源不足，无法继续创建任务，返回1
}


/* 中断服务函数，用定时器1进行任务调度 */
void timer1_task_switch(void) interrupt 3
{
	unsigned char xdata temp = 0;
	char sp_count = 0;
	
	EA = 0;	//任务切换的时候要关闭中断

/* 这里的程序不用，因为编译器编译之后的结果是不需要下面这段被注释的语句 */
//	if(!task_start_flag)	//第一次进入中断
//	{
//		SP = SP - NUM_PTR_NOPC;	//此时指针指向调用此中断时候压入的PC低地址
//		for(temp = 1; temp < NUM_PTR_NOPC + 1; temp ++)
//		{
//			((unsigned char idata *)SP)[temp] = ((unsigned char idata *)SP)[temp + 2];	//将除了PC之外的寄存器值往下平移两位
//		}
//		SP = SP + NUM_PTR_NOPC;
//		task_start_flag = 1; //设置系统运行标志，此时为已经运行
//	}

	/* 保存寄存器值，寄存器值入栈 */
//	push_ptr();	 /* 在现有编译环境下寄存器已经由系统自动保存了 */
	
	TF1 = 0;	//清除中断标志
#ifdef XDATA_SP_BUFFER
	/* 将程序实际运行的栈空间参数值存到xdata区 */
	for(sp_count = 0; SP >= task_sp_bottom; SP--)
	{
		task_sp[current_id][sp_count] = ((unsigned char idata*)task_sp_bottom)[sp_count];
		sp_count ++;
	}
	sp_count --;	//for循环退出减一
	/* 把转换之前任务的栈指针保存在其任务结构体里面 */
	task_pcb_buf[current_id].sp = sp_count;
#endif
#ifdef IDATA_SP_BUFFER
	/* 把转换之前任务的栈指针保存在其任务结构体里面 */
	task_pcb_buf[current_id].sp = SP;
#endif
#if 0
	/* 第一版，任务轮流被调用，查找本次任务之后的任务，有的话直接跳到 T2_switch_exit 处执行*/
	for(temp = current_id + 1; temp < TASK_MAX; temp ++)
	{
		if(task_run_flag & (1 << temp))
		{
			current_id = temp;
			goto T2_switch_exit;			
		}
	}

	/* 没有找到正在运行任务之后的任务，返回前面进行查找 */
	for(temp = 0; temp < current_id + 1; temp ++)
	{
		if(task_run_flag & (1 << temp))
		{
			current_id = temp;
			goto T2_switch_exit;	
		}
		else
			goto T2_switch_exit;
	}
#endif
	//将最低优先级的id赋值给正在运行的id号
	current_id = 0;
	//任务id的切换
	for(temp = 1; temp < TASK_MAX; temp ++)
	{
		if(task_run_flag & (1 << temp))
		{
			if(task_pcb_buf[temp].priority < task_pcb_buf[current_id].priority)
			{
				if(task_pcb_buf[temp].delay != 0)
				{
					task_pcb_buf[temp].delay --;			
				}
				else if(task_pcb_buf[temp].suspend == 1)
					continue;
				else
				{
					current_id = temp;		
					task_pcb_buf[current_id].run_time ++;	//任务可以切换，并且该任务的执行时间加一，用来统计CPU使用率
				}		
			}
		}
	}

	/* 如果任务0有延时的话，将任务0的延时减一（一般用不到） */
	if(task_pcb_buf[0].delay != 0)
	{
		task_pcb_buf[0].delay --;			
	}
#ifdef XDATA_SP_BUFFER
	SP ++; 	//上个for循环出来的时候SP的值多减了一次
	sp_count = task_pcb_buf[current_id].sp;		//将上次记录的sp_count的值再次赋值给sp_count
	/* 将xdata区域存储的栈参数重新赋值给实际运行的栈空间 */
	for(; sp_count >= 0; SP++)
	{
		((unsigned char idata*)task_sp_bottom)[sp_count] = task_sp[current_id][sp_count];
		sp_count --;
	}
	SP --;	//for循环退出的时候需要减去一个数
	task_pcb_buf[current_id].new_circle = 1;	//任务又一次到达
#endif
#ifdef IDATA_SP_BUFFER
	SP = task_pcb_buf[current_id].sp;	//把要运行的任务SP地址传入SP寄存器
	task_pcb_buf[current_id].new_circle = 1;	//任务又一次到达
#endif
	task_runtime ++;	//系统运行时间加1

	EA = 1;	//任务切换完成，重新开启中断
}

/* 系统任务延时 */
void task_delay(unsigned char delay)
{
	task_pcb_buf[current_id].delay = delay;
}

/* 系统新周期开始判断 */
unsigned char get_task_newcircle_flag(void)
{
	if(task_pcb_buf[current_id].new_circle == 1)	//是新周期的开始
	{
		task_pcb_buf[current_id].new_circle = 0;	//清除开始标志并返回1
		return 1;
	}
	return 0;
}

/* 打印机资源的使用 */

/* 堆栈的测试 */
void sp_test(void)
{
	SP = task_sp;

}
//	#pragma asm
//		PUSH DPL
//		PUSH DPH
//	#pragma endasm

//	void push_ptr(void)
//{
//	#pragma asm
//		PUSH ACC
//		PUSH B
//		PUSH DPL
//		PUSH DPH
//		PUSH PSW
//		PUSH 0
//		PUSH 1
//		PUSH 2
//		PUSH 3
//		PUSH 4
//		PUSH 5
//		PUSH 6
//		PUSH 7
//	#pragma endasm
//}
//

