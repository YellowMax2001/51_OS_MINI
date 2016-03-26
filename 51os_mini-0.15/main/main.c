#include "task.h"

void main(void)
{
	uart_bartvar(24, 115200);
	timer1_init(10000);
	EA = 0;
//	timer0_init(10000);
//	EA = 0;

	OS_init();
	
	task_create(255, (unsigned int)(&idle), 0, "idle");	
	task_create(2, (unsigned int)(&task1), 0, "task1");	
	task_create(1, (unsigned int)(&task_msghandler), 0, "task_msghandler");
	task_create(0, (unsigned int)(&task_tty0), 0, "task_tty0");
	task_create(3, (unsigned int)(&task2), 0, "task2");	

	OS_start();
	
	while(1);
}

