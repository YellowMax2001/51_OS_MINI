#include "key.h"

//按键结构体
KEY_STRUCT key_struct;

/*****************************************
 *按键检测初始化
 *例:key_init() //初始化按键的状态为未按下 
 *两行，三列六个按键
 *****************************************/
void key_init(void)
{
	key_struct.key1_down = 0;
	key_struct.key2_down = 0;
	key_struct.key3_down = 0;
	key_struct.key4_down = 0;
	key_struct.key5_down = 0;
	key_struct.key6_down = 0;

	key_struct.key_down_flag_turn = 0;
	key_struct.key_up_flag_turn = 0;
}

/*****************************************
 *按键检测
 *例:key_check() //检测一次按键的状态 
 *两行，三列六个按键
 *****************************************/
void key_check(void)
{
	P3 &= ~(1 << 7);	
	P3 |= (1 << 6);	//把按键的第一行设为1，第二行设为0
	_nop_();_nop_();_nop_();_nop_();
	key_struct.key_highcode = (P0 >> 5) ^ 0x07;	//将第一行采集到的键码放入结构体以此来更新结构体 
	P3 &= ~(1 << 6);
	P3 |= (1 << 7);	//把按键的第一行设为0，第二行设为1
	_nop_();_nop_();_nop_();_nop_();
	key_struct.key_lowcode  = (P0 >> 5) ^ 0x07; 	//将第二行采集到的键码放入结构体以此来更新结构体
	//将高低码加行码组合成一个长键码
	key_struct.key_long_code = (((1 << 3) | (key_struct.key_highcode)) << 8) | ((1 << 4) | (key_struct.key_lowcode));
}									    

/*****************************************
 *按键状态输出
 *例:key_stat_out() //输出一次按键的状态 
 *两行，三列六个按键
 *****************************************/
void key_stat_out(void)
{
	if((key_struct.key_highcode & 0x01) && (!key_struct.key4_down))
	{
		uart_send_str("key 4 down!\r\n");
		key_struct.key4_down = 1;
		key_struct.key_down_flag_turn = ~key_struct.key_down_flag_turn; 
	}
	if((key_struct.key_highcode & 0x02) && (!key_struct.key5_down))
	{
		uart_send_str("key 5 down!\r\n");
		key_struct.key5_down = 1;
		key_struct.key_down_flag_turn = ~key_struct.key_down_flag_turn;
	}
	if((key_struct.key_highcode & 0x04) && (!key_struct.key6_down))
	{
		uart_send_str("key 6 down!\r\n");
		key_struct.key6_down = 1;
		key_struct.key_down_flag_turn = ~key_struct.key_down_flag_turn;
	}

	if((1 == key_struct.key4_down) && (!(key_struct.key_highcode & 0x01)))
	{
		uart_send_str("key 4 up!\r\n");
		key_struct.key4_down = 0;
		key_struct.key_up_flag_turn = ~key_struct.key_up_flag_turn;
	}
	if((1 == key_struct.key5_down) && (!(key_struct.key_highcode & 0x02)))
	{
		uart_send_str("key 5 up!\r\n");
		key_struct.key5_down = 0;
		key_struct.key_up_flag_turn = ~key_struct.key_up_flag_turn;
	}
	if((1 == key_struct.key6_down) && (!(key_struct.key_highcode & 0x04)))
	{
		uart_send_str("key 6 up!\r\n");
		key_struct.key6_down = 0;
		key_struct.key_up_flag_turn = ~key_struct.key_up_flag_turn;
	}


	if((key_struct.key_lowcode & 0x01) && (!key_struct.key1_down))
	{
		uart_send_str("key 1 down!\r\n");
		key_struct.key1_down = 1;
		key_struct.key_down_flag_turn = ~key_struct.key_down_flag_turn;
	}
	if((key_struct.key_lowcode & 0x02) && (!key_struct.key2_down))
	{
		uart_send_str("key 2 down!\r\n");
		key_struct.key2_down = 1;
		key_struct.key_down_flag_turn = ~key_struct.key_down_flag_turn;
	}
	if((key_struct.key_lowcode & 0x04) && (!key_struct.key3_down))
	{
		uart_send_str("key 3 down!\r\n");
		key_struct.key3_down = 1;
		key_struct.key_down_flag_turn = ~key_struct.key_down_flag_turn;
	}

	if((1 == key_struct.key1_down) && (!(key_struct.key_lowcode & 0x01)))
	{
		uart_send_str("key 1 up!\r\n");
		key_struct.key1_down = 0;
		key_struct.key_up_flag_turn = ~key_struct.key_up_flag_turn;
	}
	if((1 == key_struct.key2_down) && (!(key_struct.key_lowcode & 0x02)))
	{
		uart_send_str("key 2 up!\r\n");
		key_struct.key2_down = 0;
		key_struct.key_up_flag_turn = ~key_struct.key_up_flag_turn;
	}
	if((1 == key_struct.key3_down) && (!(key_struct.key_lowcode & 0x04)))
	{
		uart_send_str("key 3 up!\r\n");
		key_struct.key3_down = 0;
		key_struct.key_up_flag_turn = ~key_struct.key_up_flag_turn;
	}
}

/*****************
 *以下是使用示例
 *推荐扫描时间间隔为10ms
 *****************/
/*
void main()
{
	uart_bart11(57600); 		//设置串口波特率为57600
	key_init();
	timer0_init11(10000);		//初始化串口0并定时10ms用于检测按键
}

void key_scan() interrupt 1
{
	key_check();				//先进行一次扫描
	key_stat_out();				//状态输出
	timer0_update11(10000);		//装入下一轮需要计时的间隔数
}
*/
















