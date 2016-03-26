#include "lcd1602.h"

static void lcd1602_delay(void)
{
	unsigned char xdata i;
	for(i = 0; i < 30; i ++)
	{
		_nop_();
	}
}

static void lcd1602_write_cmd(unsigned char cmd)
{
	EN = 0;
	RS = 0;
	RW = 0;
	_nop_();

	EN = 1;
	P0 = cmd;
	lcd1602_delay(); lcd1602_delay(); lcd1602_delay();
	EN = 0;
	lcd1602_delay();
}

static void lcd1602_write_dat(unsigned char dat)
{
	EN = 0;
	RS = 1;
	RW = 0;
	_nop_(); 

	EN = 1;
	P0 = dat;
	lcd1602_delay(); lcd1602_delay(); lcd1602_delay();
	EN = 0;
	lcd1602_delay();	
}

void lcd1602_clr(void)
{
	lcd1602_write_cmd(CLEAR_SCR);
}

void lcd1602_init(void)
{
	lcd1602_write_cmd(NORMAL_DIS);
	lcd1602_write_cmd(NORMAL_DIS);
	lcd1602_delay(); lcd1602_delay(); lcd1602_delay();
	lcd1602_write_cmd(NORMAL_DIS);
	lcd1602_write_cmd(0x06);
	lcd1602_write_cmd(CURR_SCRN);
	lcd1602_write_cmd(DIS_CUR_FIC);
	lcd1602_write_cmd(CLEAR_SCR);

}

void lcd1602_dis(unsigned char addr, unsigned char dat)
{
	lcd1602_write_cmd(addr);
	lcd1602_write_dat(dat);
}

void lcd1602_dis_str(unsigned char addr, unsigned char *str)
{
	while(*str)
	{
		if(addr == 0x80 + 16)
		{
			addr = 0x80 + 0x40;
		}
		lcd1602_write_cmd(addr++);
		lcd1602_write_dat(*str++); 
	}
}