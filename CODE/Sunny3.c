/** ###################################################################
**     Filename  : Sunny3.c
**     Project   : Sunny3
**     Processor : MC9S12XS128MAA
**     Version   : Driver 01.14
**     Compiler  : CodeWarrior HCS12X C Compiler
**     Date/Time : 2014/5/11, 20:53
**     Abstract  :
**         Main module.
**         This module contains user's application code.
**     Settings  :
**     Contents  :
**         No public methods
**
** ###################################################################*/
/* MODULE Sunny3 */

/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "DigitalRST.h"
#include "FIFORCK.h"
#include "FIFOWRST.h"
#include "FIFOWRITE.h"
#include "FIFORRST.h"
#include "PA.h"
#include "PB3.h"
#include "PB2.h"
#include "PB1.h"
#include "PB0.h"
#include "LED1.h"
#include "LED0.h"
#include "VSYNC.h"
#include "HREF.h"
#include "Pulse.h"
#include "Pitms.h"
#include "MotLefPWM.h"
#include "MotRigPWM.h"
#include "MotLefSetDir.h"
#include "MotRigSetDir.h"
#include "MotLefGetSpeed.h"
#include "MotRigGetSpeed.h"
#include "MotLefGetDir.h"
#include "MotRigGetDir.h"
#include "SW.h"
#include "Bit1.h"
#include "Bit2.h"
#include "Bit3.h"
#include "Bit4.h"
/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

/* User includes (#include below this line is not maintained by Processor Expert) */
#include "Variable.h"
#include "MotPID.h"
#include "Sensor.h"
#include "mma845x.h"
#include "L3G4200D.h"
#include "I2C.h"
#include "SCCB.h"
//#include "chuankou.h"
#include "TFT.h"
//#include "DFlash.h"

/*
//=1输出    =0输入
DDRE_DDRAx; 
DDRE_DDRBx; 
DDRE_DDREx;  
DDRP_DDRPx;
DDR1AD0_DDR1ADxx;
DDRT_DDRTx;
DDRM_DDRMx;
DDRS_DDRSx;
//=1高电平  =0低电平
PORTB_PAx;
PORTB_PBx;
PORTE_PEx;
PTP_PTPx;
PT1AD0_PT1ADxx;
PTT_PTTx;
PTM_PTMx;
PTS_PTSx
*/


//---------------------------------函数声明---------------------------------//
void SW_Control(void);
void Ima_Display(void); //图像显示
void Ima_GetDataReady(void);            //图像获取准备(FIFO复位和滤过无效像素点)
void Ima_GetData(void);                 //图像获取
void Get_Edge(unsigned char *pLE,unsigned char *pRE,unsigned char *pMid);  //取得边线

unsigned char Find_LeftEdge(unsigned char CBL,unsigned char CBLimL,unsigned char RRow);
unsigned char Find_RightEdge(unsigned char CBR,unsigned char CBLimR,unsigned char RRow);

unsigned char Road_JdgST(void);
unsigned char Road_JdgPM1(void);

unsigned char Road_JdgStpLin(void);
void Stp_Delay(unsigned char Hundredms);



void main(void)
{
  /* Write your local variable definition here */
  /*
  每次修改记录:
  5-17 20:50:
      把5MS中断去掉取余数的计算方法 分别用5msFlg 和 100msFlg来判断
      直道信息把 010 和 101 认为弯道处理
      另外设置了个 fDirWatch 和 fDirOutWatch 观察
      人字识别  用拨码控制 3 / 5.1 / 6.5  *5ms
      认为直道信息 111/010时采集比较准 直接控制
      另外对于 101 未加保护 如何处理?
  
  

  
*/
  
  //---------------------------------变量声明---------------------------------//
  static unsigned char i; //循环临时变量      
  static unsigned char *p;
   
  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  Cpu_DisableInt();//禁止中断
  //--------------------------------初始化函数---------------------------------//

  SCCB_Init();              //摄像头初始化
  L3g4200d_Init();          //L3G4200D初始化 
  MMA845x_Init();           //MMA8451初始化
  LCD_Init();               //屏幕初始化
  Mot_Init();               //电机初始化，PWM置0 
  //----------------------------------开启中断---------------------------------//
  VSYNC_Disable();          //禁止场中断
  HREF_Disable();           //禁止行中断
  while(Pitms_Enable());    //允许定时中断
  Cpu_EnableInt();          //允许中断 
  
  /* Write your code here */
  
  //直道信息预测
  uSTMode[0][0][0] = 0;
  uSTMode[1][0][1] = 0;
  uSTMode[0][1][0] = 0;
  
  uSTMode[1][1][1] = 1;
  
  uSTMode[1][1][0] = -2;
  uSTMode[0][1][1] = 2;  
  
  uSTMode[1][0][0] = -3;
  uSTMode[0][0][1] = 3;
  
  fDirKp = fDirKpCN;
  //拨码策略
  //检测全0   观察图像 直立
  //2档/3档/4档/5档控制 特定速度涉及的参数
  //人字检测  6,7位拨码
   
  nSpeedSet = 0;                         
  fAngleOut = 0;
  //*---------------------------------主循环(主要版本)--------------------------------*// 
   while(TRUE) 
  {     
      LED0 = ~LED0;//观察主循环有没挂掉,一直开着   
      if( uStrReady )
      {
          if( w5msCount > 450 )  
          {
              SW_Control(); 
              w5msCount = 0;
              wImaCount = 0;
              uStrReady = 0;   
          }
      }                      
      
      
      //保护和停车相关参数
      if( (uStpReady==TRUE) || (nSpeedAvr > 300) )
      {
          if( uStpReady==TRUE ) 
          {
            Stp_Delay(uStpTime);
            nSpeedSet = 0;
            fAngleKp += 500;
            fAngleKd =0;   
            Stp_Delay(uStpTime); 
            fDirKp = 0; 
            fDirKd = 0;       
          }
          Pitms_Disable();
        	Mot_SetLefPWM(0,0,0);
          Mot_SetRigPWM(0,0,0);	
      }
       
  
      //-----------------------取出图像数据-----------------------//
      //-----标志复位-----//
      wImaCount++;                          //主循环计数
      uCapRow    = 0;                       //采集行清零
      uImaCapFlg = 0;                       //图像采集标志
      uImaGetFlg = 0;                       //图像取出标志
      VSYNC_Enable();                       //开启场中断
      //-----提取图像-----//                        
      while( !(uImaCapFlg && uImaGetFlg) )  
          Ima_GetData(); 
      //-----------------------开始图像处理----------------------//     
      //-----标志复位-----//
      uStrFlg = 0;
      uStpFlg = 0;
      //-----计算边沿-----//
      Get_Edge(LefEdge, RigEdge,MidLine);           
      
      //-----------方向控制-----------//
      for( nDirSum=0, p=MidLine,i=0; i<25;i++,p++) 
          nDirSum += *p;
      fDir = nDirSum / 25 - uDirZero;
      if(fDir > 65) 
          fDir = 65;
      else if(fDir < -65) 
          fDir = -65;
      
 
      //判断道路状况 
      //直道  uRoadSym 0
      //弯道  uRoadSym 1
      //人字  uRoadSym 2 
      if( Road_JdgST() == TRUE ) 
      {    
          uRoadSym = 0;     	
      }
      else 
      {
          //NOPM = 0 LEFPM = 1 RIGPM = 2
          if( uRenJdgSym ) 
          {
              if( Road_JdgPM1() == TRUE ) 
              {
                  LED1 = 1;
                  uRoadSym = 2;  
              }
              else 
              {
                  LED1 = 0;
                  uRoadSym = 1;
              }
          } 
          else
          {  
              uRoadSym = 1;   
          }
              
      }

      //根据道路情况 改变 fDirKp 以及 控制 fDir 的范围
      switch( uRoadSym ) 
      {
        case 0:
              //不
              if( cDirSTRef != 0 )
              {
                  switch(cDirSTRef)
                  {
                    case -3:
                        if(fDir < -10)
                            fDir = -10;
                        break;
                    case -2:
                        if(fDir < -4)
                            fDir = -4;
                        break;
                    case 1: 
                         break;
                    case 2:
                        if(fDir > 4)
                            fDir = 4;
                        break;
                    case 3:
                        if(fDir > 10)
                            fDir = 10;
                        break;
                  }
              }
              
              if(uStpReady == 0)
                  if(w5msCount > 5000)
                    uStpReady = Road_JdgStpLin();
              break;
              
        case 1:
              fDirKp = fDirKpCN;
              break;  
              
        case 2:
              fDirKp = fDirKpPM;
              break;
        default:break;
              
      }
       
      Dir_PID();
      
       //-----图像显示-----//
      if( uImaFlg )
          Ima_Display();//包括图像,边线,中线           
  } 
      //---------------------------------主循环--------------------------------// 

  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

//*------------------------------------------其他函数-------------------------------------------*//


//*---------------得到边线---------------*//
void Get_Edge(unsigned char *pLE,unsigned char *pRE,unsigned char *pMid)
{
    unsigned char i;
    unsigned char Row;
    unsigned char LCB,RCB,LCBLim,RCBLim;      //CB Count Back
    unsigned char *pLEdge  = pLE;
    unsigned char *pREdge  = pRE;
    unsigned char *pMidLin = pMid;
    unsigned char *pLEdgeOld = pLEdge;
    unsigned char *pREdgeOld = pREdge;
    int EdgPredict;
    
    //*-----------------寻线开始-----------------*//
    for( Row=0; Row<40; Row++,pLEdgeOld=pLEdge++,pREdgeOld=pREdge++,pMidLin++)
    {     
    
        //*--------确定边线寻找起点开始--------*//          
        if( Row < 3 )  { LCB=80; RCB=LCB; LCBLim=1; RCBLim=158; }   
        else 
        {
            //左线
            if( *pLEdgeOld < 10)  { LCBLim = 1; LCB = *pLEdgeOld + 8; }
            else
            {
                LCBLim = *pLEdgeOld - 8;
                if( *pLEdgeOld < 150 ) LCB = *pLEdgeOld + 8;
                else                              LCB = 157;
            }
            //右线
            if( *pREdgeOld > 150)  {RCBLim = 158;RCB = *pREdgeOld - 8;} 
            else
            {
                RCBLim = *pREdgeOld + 8; 
                if( *pREdgeOld > 9) RCB = *pREdgeOld - 8;
                else                RCB = 2;
            }   
        }  
        //*--------确定边线寻找起点结束--------*//
        
        //*--------开始寻找左边线--------*// 
        if( Find_LeftEdge(LCB,LCBLim,Row) ) 
        {   
            uLosNum_L = 0;    //找到线 左丢线累计次数 清0 
        } 
        else 
        {   //找不到此行边线 预测本行边线位置
            if( Row ) 
            { 
                if(Row > 1) EdgPredict = (*pLEdgeOld) * 2 - *(pLEdgeOld-1);//根据前2次预测此次位置 
                else        EdgPredict = *pLEdgeOld; 
            }
            else  EdgPredict = 0;
            //根据预测结果确定边线位置
            if( EdgPredict > 0)  
            {
                if(EdgPredict < 160)  *pLEdge = EdgPredict;   
                else                  *pLEdge = 159;               
            } 
            else  *pLEdge = 0;
            
            uLosNum_L++;//丢线次数自加
        }
        //*--------寻找左边线结束--------*//
        
        //*--------开始寻找右边线--------*//        
        if( Find_RightEdge(RCB,RCBLim,Row) ) 
        {   
            uLosNum_R = 0;    //找到线 丢线累计次数 清0 
        }   
        else 
        {   //找不到边线 预测本行边线位置
            if( Row ) 
            { 
                if(Row > 1) EdgPredict = (*pREdgeOld) * 2 - *(pREdgeOld-1);//根据前2次预测此次位置 
                else        EdgPredict = *pREdgeOld; 
            }
            else  EdgPredict = 159;
            //根据预测结果确定边线位置
            if( EdgPredict > 0)  
            {
                if(EdgPredict < 160)  *pREdge = EdgPredict;   
                else                  *pREdge = 159;               
            } 
            else  *pREdge = 0;
            
            
            uLosNum_R++;
        }
        //*--------寻找右边线结束--------*//
        
        if( Row < 30 ) 
        {
            if( *pLEdge > 120 )
            {
                *pLEdge = 0;
            }
            if( *pREdge < 40 ) 
            {
                *pREdge = 159;
            }
        }
        
        //判断有效边界 上结束行 下结束行 起始行
            if( ( *pLEdge + *(pLEdge-1) + *(pLEdge-2)  ) < 6 )
            {
                if( Row < 3 )//从一开始就找不到边线
                {
                    uDirJdg   = 0;    //左线全丢 左转 
                    uStpLin_B = 0;    //下结束行与起始行都为0 认为左线全丢                    
                    uStrFlg   = 1;    //起始行确定标志
                    uStrLin   = 0;    //左线全丢 起始行为第一行
                    *(pLEdgeOld-1)=0;
                    *pLEdgeOld=0;
                    *pLEdge=0;  
                }   
                else
                {
                    if( *pLEdgeOld < 15)              //之前很靠近左边界 
                    {    
                        uDirJdg   = 0;                //判定左转                                       
                        uStpLin_B =  Row - uLosNum_L; //左拐时 左边线 为有效下末行
                    }
                    else  if( *pLEdgeOld > 145)       //之前靠近右侧
                    {
                        uStpFlg   = 1;
                        uDirJdg   = 1;                //判定右转
                        uStpLin_T =  Row - uLosNum_L; //右拐时 左边线 为有效上末行
                    }  
                }
            }
        
        if( ( *pREdge + *(pREdge-1) + *(pREdge-2)  ) > 450 )
            {
                if( Row < 3 )
                {
                    uDirJdg   = 1;        //判断右转
                    uStpLin_B = 0;        //右线全丢
                    uStrFlg   = 1;        //起始行标志
                    uStrLin   = 0;        //起始行为第一行
                    *(pREdgeOld-1)=159;
                    *pREdgeOld=159;
                    *pREdge=159;   
                } 
                else 
                {
                    if(*pREdgeOld > 145)//之前靠近右侧 
                    {
                        uDirJdg = 1;
                        uStpLin_B =  Row - uLosNum_R;
                    }
                    else  if(*pREdgeOld < 15)//之前靠近左侧
                    {
                        uStpFlg = 1;
                        uStpLin_T =  Row - uLosNum_R;
                        uDirJdg = 0;
                    }   
                }
            }       
        
        //*--------确定起始行--------*// 
        if((uStrFlg||uLosNum_L||uLosNum_R)==0) { uStrFlg = 1; uStrLin = Row;  }
        //*--------起始行--------*//
        *pMidLin = ( (*pLEdge + *pREdge) >> 1 ) ;
        //*--------寻找中线--------*//
        if(uStpFlg == 1)
        {
            uStpLin_C = ((uStpLin_T + uStpLin_B)>>1);
            if(uDirJdg == 0)  //左转
            {
                for(i=uStpLin_B; i<uStpLin_C; i++) 
                {
                    *pMidLin = *(pMidLin-1) + *pREdge - *(pREdge-1); 
                }
            }
            else                //右转
            {   
                for(i=uStpLin_B; i<uStpLin_C; i++) 
                {
                    *pMidLin = *(pMidLin-1) + *pLEdge - *(pLEdge-1);  
                }
            }
            break;
        } 
        else if(Row == 39)
        {
            uStpLin_B = 39;
            uStpLin_T = 39;
            uStpLin_C = 39;
        }
        //*--------寻找中线--------*//
        
    /***********************************一场图像处理***********************************/    
    }
}
//*---------------得到边线---------------*//

//*-----寻左边线-----*//
unsigned char Find_LeftEdge(unsigned char CBL,unsigned char CBLimL,unsigned char RRow)
{  
    unsigned char LosFlg = 0; //边线寻找标志
    unsigned char CB_L   = CBL;
    unsigned char CBLim_L = CBLimL;  
    
    for(; CB_L > CBLim_L; CB_L--) 
    {                         
        if( Image[RRow][CB_L] == 0 ) 
        {
            //判断是否白白黑黑这样的四个点,若是,认为这位边线
            if( (Image[RRow][CB_L+2] + Image[RRow][CB_L+1] - Image[RRow][CB_L] - Image[RRow][CB_L-1]) > 500 );//边线判断               
            {                  
                LosFlg = 1;                   //采集 成功标志 置1
                LefEdge[RRow] = CB_L;  //记录该点在该行的位置
                break;                        //假如已经采到点,跳出当前行的左线寻找循环
            }
        } 
    } 
    
    
    /*
    for(; CB_L > CBLim_L; pImage--,CB_L--) 
    {                         
        if( *pImage == 0 ) 
        {
            //判断是否白白黑黑这样的四个点,若是,认为这位边线
            if( (*(pImage+2) + *(pImage+1) - *pImage - *(pImage-1)) > 500 );//边线判断               
            {                  
                LosFlg = 1;                   //采集 成功标志 置1
                *pppEdge = CB_L;  //记录该点在该行的位置
                break;                        //假如已经采到点,跳出当前行的左线寻找循环
            }
        } 
    } 
    */
    return LosFlg; 
}
//*-----寻左边线-----*//

//*-----寻右边线-----*//
unsigned char Find_RightEdge(unsigned char CBR,unsigned char CBLimR,unsigned char RRow)
{
    unsigned char LosFlg = 0;//边线寻找标志  
    unsigned char CB_R   = CBR;
    unsigned char CBLim_R  = CBLimR;
    //unsigned char *pImage  = pIma;
    //unsigned char *pppEdge = ppEdg;
    
    for(; CB_R < CBLim_R; CB_R++)  
    {                         
        if( Image[RRow][CB_R] == 0 )
        {
            //判断是否白白黑黑这样的四个点,若是,认为这位边线
            if( (Image[RRow][CB_R-2]  + Image[RRow][CB_R-1]  - Image[RRow][CB_R]  - Image[RRow][CB_R+1] ) > 500 );//边线判断               
            {                  
                LosFlg = 1;                   //采集 成功标志 置1
                RigEdge[RRow] = CB_R;  //记录该点在该行的位置
                break;                        //假如已经采到点,跳出当前行的左线寻找循环
            }
        } 
    } 
    
    /*
    for(; CB_R < CBLim_R; pImage++,CB_R++)  
    {                         
        if( *pImage == 0 ) 
        {
            //判断是否白白黑黑这样的四个点,若是,认为这位边线
            if( (*(pImage-2) + *(pImage-1) - *pImage - *(pImage+1)) > 500 );//边线判断               
            {                  
                LosFlg = 1;                   //采集 成功标志 置1
                *pppEdge = CB_R;  //记录该点在该行的位置
                break;                        //假如已经采到点,跳出当前行的左线寻找循环
            }
        } 
    } 
    */
    return LosFlg; 
}
//*-----寻右边线-----*//

//--------------------图像数据取回----------------------//
void Ima_GetData(void)
{
    unsigned char ii,jj;
    unsigned char *pp;
    //---------一场图像数据读取----------//
    if( uCapRow > 30 )  //50 读取不会快于采集
    {  
        Ima_GetDataReady();    
        for( ii=MINROW; ii<RANGEROW; ii++ ) 
        {  
            //if( ii > uCapRow ) Text = 1;  检测最快什么时候可以读取
            pp = &Image[ii][MAXCOL];
            for( jj=MINCOL; jj<RANGECOL; jj++ ) 
            {   
                FIFORCK = 0;FIFORCK = 1;  //FIFORCK 1行了每两格取1点                        
                FIFORCK = 0;      
                *pp = PORTA;             //读FIFO数据;PORTA;
                //黑点一般灰度为30-50  白点一般170以上 相邻点会降到100左右 
                if(PORTA > THRESHOLD ) *pp = 255; 
                else *pp = 0;
                FIFORCK = 1;    
                pp--;  
            }    
        } 
        uImaGetFlg = 1;    
    }
    //-------一场图像数据读取--------//
}
//-----------------------图像数据取回------------------------//
void Ima_GetDataReady(void)
{
    unsigned char ii,jj;
    //---FIFO读复位操作---// 
    FIFORRST = 0; //读复位使能 
    FIFORCK  = 0; 
    FIFORCK  = 1; 
    FIFORCK  = 0; //FIFO时钟信号
    FIFORRST = 1; //读复位关闭
    FIFORCK  = 1;  
    //---FIFO读复位操作---// 
    for(ii=0; ii<2; ii++)  
        for(jj=0; jj<150; jj++)
            {FIFORCK=0;FIFORCK=1;} 
}

//-------------------------图像显示--------------------------//
void Ima_Display(void)
{
    unsigned char ii,jj;
    unsigned char *pp;
    //unsigned int  RGB;
    
    //二值化图像显示
    pp = &Image[0][0];
    for(ii=0; ii<RANGEROW; ii++) 
    {  
        for(jj=0; jj<RANGECOL; jj++,pp++) 
        {   
            /*
            //RGB格式显示
            RGB = (*pp>>3) + (*pp>>2)*32 + (*pp>>3)*2048;  
            LCD_OutLCD_PutPixel(ii,jj,RGB); 
            */
            //*
            if( *pp ) 
                LCD_OutLCD_PutPixel(ii,jj,WHITECOLOR);
            else
                LCD_OutLCD_PutPixel(ii,jj,LIGHTBLUE); 
            //*/     
        }  
    }            
                                                        
    //图像分割行
    for(ii=0;ii<RANGECOL;ii++) LCD_OutLCD_PutPixel(40,ii,BLACKCOLOR);   
    
    for(ii=0; ii<RANGEROW; ii++) 
    {
        LCD_OutLCD_PutPixel(ii,MidLine[ii],PURPLE);                     //中线显示
        LCD_OutLCD_PutPixel(ii,LefEdge[ii],BLACKCOLOR);                 //左线显示
        LCD_OutLCD_PutPixel(ii,RigEdge[ii],BLACKCOLOR);                 //右线显示
    }
    //直道检测指导线
    for(ii=15; ii<40; ii++) 
    {
        LCD_OutLCD_PutPixel(ii,80,PURPLE);                     //中线显示
        LCD_OutLCD_PutPixel(ii,50,PURPLE);                 //左线显示
        LCD_OutLCD_PutPixel(ii,110,PURPLE);                 //右线显示
    }
    //人字检测指导线
  	for( ii=10; ii<45; ii++ )
    {        
      LCD_OutLCD_PutPixel(ii,70-ii,RED); 
      LCD_OutLCD_PutPixel(ii,55-ii,RED);    
      LCD_OutLCD_PutPixel(ii,80+ii,RED);     
      LCD_OutLCD_PutPixel(ii,95+ii,RED);  
    }
    
    if( uRoadSym == 2 ) 
    { 
        for( ii=75; ii<95; ii++ )  
        {   
            for( jj=75; jj<95; jj++ )  
            {
                LCD_OutLCD_PutPixel(ii,jj,RED);  
            }
        }
    }
      
}
/*************图像显示*************/
unsigned char Road_JdgST(void)
{
  	unsigned char Row;
  	unsigned char *pJdgST;
  	unsigned char LefSTFlg = 1;
  	unsigned char MidSTFlg = 1;
  	unsigned char RigSTFlg = 1;
  	
  	cDirSTRef = 0;

    //中线判断
    pJdgST = &Image[15][80]; 
  	for( Row=15; Row<RANGEROW; pJdgST+=160,Row++ )
  	{
    		if( *pJdgST == BLACK )
    		{
      			MidSTFlg = 0;
      			break;
    		}
  	}
  	//左右直线判断

  	pJdgST = &Image[15][60];
  	for( Row=15; Row<RANGEROW; pJdgST+=160,Row++ )
  	{
    		if( *pJdgST == BLACK )
    		{
      			LefSTFlg = 0;
      			break;
    		}
  	}
  	pJdgST = &Image[15][100];
  	for( Row=15; Row<RANGEROW; pJdgST+=160,Row++ )
  	{
    		if( *pJdgST == BLACK )
    		{
      			RigSTFlg = 0;
      			break;
    		}
  	}	
  	
  	if( LefSTFlg  || MidSTFlg || RigSTFlg )
  	    return TRUE;
  	
  	cDirSTRef = uSTMode[LefSTFlg][MidSTFlg][RigSTFlg];
  	
     
    
}


unsigned char Road_JdgStpLin(void)
{
    unsigned char ii;
  	unsigned char LefOld = WHITE;
  	unsigned char RigOld = WHITE;
  	unsigned char LefCount = 0;
  	unsigned char RigCount = 0;
  	unsigned char LefSTFlg = FALSE;
  	unsigned char RigSTFlg = FALSE;
    //50&110
    
    
    for( ii=0; ii<20; ii++ ) 
    {
        if( Image[ii][50] != LefOld ) 
        {
            LefOld = Image[ii][50];
            LefCount++;
            if( LefCount > 2 )
                return FALSE;
        }
    }
    if( LefCount == 2 )
        LefSTFlg = TRUE;
    
    for( ii=0; ii<20; ii++ ) 
    {
        if( Image[ii][110] != RigOld ) 
        {
            RigOld = Image[ii][110];
            RigCount++;
            if( RigCount > 2 )
                return FALSE;
        }
    }
    if( RigCount == 2 )
        RigSTFlg = TRUE;
    
    
    if( LefSTFlg && RigSTFlg )
        return TRUE;
    else
        return FALSE;
}


unsigned char Road_JdgPM1(void)
{
	//10-39行
	//右95-124
	//左65-26  
	unsigned char ii;
	unsigned char **pJdgPM;
	unsigned char  PMOld;
	unsigned char PMHighOld = 0;
  unsigned char PMDHigh = 0;	
	
	unsigned char PMWide = 0;
	unsigned char PMWideOld = 0;
  unsigned char PMChangeNum;
	
	PMOld = WHITE;
	PMChangeNum = 0;
	pJdgPM = &pRigJdgPM[0];
	for( ii=0; ii<30; pJdgPM++,ii++ )
    {        
        if( **pJdgPM != PMOld ) 
        {
			PMChangeNum++;
			PMOld = **pJdgPM;
        }
        if( PMChangeNum >= uRenJdgNum )
			return TRUE;
    }
	
}

/*
unsigned char Road_JdgPM1(void)
{
	//10-39行
	//右95-124
	//左65-26  
	unsigned char ii;
	unsigned char **pJdgPM;
	unsigned char  PMOld;
	unsigned char PMWide = 0;
  unsigned char PMChangeNum;
	
	PMOld = WHITE;
	PMChangeNum = 0;
	pJdgPM = &pRigJdgPM[0];
	for( ii=0; ii<30; pJdgPM++,ii++ )
    {        
        if( **pJdgPM != PMOld ) 
        {
    			PMChangeNum++;
    			PMOld = **pJdgPM;
        }
        if( PMChangeNum >= 7 )
			return TRUE;
    }
	return FALSE; 
	
}

unsigned char Road_JdgPM2(void)
{
	//10-39行
	//右95-124
	//左65-26  
	unsigned char ii;
		unsigned char  PMOld;
	unsigned char **pJdgPM;
	unsigned char PMWide = 0;
  unsigned char PMChangeNum;
	
	PMOld = WHITE;
	PMChangeNum = 0;
	pJdgPM = &pRigJdgPM2[0];
	for( ii=0; ii<30; pJdgPM++,ii++ )
    {        
        if( **pJdgPM != PMOld ) 
        {
    			PMChangeNum++;
    			PMOld = **pJdgPM;
        }
        if( PMChangeNum >= 7 )
			return TRUE;
    }
	return FALSE; 
	
}
*/




void Stp_Delay(unsigned char Hundredms) 
{
    unsigned char ii;
    for( ii=0; ii<Hundredms; ii++ )
        Cpu_Delay100US(1000);
}


/*************拨码策略***************/
void SW_Control(void)
{

    uSWValue = SW_GetVal();
    switch( uSWValue & 0x3f ) 
    {
        case 0:
            nSpeedSet = 0;
            uImaFlg  = 1; 
            break;
        case 1:
            nSpeedSet = 80;
            fDirKpCN = 26; 
            fDirKpPM = 150;
            uStpTime = 2; 
            break;
        case 3:
            nSpeedSet = 100; 
            fDirKpCN = 28; 
            fDirKpPM = 170;
            uStpTime = 1; 
            break;
        case 7:
            nSpeedSet = 125; 
            fDirKpCN = 30;
            fDirKpPM = 180;
            uStpTime = 1; 
            break;
        case 15:
            nSpeedSet = 150; 
            fDirKpCN = 35;
            fDirKpPM = 200;
            uStpTime = 1; 
        default:break;
       
    }   
    
    if(  ( uSWValue & 0x10 ) == 0x10 ) 
    {
        uRenJdgNum = 8;
    }
    if(  ( uSWValue & 0x20 ) == 0x20 )
    {     
        uRenJdgNum = 7;
    }
    if(  ( uSWValue & 0x40 ) == 0x40 )
    {
        uRenJdgNum = 6;
    } 

    //最右1位   拨下      开启人字 
    if(  ( uSWValue & 0x80 ) == 0x80 ) 
    {
            uRenJdgSym  = 1; 
            nSpeedSet = 80;
            fDirKpCN = 27; 
            fDirKpPM = 200;
            uStpTime = 1;
    }
    else
    {
            uRenJdgSym  = 0;
    }
}

/*************拨码策略***************/


/* END Sunny3 */
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.02 [04.44]
**     for the Freescale HCS12X series of microcontrollers.
**
** ###################################################################
*/
