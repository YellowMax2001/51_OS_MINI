#include <nt_led.h>

code uchar duan[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};
code uchar wei[]  = {0xF0,0xF1,0xF2,0xF3,0xF4,0xF5};

code uchar dula_yin[] = {0x3f,0x06,0x5b,0x4f, 0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c, 0x39,0x5e,0x79,0x71};
code uchar wela_yin[] = {0xFE,0xFD,0xFB,0xF7}; 

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

	 P0 = duan[num_div[0]]; P2 |= 0x07; P2 &= wei[0];	delay_ms(1);
	 P0 = duan[num_div[1]]; P2 |= 0x07; P2 &= wei[1];	delay_ms(1);
	 P0 = duan[num_div[2]]; P2 |= 0x07; P2 &= wei[2];	delay_ms(1);
	 P0 = duan[num_div[3]]; P2 |= 0x07; P2 &= wei[3];	delay_ms(1);
	 P0 = duan[num_div[4]]; P2 |= 0x07; P2 &= wei[4];	delay_ms(1);
	 P0 = duan[num_div[5]]; P2 |= 0x07; P2 &= wei[5];	delay_ms(1);
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
