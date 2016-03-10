#include <stc15f2k60s2.h>
#include "uart.h"
#include "common.h"
#include "nt_led.h"
#include "vector.h"
#include "eeprom.h"
#include "core.h"

unsigned long xdata timer_dis = 0;

void task1(void)
{
	while(1)
	{
		delay_ms(100);
		
		uart_send_byte('A');
	}
	
}

void task0(void)
{
	while(1)
	{
//		uart_send_byte('A');
//		if(timer_dis >= 10000)
//		{
//			uart_send_byte('B');
//			timer_dis = 0;
//		}
//		uart_send_byte('\r');
//		uart_send_byte('\n');
	}
}

void task2(void)
{
	while(1)
	{
//		uart_send_byte('\r');
//		uart_send_byte('\n');
		delay_ms(150);
		uart_send_byte('B');
	}
}

void main(void)
{
	uart_bartvar(24, 115200);
//	timer0_init(1000);
	timer1_init(10000);
	EA = 0;
	
	OS_init();

	task_create(0, (unsigned int)(&task0), 0);	
	task_create(0, (unsigned int)(&task1), 0);	
	task_create(0, (unsigned int)(&task2), 0);	

	OS_start();
	
	while(1);
}

void uart() interrupt 4
{
	
}
