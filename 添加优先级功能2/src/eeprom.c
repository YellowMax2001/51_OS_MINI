#include <eeprom.h>
#include <common.h>

/*****************************************
 * EEPROM写入数据
 * 例:iap_write(10,'A'); 向地址为10的EEPROM写入数据A
 *****************************************/
void iap_write(uint address, uchar wdata)
{
	clear_iap;
	if(!check_iap)						//判断电压是否满足需求
	{
		IAP_CONTR = 0x01;				//24M设为1
		IAP_CONTR |= 0x80;				//使能
		IAP_CMD   = CMD_WRITE;          //选择命令为‘写命令’
		IAP_ADDRH = address >> 8;		//写地址高位
		IAP_ADDRL = address;		    //写地址低位
	
		IAP_DATA  = wdata;
		IAP_TRIG=0x5A;IAP_TRIG=0XA5;
		_nop_();_nop_();				//触发操作，写入数据

	}
}

/*****************************************
 * EEPROM读出数据
 * 例:dat = iap_read(10); 读地址为10的EEPROM存入dat
 *****************************************/
uchar iap_read(uint address)
{
	uchar rdata;

	clear_iap;
	if(!check_iap)		                //判断电压是否满足需求
	{
		IAP_CONTR = 0x01;				//24M设为1
		IAP_CONTR |= 0x80;				//使能
		IAP_CMD   = CMD_READ;           //选择命令为‘写命令’
		IAP_ADDRH = address >> 8;		//写地址高位
		IAP_ADDRL = address;		    //写地址低位
		
		IAP_TRIG=0x5A;IAP_TRIG=0XA5;
		_nop_();_nop_();				//触发操作，触发读入数据
		rdata     = IAP_DATA;

	}

	return rdata;
}

/*****************************************
 * EEPROM擦除数据
 * 例:iap_erase(0); 擦除EEPROM数据
 *****************************************/
void iap_erase(uint address)
{
	clear_iap;
	if(!check_iap)						//判断电压是否满足需求
	{
		IAP_CONTR = 0x01;				//24M设为1
		IAP_CONTR |= 0x80;				//使能
		IAP_CMD   = CMD_ERASE;          //选择命令为‘写命令’
		IAP_ADDRH = address / 256;		//写地址高位
		IAP_ADDRL = address % 256;		//写地址低位
		
		IAP_TRIG=0x5A;IAP_TRIG=0XA5;
		_nop_();_nop_();				//触发操作，触发擦除数据

	}
}