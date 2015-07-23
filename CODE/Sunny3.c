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
//=1���    =0����
DDRE_DDRAx; 
DDRE_DDRBx; 
DDRE_DDREx;  
DDRP_DDRPx;
DDR1AD0_DDR1ADxx;
DDRT_DDRTx;
DDRM_DDRMx;
DDRS_DDRSx;
//=1�ߵ�ƽ  =0�͵�ƽ
PORTB_PAx;
PORTB_PBx;
PORTE_PEx;
PTP_PTPx;
PT1AD0_PT1ADxx;
PTT_PTTx;
PTM_PTMx;
PTS_PTSx
*/


//---------------------------------��������---------------------------------//
void SW_Control(void);
void Ima_Display(void); //ͼ����ʾ
void Ima_GetDataReady(void);            //ͼ���ȡ׼��(FIFO��λ���˹���Ч���ص�)
void Ima_GetData(void);                 //ͼ���ȡ
void Get_Edge(unsigned char *pLE,unsigned char *pRE,unsigned char *pMid);  //ȡ�ñ���

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
  ÿ���޸ļ�¼:
  5-17 20:50:
      ��5MS�ж�ȥ��ȡ�����ļ��㷽�� �ֱ���5msFlg �� 100msFlg���ж�
      ֱ����Ϣ�� 010 �� 101 ��Ϊ�������
      ���������˸� fDirWatch �� fDirOutWatch �۲�
      ����ʶ��  �ò������ 3 / 5.1 / 6.5  *5ms
      ��Ϊֱ����Ϣ 111/010ʱ�ɼ��Ƚ�׼ ֱ�ӿ���
      ������� 101 δ�ӱ��� ��δ���?
  
  

  
*/
  
  //---------------------------------��������---------------------------------//
  static unsigned char i; //ѭ����ʱ����      
  static unsigned char *p;
   
  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  Cpu_DisableInt();//��ֹ�ж�
  //--------------------------------��ʼ������---------------------------------//

  SCCB_Init();              //����ͷ��ʼ��
  L3g4200d_Init();          //L3G4200D��ʼ�� 
  MMA845x_Init();           //MMA8451��ʼ��
  LCD_Init();               //��Ļ��ʼ��
  Mot_Init();               //�����ʼ����PWM��0 
  //----------------------------------�����ж�---------------------------------//
  VSYNC_Disable();          //��ֹ���ж�
  HREF_Disable();           //��ֹ���ж�
  while(Pitms_Enable());    //����ʱ�ж�
  Cpu_EnableInt();          //�����ж� 
  
  /* Write your code here */
  
  //ֱ����ϢԤ��
  uSTMode[0][0][0] = 0;
  uSTMode[1][0][1] = 0;
  uSTMode[0][1][0] = 0;
  
  uSTMode[1][1][1] = 1;
  
  uSTMode[1][1][0] = -2;
  uSTMode[0][1][1] = 2;  
  
  uSTMode[1][0][0] = -3;
  uSTMode[0][0][1] = 3;
  
  fDirKp = fDirKpCN;
  //�������
  //���ȫ0   �۲�ͼ�� ֱ��
  //2��/3��/4��/5������ �ض��ٶ��漰�Ĳ���
  //���ּ��  6,7λ����
   
  nSpeedSet = 0;                         
  fAngleOut = 0;
  //*---------------------------------��ѭ��(��Ҫ�汾)--------------------------------*// 
   while(TRUE) 
  {     
      LED0 = ~LED0;//�۲���ѭ����û�ҵ�,һֱ����   
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
      
      
      //������ͣ����ز���
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
       
  
      //-----------------------ȡ��ͼ������-----------------------//
      //-----��־��λ-----//
      wImaCount++;                          //��ѭ������
      uCapRow    = 0;                       //�ɼ�������
      uImaCapFlg = 0;                       //ͼ��ɼ���־
      uImaGetFlg = 0;                       //ͼ��ȡ����־
      VSYNC_Enable();                       //�������ж�
      //-----��ȡͼ��-----//                        
      while( !(uImaCapFlg && uImaGetFlg) )  
          Ima_GetData(); 
      //-----------------------��ʼͼ����----------------------//     
      //-----��־��λ-----//
      uStrFlg = 0;
      uStpFlg = 0;
      //-----�������-----//
      Get_Edge(LefEdge, RigEdge,MidLine);           
      
      //-----------�������-----------//
      for( nDirSum=0, p=MidLine,i=0; i<25;i++,p++) 
          nDirSum += *p;
      fDir = nDirSum / 25 - uDirZero;
      if(fDir > 65) 
          fDir = 65;
      else if(fDir < -65) 
          fDir = -65;
      
 
      //�жϵ�·״�� 
      //ֱ��  uRoadSym 0
      //���  uRoadSym 1
      //����  uRoadSym 2 
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

      //���ݵ�·��� �ı� fDirKp �Լ� ���� fDir �ķ�Χ
      switch( uRoadSym ) 
      {
        case 0:
              //��
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
      
       //-----ͼ����ʾ-----//
      if( uImaFlg )
          Ima_Display();//����ͼ��,����,����           
  } 
      //---------------------------------��ѭ��--------------------------------// 

  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

//*------------------------------------------��������-------------------------------------------*//


//*---------------�õ�����---------------*//
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
    
    //*-----------------Ѱ�߿�ʼ-----------------*//
    for( Row=0; Row<40; Row++,pLEdgeOld=pLEdge++,pREdgeOld=pREdge++,pMidLin++)
    {     
    
        //*--------ȷ������Ѱ����㿪ʼ--------*//          
        if( Row < 3 )  { LCB=80; RCB=LCB; LCBLim=1; RCBLim=158; }   
        else 
        {
            //����
            if( *pLEdgeOld < 10)  { LCBLim = 1; LCB = *pLEdgeOld + 8; }
            else
            {
                LCBLim = *pLEdgeOld - 8;
                if( *pLEdgeOld < 150 ) LCB = *pLEdgeOld + 8;
                else                              LCB = 157;
            }
            //����
            if( *pREdgeOld > 150)  {RCBLim = 158;RCB = *pREdgeOld - 8;} 
            else
            {
                RCBLim = *pREdgeOld + 8; 
                if( *pREdgeOld > 9) RCB = *pREdgeOld - 8;
                else                RCB = 2;
            }   
        }  
        //*--------ȷ������Ѱ��������--------*//
        
        //*--------��ʼѰ�������--------*// 
        if( Find_LeftEdge(LCB,LCBLim,Row) ) 
        {   
            uLosNum_L = 0;    //�ҵ��� �����ۼƴ��� ��0 
        } 
        else 
        {   //�Ҳ������б��� Ԥ�Ȿ�б���λ��
            if( Row ) 
            { 
                if(Row > 1) EdgPredict = (*pLEdgeOld) * 2 - *(pLEdgeOld-1);//����ǰ2��Ԥ��˴�λ�� 
                else        EdgPredict = *pLEdgeOld; 
            }
            else  EdgPredict = 0;
            //����Ԥ����ȷ������λ��
            if( EdgPredict > 0)  
            {
                if(EdgPredict < 160)  *pLEdge = EdgPredict;   
                else                  *pLEdge = 159;               
            } 
            else  *pLEdge = 0;
            
            uLosNum_L++;//���ߴ����Լ�
        }
        //*--------Ѱ������߽���--------*//
        
        //*--------��ʼѰ���ұ���--------*//        
        if( Find_RightEdge(RCB,RCBLim,Row) ) 
        {   
            uLosNum_R = 0;    //�ҵ��� �����ۼƴ��� ��0 
        }   
        else 
        {   //�Ҳ������� Ԥ�Ȿ�б���λ��
            if( Row ) 
            { 
                if(Row > 1) EdgPredict = (*pREdgeOld) * 2 - *(pREdgeOld-1);//����ǰ2��Ԥ��˴�λ�� 
                else        EdgPredict = *pREdgeOld; 
            }
            else  EdgPredict = 159;
            //����Ԥ����ȷ������λ��
            if( EdgPredict > 0)  
            {
                if(EdgPredict < 160)  *pREdge = EdgPredict;   
                else                  *pREdge = 159;               
            } 
            else  *pREdge = 0;
            
            
            uLosNum_R++;
        }
        //*--------Ѱ���ұ��߽���--------*//
        
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
        
        //�ж���Ч�߽� �Ͻ����� �½����� ��ʼ��
            if( ( *pLEdge + *(pLEdge-1) + *(pLEdge-2)  ) < 6 )
            {
                if( Row < 3 )//��һ��ʼ���Ҳ�������
                {
                    uDirJdg   = 0;    //����ȫ�� ��ת 
                    uStpLin_B = 0;    //�½���������ʼ�ж�Ϊ0 ��Ϊ����ȫ��                    
                    uStrFlg   = 1;    //��ʼ��ȷ����־
                    uStrLin   = 0;    //����ȫ�� ��ʼ��Ϊ��һ��
                    *(pLEdgeOld-1)=0;
                    *pLEdgeOld=0;
                    *pLEdge=0;  
                }   
                else
                {
                    if( *pLEdgeOld < 15)              //֮ǰ�ܿ�����߽� 
                    {    
                        uDirJdg   = 0;                //�ж���ת                                       
                        uStpLin_B =  Row - uLosNum_L; //���ʱ ����� Ϊ��Ч��ĩ��
                    }
                    else  if( *pLEdgeOld > 145)       //֮ǰ�����Ҳ�
                    {
                        uStpFlg   = 1;
                        uDirJdg   = 1;                //�ж���ת
                        uStpLin_T =  Row - uLosNum_L; //�ҹ�ʱ ����� Ϊ��Ч��ĩ��
                    }  
                }
            }
        
        if( ( *pREdge + *(pREdge-1) + *(pREdge-2)  ) > 450 )
            {
                if( Row < 3 )
                {
                    uDirJdg   = 1;        //�ж���ת
                    uStpLin_B = 0;        //����ȫ��
                    uStrFlg   = 1;        //��ʼ�б�־
                    uStrLin   = 0;        //��ʼ��Ϊ��һ��
                    *(pREdgeOld-1)=159;
                    *pREdgeOld=159;
                    *pREdge=159;   
                } 
                else 
                {
                    if(*pREdgeOld > 145)//֮ǰ�����Ҳ� 
                    {
                        uDirJdg = 1;
                        uStpLin_B =  Row - uLosNum_R;
                    }
                    else  if(*pREdgeOld < 15)//֮ǰ�������
                    {
                        uStpFlg = 1;
                        uStpLin_T =  Row - uLosNum_R;
                        uDirJdg = 0;
                    }   
                }
            }       
        
        //*--------ȷ����ʼ��--------*// 
        if((uStrFlg||uLosNum_L||uLosNum_R)==0) { uStrFlg = 1; uStrLin = Row;  }
        //*--------��ʼ��--------*//
        *pMidLin = ( (*pLEdge + *pREdge) >> 1 ) ;
        //*--------Ѱ������--------*//
        if(uStpFlg == 1)
        {
            uStpLin_C = ((uStpLin_T + uStpLin_B)>>1);
            if(uDirJdg == 0)  //��ת
            {
                for(i=uStpLin_B; i<uStpLin_C; i++) 
                {
                    *pMidLin = *(pMidLin-1) + *pREdge - *(pREdge-1); 
                }
            }
            else                //��ת
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
        //*--------Ѱ������--------*//
        
    /***********************************һ��ͼ����***********************************/    
    }
}
//*---------------�õ�����---------------*//

//*-----Ѱ�����-----*//
unsigned char Find_LeftEdge(unsigned char CBL,unsigned char CBLimL,unsigned char RRow)
{  
    unsigned char LosFlg = 0; //����Ѱ�ұ�־
    unsigned char CB_L   = CBL;
    unsigned char CBLim_L = CBLimL;  
    
    for(; CB_L > CBLim_L; CB_L--) 
    {                         
        if( Image[RRow][CB_L] == 0 ) 
        {
            //�ж��Ƿ�װ׺ں��������ĸ���,����,��Ϊ��λ����
            if( (Image[RRow][CB_L+2] + Image[RRow][CB_L+1] - Image[RRow][CB_L] - Image[RRow][CB_L-1]) > 500 );//�����ж�               
            {                  
                LosFlg = 1;                   //�ɼ� �ɹ���־ ��1
                LefEdge[RRow] = CB_L;  //��¼�õ��ڸ��е�λ��
                break;                        //�����Ѿ��ɵ���,������ǰ�е�����Ѱ��ѭ��
            }
        } 
    } 
    
    
    /*
    for(; CB_L > CBLim_L; pImage--,CB_L--) 
    {                         
        if( *pImage == 0 ) 
        {
            //�ж��Ƿ�װ׺ں��������ĸ���,����,��Ϊ��λ����
            if( (*(pImage+2) + *(pImage+1) - *pImage - *(pImage-1)) > 500 );//�����ж�               
            {                  
                LosFlg = 1;                   //�ɼ� �ɹ���־ ��1
                *pppEdge = CB_L;  //��¼�õ��ڸ��е�λ��
                break;                        //�����Ѿ��ɵ���,������ǰ�е�����Ѱ��ѭ��
            }
        } 
    } 
    */
    return LosFlg; 
}
//*-----Ѱ�����-----*//

//*-----Ѱ�ұ���-----*//
unsigned char Find_RightEdge(unsigned char CBR,unsigned char CBLimR,unsigned char RRow)
{
    unsigned char LosFlg = 0;//����Ѱ�ұ�־  
    unsigned char CB_R   = CBR;
    unsigned char CBLim_R  = CBLimR;
    //unsigned char *pImage  = pIma;
    //unsigned char *pppEdge = ppEdg;
    
    for(; CB_R < CBLim_R; CB_R++)  
    {                         
        if( Image[RRow][CB_R] == 0 )
        {
            //�ж��Ƿ�װ׺ں��������ĸ���,����,��Ϊ��λ����
            if( (Image[RRow][CB_R-2]  + Image[RRow][CB_R-1]  - Image[RRow][CB_R]  - Image[RRow][CB_R+1] ) > 500 );//�����ж�               
            {                  
                LosFlg = 1;                   //�ɼ� �ɹ���־ ��1
                RigEdge[RRow] = CB_R;  //��¼�õ��ڸ��е�λ��
                break;                        //�����Ѿ��ɵ���,������ǰ�е�����Ѱ��ѭ��
            }
        } 
    } 
    
    /*
    for(; CB_R < CBLim_R; pImage++,CB_R++)  
    {                         
        if( *pImage == 0 ) 
        {
            //�ж��Ƿ�װ׺ں��������ĸ���,����,��Ϊ��λ����
            if( (*(pImage-2) + *(pImage-1) - *pImage - *(pImage+1)) > 500 );//�����ж�               
            {                  
                LosFlg = 1;                   //�ɼ� �ɹ���־ ��1
                *pppEdge = CB_R;  //��¼�õ��ڸ��е�λ��
                break;                        //�����Ѿ��ɵ���,������ǰ�е�����Ѱ��ѭ��
            }
        } 
    } 
    */
    return LosFlg; 
}
//*-----Ѱ�ұ���-----*//

//--------------------ͼ������ȡ��----------------------//
void Ima_GetData(void)
{
    unsigned char ii,jj;
    unsigned char *pp;
    //---------һ��ͼ�����ݶ�ȡ----------//
    if( uCapRow > 30 )  //50 ��ȡ������ڲɼ�
    {  
        Ima_GetDataReady();    
        for( ii=MINROW; ii<RANGEROW; ii++ ) 
        {  
            //if( ii > uCapRow ) Text = 1;  ������ʲôʱ����Զ�ȡ
            pp = &Image[ii][MAXCOL];
            for( jj=MINCOL; jj<RANGECOL; jj++ ) 
            {   
                FIFORCK = 0;FIFORCK = 1;  //FIFORCK 1����ÿ����ȡ1��                        
                FIFORCK = 0;      
                *pp = PORTA;             //��FIFO����;PORTA;
                //�ڵ�һ��Ҷ�Ϊ30-50  �׵�һ��170���� ���ڵ�ή��100���� 
                if(PORTA > THRESHOLD ) *pp = 255; 
                else *pp = 0;
                FIFORCK = 1;    
                pp--;  
            }    
        } 
        uImaGetFlg = 1;    
    }
    //-------һ��ͼ�����ݶ�ȡ--------//
}
//-----------------------ͼ������ȡ��------------------------//
void Ima_GetDataReady(void)
{
    unsigned char ii,jj;
    //---FIFO����λ����---// 
    FIFORRST = 0; //����λʹ�� 
    FIFORCK  = 0; 
    FIFORCK  = 1; 
    FIFORCK  = 0; //FIFOʱ���ź�
    FIFORRST = 1; //����λ�ر�
    FIFORCK  = 1;  
    //---FIFO����λ����---// 
    for(ii=0; ii<2; ii++)  
        for(jj=0; jj<150; jj++)
            {FIFORCK=0;FIFORCK=1;} 
}

//-------------------------ͼ����ʾ--------------------------//
void Ima_Display(void)
{
    unsigned char ii,jj;
    unsigned char *pp;
    //unsigned int  RGB;
    
    //��ֵ��ͼ����ʾ
    pp = &Image[0][0];
    for(ii=0; ii<RANGEROW; ii++) 
    {  
        for(jj=0; jj<RANGECOL; jj++,pp++) 
        {   
            /*
            //RGB��ʽ��ʾ
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
                                                        
    //ͼ��ָ���
    for(ii=0;ii<RANGECOL;ii++) LCD_OutLCD_PutPixel(40,ii,BLACKCOLOR);   
    
    for(ii=0; ii<RANGEROW; ii++) 
    {
        LCD_OutLCD_PutPixel(ii,MidLine[ii],PURPLE);                     //������ʾ
        LCD_OutLCD_PutPixel(ii,LefEdge[ii],BLACKCOLOR);                 //������ʾ
        LCD_OutLCD_PutPixel(ii,RigEdge[ii],BLACKCOLOR);                 //������ʾ
    }
    //ֱ�����ָ����
    for(ii=15; ii<40; ii++) 
    {
        LCD_OutLCD_PutPixel(ii,80,PURPLE);                     //������ʾ
        LCD_OutLCD_PutPixel(ii,50,PURPLE);                 //������ʾ
        LCD_OutLCD_PutPixel(ii,110,PURPLE);                 //������ʾ
    }
    //���ּ��ָ����
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
/*************ͼ����ʾ*************/
unsigned char Road_JdgST(void)
{
  	unsigned char Row;
  	unsigned char *pJdgST;
  	unsigned char LefSTFlg = 1;
  	unsigned char MidSTFlg = 1;
  	unsigned char RigSTFlg = 1;
  	
  	cDirSTRef = 0;

    //�����ж�
    pJdgST = &Image[15][80]; 
  	for( Row=15; Row<RANGEROW; pJdgST+=160,Row++ )
  	{
    		if( *pJdgST == BLACK )
    		{
      			MidSTFlg = 0;
      			break;
    		}
  	}
  	//����ֱ���ж�

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
	//10-39��
	//��95-124
	//��65-26  
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
	//10-39��
	//��95-124
	//��65-26  
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
	//10-39��
	//��95-124
	//��65-26  
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


/*************�������***************/
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

    //����1λ   ����      �������� 
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

/*************�������***************/


/* END Sunny3 */
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.02 [04.44]
**     for the Freescale HCS12X series of microcontrollers.
**
** ###################################################################
*/
