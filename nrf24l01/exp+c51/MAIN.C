
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
//初始化cpu
  init_cpu();
//初始话nrf24l01_io口
  init_nrf24l01_io();

  for(i=0;i<32;i++)
  {
    tx_buf[i]=i;
  }
//我记得貌似是开全局中断，记不清楚了
  TI=1;
//uart输出字符
  printf("hello world");
  printf("hello world");
//切换到发送工作模式
  ifnnrf_rx_mode();

  while(1)
  {
//置位key键IO口为高电平
     KEY=1;
//当按键被按下时
     while(KEY==0)
	 {
//？？？？
	   IRQ=1;
	//向STATUS 寄存器写入数据0XFF
	   SPI_RW_Reg(WRITE_REG+STATUS,0xff);
	   count_send++;
	   tx_buf[30]=count_send/256;
	   tx_buf[31]=count_send%256;
	//切换到接收模式
	   ifnnrf_tx_mode();
	//一直到IRQ的电平变成低电平
	   while(IRQ);
	//读状态寄存器中的数值
	   sta=SPI_Read(STATUS);
	//写状态寄存器全1
	   SPI_RW_Reg(WRITE_REG+STATUS,0xff);
	//接收到了ack包？ or 判断发送是否成功？
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
		//清空寄存器
        	ifnnrf_CLERN_ALL();
	   }
	   lock_key=1;

	 }	//发送
	 if(lock_key)
	 {
	 lock_key=0;
	//切换成接收模式
	 ifnnrf_rx_mode();
	 IRQ=1;
	//接收到数据
	 while(IRQ==0);
	 _delay_us(400);
	 }
	 //ifnnrf_rx_mode();  每次都切换到输入 要过一段时间才能正常接收，而对方在一直发送，在发送过来后，这里还没初始化完，于是数据错误
       //ifnnrf_CLERN_ALL();
	 IRQ=1;
	 if(IRQ==0)
	 {
	  
	  sta=SPI_Read(STATUS);
	  SPI_RW_Reg(WRITE_REG+STATUS,0xff);
	 //读状态寄存器，确定是否接收到数据
	   if(sta&STA_MARK_RX)
	   {
	 //读RX_PLOAD寄存器的数值
	     SPI_Read_Buf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH);// read receive payload from RX_FIFO buffer;
	 //重新构建数据结构
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
	 } //接收
	  
  }
}
