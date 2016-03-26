#ifndef __uart_h__
#define __uart_h__

#include <stc15f2k60s2.h>
#include "char.h"

#define putchar(word) uart_send_byte(word)
#define printf(str) uart_send_str(str)

extern void uart_bartvar(uint crystal, uint32 baud);
extern void uart_send_byte(uchar tx_data);
extern unsigned char uart_send_str(uchar* str);
extern void uart_init();
extern uchar uart_reve_byte(void);
extern uchar uart_wait_byte(void);

#endif