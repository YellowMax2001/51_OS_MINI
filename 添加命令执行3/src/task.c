#include "task.h"

/* 任务字串解析标志,解析完成标志置0,正在解析标志置1 */
xdata unsigned char parse_commond_flag = 0;

/* 等待命令解析 */
void wait_parse(void)
{
	parse_commond_flag = 1;

	while(parse_commond_flag)
	{
		task_delay(2);	//阻塞，等待一个周期再来查询命令是否解析完毕
	}

}

void task1(void)
{
	xdata unsigned char i = 1;

	P0 = 0x01;
	OS_delay(1000);
	while(1)
	{
		if(P0 == 0x80)
		{
			P0 = 0x01;
		}
		else
			P0 = P0 << 1;
		OS_delay(50);		
	}
		
}

void task0(void)
{
	while(1)
	{

	}
}

/* 串口接收字符串进行回显，并把字符串放入一个缓冲区，之后发送命令消息给消息处理函数并解析执行 */
void task_tty0(void)
{
	static xdata unsigned int reve_buf_len = 0;	//初始化串口接收到的数据串长度为0

	printf("#");   	//开机打印出提示符

	while(1)
	{
		while(1)
		{	
			reve_buff[reve_buf_len] = uart_reve_byte();	//接收到一个字符并存取到接收缓冲区里面
			if(reve_buff[reve_buf_len] == 0) 			//如果没有接收到字串，继续查询
			{
				task_delay(2);							//延时两个任务周期，以便其他任务继续执行
			}
			else
				break;
		}

		if(reve_buf_len >= 254)
		{
			
			printf("\r\nToo much words\r\n");		//用户输入的字符数量过多
			printf("Please input your commond again and dont't input too much\r\n#");		
//			clear_char_str(reve_buff, 256);		//清除整个数组，其实清除不清除并不影响，此处暂且注释掉
			reve_buf_len = 0;
		}	
		else
		{
			if(reve_buff[reve_buf_len] == '\r')	  	//接收到换行符
			{
				reve_buff[reve_buf_len] = '\0';		//将当前的换行字符替换为字符串结束符用来比较
				DESUSPEND_TASK(TASK_MSGHANDLER);	//消息处理函数解挂起
				wait_parse();						//等待命令解析完成
//				clear_char_str(reve_buff, 256);		//清除整个数组，其实清除不清除并不影响，此处暂且注释掉
				reve_buf_len = 0;					//字串的索引置0以便重新接受输入
			}
			else
			{
				uart_send_byte(reve_buff[reve_buf_len]);	//将刚刚接收到的一个字节并且发送给串口软件显示
				reve_buf_len ++;							//字串索引值加1
			}
			
		}
	}	
}

/* 消息处理函数，对串口接收到的命令进行相应的处理 */
void task_msghandler(void)
{
	while(1)
	{
		uart_send_str("\r\n");
		if(!os_strcmp("test", reve_buff))
		{
			uart_send_str("You do a 'test' commond");
		}
		else if(!os_strcmp("kill", reve_buff))
		{
			uart_send_str("You do a 'kill' commond, please input the pid number");
		}
		else
		{
			uart_send_str(reve_buff);
			uart_send_str(": undefined commond");
		}
		
		uart_send_str("\r\n#");	//发送提示符
		parse_commond_flag = 0;	//任务解析完毕
		SUSPEND_TASK(TASK_MSGHANDLER);	//自挂起，应该先挂起再等待
		OS_delay(10);			//等待一个任务周期，避免多次执行到任务	
	}
}