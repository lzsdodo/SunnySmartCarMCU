#ifndef __Function_H
#define __Function_H

#include "IO_Map.h"

#define SCCB_SCL			PORTB_PB4
#define SCCB_SDA			PORTB_PB5
#define SCCB_SDA_DR	  DDRB_DDRB5
#define SCCB_SCL_DR	  DDRB_DDRB4
#define SCCB_OUT			1
#define SCCB_IN				0
#define OV7620ID 0x42

void SCCB_Wait(void);
void SCCB_Stop(void) ;
void SCCB_Start(void);
void SCCB_SendAck(unsigned char ack);
unsigned char SCCB_SendByte(unsigned char bytedata);
//unsigned char SCCB_ReceiveByte(void);
//unsigned char SCCB_ByteRead(unsigned char device,unsigned char address);
void SCCB_ByteWrite(unsigned char device,unsigned char address,unsigned char bytedata);
void SCCB_Init(void);

/*
#define nops() {_asm nop;  _asm nop; _asm nop; _asm nop;}
#define nopss() {_asm nop;  _asm nop; _asm nop; _asm nop;_asm nop;  _asm nop; _asm nop; _asm nop; }
#define hnops() { _asm nop;  _asm nop;}
 void  hnops(void) 
{
 _asm nop;  _asm nop;
}
void  nops(void) 
{
 _asm nop;  _asm nop; _asm nop; _asm nop; 
}
 void  nopss(void) 
{
   nops();nops();
}
*/

#endif //__Function_H