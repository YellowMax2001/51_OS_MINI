#ifndef __interrupt_h__
#define __interrupt_h__

#include <stc12c5a60s2.h>

typedef enum
{
	zero,
	first,
	second,
	three,
}inrupt_priority;

typedef enum
{
	PCA,
	LVD,
	ADC,
	ES1,
	IIT1,
	IEX1,
	IIT0,
	IEX0,
	SPI,
	ES2,
}intrupt_select;

#endif