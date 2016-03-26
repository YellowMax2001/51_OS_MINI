#include <uart.h>

/*****************************************
 *独立波特率发生器串口初始化
 *例:uart_bartvar(24, 115200);  24M晶振初始化串口波特率115200 
 *可指定晶振的频率
 *****************************************/
void uart_bartvar(uint crystal, uint32 baud)
{
	SCON |= 0x50;          //REN置位为1，允许接收，SM1 = 0；SM0 = 1；

	AUXR |= 0x05;		   //定时器2作为串口的波特率发生器
	//为定时器2赋值，进行波特率的产生
	baud = baud / 100;
	T2H = (65536 - 250000 / baud * crystal / 100) / 256;
	T2L = (65536 - 250000 / baud * crystal / 100) % 256;

	AUXR |= (1 << 4);      //允许定时器2运行(开启独立波特率发生器)

	EA = 1;	               //开全局中断
//	ES = 1;				   //开串口中断
}

/*****************************************
 *独立波特率发生器串口初始化
 *例:uart_init();  默认初始化串口波特率115200 
 *24M晶振专用
 *****************************************/
void uart_init()
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR |= 0x04;		//定时器2时钟为Fosc,即1T
	T2L = 0xCC;		//设定定时初值
	T2H = 0xFF;		//设定定时初值
	AUXR |= 0x10;		//启动定时器2
}

/*****************************************
 *串口发送一个字节
 *例:uart_send_byte('A');  发送字符A
 *****************************************/
void uart_send_byte(uchar tx_data)
{
	EA = 0;
	SBUF = tx_data;
	while(!TI);
	TI = 0;
	EA = 1;
}

/*****************************************
 *串口发送一个字符串并返回字串的长度
 *例:uart_send_str("www.yellowmax.top");  发送字符串www.yellowmax.top
 *****************************************/
unsigned char uart_send_str(uchar* str)
{
	xdata uint8 i = 0;
	
	while(*str)
	{
		uart_send_byte(*str ++);
		i ++;
	}

	return i;
}

/*****************************************
 *串口接收一个字节
 *例:receive_byte = uart_reve_byte(); 接收一个字节到receive_byte里面  
 *****************************************/
uchar uart_reve_byte(void)
{
	if(RI)
	{
		RI = 0;
		return SBUF;
	}
	return 0;
}

/*****************************************
 *串口等待接收一个字节
 *例:receive_byte = uart_wait_byte(); 接收一个字节到receive_byte里面  
 *****************************************/
uchar uart_wait_byte(void)
{
	while(!RI);
	RI = 0;

	return SBUF;
}
