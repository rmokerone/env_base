#include "oled.h"
#include <util/delay.h>

void show_tmp(int tmp);

int main(void)
{
 
   uint16_t tmp = 2130;
   unsigned char ch[]="hello world";
    DDRD |= (1<<PD6);
    PORTD &= (~(1<<PD6));
    oled_init();
//    OLED_Display('G');
//    switchline(0);
    //writestring("Hello,World!");
//    writestring("The Advanced Encryption Standard is a bad ppp");
//    oled_clear();
    while(1)
    {
	show_tmp(tmp);
//    	oled_init();
// 	switchline(0);
//	writestring("fuck the world");
	_delay_ms (1000);
	oled_clear();
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
