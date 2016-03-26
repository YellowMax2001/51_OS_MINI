#include "at24c02.h"

static void i2c_delay(void)
{
	unsigned char xdata i;
	for(i = 0; i < 10; i ++)
	{
		_nop_();
	}
}

static void i2c_start(void)
{
	SDA = 1;
//	_nop_(); _nop_(); _nop_(); _nop_();
	SCL = 1;
	i2c_delay();
	SDA = 0;
	i2c_delay();
}

static void i2c_stop(void)
{
	SDA = 0;
//	_nop_(); _nop_(); _nop_(); _nop_();
	SCL = 1;
	i2c_delay();
	SDA = 1;
	i2c_delay();	
}

static void i2c_ack(void)
{
	SCL = 0;
	i2c_delay();
	SCL = 1;
	i2c_delay();
//	while(SDA);
	SCL = 0;
	i2c_delay();
}

static void i2c_write_cmd(unsigned char cmd)
{
	unsigned char xdata i;

	for(i = 0; i < 8; i ++)
	{
		SCL = 0;
//		i2c_delay();
		SDA = cmd >> 7;
		i2c_delay();
		SCL = 1;
		i2c_delay();
		cmd = cmd << 1;
	}
}

static unsigned char i2c_readcmd(void)
{
	unsigned char xdata i;
	unsigned char read_mod = 0;	

	for(i = 0; i < 8; i ++)
	{
		SCL = 0;
		i2c_delay();
		SCL = 1;
		i2c_delay();
		read_mod = read_mod << 1;
		read_mod |= SDA;			
	}

	return read_mod;
}

unsigned char i2c_readbyte(unsigned char addr)
{
	unsigned char read_mod = 0;
	
	i2c_start();
	i2c_write_cmd(WRTIE_ADDR);
	i2c_ack();
	i2c_write_cmd(addr);
	i2c_ack();
	i2c_start();
	i2c_write_cmd(READ_ADDR);
	i2c_ack();

	read_mod = i2c_readcmd();
	i2c_stop();

	return read_mod;
}

unsigned char *i2c_readpage(unsigned char addr, unsigned char len)
{
	unsigned char xdata *read_str;	

	i2c_start();
	i2c_write_cmd(WRTIE_ADDR);
	i2c_ack();
	i2c_write_cmd(addr);
	i2c_ack();
	i2c_start();
	i2c_write_cmd(READ_ADDR);
	i2c_ack();

	while(len --)
	{
		*read_str++ = i2c_readcmd();
		i2c_ack();
	}
	i2c_stop();

	return read_str;
}

void i2c_writebyte(unsigned char addr, unsigned char dat)
{	
	i2c_start();
	i2c_write_cmd(WRTIE_ADDR);
	i2c_ack();
	i2c_write_cmd(addr);
	i2c_ack();
	i2c_write_cmd(dat);
	i2c_ack();
	i2c_stop();	
}

void i2c_writepage(unsigned char addr, unsigned char *dat)
{
	i2c_start();
	i2c_write_cmd(WRTIE_ADDR);
	i2c_ack();
	i2c_write_cmd(addr);
	i2c_ack();
	while(*dat)
	{
		i2c_write_cmd(*dat++);
		i2c_ack();
		i2c_delay();
	}
	i2c_stop();
}