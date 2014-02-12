//DS18B20模块主程序
#include "Ds18b20.h"

unsigned char * p;
unsigned char id_buff [8];
unsigned char tmp_buff[9];

unsigned char CrcTable [256]={
0,  94, 188,  226,  97,  63,  221,  131,  194,  156,  126,  32,  163,  253,  31,  65,
157,  195,  33,  127,  252,  162,  64,  30,  95,  1,  227,  189,  62,  96,  130,  220,
35,  125,  159,  193,  66,  28,  254,  160,  225,  191,  93,  3,  128,  222,  60,  98,
190,  224,  2,  92,  223,  129,  99,  61,  124,  34,  192,  158,  29,  67,  161,  255,
70,  24,  250,  164,  39,  121,  155,  197,  132,  218,  56,  102,  229,  187,  89,  7,
219,  133, 103,  57,  186,  228,  6,  88,  25,  71,  165,  251,  120,  38,  196,  154,
101,  59, 217,  135,  4,  90,  184,  230,  167,  249,  27,  69,  198,  152,  122,  36,
248,  166, 68,  26,  153,  199,  37,  123,  58,  100,  134,  216,  91,  5,  231,  185,
140,  210, 48,  110,  237,  179,  81,  15,  78,  16,  242,  172,  47,  113,  147,  205,
17,  79,  173,  243,  112,  46,  204,  146,  211,  141,  111,  49,  178,  236,  14,  80,
175,  241, 19,  77,  206,  144,  114,  44,  109,  51,  209,  143,  12,  82,  176,  238,
50,  108,  142,  208,  83,  13,  239,  177,  240,  174,  76,  18,  145,  207,  45,  115,
202,  148, 118,  40,  171,  245,  23,  73,  8,  86,  180,  234,  105,  55,  213, 139,
87,  9,  235,  181,  54,  104,  138,  212,  149,  203,  41,  119,  244,  170,  72,  22,
233,  183,  85,  11,  136,  214,  52,  106,  43,  117,  151,  201,  74,  20,  246,  168,
116,  42,  200,  150,  21,  75,  169,  247,  182,  232,  10,  84,  215,  137,  107,  53};

unsigned char CRC (unsigned char bit)
{
    unsigned char i, crc_data = 0;
    for (i = 0; i< bit; i ++)
	crc_data = CrcTable [crc_data ^ tmp_buff[i]];
    return (crc_data);
}

char Ds_init (void)
{
    char flag;
    DDRC |= DQ;
    PORTC &= ~(DQ);
    _delay_us (500);
    PORTC |= DQ;    //多次取样 减少误判断
    	_delay_us (80);
     	if ((PINC & DQ) == 0)
	{
	    flag = 1;
	}
   	else
	    flag = 0;
    _delay_us (240);
    PORTC |= DQ;
    return flag;
}

//数据发送函数 先发低位
void Ds_write (unsigned char data)
{
    char i;
    for (i = 0; i < 8; i ++)
    {	
	PORTC &= ~DQ;
	_delay_us (5);
        if (data & 0x01)
	    PORTC |= DQ;
	else
	    PORTC &= ~DQ;
	_delay_us (60);
	PORTC |= DQ;
	_delay_us (10);
	data >>= 1;
     }
}

//数据读取函数 先接低位
//初始版本这里范了两处错误，第一点：data >>= 1;放在了最后
unsigned char Ds_read (void)
{
    unsigned char data = 0, i;
    for (i = 0; i < 8; i ++)
    {
	data >>= 1;
	PORTC &= ~DQ;
	_delay_us (5);
	PORTC |= DQ;
	_delay_us (5);
	if (PINC & DQ)
	    data |= 0x80;
	_delay_us (80);
	PORTC |= DQ;
	//data >>= 1;
    }
    PORTC |= DQ;
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

//配置DS18b20寄存器
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
    Ds_init (); 
    Ds_write (0xcc);
    Ds_write (0xb8);
}

//获取温度
void Ds_get_tmp (void)
{
    Ds_read_bytes (9);
    if (CRC (9) == 0)
    {
    	tmp = ((uint16_t)(tmp_buff[1] << 8) | (tmp_buff[0]));
    	tmp = (tmp >> 4) * 100 + ((tmp << 12) / 6553) * 10;
    }
//    tmp /= 16;
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

