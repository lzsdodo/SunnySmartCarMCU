#include "TFT.h"

/******************************************************
*                岱默科技DEMOK XS128开发小组
*
*  函数名称：Delay_nms
*  功能说明：延时Nms子函数
*  参数说明：无
*  函数返回：无
*******************************************************/
void Delay_nms(unsigned int n)       //N ms延时函数
{
   unsigned int i=0;
   unsigned int j=0;
   for (i=0;i<n;i++) 
   {
      for (j=0;j<500;j++);
   }
}

/******************************************************
*                岱默科技DEMOK XS128开发小组
*
*  函数名称：LCD_Init
*  功能说明：LCD初始化函数
*  参数说明：无
*  函数返回：无
*******************************************************/
void LCD_Init(void)
{
    TFT_RESET_DR = 1;
    TFT_AO_DR    = 1;
    TFT_SDA_DR   = 1;
    TFT_SCK_DR   = 1;
    TFT_CS_DR    = 1;

    TFT_RESET = 0;
    Delay_nms(100);
    TFT_RESET = 1;
    Delay_nms(100);

    //------Software Reset-----------//
    LCD_Write_Command(0x11); //Sleep out
    Delay_nms (120); //Delay 120ms
    //------ST7735S Frame Rate--------//
    LCD_Write_Command(0xB1);
    LCD_Write_Data(0x05);
    LCD_Write_Data(0x3C);
    LCD_Write_Data(0x3C);
    LCD_Write_Command(0xB2);
    LCD_Write_Data(0x05);
    LCD_Write_Data(0x3C);
    LCD_Write_Data(0x3C);
    LCD_Write_Command(0xB3);
    LCD_Write_Data(0x05);
    LCD_Write_Data(0x3C);
    LCD_Write_Data(0x3C);
    LCD_Write_Data(0x05);
    LCD_Write_Data(0x3C);
    LCD_Write_Data(0x3C);
    //-------End ST7735S Frame Rate-------//
    LCD_Write_Command(0xB4); //Dot inversion
    LCD_Write_Data(0x03);
    LCD_Write_Command(0xC0);
    LCD_Write_Data(0x28);
    LCD_Write_Data(0x08);
    LCD_Write_Data(0x04);
    LCD_Write_Command(0xC1);
    LCD_Write_Data(0XC0);
    LCD_Write_Command(0xC2);
    LCD_Write_Data(0x0D);
    LCD_Write_Data(0x00);
    LCD_Write_Command(0xC3);
    LCD_Write_Data(0x8D);
    LCD_Write_Data(0x2A);
    LCD_Write_Command(0xC4);
    LCD_Write_Data(0x8D);
    LCD_Write_Data(0xEE);
    //-------End ST7735S Power Sequence----//
    LCD_Write_Command(0xC5); //VCOM
    LCD_Write_Data(0x1A);
    LCD_Write_Command(0x36); //MX, MY, RGB mode
    LCD_Write_Data(0x00);
    //-----ST7735S Gamma Sequence----------//
    LCD_Write_Command(0xE0);
    LCD_Write_Data(0x04);
    LCD_Write_Data(0x22);
    LCD_Write_Data(0x07);
    LCD_Write_Data(0x0A);
    LCD_Write_Data(0x2E);
    LCD_Write_Data(0x30);
    LCD_Write_Data(0x25);
    LCD_Write_Data(0x2A);
    LCD_Write_Data(0x28);
    LCD_Write_Data(0x26);
    LCD_Write_Data(0x2E);
    LCD_Write_Data(0x3A);
    LCD_Write_Data(0x00);
    LCD_Write_Data(0x01);
    LCD_Write_Data(0x03);
    LCD_Write_Data(0x13);
    LCD_Write_Command(0xE1);
    LCD_Write_Data(0x04);
    LCD_Write_Data(0x16);
    LCD_Write_Data(0x06);
    LCD_Write_Data(0x0D);
    LCD_Write_Data(0x2D);
    LCD_Write_Data(0x26);
    LCD_Write_Data(0x23);
    LCD_Write_Data(0x27);
    LCD_Write_Data(0x27);
    LCD_Write_Data(0x25);
    LCD_Write_Data(0x2D);
    LCD_Write_Data(0x3B);
    LCD_Write_Data(0x00);
    LCD_Write_Data(0x01);
    LCD_Write_Data(0x04);
    LCD_Write_Data(0x13);
    //--------End ST7735S Gamma Sequence--------//
    LCD_Write_Command(0x3A); //65k mode
    LCD_Write_Data(0x05);
    LCD_Write_Command(0x29); //Display on
    LCD_Write_Command(0x2C);
    Delay_nms(10);
    LCD_Dispaly_SingleColor(0x84,0x10);//紫色0xA0B4
    Delay_nms(10);
}

/******************************************************
*                岱默科技DEMOK XS128开发小组
*
*  函数名称：LCD_Write_Data
*  功能说明：LCD写数据函数
*  参数说明：dat 写入的字符
*  函数返回：无
*******************************************************/
void  LCD_Write_Data(unsigned char dat)
{
  unsigned char i;
  TFT_CS = 0;
  TFT_AO = 1;
  for(i=0; i<8; i++) 
  {
    if(dat&0x80)
		{
			TFT_SDA = 1;
		}
		else
		{
			TFT_SDA = 0;
		}
		TFT_SCK = 0;
		dat = dat << 1;
		TFT_SCK = 1;
  }
  TFT_CS = 1;
}

/******************************************************
*                岱默科技DEMOK XS128开发小组
*
*  函数名称：LCD_Write_Command
*  功能说明：LCD写命令函数
*  参数说明：dat 写入的命令
*  函数返回：无
*******************************************************/
void  LCD_Write_Command(unsigned char dat)
{
  unsigned char i;
  TFT_CS = 0;
  TFT_AO = 0;
  for(i=0; i<8; i++) 
  {
    if(dat&0x80)
		{
			TFT_SDA = 1;
		}
		else
		{
			TFT_SDA = 0;
		}
		TFT_SCK = 0;
		dat = dat << 1;
		TFT_SCK = 1;
  }
  TFT_CS = 1;
}

/******************************************************
*                岱默科技DEMOK XS128开发小组
*
*  函数名称：LCD_DataWrite
*  功能说明：LCD写数据函数
*  参数说明：LCD_DataH   数据高字节
*            LCD_DataL   数据低字节
*  函数返回：无
*******************************************************/

void LCD_DataWrite(unsigned char LCD_DataH,unsigned char LCD_DataL)
{
  LCD_Write_Data(LCD_DataH);
  LCD_Write_Data(LCD_DataL);
}


/******************************************************
*                岱默科技DEMOK XS128开发小组
*
*  函数名称：LCD_PutPixel
*  功能说明：LCD在指定位置显示指定颜色
*  参数说明：x_start   起始行坐标
*            y_start   起始列坐标
*            color     指定颜色
*  函数返回：无
*******************************************************/
/*
void LCD_PutPixel(unsigned int x_start,unsigned int y_start,unsigned int color)
{
	LCD_CtrlWrite_IC(0x2a);
	LCD_DataWrite_IC(x_start);
	LCD_DataWrite_IC(0x5f);
	LCD_CtrlWrite_IC(0x2b);
	LCD_DataWrite_IC(y_start+0x34);
	LCD_DataWrite_IC(0x7F);
	LCD_CtrlWrite_IC(0x2c);
	LCD_DataWrite_IC(color>>8);
	LCD_DataWrite_IC(color&0xff);
	
}
*/


 
/******************************************************
*                岱默科技DEMOK XS128开发小组
*
*  函数名称：LCD_Reset
*  功能说明：LCD复位子函数
*  参数说明：无
*  函数返回：无
******************************************************* 
void LCD_Reset()
{
  TFT_RESET = 0;
  Delay_nms(100);
  TFT_RESET = 1;
  Delay_nms(100);                                    
}                                                   */

/******************************************************
*                岱默科技DEMOK XS128开发小组
*
*  函数名称：LCD_RamAdressSet
*  功能说明：设置LCD RAM地址
*  参数说明：无
*  函数返回：无
*******************************************************/
void  LCD_RamAdressSet()
{
   LCD_Write_Command(0x2A);
   LCD_Write_Data(0x00);
   LCD_Write_Data(0x00);
   LCD_Write_Data(0x00);
   LCD_Write_Data(0x7f);
  
   LCD_Write_Command(0x2B);
   LCD_Write_Data(0x00);
   LCD_Write_Data(0x00);
   LCD_Write_Data(0x00);
   LCD_Write_Data(0x9f);		  
}


/******************************************************
*                岱默科技DEMOK XS128开发小组
*
*  函数名称：LCD_OutLCD_PutPixel
*  功能说明：LCD像素输出
*  参数说明：
*  函数返回：无
*******************************************************/
void LCD_OutLCD_PutPixel(unsigned char x,unsigned char y,unsigned int color)
{
  LCD_Write_Command(0x2A);  //Column Address Set        ****
	LCD_Write_Data(0);//xsh 
	LCD_Write_Data(x);//xsl 	
	LCD_Write_Command(0x2B);  //Row Address Set             ****
	LCD_Write_Data(0);//ysl 
	LCD_Write_Data(y);//ysl 
	LCD_Write_Command(0x2c);
	LCD_Write_Data(color>>8);
	LCD_Write_Data(color&0xff);
}


/******************************************************
*                岱默科技DEMOK XS128开发小组
*
*  函数名称：LCD_Display_ASCII8X16
*  功能说明：LCD在指定位置显示字符串
*  参数说明：x0   起始行坐标
*            y0   起始列坐标
*            *s   字符串
*  函数返回：无
*******************************************************/
/*
void LCD_Display_ASCII8X16(unsigned int x0,unsigned int y0,unsigned char *s)
{
	int i,j,k,x,y,xx;
	
	unsigned char qm;
	
	long int ulOffset;
	
	char  ywbuf[32],temp[2];

	for(i = 0; i<strlen((char*)s);i++)
	{
		if(((unsigned char)(*(s+i))) >= 161)
		{
			temp[0] = *(s+i);
			temp[1] = '\0';
			return;
		}
		
		else
		{
			qm = *(s+i);

			ulOffset = (long int)(qm) * 16;
			
            for (j = 0; j < 16; j ++)
            {
				ywbuf[j]=Zk_ASCII8X16[ulOffset+j];
           	}
             
            for(y = 0;y < 16;y++)
            {
	          	for(x=0;x<8;x++) 
	           	{
                	k=x % 8;
                	
				  	if(ywbuf[y]&(0x80 >> k))
				   	{
				  		xx=x0+x+i*8;
				    	LCD_OutLCD(xx,y+y0);
				  	}
			   	}
           }

		}
	}     	
}
*/

/******************************************************
*                岱默科技DEMOK XS128开发小组
*
*  函数名称：LCD_Dispaly_SingleColor
*  功能说明：LCD显示单色子函数
*  参数说明：LCD_DataH   颜色数据高字节
*            LCD_DataL   颜色数据低字节
*  函数返回：无
*******************************************************/  
void LCD_Dispaly_SingleColor(unsigned char DH,unsigned char DL)
{
 unsigned char i,j;
 //LCD_RamAdressSet();
 for (i=0;i<160;i++)
    for (j=0;j<128;j++)
        LCD_DataWrite(DH,DL);
}


/******************************************************
*                岱默科技DEMOK XS128开发小组
*
*  函数名称：LCD_RedLine
*  功能说明：LCD显示一条红色分割线
*  参数说明：无
*  函数返回：无
*******************************************************/
/*
void LCD_RedLine()
{
	unsigned char i=0,j=0;
	i=66;   // 11格灰阶
	for(j=0;j<16;j++)
	{
		LCD_PutPixel(i,j,0xf800);
	}

}
*/

/******************************************************
*                岱默科技DEMOK XS128开发小组
*
*  函数名称：LCD_Display_Str
*  功能说明：LCD显示字符串
*  参数说明：无
*  函数返回：无
*******************************************************/
/*
void LCD_Display_Str()
{ 
    LCD_Display_ASCII8X16(1,52,"MD:T18003");
    LCD_Display_ASCII8X16(1,64,"Pro:T4528B00");
    LCD_Display_ASCII8X16(1,76,"Dots:128*160");
    LCD_Display_ASCII8X16(1,88,"IC: ST7735B");
    LCD_Display_ASCII8X16(1,100,"DEMOK");
    LCD_Display_ASCII8X16(1,112,"2013.1.7");
}
*/  

/****************************************************** 
*               岱默科技DEMOK XS128开发小组
*
*  函数名称：主函数
*  功能说明：LCD5510显示
*  参数说明：无
*  函数返回：无
*******************************************************/
 // LCD_Init();
 //LCD_Write_Command(0x2C);

  //-------显示字符串----------//
  /*
  LCD_Dispaly_SingleColor(0xff,0xff);
  LCD_Display_Str();         //版本
  Delay_nms(500);
*/

  //--------显示色阶--------//  
  /*
  LCD_Dispaly_SingleColor(0x84,0x10);//灰色
  Delay_nms(500);	
  LCD_Dispaly_SingleColor(0xff,0xff);//白色
  Delay_nms(500);		
  LCD_Dispaly_SingleColor(0x00,0x00);//黑色
  Delay_nms(500);	  	
  LCD_Dispaly_SingleColor(0xf8,0x00);//红色
  Delay_nms(500);	 	
  LCD_Dispaly_SingleColor(0x07,0xe0);//绿色
  Delay_nms(500);	
  LCD_Dispaly_SingleColor(0x00,0x1f);//蓝色
  Delay_nms(500);	
  */
