#ifndef __uart_h__
#define __uart_h__

#include <stc15f2k60s2.h>

extern void uart_bartvar(uint crystal, uint32 baud);
extern void uart_send_byte(uchar tx_data);
extern void uart_send_str(uchar* str);
extern void uart_init();
extern uchar uart_reve_byte(void);

#endif