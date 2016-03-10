#include <mode_set.h> 

/*****************************************
 *单片机引脚模式选择，有标准，推挽，开漏等
 *例:com_set(p0,pn2,output); 设置P0^2脚为输出 
 *****************************************/
void com_set(m_com_select p,com_select pn,mode_select mode)
{
	switch(p)
	{
		case p0:
		{
			P0M0 = (P0M0 & (1 << pn)) ? ((P0M0 ^ (1 << pn)) | (((mode << 7) >> 7) << pn)) : (P0M0 | (((mode << 7) >> 7) << pn));		
			P0M1 = (P0M1 & (1 << pn)) ? ((P0M1 ^ (1 << pn)) | ((mode >> 1) << pn)) : (P0M1 | ((mode >> 1) << pn));
		}
		case p1:
		{
			P1M0 = (P1M0 & (1 << pn)) ? ((P1M0 ^ (1 << pn)) | (((mode << 7) >> 7) << pn)) : (P1M0 | (((mode << 7) >> 7) << pn));		
			P1M1 = (P1M1 & (1 << pn)) ? ((P1M1 ^ (1 << pn)) | ((mode >> 1) << pn)) : (P1M1 | ((mode >> 1) << pn));
		}
		case p2:
		{
			P2M0 = (P2M0 & (1 << pn)) ? ((P2M0 ^ (1 << pn)) | (((mode << 7) >> 7) << pn)) : (P2M0 | (((mode << 7) >> 7) << pn));		
			P2M1 = (P2M1 & (1 << pn)) ? ((P2M1 ^ (1 << pn)) | ((mode >> 1) << pn)) : (P2M1 | ((mode >> 1) << pn));
		}
		case p3:
		{
			P3M0 = (P3M0 & (1 << pn)) ? ((P3M0 ^ (1 << pn)) | (((mode << 7) >> 7) << pn)) : (P3M0 | (((mode << 7) >> 7) << pn));		
			P3M1 = (P3M1 & (1 << pn)) ? ((P3M1 ^ (1 << pn)) | ((mode >> 1) << pn)) : (P3M1 | ((mode >> 1) << pn));
		}
		case p4:
		{
			P0M0 = (P4M0 & (1 << pn)) ? ((P4M0 ^ (1 << pn)) | (((mode << 7) >> 7) << pn)) : (P4M0 | (((mode << 7) >> 7) << pn));		
			P0M1 = (P4M1 & (1 << pn)) ? ((P4M1 ^ (1 << pn)) | ((mode >> 1) << pn)) : (P4M1 | ((mode >> 1) << pn));
		}
	}
}