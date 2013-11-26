/*uart.h*/
#define BAUD 9600

#define CRYSTAL 8000000

#define BAUD_SETTING (unsigned int)((unsigned long)CRYSTAL/(16*(unsigned long)BAUD)-1)

void init_uart(unsigned int baud);

int uart_putchar (char c, FILE * stream);

//unsigned char uart_rcvchar (void);
//定义UART接收到的数据类型为volatile (类型限定词）
volatile unsigned char uart_data;

