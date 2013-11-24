#include "oled.h"
#include <util/delay.h>

int main(void)
{
    oled_init();
    //OLED_Display('G');
    switchline(0);
    //writestring("Hello,World!");
//    writestring("The Advanced Encryption Standard is a bad ppp");
    while(1)
    {
        //writestring("Hello,World!");

	writestring("1234");
	_delay_ms (1000);
	oled_clear();
        //TODO:: Please write your application code
    }
        return 0;
}



