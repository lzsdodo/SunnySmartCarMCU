#ifndef _I2C_H_
#define _I2C_H_

#include "IO_Map.h"
/*
#define SCL			PORTB_PB4 //IIC时钟线定义	//初始化为输出
#define SDA			PORTB_PB5 //IIC数据线定义
#define SCL_DR	DDRB_DDRB4
#define SDA_DR	DDRB_DDRB5
#define SDA_in		DDRB_DDRB5=0   
#define SDA_out	  DDRB_DDRB5=1 
*/

#define SCL			PORTE_PE2 //IIC时钟线定义	//初始化为输出
#define SDA			PORTE_PE3 //IIC数据线定义
#define SCL_DR	DDRE_DDRE2
#define SDA_DR	DDRE_DDRE3
#define SDA_in		DDRE_DDRE3=0   
#define SDA_out	  DDRE_DDRE3=1 



#define nop();    {__asm nop;}
#define Start;    {I2C_Start();}
#define Stop;     {I2C_Stop();}
#define WaitAck;  {I2C_WaitAck();} 
#define Ack 1
#define NoAck 0

void I2C_Delay(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_WaitAck(void);
void I2C_Ack(void);
void I2C_NoAck(void);

void I2C_Send_Byte(unsigned char Send_Byte);
unsigned char I2C_Read_Byte(unsigned char AckOrNoAck);

void Single_Write_Byte(unsigned char Slave_Address,unsigned char REG_Address,unsigned char REG_Data);
unsigned char Single_Read_Byte(unsigned char Slave_Address,unsigned char REG_Address);

//void Multiple_Write_Byte(unsigned char Slave_Address,unsigned char REG_Address,unsigned char *REG_Data,unsigned char Num);
void Multiple_Read_Byte(unsigned char Slave_Address,unsigned char REG_Address,unsigned char *REG_Data,unsigned char Num);

//void SCCB_Wait(void);		// I2C_Delay
//void SCCB_Stop(void) ;		//I2C_Stop
//void SCCB_Start(void);		//I2C_Start
//void SCCB_SendAck(unsigned char ack);


#endif  /* _I2C_H_ */