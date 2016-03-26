#ifndef __lcd1602_H__
#define __lcd1602_H__

#include <stc15f2k60s2.h>
#include <intrins.h>

#define RS P20
#define RW P21
#define EN P12

/* 写入指令 */
#define CLEAR_SCR 	0x01	//清屏
#define RESET_CUR 	0x02	//光标复位

#define CURR_SCRN	0x14	//写入数据的时候光标右移，屏幕不移动
#define CURL_SCRN	0x10	//写入数据的时候光标左移，屏幕不移动
#define CURN_SCRR	0x1C	//写入数据的时候光标不移动，屏幕右移
#define CURN_SCRL	0x18	//写入数据的时候光标不移动，屏幕左移
/* 0 0 0 0 1 D C B
 * D	1 显示开		0 显示关
 * C	1 有光标		0 无光标
 * B	1 光标闪烁		0 光标不闪烁
 */
#define DIS_CUR_FIC	0x0F	//显示开，光标开，光标闪烁
/* 0 0 0 1 S/C R/L x x
 * S/C	1 全部字符	0 只光标
 * R/L	1 右移		0 左移
 */

/* 0 0 1 DL N F x x
 * DL	0 数据4位	1 数据8位
 * N 	0 显示一行 	1 显示两行
 * F	0 5*7点阵	1 5*10点阵
 */
#define	NORMAL_DIS	0x38	//八位数据，显示两行，5*7点阵

/* 0 1 CGRAM_ADDRESS(6bits)
 * 设定CGRAM的地址
 * 543 字符号
 * 210 行号
 */

/* 0 1 DDRAM_ADDRESS(6bits)
 * 设定DDRAM的地址
 */


/* 读取指令 */
/* BF AC(7bits)
 * SF = 1时为忙
 */


extern void lcd1602_init(void);
extern void lcd1602_clr(void);
extern void lcd1602_dis(unsigned char addr, unsigned char dat);
extern void lcd1602_dis_str(unsigned char addr, unsigned char *str);

#endif