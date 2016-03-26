#include <interrupt_ctrl.h>

/*****************************************
 *优先级设定(可设优先级有四个等级)
 *例: set_priority(IIT0,three); 设定IT0优先级3，最高优先级
 *由于 IT0已经定义过，所以改用IIT0 
 *****************************************/
void set_priority(intrupt_select is,inrupt_priority ip)
{
	switch(is)
	{
		case PCA:{switch(ip){
					case zero:   {IPH = (IPH & 0x80) ? (IPH ^ 0x80) : IPH; PPCA = 0; break;}
					case first:  {IPH = (IPH & 0x80) ? (IPH ^ 0x80) : IPH; PPCA = 1; break;}
					case second: {IPH = (IPH | 0x80);    PPCA = 0; break;}
					case three:  {IPH = (IPH | 0x80);    PPCA = 1; break;}
					default: break;}
				 }
		case LVD:{switch(ip){
					case zero:   {IPH = (IPH & 0x40) ? (IPH ^ 0x40) : IPH; PLVD = 0; break;}
					case first:  {IPH = (IPH & 0x40) ? (IPH ^ 0x40) : IPH; PLVD = 1; break;}
					case second: {IPH = (IPH | 0x40);    PLVD = 0; break;}
					case three:  {IPH = (IPH | 0x40);    PLVD = 1; break;}
					default: break;}
				 }
		case ADC:{switch(ip){
					case zero:   {IPH = (IPH & 0x20) ? (IPH ^ 0x20) : IPH; PADC = 0; break;}
					case first:  {IPH = (IPH & 0x20) ? (IPH ^ 0x20) : IPH; PADC = 1; break;}
					case second: {IPH = (IPH | 0x20);    PADC = 0; break;}
					case three:  {IPH = (IPH | 0x20);    PADC = 1; break;}
					default: break;}
				 }
		case ES1:{switch(ip){
					case zero:   {IPH = (IPH & 0x10) ? (IPH ^ 0x10) : IPH; PS = 0; break;}
					case first:  {IPH = (IPH & 0x10) ? (IPH ^ 0x10) : IPH; PS = 1; break;}
					case second: {IPH = (IPH | 0x10);    PS = 0; break;}
					case three:  {IPH = (IPH | 0x10);    PS = 1; break;}
					default: break;}
				 }
		case IIT1:{switch(ip){
					case zero:   {IPH = (IPH & 0x08) ? (IPH ^ 0x08) : IPH; PT1 = 0; break;}
					case first:  {IPH = (IPH & 0x08) ? (IPH ^ 0x08) : IPH; PT1 = 1; break;}
					case second: {IPH = (IPH | 0x08);    PT1 = 0; break;}
					case three:  {IPH = (IPH | 0x08);    PT1 = 1; break;}
					default: break;}
				 }
		case IEX1:{switch(ip){
					case zero:   {IPH = (IPH & 0x04) ? (IPH ^ 0x04) : IPH; PX1 = 0; break;}
					case first:  {IPH = (IPH & 0x04) ? (IPH ^ 0x04) : IPH; PX1 = 1; break;}
					case second: {IPH = (IPH | 0x04);    PX1 = 0; break;}
					case three:  {IPH = (IPH | 0x04);    PX1 = 1; break;}
					default: break;}
				 }
		case IIT0:{switch(ip){
					case zero:   {IPH = (IPH & 0x02) ? (IPH ^ 0x02) : IPH; PT0 = 0; break;}
					case first:  {IPH = (IPH & 0x02) ? (IPH ^ 0x02) : IPH; PT0 = 1; break;}
					case second: {IPH = (IPH | 0x02);    PT0 = 0; break;}
					case three:  {IPH = (IPH | 0x02);    PT0 = 1; break;}
					default: break;}
		    	 }
		case IEX0:{switch(ip){
					case zero:   {IPH = (IPH & 0x01) ? (IPH ^ 0x01) : IPH; PX0 = 0; break;}
					case first:  {IPH = (IPH & 0x01) ? (IPH ^ 0x01) : IPH; PX0 = 1; break;}
					case second: {IPH = (IPH | 0x01);    PX0 = 0; break;}
					case three:  {IPH = (IPH | 0x01);    PX0 = 1; break;}
					default: break;}
				 }
		case SPI:{switch(ip){
					case zero:   {IP2H = (IP2H & 0x02) ? (IP2H ^ 0x02) : IP2H; IP2 = (IP2 & 0x02) ? (IP2 ^ 0x02) : IP2; break;}
					case first:  {IP2H = (IP2H & 0x02) ? (IP2H ^ 0x02) : IP2H; IP2 = (IP2 | 0x02); break;}
					case second: {IP2H = (IP2H | 0x02);    IP2 = (IP2 & 0x02) ? (IP2 ^ 0x02) : IP2; break;}
					case three:  {IP2H = (IP2H | 0x02);    IP2 = (IP2 | 0x02); break;}
					default: break;}
				 }
		case ES2:{switch(ip){
					case zero:   {IP2H = (IP2H & 0x01) ? (IPH ^ 0x01) : IP2H; IP2 = (IP2 & 0x01) ? (IP2 ^ 0x01) : IP2; break;}
					case first:  {IP2H = (IP2H & 0x01) ? (IPH ^ 0x01) : IP2H; IP2 = (IP2 | 0x01); break;}
					case second: {IP2H = (IP2H | 0x01);    IP2 = (IP2 & 0x01) ? (IP2 ^ 0x01) : IP2; break;}
					case three:  {IP2H = (IP2H | 0x01);    IP2 = (IP2 | 0x01); break;}
					default: break;}
				 }		 		 		
				 
		default: break;		 
	}
}