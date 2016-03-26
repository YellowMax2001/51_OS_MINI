#include "hc_sr04.h"

static void sr04_delay(unsigned int period)
{
	xdata unsigned int i;

	for(i = 0; i < period; i ++)
	{
		_nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_();
		_nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_();
	}
}

static void sr04_timer_init(void)
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0;		//设置定时初值
	TH0 = 0;		//设置定时初值
	TF0 = 0;		//清除TF0标志
}

unsigned int get_distance(void)
{
	xdata unsigned char i;

	sr04_timer_init();
	for(i = 0; i < 8; i ++)
	{
		HC_TX = 1;
		sr04_delay(2);
		HC_TX = 0;
		sr04_delay(2);
	}
	TR0 = 1;		//定时器0开始计时
	while((HC_RX == 1) && (TF0 == 0));
	TR0 = 0;		//关闭计时
	if(TF0 == 1)
	{
		TF0 = 0;
		return 255;
	}
	return((TH0 * 256 + TL0) * 17 / 1000);
}