#include "common.h"

/*****************************************
 *������ʱ����
 *��:delay_ms(1000);    ��ʱ1000���� 
 *****************************************/
void delay(unsigned int ms)
{
	xdata unsigned int i = 50;

	while(ms --)
	{
		while(i --);
		i = 50;
	}

}

/*****************************************
 *��ʱ10ms����
 *��:delay_10ms();    ��ʱ10���� 
 *****************************************/
void delay_10ms(void)
{

}

/*****************************************
 *΢����ʱ����
 *��:delay_us(1000);    ��ʱ1000΢�� 
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