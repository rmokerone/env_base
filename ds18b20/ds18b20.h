//ds18b20.h

#define F_CPU 8E6
#define DQ (1<<PIN0)

#include <avr/io.h>
#include <util/delay.h>

//DS18B20复位函数 
char Ds_init (void);
void Ds_write (unsigned char data);
unsigned char Ds_read (void);
void Ds_read_bytes (unsigned char i);
void Ds_read_id (void);
void Ds_config (void);
void Ds_get_tmp (void);
void Ds_tmp_result (void);

float tmp;
