#ifndef __vector_h__
#define __vector_h__

#include <stc15f2k60s2.h>
#include "char.h"

extern void timer1_init(uint time_us);
extern void timer1_update(uint time_us);
extern void timer0_init(uint time_us);
extern void timer0_update(uint time_us);
extern void timer1_init11(uint time_us);
extern void timer1_update11(uint time_us);
extern void timer0_init11(uint time_us);
extern void timer0_update11(uint time_us);

#endif