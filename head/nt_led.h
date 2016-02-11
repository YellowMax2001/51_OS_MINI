#ifndef __nt_led_h__
#define __nt_led_h__

#include <stc12c5a60s2.h>
#include <intrins.h>
#include <common.h>

sbit ENLED = P1^1;

extern void nt_disp(uint num,uint num_check,uchar num_div[]);
extern void nt_disp_vector(uint num,uint num_check,uchar cont,uchar num_div[]);

#endif