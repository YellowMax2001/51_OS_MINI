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
//		delay_ms(80);
//		
//		uart_send_byte('A');
		
	}	
}

void task0(void)
{
	while(1)
	{
//		delay_ms(125);
//		
//		uart_send_byte('C');
	}
}

void task3(void)
{
	static xdata unsigned char reve_buf_len = 0;	//初始化串口接收到的数据串长度为0

	while(1)
	{
		while(1)
		{	
			reve_buff[reve_buf_len] = uart_reve_byte();	//接收到一个字符并存取到接收缓冲区里面
			if(reve_buff[reve_buf_len] == 0)
			{
				task_delay(2);
			}
			else
				break;
		}

		if(reve_buf_len >= 253)
		{
			printf("Too much words\r\n");
			printf("Please double click the 'SPACE' key to end input\r\n");	
		}	
		else
		{
			if(reve_buff[reve_buf_len-1] == ' ' && reve_buff[reve_buf_len] == ' ')
			{
				uart_send_str("\r\nDo commond\r\n");
				reve_buf_len = 0;
			}
			else
				uart_send_byte(reve_buff[reve_buf_len]);		//将刚刚接收到的一个字节并且发送给串口软件显示

			reve_buf_len ++;
		}
	}	
}

void task2(void)
{
	xdata unsigned char i = 1;

	while(1)
	{
		P0 = ~P0;
		OS_delay(2000);		
	}
}

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
	task_create(1, (unsigned int)(&task2), 0);
	task_create(0, (unsigned int)(&task3), 0);	

	OS_start();
	
	while(1);
}

void uart() interrupt 4
{
	
}
