#ifndef __key_h__
#define __key_h__

#include <stc10f08xe.h>
#include <intrins.h>
#include <uart.h>

typedef struct
{
	uchar	key_highcode;
	uchar	key_lowcode;
	uint	key_long_code;

	uchar	key1_down;
	uchar	key2_down;
	uchar	key3_down;
	uchar	key4_down;
	uchar	key5_down;
	uchar	key6_down;

	uchar   key_down_flag_turn;		//设置一个按键按下的标志，每次按下的时候都会翻转，以此来检测任意一个按键的按下并进行相关动作
	uchar   key_up_flag_turn;		//设置一个按键松开的标志，每次松开的时候都会翻转，以此来检测任意一个按键的松开并进行相关动作

}KEY_STRUCT;

extern KEY_STRUCT key_struct;

extern void key_check(void);
extern void key_stat_out(void);
extern void key_init(void);

#endif