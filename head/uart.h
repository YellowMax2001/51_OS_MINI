#ifndef __uart_h__
#define __uart_h__

#include <stc12c5a60s2.h>

extern void uart_bart(uint baud);
extern void uart_bart11(uint baud);
extern void uart_send_byte(uchar tx_data);
extern void uart_send_str(uchar* str);

#endif