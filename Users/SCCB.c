#include "SCCB.h"

//******************************************************************************************
void  SCCB_Wait(void)    //file://��/ʱ��I2c��Լ30k/s
{
  unsigned char i;

  for(i=0;i<10;i++)
  {
    _asm nop;
  }
}

void SCCB_Start(void)   // file://��/ʼ�ź�
{
  SCCB_SDA = 1;
  SCCB_SCL = 1;
  SCCB_Wait();
  SCCB_SDA = 0;
  SCCB_Wait();
  SCCB_SCL = 0;
}

void SCCB_Stop(void)    //file://ͣ/ֹ�ź�
{
  SCCB_SDA_DR=SCCB_OUT;
  SCCB_SDA = 0;
  SCCB_Wait();
  SCCB_SCL = 1;
  SCCB_Wait();
  SCCB_SDA = 1;
  SCCB_Wait();
}

void SCCB_SendAck(unsigned char ack)  //  file://��/ȷ���źŵ�slave
{
  SCCB_SDA_DR=SCCB_OUT;
  SCCB_SDA = ack;
  SCCB_SCL = 1;
  SCCB_Wait();
  SCCB_SCL = 0;
}

unsigned char SCCB_SendByte(unsigned char bytedata)  //file://��/һ�ֽ����ݵ�slave
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

  ack = SCCB_SDA;     // file://��/��ȷ���ź�:1,û�յ�Ӧ��0���յ�Ӧ��

  SCCB_SCL = 0;
  SCCB_Wait();

  return ack;
}
/*
unsigned char SCCB_ReceiveByte(void)  //file://��/��1�ֽ�����
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
{      //file://��/slave����1�ֽ�����
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
{      //file://��/����д��ָ��slave�ĵ�ַ��
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
     if(ack==0)  break;    //file://��/��������ѭ��
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
  //SCCB_ByteWrite(OV7620ID,0x12,0x64);  //�������� ȱʡֵ0x24
  //SCCB_ByteWrite(OV7620ID,0x06,0xA0);  //���ȵ��� 0xff��� 0x00��С
  //SCCB_ByteWrite(OV7620ID,0x03,0x80);  //ɫ���Ͷȵ����DFF "���"00h"��С��
  //SCCB_ByteWrite(OV7620ID,0x14,0x24);  //COM5 QVGA 1-320x240; 0-640x480.
  //SCCB_ByteRead(OV7620ID,0x14);          //0x04;0x24 
  SCCB_ByteWrite(0x42,0x14,0x24);
                                          //0x56,0x60
  SCCB_Wait();
}
*/
