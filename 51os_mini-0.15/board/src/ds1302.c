#include "ds1302.h"

unsigned char code ds1302_timer_init[] = {0x00,0x30,0x12, 0x08, 0x10, 0x02, 0x13};

static void ds1302_delay(unsigned char period)
{
	xdata unsigned char i = 0;

	for(i = 0; i < period; i++)
	{
		_nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_();
		_nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_();
		_nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_();
	}
}


static unsigned char read_byte(unsigned char addr)
{
	xdata unsigned char i = 0;
	RESET = 1;
 
	for(i = 0; i < 8; i ++)
	{
		IO = addr & 0x01;
		_nop_(); _nop_();
		SCLK = 1;
		SCLK = 0;
		addr = addr >> 1;		
	}

	for(i = 0; i < 8; i ++)
	{
		addr = addr >> 1;
		if(IO == 1)
			addr = addr | 0x80;
		SCLK = 1;	
		SCLK = 0;				
	}
	RESET = 0; 

	return addr;
}

static void write_byte(unsigned char addr, unsigned char dat)
{
	xdata unsigned char i = 0;
	RESET = 1;
 
	for(i = 0; i < 8; i ++)
	{
		IO = addr & 0x01;
		SCLK = 1;
		SCLK = 0;
		addr = addr >> 1;
	}

	for(i = 0; i < 8; i ++)
	{
		IO = dat & 0x01;
		SCLK = 1;
		SCLK = 0;
		dat = dat >> 1;
	}
	RESET = 0;
}

void ds1302_init(void)
{
	xdata unsigned char i;
	
	RESET = 0;
	SCLK  = 0;
	
	write_byte(0x8E, 0x00);
	for(i = 0; i < 7; i ++)
	{
		write_byte(0x80 + i * 2, ds1302_timer_init[i]);
	}
}

void get_time(unsigned char *time)
{	
	xdata unsigned char i;

	for(i = 0; i < 7; i ++)
	{
		write_byte(0x8E, 0x00);
		time[i] = read_byte(0x81 + i * 2);
	}
}