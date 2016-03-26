#ifndef __nt_led_h__
#define __nt_led_h__

#include <stc15f2k60s2.h>
#include <intrins.h>
#include "char.h"

sbit ENLED = P1^1;

/*****************************************
	led灯的脚位定义
******************************************/
#define LED_1 1
#define LED_2 2
#define LED_3 3
#define LED_4 4
#define LED_5 5
#define LED_6 6
#define LED_7 7
#define LED_8 8
#define LED_9 9
#define LED_10 10
#define LED_11 11
#define LED_12 12
#define LED_ALL 13

#define LED_ON 0
#define LED_OFF 1

sbit led1 = P0^0;
sbit led2 = P0^1;
sbit led3 = P0^2;
sbit led4 = P0^3;
sbit led5 = P0^4;
sbit led6 = P0^5;
sbit led7 = P0^6;
sbit led8 = P0^7;

/*****************************************/

#define LEDn_JUMP(n,st) (led##n = st)
#define LEDn_CTRL(m,stat)	(LEDn_JUMP(m,stat))

#define LED_ALLOFF do{P2 |= 0xFF;P3 |= 0x3C;}while(0)
#define LED_ALLON do{P2 = 0;P3 &= ~0x3C;}while(0)

extern void lq_disp(uint32 num, uchar bcd_flag);
extern void nt_disp_vector(uint32, uchar cont);

#endif