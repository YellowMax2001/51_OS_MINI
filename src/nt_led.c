#include <nt_led.h>

uchar duan[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};
uchar wei[]  = {0x00,0x01,0x02,0x03,0x04,0x05};

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

	P2 = wei[0]; P0 = duan[num_div[0]];	delay_ms(1);
	P2 = wei[1]; P0 = duan[num_div[1]];	delay_ms(1);
	P2 = wei[2]; P0 = duan[num_div[2]];	delay_ms(1);
	P2 = wei[3]; P0 = duan[num_div[3]];	delay_ms(1);
	P2 = wei[4]; P0 = duan[num_div[4]];	delay_ms(1);
	P2 = wei[5]; P0 = duan[num_div[5]];	delay_ms(1);
}

/*****************************************
 *数码管显示函数，用于中断函数内部 
 *例:nt_disp(32,num_check,cont,num_div) 显示32 
 *****************************************/
void nt_disp_vector(uint num,uint num_check,uchar cont,uchar num_div[])
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