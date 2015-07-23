#ifndef _TFT_H_
#define _TFT_H_
#include "IO_Map.h"

#define   TFT_RESET			  PTP_PTP7     //AD7
#define   TFT_AO					PTM_PTM0     //AD6
#define   TFT_SDA				  PTS_PTS3     //AD5
#define   TFT_SCK				  PTS_PTS2    //AD4
#define   TFT_CS					PTM_PTM1    //AD3

#define   TFT_RESET_DR			DDRP_DDRP7     //AD7
#define   TFT_AO_DR					DDRM_DDRM0     //AD6
#define   TFT_SDA_DR				DDRS_DDRS3     //AD5
#define   TFT_SCK_DR				DDRS_DDRS2    //AD4
#define   TFT_CS_DR					DDRM_DDRM1    //AD3

//--------º¯ÊýÉùÃ÷---------//
void  Delay_nms(unsigned int n) ;
void  LCD_Init(void);
void  LCD_Write_Data(unsigned char dat);
void  LCD_Write_Command(unsigned char dat);
void  LCD_DataWrite(unsigned char LCD_DataH,unsigned char LCD_DataL);
//void  LCD_Reset();
void  LCD_Dispaly_SingleColor(unsigned char DH,unsigned char DL);
//void  LCD_RedLine();
void  LCD_RamAdressSet();
//void  LCD_OutLCD_PutPixel(unsigned int x,unsigned int y);
void  LCD_OutLCD_PutPixel(unsigned char x,unsigned char y,unsigned int color);
//void  LCD_Display_ASCII8X16(unsigned int x0,unsigned int y0,unsigned char *s);


#endif  /* _TFT_H_ */