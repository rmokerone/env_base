
#include"config.h"
#include"stdio.h"
sbit led=P0^0;
/*
unsigned char xdata  re_send[100];
void send_image(unsigned char *pic,unsigned char len)
{
  unsigned int send_k=0;
  unsigned char temp1,temp2,temp3,a=0;
  unsigned char mode_rec=0;
  init_nrf24l01_io();
  while(1)
  {
  ifnnrf_rx_mode();
  IRQ=1;
  SPI_RW_Reg(WRITE_REG+STATUS,0xff);
  tx_buf[0]=0xaa;
  tx_buf[1]=0x55;
  tx_buf[2]=0xaa;
  tx_buf[3]=0x55;
  tx_buf[4]=0x01;
  tx_buf[5]=(len/256);
  tx_buf[6]=(len%256);
  ifnnrf_tx_mode();
  while(IRQ);
  sta=SPI_Read(STATUS);
  SPI_RW_Reg(WRITE_REG+STATUS,0xff);
  if(sta&STA_MARK_TX)
   {
	com_printf("TX OK!",sta);
    break;
   }
  else
   {
	com_printf("TX ER!",sta);
    ifnnrf_CLERN_ALL();
   }
  }
  while(1)
  {
  ifnnrf_rx_mode();
  ifnnrf_CLERN_ALL();
  while(IRQ==0);
  _delay_us(1000);
  if(IRQ==0)
   {
	sta=SPI_Read(STATUS);
	SPI_RW_Reg(WRITE_REG+STATUS,0xff);
	if(sta&STA_MARK_RX)
	  {
	   SPI_Read_Buf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH);// read receive payload from RX_FIFO buffer;
       if((rx_buf[0]==0xaa)&&(rx_buf[1]==0x55)&&(rx_buf[2]==0xaa)&&(rx_buf[3]==0x55)&&(rx_buf[0]==0x01))
       break;
	  }
    } 
   }
   while(1)
   {
    IRQ=1;
    SPI_RW_Reg(WRITE_REG+STATUS,0xff);
    tx_buf[0]=(send_k/256);
    tx_buf[1]=(send_k%256);
	for(a=0;a<30;a++)
	tx_buf[a+2]=*(pic+send_k*30+a);
    ifnnrf_tx_mode();
    while(IRQ);
	if(send_k*30<len)
	send_k++;
	else
	break;
   }
    while(1)
    {
	  ifnnrf_rx_mode();
	  ifnnrf_CLERN_ALL();
	  while(IRQ==0);
	  _delay_us(1000);
	  if(IRQ==0)
	   {
		sta=SPI_Read(STATUS);
		SPI_RW_Reg(WRITE_REG+STATUS,0xff);
		if(sta&STA_MARK_RX)
		  {
		   SPI_Read_Buf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH);// read receive payload from RX_FIFO buffer;
	       if((rx_buf[0]==0xaa)&&(rx_buf[1]==0x55)&&(rx_buf[2]==0xaa)&&(rx_buf[3]==0x55)&&(rx_buf[4]==0x02))
		   {
           mode_rec=rx_buf[4];
	       break;
		   }
		  }
	    } 
     }

	while(temp2<mode_rec)
    {
	  ifnnrf_rx_mode();
	  ifnnrf_CLERN_ALL();
	  while(IRQ==0);
	  _delay_us(1000);
	  if(IRQ==0)
	   {
		sta=SPI_Read(STATUS);
		SPI_RW_Reg(WRITE_REG+STATUS,0xff);
		if(sta&STA_MARK_RX)
		  {
		   SPI_Read_Buf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH);// read receive payload from RX_FIFO buffer;
           for(temp1=0;temp1<30;temp1++)
		   re_send[temp2+temp1]=rx_buf[temp1];
		  }
	    } 
     }
   while(temp3<mode_rec)
   {
   
   while(1)
   {
    IRQ=1;
    SPI_RW_Reg(WRITE_REG+STATUS,0xff);
    tx_buf[0]=0;
    tx_buf[1]=temp3;
	for(a=0;a<30;a++)
	tx_buf[a+2]=*(pic+re_send[temp3]*30+a);
    ifnnrf_tx_mode();
    while(IRQ);
	if(send_k*30<len)
	send_k++;
	else
	break;
   }
   temp3++;
   }
 
       
}	
*/  
main()
{
  unsigned char i=0;

  unsigned int count_send=0;
  unsigned int rec_new=0;
  unsigned int rec_old=0;
  unsigned int rec_err=0;
  unsigned int lock_key=0;
//��ʼ��cpu
  init_cpu();
//��ʼ��nrf24l01_io��
  init_nrf24l01_io();

  for(i=0;i<32;i++)
  {
    tx_buf[i]=i;
  }
//�Ҽǵ�ò���ǿ�ȫ���жϣ��ǲ������
  TI=1;
//uart����ַ�
  printf("hello world");
  printf("hello world");
//�л������͹���ģʽ
  ifnnrf_rx_mode();

  while(1)
  {
//��λkey��IO��Ϊ�ߵ�ƽ
     KEY=1;
//������������ʱ
     while(KEY==0)
	 {
//��������
	   IRQ=1;
	//��STATUS �Ĵ���д������0XFF
	   SPI_RW_Reg(WRITE_REG+STATUS,0xff);
	   count_send++;
	   tx_buf[30]=count_send/256;
	   tx_buf[31]=count_send%256;
	//�л�������ģʽ
	   ifnnrf_tx_mode();
	//һֱ��IRQ�ĵ�ƽ��ɵ͵�ƽ
	   while(IRQ);
	//��״̬�Ĵ����е���ֵ
	   sta=SPI_Read(STATUS);
	//д״̬�Ĵ���ȫ1
	   SPI_RW_Reg(WRITE_REG+STATUS,0xff);
	//���յ���ack���� or �жϷ����Ƿ�ɹ���
	   if(sta&STA_MARK_TX)
	   {
               if((count_send%100)==0)
		{
		   TI=1;
        	   printf("TX OK!%d\n\t",count_send);
		}
	   }
	   else
	   {
		TI=1;
        	printf("TX ER!%2XH\n\t",sta);
		//��ռĴ���
        	ifnnrf_CLERN_ALL();
	   }
	   lock_key=1;

	 }	//����
	 if(lock_key)
	 {
	 lock_key=0;
	//�л��ɽ���ģʽ
	 ifnnrf_rx_mode();
	 IRQ=1;
	//���յ�����
	 while(IRQ==0);
	 _delay_us(400);
	 }
	 //ifnnrf_rx_mode();  ÿ�ζ��л������� Ҫ��һ��ʱ������������գ����Է���һֱ���ͣ��ڷ��͹��������ﻹû��ʼ���꣬�������ݴ���
       //ifnnrf_CLERN_ALL();
	 IRQ=1;
	 if(IRQ==0)
	 {
	  
	  sta=SPI_Read(STATUS);
	  SPI_RW_Reg(WRITE_REG+STATUS,0xff);
	 //��״̬�Ĵ�����ȷ���Ƿ���յ�����
	   if(sta&STA_MARK_RX)
	   {
	 //��RX_PLOAD�Ĵ�������ֵ
	     SPI_Read_Buf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH);// read receive payload from RX_FIFO buffer;
	 //���¹������ݽṹ
	     rec_new=(rx_buf[30]*256+rx_buf[31]);
		 if(rec_new==(rec_old+1)) 
		 {
		    P2=0XFF;
			if((rec_new%100)==0)
			{
			TI=1;
	        printf("RX OK!%d\n\t",rec_new);
			}
		    rec_old=rec_new;
		    P2=0X00;
		 }

		 else
		 {
		    rec_err+=1;
		    rec_old=rec_new;
			TI=1;
	        printf("RX ER!%d\n\t",rec_err);
		 }
	   }
	   else
	   {
	     ifnnrf_CLERN_ALL();
	     ifnnrf_rx_mode();
	     IRQ=1;
	     while(IRQ==0);
	   }
	 } //����
	  
  }
}