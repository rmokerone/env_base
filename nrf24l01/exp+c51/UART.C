#include "STC5A60S2.H"
#include <intrins.h>
#include <string.h>
void _delay_us(unsigned int _us)
{
  char a=0;
  for(_us;_us;_us--)
   for(a=0;a<1;a++);
}

void _delay_ms(unsigned int _ms)
{
  for(_ms;_ms;_ms--)
  {
    _delay_us(1000);
  }
}

//初始化CPU


void init_cpu(void)
{

//相关寄存器的设置
//大部分已经记不清楚了
  SCON=0X50;
  BRT= 250;
  AUXR=0X15;
  AUXR1=0X80;
  
  //ES=1;


  S2CON=0X50;
  IE2=0X01;//ES2=1

  EA=1;
 // GsmType = GSM_TYPE;
    
}


void rec_from_s2()interrupt 8
{
  unsigned char k=0;
  unsigned char rcv,isSave;

  k=S2CON;
  k=k&0x01;
  if(k==0x01)
  {
   isSave = 1;
   rcv = S2BUF;
   if(isSave == 1)
    {
      ;
    }
   S2CON=S2CON&0XFE;
  }
  else
  {
   S2CON=S2CON&0XFD;
  }
}



void TxData (unsigned char JK)
{	ES=0;
    TI=0;
	SBUF=JK;
	while(TI==0);
	TI=0;
	ES=1;
}


void com_printf(unsigned char *pc,unsigned ass)
{

  char temp_str[10];
  char s [40];
  char *ptr=s;

  temp_str[0]=(ass/10000)+0x30;
  temp_str[1]=((ass%10000)/1000)+0x30;
  temp_str[2]=((ass%1000)/100)+0x30;
  temp_str[3]=((ass%100)/10)+0x30;
  temp_str[4]=((ass%10)/1)+0x30;
  temp_str[5]=0;

  strcpy (s, pc);

  strcat (s, temp_str);

  strcat (s, "\n");

  ES=0;
  while(*ptr)
  {
    TI=0;
	SBUF=*ptr;
	while(TI==0);
	TI=0;
	ptr++;
  }
  ES=1;
    
}

void Wait(unsigned int t)
{
   unsigned int tt;
	tt = t;
    while(tt >= 30 )
    {
        _delay_ms(30);
        tt -= 30 ;
    }
    if(tt > 0) _delay_ms(tt);
}


