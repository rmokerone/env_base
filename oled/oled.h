/*
 * OLED.c
 *
 * Created: 2013/4/11 21:48:13
 *  Author: MAGI
 */


//M8��PB0-RST,PB1-D/C


# define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include  <avr/pgmspace.h>

unsigned int point;

void SPI_MasterInit(void);
void SPI_MasterTransmit(char cData);
void OLED_Display(char display);
void oled_init();
void oled_clear();
void writestring(char word[]);
void switchpoint(unsigned char x,unsigned char y);
void switchline(unsigned char line);
void oled_set_pos (unsigned char x, unsigned char y);
void OLED_Display(char display);
void OLED_Display_32(unsigned char x, unsigned char y, char num);
void OLED_Display_tmpchar (unsigned char x, unsigned char y);
#define RST PD4
#define DC  PD5
#define OLED_CS  PD6
