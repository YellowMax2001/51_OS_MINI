#include <nt_led.h>

code uchar duan[]  = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0xFF};
code uchar wei[]   = {0xF0,0xF1,0xF2,0xF3,0xF4,0xF5};
code uchar l_wei[] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};	   //蓝桥杯板子的位选

code uchar dula_yin[] = {0x3f,0x06,0x5b,0x4f, 0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c, 0x39,0x5e,0x79,0x71};
code uchar wela_yin[] = {0xFE,0xFD,0xFB,0xF7}; 

/*****************************************
 *蓝桥杯板子数码管显示函数 
 *例:lq_disp(32,num_check,num_div) 显示32 
 *****************************************/
void lq_disp(uint32 num, uchar bcd_flag)
{
	static xdata uint32 change_flag = 0;
	static xdata uchar cont = 0;
	static xdata uchar num_div[6];
	xdata unsigned char zero_flag = 0; 

	if(num != change_flag)
	{
		change_flag = num;
		
		if(bcd_flag)
		{
			num_div[0] = num >> 16 & 0x0F;
			num_div[1] = num >> 14 & 0x0F;
			num_div[2] = num >> 12 & 0x0F;
			num_div[3] = num >> 8 & 0x0F;
			num_div[4] = num >> 4 & 0x0F;
			num_div[5] = num & 0x0F;	
		}
		else
		{
			num_div[0] = num / 100000;
			num_div[1] = num % 100000 / 10000;
			num_div[2] = num % 10000  / 1000;
			num_div[3] = num % 1000   / 100;
			num_div[4] = num % 100    / 10;
			num_div[5] = num % 10;
		
			for(zero_flag = 0; zero_flag < 6; zero_flag ++)
			{
				if(num_div[zero_flag] != 0)
				{
					break;
				}
				num_div[zero_flag] = 10;
			}
		}
	}
	if(cont >= 6)
	{
		cont = 0;
	}
	switch(cont)
	{
		case 0: {P0 = l_wei[2]; P2 = (6 << 5); P42 = 0; P42 = 1; P0 = 0xFF; P2 = (7 << 5); P42 = 0; P0 = duan[num_div[0]]; P42 = 1; cont++; break;}
		case 1: {P0 = l_wei[3]; P2 = (6 << 5); P42 = 0; P42 = 1; P0 = 0xFF; P2 = (7 << 5); P42 = 0; P0 = duan[num_div[1]]; P42 = 1; cont++; break;}
		case 2: {P0 = l_wei[4]; P2 = (6 << 5); P42 = 0; P42 = 1; P0 = 0xFF; P2 = (7 << 5); P42 = 0; P0 = duan[num_div[2]]; P42 = 1; cont++; break;}
		case 3: {P0 = l_wei[5]; P2 = (6 << 5); P42 = 0; P42 = 1; P0 = 0xFF; P2 = (7 << 5); P42 = 0; P0 = duan[num_div[3]]; P42 = 1; cont++; break;}
		case 4: {P0 = l_wei[6]; P2 = (6 << 5); P42 = 0; P42 = 1; P0 = 0xFF; P2 = (7 << 5); P42 = 0; P0 = duan[num_div[4]]; P42 = 1; cont++; break;}
		case 5: {P0 = l_wei[7]; P2 = (6 << 5); P42 = 0; P42 = 1; P0 = 0xFF; P2 = (7 << 5); P42 = 0; P0 = duan[num_div[5]]; P42 = 1; cont++; break;}
		default: break;
	}
}

/*****************************************
 *数码管显示函数 
 *例:nt_disp(32,num_check,num_div) 显示32 
 *****************************************/
void nt_disp(uint num,uint num_check,uchar num_div[])
{
	if(num - num_check)
	{
		num_div[0] = num / 100000;
		num_div[1] = num % 100000 / 10000;
		num_div[2] = num % 10000  / 1000;
		num_div[3] = num % 1000   / 100;
		num_div[4] = num % 100    / 10;
		num_div[5] = num % 10;

		num_check = num;
	}

	 P0 = duan[num_div[0]]; P2 |= 0x07; P2 &= wei[0];	delay(1);
	 P0 = duan[num_div[1]]; P2 |= 0x07; P2 &= wei[1];	delay(1);
	 P0 = duan[num_div[2]]; P2 |= 0x07; P2 &= wei[2];	delay(1);
	 P0 = duan[num_div[3]]; P2 |= 0x07; P2 &= wei[3];	delay(1);
	 P0 = duan[num_div[4]]; P2 |= 0x07; P2 &= wei[4];	delay(1);
	 P0 = duan[num_div[5]]; P2 |= 0x07; P2 &= wei[5];	delay(1);
}

/*****************************************
 *数码管显示函数，用于中断函数内部 
 *例:nt_disp(32, cont) 显示32 
 *****************************************/
void nt_disp_vector(uint32 num, uchar cont)
{
	uchar num_div[6];
	
	num_div[0] = num / 100000;
	num_div[1] = num % 100000 / 10000;
	num_div[2] = num % 10000  / 1000;
	num_div[3] = num % 1000   / 100;
	num_div[4] = num % 100    / 10;
	num_div[5] = num % 10;

	switch(cont)
	{
		case 0:	{P2 = wei[0]; P0 = duan[num_div[0]]; break;}
		case 1: {P2 = wei[1]; P0 = duan[num_div[1]]; break;}
		case 2: {P2 = wei[2]; P0 = duan[num_div[2]]; break;}
		case 3: {P2 = wei[3]; P0 = duan[num_div[3]]; break;}
		case 4: {P2 = wei[4]; P0 = duan[num_div[4]]; break;}
		case 5: {P2 = wei[5]; P0 = duan[num_div[5]]; break;}	

		default: break;
	}
}
