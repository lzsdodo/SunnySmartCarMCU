#include "I2C.h"

/*******************************************************************************
* Function Name  : I2C_Delay
****************************************************************************** */
void I2C_Delay(void)
{         
   unsigned char  i=5; //��������Ż��ٶ�     ����������͵�5����д��
   while(i)
   {
     i--;
   } 
}

/*******************************************************************************
* Function Name  : I2C_Start
****************************************************************************** */
void I2C_Start(void)
{
	  SCL=0;
    SDA_out;
    SDA=1;
	  nop();
    SCL=1;
    I2C_Delay();
    SDA=0;
	  I2C_Delay();
    SCL=0;
}

/*******************************************************************************
* Function Name  : I2C_Stop
****************************************************************************** */
void I2C_Stop(void)
{
    //SCL=0;
	  //I2C_Delay(); 
    SDA=0;
    I2C_Delay(); 
    SCL=1;
	  I2C_Delay(); 
    SDA=1;
    I2C_Delay(); 
	  SCL=0;
}

/*******************************************************************************
* Function Name  : I2C_Ack
****************************************************************************** */
void I2C_Ack(void)
{    
    //SCL=0;
	  //nop();
    SDA=0;
    I2C_Delay();
    SCL=1;
    I2C_Delay();
    SCL=0;
}  

/*******************************************************************************
* Function Name  : I2C_NoAck
****************************************************************************** */
void I2C_NoAck(void)
{    
    //SCL=0;
    //nop();
    SDA=1;
    I2C_Delay();
    SCL=1;
    I2C_Delay();
    SCL=0;
}

/*******************************************************************************
* Function Name  : I2C_WaitAck
****************************************************************************** */
void I2C_WaitAck(void)      //����Ϊ:=1��ACK,=0��ACK �ֶ���ACK
{
    //SCL=0;
    //I2C_Delay(); 
    SDA=1;   
    nop();
    SCL=1;
    I2C_Delay();
    SCL=0;
}

/*******************************************************************************
* Function Name  : I2C_Send_Byte
****************************************************************************** */
void I2C_Send_Byte(unsigned char Send_Byte)
{
    unsigned char i;
    for(i=0;i<8;i++)
    {
      SCL=0;
      I2C_Delay();
      if(Send_Byte& 0x80) SDA=1;  
      else SDA=0;   
      Send_Byte<<=1;
      I2C_Delay();
      SCL=1;
      I2C_Delay();
    }
    SCL=0; 
}

/*******************************************************************************
* Function Name  : I2C_Read_Byte
****************************************************************************** */
unsigned char I2C_Read_Byte(unsigned char AckOrNoAck)//Ack=1;NAck=0; //���ݴӸ�λ����λ//
{
    unsigned char i;
    unsigned char Receive_Byte=0;  
    SDA=1; //��������Ϊ���뷽ʽ
    SDA_in;
    for(i=0;i<8;i++)
    {
  		Receive_Byte<<=1;
      SCL=0;
      I2C_Delay();
      SCL=1;
      I2C_Delay();
      if(SDA) Receive_Byte|=0x01;
    }
    SCL=0;
  	SDA_out;
  	if(AckOrNoAck) I2C_Ack();
  	else I2C_NoAck(); 
    return Receive_Byte;
}

/*******************************************************************************
* Function Name  : Single_Write_Byte
****************************************************************************** */
void Single_Write_Byte(unsigned char Slave_Address,unsigned char REG_Address,unsigned char REG_Data)
{
  	Start;
    I2C_Send_Byte(Slave_Address);   //�����豸��ַ+д�ź�//I2C_SendByte(((REG_Address & 0x0700) >>7) | Slave_Address & 0xFFFE);//���ø���ʼ��ַ+������ַ 
	  WaitAck; 
    I2C_Send_Byte(REG_Address);   //���õ���ʼ��ַ
	  WaitAck;
    I2C_Send_Byte(REG_Data); 
	  WaitAck;
    Stop; 
}

/*******************************************************************************
* Function Name  : Single_Read_Byte
****************************************************************************** */
unsigned char Single_Read_Byte(unsigned char Slave_Address,unsigned char REG_Address)
{
    unsigned char REG_Data;     	
    Start;
    I2C_Send_Byte(Slave_Address);
    //I2C_Send_Byte(((REG_Address & 0x0700) >>7) | REG_Address & 0xFFFE);
    //���ø���ʼ��ַ+������ַ 
	  WaitAck;
    I2C_Send_Byte(REG_Address);   //���õ���ʼ��ַ      
	  WaitAck;
    I2C_Start();
    I2C_Send_Byte(Slave_Address+1);
	  WaitAck;
    REG_Data = I2C_Read_Byte(NoAck);
    Stop;
    return REG_Data;
}

/*******************************************************************************
* Function Name  :Multiple_Write_Byte
****************************************************************************** */
/*
void Multiple_Write_Byte(unsigned char Slave_Address,unsigned char REG_Address,unsigned char *REG_Data,unsigned char Num)
{
    unsigned char i;
    I2C_Start();
    I2C_Send_Byte(Slave_Address);   //�����豸��ַ+д�ź�
    //I2C_Send_Byte(((REG_Address & 0x0700) >>7) | Slave_Address & 0xFFFE);//���ø���ʼ��ַ+������ַ
    WaitAck;
    I2C_Send_Byte(REG_Address);   //���õ���ʼ��ַ  
	  WaitAck;
    for(i=0;i<Num;i++)
    {
      I2C_Send_Byte(*REG_Data); //��������*REG_Data
      REG_Data++;
	    WaitAck;
    }
    I2C_Stop();
}
*/

/*******************************************************************************
* Function Name  : Multiple_Read_Byte
****************************************************************************** */

void Multiple_Read_Byte(unsigned char Slave_Address,unsigned char REG_Address,unsigned char *REG_Data,unsigned char Num)
{
    unsigned char i;
    Start;
    nop();
    I2C_Send_Byte(Slave_Address);
	  WaitAck;
    I2C_Send_Byte(REG_Address);   //���õ���ʼ��ַ 
	  WaitAck;
    I2C_Start();
    nop();
    I2C_Send_Byte(Slave_Address+1);
	  WaitAck;
    for(i=0;i<Num-1;i++)
    {
      *REG_Data=I2C_Read_Byte(Ack);
      REG_Data++;
    }
    *REG_Data=I2C_Read_Byte(NoAck); 
    Stop;
}

