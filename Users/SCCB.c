#include "SCCB.h"

//******************************************************************************************
void  SCCB_Wait(void)    //file://延/时，I2c大约30k/s
{
  unsigned char i;

  for(i=0;i<10;i++)
  {
    _asm nop;
  }
}

void SCCB_Start(void)   // file://起/始信号
{
  SCCB_SDA = 1;
  SCCB_SCL = 1;
  SCCB_Wait();
  SCCB_SDA = 0;
  SCCB_Wait();
  SCCB_SCL = 0;
}

void SCCB_Stop(void)    //file://停/止信号
{
  SCCB_SDA_DR=SCCB_OUT;
  SCCB_SDA = 0;
  SCCB_Wait();
  SCCB_SCL = 1;
  SCCB_Wait();
  SCCB_SDA = 1;
  SCCB_Wait();
}

void SCCB_SendAck(unsigned char ack)  //  file://送/确认信号到slave
{
  SCCB_SDA_DR=SCCB_OUT;
  SCCB_SDA = ack;
  SCCB_SCL = 1;
  SCCB_Wait();
  SCCB_SCL = 0;
}

unsigned char SCCB_SendByte(unsigned char bytedata)  //file://送/一字节数据到slave
{
  unsigned char i;
  unsigned char ack;
  SCCB_SDA_DR=SCCB_OUT;
  for(i=0;i<8;i++)
  {
    if(bytedata & 0x80)
      SCCB_SDA = 1;
    else
      SCCB_SDA = 0;

    bytedata <<= 1;
    SCCB_Wait();

    SCCB_SCL = 1;
    SCCB_Wait();
    SCCB_SCL = 0;
    SCCB_Wait();
  }
 
  SCCB_SDA = 1;
  SCCB_SDA_DR=SCCB_IN;
  SCCB_Wait();
  SCCB_SCL = 1;
  SCCB_Wait();

  ack = SCCB_SDA;     // file://接/收确认信号:1,没收到应答；0，收到应答

  SCCB_SCL = 0;
  SCCB_Wait();

  return ack;
}
/*
unsigned char SCCB_ReceiveByte(void)  //file://接/收1字节数据
{
  unsigned char i;
  unsigned char bytedata = 0;
  SCCB_SDA_DR=SCCB_IN;
  for(i=0;i<8;i++)
  {
    SCCB_SCL = 1;
    SCCB_Wait();

    bytedata <<= 1;

    if(SCCB_SDA)
    {
      bytedata |= 0x01;
    }
    SCCB_SCL = 0;
    SCCB_Wait();
  }

  return bytedata;
}

unsigned char SCCB_ByteRead(unsigned char device,unsigned char address)
{      //file://从/slave接收1字节数据
  unsigned char bytedata;

  SCCB_Start();
  SCCB_SendByte(device);
  //I2cSendByte(0);
  SCCB_SendByte(address);
  SCCB_Start();
  SCCB_SendByte(device|0x01);
  bytedata = SCCB_ReceiveByte();   //  add
  SCCB_SendAck(0);                    //  add
  bytedata = SCCB_ReceiveByte();
  SCCB_SendAck(1);
  SCCB_Stop();

  return bytedata;
}
*/

void SCCB_ByteWrite(unsigned char device,unsigned char address,unsigned char bytedata)
{      //file://将/数据写入指定slave的地址内
   unsigned char i;

   unsigned char ack;


   for(i=0;i<20;i++)
   {
     SCCB_Start();
     ack = SCCB_SendByte(device);
     if(ack==1)
     {
       SCCB_Stop();
       continue;
     }
     
     
     ack = SCCB_SendByte(address);
     if(ack==1)
     {
       SCCB_Stop();
       continue;
     }
     
     

     ack = SCCB_SendByte(bytedata);
     if(ack==1)
     {
       SCCB_Stop();
       continue;
     }
     SCCB_Stop();
     if(ack==0)  break;    //file://正/常，跳出循环
   }
}

 //************************************************************************

void SCCB_Init(void)
{
    DDRB_DDRB4 = 1;         //SCCB_SCL
    DDRB_DDRB5 = 1;         //SCCB_SDA
    SCCB_ByteWrite(0x42,0x14,0x24);   //COM5 QVGA 1-320x240; 0-640x480
    SCCB_Wait();
}
  
/*void SCCB_Init(void)
{
  SCCB_SDA_DR=1;
  SCCB_SCL_DR=1;
  //SCCB_ByteWrite(OV7620ID,0x12,0x64);  //镜像设置 缺省值0x24
  //SCCB_ByteWrite(OV7620ID,0x06,0xA0);  //亮度调节 0xff最大 0x00最小
  //SCCB_ByteWrite(OV7620ID,0x03,0x80);  //色饱和度调整―FF "最大，"00h"最小节
  //SCCB_ByteWrite(OV7620ID,0x14,0x24);  //COM5 QVGA 1-320x240; 0-640x480.
  //SCCB_ByteRead(OV7620ID,0x14);          //0x04;0x24 
  SCCB_ByteWrite(0x42,0x14,0x24);
                                          //0x56,0x60
  SCCB_Wait();
}
*/
