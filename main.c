#include <stdio.h>
#include "ds18b20.h"
#include "nRF24L01.h"
#include "oled.h"
#include "uart.h"

#define TX 1
#define RX 0

#define AP0 0
#define AP1 1

uchar TX_ADDRESS[TX_ADR_WIDTH] = {0X34,0X43,0X10,0X10,0X01};
unsigned char TX_AP0[TX_ADR_WIDTH] =  {0X34,0X43,0X10,0X10,0X02};
static FILE mystdout = FDEV_SETUP_STREAM (uart_putchar, NULL, _FDEV_SETUP_WRITE);

uchar table[10]= {'0','1','2','3','4','5','6','7','8','9'};

void show_tmp(int tmp);
void ifap1(unsigned char ap);
int main (void)
{
     uint16_t tmp_show;
     uchar times=0;
	
	flag = RX;
	//若为第一个节点则AP = 0，第二个节点AP = 1
	ifap1(AP1);

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
	    nrf_tx(TX_ADDRESS);
	}
	else
	{
	    nrf_rx(TX_ADDRESS);
	    tmp_show = (uint16_t)((rx_buf[0] << 8)|(rx_buf[1]));
	    PORTD &= ~(1<<PD6);
	    switchline (2);
	    writestring ("The Temperature is :");
	    show_tmp(tmp_show);
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

void show_tmp (int tmp)
{
    	OLED_Display_32(0,3,tmp/1000);
	OLED_Display_32(16,3,tmp%1000/100);
	OLED_Display_32(32,3,10);
	OLED_Display_32(48,3,tmp%100/10);
	OLED_Display_32(64,3,tmp%10);
	OLED_Display_tmpchar(80,3);
}

void ifap1(unsigned char ap)
{
	unsigned char i;
  	if (ap == 1)
	{
		for (i = 0; i< TX_ADR_WIDTH; i++)
		{
			TX_ADDRESS[i] = TX_AP0[i];
		}
	}
}

