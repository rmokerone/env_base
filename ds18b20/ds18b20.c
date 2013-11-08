//DS18B20模块主程序

#define F_CPU 4E6

#include <avr/io.h>
#include <util/delay.h>

#define DQ (1<<PIN0)

unsigned char * p;
unsigned char id_buff [8];
unsigned char tmp_buff[9];
double tmp; //final data;

//DS18B20复位函数 
char Ds_init (void);
void Ds_write (unsigned char data);
unsigned char Ds_read (void);
void Ds_read_bytes (unsigned char i);
void Ds_read_id (void);
void Ds_config (void);
void Ds_get_tmp (void);
void Ds_tmp_result (void);

//DS18B20初始函数 正常返回1 错误返回0
char Ds_init (void)
{
    char flag;
    char i;
    DDRA |= DQ;
    PORTA &= ~(DQ);
    _delay_us (480);
    PORTA |= DQ;
    //多次取样 减少误判断
    for (i = 0; i < 240; i++)
    {
    	_delay_us (1);
     	if ((PINA & DQ) == 0)
	{
	    flag = 1;
	    break;
	}
   	else
	    flag = 0;
    }
    _delay_us (290);
    PORTA |= DQ;
    return flag;
}

//数据发送函数 先发低位
void Ds_write (unsigned char data)
{
    char i;
    for (i = 0; i < 8; i ++)
    {	
	PORTA &= ~DQ;
	_delay_us (15);
	if (data & 0x01)
	    PORTA |= DQ;
	else
	    PORTA &= ~DQ;
	_delay_us (45);
	PORTA |= DQ;
	data >>= 1;
     }
}

//数据读取函数 先接低位
unsigned char Ds_read (void)
{
    unsigned char data, i;
    for (i = 0; i < 8; i ++)
    {
	PORTA &= ~DQ;
	_delay_us (1);
	PORTA |= DQ;
	_delay_us (1);
	if (PINA & DQ)
	    data |= 0x80;
	_delay_us (45);
	data >>= 1;
    }
    PORTA |= DQ;
    return data;
}

//read i bytes
void Ds_read_bytes (unsigned char i)
{
     unsigned char count;
     for (count = 0; count < i; count ++)
     {
	*p = Ds_read ();
	p ++;
     }
}

//Read_ID 
void Ds_read_id (void)
{
    Ds_init ();
    Ds_write (0x33);
    Ds_read_bytes (8);
}	

void Ds_config (void)
{
    Ds_init ();
    Ds_write (0xcc);
    Ds_write (0x4e);
    Ds_write (0x19);
    Ds_write (0x1a);
    Ds_write (0x7f);
    Ds_init ();
    Ds_write (0xcc);
    Ds_write (0x48);
    _delay_ms (6);
    Ds_init (); 
    Ds_write (0xcc);
    Ds_write (0xb8);
}

void Ds_get_tmp (void)
{
    Ds_read_bytes (9);
    tmp = tmp_buff[1] * 0x100 + tmp_buff[0];
    tmp /= 16;
}
    	
void Ds_tmp_result (void)
{
    p = id_buff;
    Ds_read_id ();
    Ds_config ();
    Ds_init ();
    Ds_write (0xcc); //skip rom
    Ds_write (0x44); //Tmperature convert
    
    Ds_init ();
    Ds_write (0xcc); //skip rom 
    Ds_write (0xbe); //read Tmperature 
    p = tmp_buff;
    Ds_get_tmp ();
}

