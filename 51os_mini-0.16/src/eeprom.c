#include <eeprom.h>
#include <common.h>

/*****************************************
 * 关闭IAP
 * 例:iap_idle();
 *****************************************/
static void iap_idle(void)
{
	IAP_CONTR = 0;		//关闭IAP功能
	IAP_CMD   = 0;		//清除命令寄存器
	IAP_TRIG  = 0;		//清除触发寄存器
//	IAP_ADDRH = 0x00;	//将地址设置到非IAP区域
//	IAP_ADDRL = 0x00;
}

/*****************************************
 * EEPROM写入数据
 * 例:iap_write(10,'A'); 向地址为10的EEPROM写入数据A
 *****************************************/
void iap_write(uint address, uchar wdata)
{
	IAP_CONTR = 0x01;				//24M设为1
	IAP_CONTR |= 0x80;				//使能
	IAP_CMD   = CMD_WRITE;          //选择命令为‘写命令’
	IAP_ADDRH = address >> 8;		//写地址高位
	IAP_ADDRL = address;		    //写地址低位

	IAP_DATA  = wdata;
	IAP_TRIG=0x5A;IAP_TRIG=0xA5;
	_nop_();_nop_();				//触发操作，写入数据
	iap_idle();		//关闭IAP
}

/*****************************************
 * EEPROM写入长数据
 * 例:iap_write_str(10, "1234567", 7); 向地址为10的EEPROM写入字符串"1234567",写入长度为7
 *****************************************/
void iap_write_str(uint address, uchar wdata, uint len)
{
	IAP_CONTR = 0x01;				//24M设为1
	IAP_CONTR |= 0x80;				//使能
	IAP_CMD   = CMD_WRITE;          //选择命令为‘写命令’
	IAP_ADDRH = address >> 8;		//写地址高位
	IAP_ADDRL = address;		    //写地址低位

	IAP_DATA  = wdata;
	IAP_TRIG=0x5A;IAP_TRIG=0xA5;
	_nop_();_nop_();				//触发操作，写入数据
	iap_idle();		//关闭IAP
}

/*****************************************
 * EEPROM读出数据
 * 例:dat = iap_read(10); 读地址为10的EEPROM存入dat
 *****************************************/
uchar iap_read(uint address)
{
	uchar rdata;

	IAP_CONTR = 0x01;				//24M设为1
	IAP_CONTR |= 0x80;				//使能
	IAP_CMD   = CMD_READ;           //选择命令为‘写命令’
	IAP_ADDRH = address >> 8;		//写地址高位
	IAP_ADDRL = address;		    //写地址低位
	
	IAP_TRIG=0x5A;IAP_TRIG=0xA5;
	_nop_();_nop_();				//触发操作，触发读入数据
	rdata     = IAP_DATA;
	iap_idle();		//关闭IAP

	return rdata;
}

/*****************************************
 * EEPROM擦除数据
 * 例:iap_erase(0); 擦除EEPROM数据
 *****************************************/
void iap_erase(uint address)
{
	IAP_CONTR = 0x01;				//24M设为1
	IAP_CONTR |= 0x80;				//使能
	IAP_CMD   = CMD_ERASE;          //选择命令为‘擦除’
	IAP_ADDRH = address / 256;		//写地址高位
	IAP_ADDRL = address % 256;		//写地址低位
	
	IAP_TRIG=0x5A;IAP_TRIG=0xA5;
	_nop_();_nop_();_nop_();_nop_();				//触发操作，触发擦除数据

	iap_idle();		//关闭IAP
}