#ifndef __NRF24L01__
#define __NRF24L01__
extern void _delay_us(unsigned int _us);
extern void _delay_ms(unsigned int _ms);
extern init_cpu(void);
extern void com_printf(unsigned char *pc,unsigned ass);
extern void uart0_init(unsigned long baud);
#endif