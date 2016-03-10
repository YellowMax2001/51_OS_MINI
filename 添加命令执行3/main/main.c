#include "task.h"

unsigned long xdata timer_dis = 0;

void main(void)
{
	uart_bartvar(24, 115200);
//	timer0_init(1000);
	timer1_init(10000);
	EA = 0;
	ENLED = 0;
	P2 = 0x06;
	
	OS_init();

	task_create(3, (unsigned int)(&task0), 0);	
	task_create(2, (unsigned int)(&task1), 0);	
	task_create(1, (unsigned int)(&task_msghandler), 0);
	task_create(0, (unsigned int)(&task_tty0), 0);	

	OS_start();
	
	while(1);
}

void uart() interrupt 4
{
	
}
