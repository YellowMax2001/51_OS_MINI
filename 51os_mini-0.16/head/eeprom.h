#ifndef __eeprom_h__
#define __eeprom_h__

#include <stc15f2k60s2.h>
#include <intrins.h>
#include "char.h"

#define IAP_EN  IAP_CONTR |= 0x80									 //使能IAP操作
#define LOOP    do{IAP_TRIG=0x5A;IAP_TRIG=0XA5;		 \
				   _nop_();_nop_();}while(0);	     //进行一次操作
#define CLEAR   do{IAP_CMD=0;IAP_TRIG=0;             \	   
				   IAP_ADDRH=0x80;IAP_ADDRL=0;		 \
				  }while(0);                                         //清除所有标志
#define check_iap    (PCON & 0x20)									 //检测电压是否满足条件
#define clear_iap    (PCON &= 0x00)									 //清除不满足标志

#define CMD_READ     0x01                                            //读命令
#define CMD_WRITE    0x02 											 //写命令
#define CMD_ERASE    0x03                                            //擦除命令

extern void  iap_write(uint address, uchar wdata);
extern uchar iap_read(uint address);
extern void iap_erase(uint address);

#endif