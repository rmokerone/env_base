#include <stdio.h>
#include "ds18b20.h"
#include "nRF24L01.h"
#include "oled.h"
#include "uart.h"

#define TX 1
#define RX 0

static FILE mystdout = FDEV_SETUP_STREAM (uart_putchar, NULL, _FDEV_SETUP_WRITE);

uchar table[10]= {'0','1','2','3','4','5','6','7','8','9'};

int main (void)
{
     flag = TX;
     uint16_t tmp_show;
     uchar times=0;
     init_uart(BAUD_SETTING);
     stdout = &mystdout;
     SPI_Init();
     DDRD  |= (1<<PD6); 
     PORTD &= ~(1<<PD6);	
     oled_init();
//     PORTD |= (1<<PD6);	
     while (1)
     {
	times ++;
	PORTD &= ~(1<<PD6);	
	oled_clear();
	switchline (1);
	writestring ("UESC env_base");
	PORTD |= (1<<PD6);
	if (flag == TX)
	{
	    Ds_tmp_result();
		printf ("the tmp is %d\n",tmp);
	    tx_buf[0] = (tmp >> 8);
	    tx_buf[1] = tmp;
	    nrf_tx();
	}
	else
	{
	    nrf_rx();
	    tmp_show = (uint16_t)((rx_buf[0] << 8)|(rx_buf[1]));
	    PORTD &= ~(1<<PD6);
	    switchline (2);
	    writestring ("The Temperature is :");
	    switchline (3);
	    OLED_Display(table[tmp_show/1000]);
	    OLED_Display(table[tmp_show%1000/100]);
	    OLED_Display('.');
	    OLED_Display(table[tmp_show%100/10]);
	    OLED_Display(table[tmp_show%10]);
	    OLED_Display('C');
	    PORTD |= (1<<PD6);
	  
	    printf ("the rcv data is %d\n", tmp_show);
	}
	_delay_ms (1000);
	if (times %200 == 0)
	{
	  PORTD &= ~(1<<PD6);	
     	  oled_init();
	}

//	oled_clear();
//	printf ("I'm fine\n");
	
     }
     return 0;
}
