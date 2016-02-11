#include "common.h"
#include "uart.h"

void main()
{
	uart_bart(38400);

	while(1)
	{
		uart_send_str("First step of 51 operation!\n");
		delay_ms(500);
	}
}