#include "common.h"

/*****************************************
 *∫¡√Î—” ±∫Ø ˝
 *¿˝:delay_ms(1000);    —” ±1000∫¡√Î 
 *****************************************/
void delay_ms(unsigned int ms)
{
	unsigned int onetime = 500;

	while(ms --)
	{
		while(onetime --);
		onetime = 500;
	}
}

/*****************************************
 *Œ¢√Î—” ±∫Ø ˝
 *¿˝:delay_us(1000);    —” ±1000Œ¢√Î 
 *****************************************/
void delay_us(unsigned int us)
{
	while(us --)
	{
		_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
		_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
		_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
		
	}
}