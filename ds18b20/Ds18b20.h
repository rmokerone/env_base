//Ds18b20.h
#ifndef DS18B20_H
#define DS18B20_H

#define F_CPU 8E6
#define DQ (1<<PIN0)

#include <avr/io.h>
#include <util/delay.h>

//DS18B20复位函数 
char Ds_init (void);
//CRC_8校验函数
unsigned char CRC (unsigned char bit);
//数据发送函数 先发低位
void Ds_write (unsigned char data);
//数据读取函数 先接低位
unsigned char Ds_read (void);
//连读i字节
void Ds_read_bytes (unsigned char i);
//读id地址
void Ds_read_id (void);
//配置DS18b20寄存器
void Ds_config (void);
//获取温度
void Ds_get_tmp (void);
//直接进行温度的获取
void Ds_tmp_result (void);
//温度缓存
uint16_t tmp;
#endif
