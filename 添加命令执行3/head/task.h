#ifndef	__task_H__
#define __task_H__

#define TASK_MSGHANDLER 2	//宏定义task_msghandler任务号为2

#include <stc15f2k60s2.h>
#include "uart.h"
#include "common.h"
#include "nt_led.h"
#include "vector.h"
#include "eeprom.h"
#include "core.h"
#include "char.h"

extern void task0(void);
extern void task1(void);
extern void task_msghandler(void);
extern void task_tty0(void);

#endif