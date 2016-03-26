#ifndef __at24c02_H__
#define __at24c02_H__

#include <stc15f2k60s2.h>
#include <intrins.h>

#define SDA P44
#define SCL P42
#define WRTIE_ADDR 0xA2
#define READ_ADDR  0xA3

extern unsigned char i2c_readbyte(unsigned char addr);
extern void i2c_writebyte(unsigned char addr, unsigned char dat);
extern unsigned char *i2c_readpage(unsigned char addr, unsigned char len);
extern void i2c_writepage(unsigned char addr, unsigned char *dat);

#endif