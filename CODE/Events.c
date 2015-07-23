/** ###################################################################
**     Filename  : Events.c
**     Project   : Sunny3
**     Processor : MC9S12XS128MAA
**     Component : Events
**     Version   : Driver 01.04
**     Compiler  : CodeWarrior HCS12X C Compiler
**     Date/Time : 2014/5/11, 20:53
**     Abstract  :
**         This is user's event module.
**         Put your event handler code here.
**     Settings  :
**     Contents  :
**         No public methods
**
** ###################################################################*/
/* MODULE Events */


#include "Cpu.h"
#include "Events.h"

/* User includes (#include below this line is not maintained by Processor Expert) */
#include "IO_Map.h"
#include "Sensor.h"
#include "MotPID.h"


#define FIFOWRITE PORTE_PE5  
#define FIFOWRST  PORTE_PE7

#pragma CODE_SEG DEFAULT

/*
** ===================================================================
**     Event       :  Pitms_OnInterrupt (module Events)
**
**     Component   :  Pitms [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

//ms��ʱ����־����
extern unsigned char uPit5msFlg; 
extern unsigned char uPit100msFlg; 
extern word w5msCount;              
//�����ر���
extern float fAngleOut;
extern float fSpeedOut;
extern float fSpeedOutSuperp;
extern float fDirOut;

void Pitms_OnInterrupt(void)
{
  /* Write your code here ... */
  
    /****************************���Ĳ���****************************/                           
    Cpu_EnableInt();
    //ms�����Լ�
    uPit5msFlg++;  //5msһ���������� 
    switch( uPit5msFlg ) 
    {
      case 1:         //5ms�������
              uPit100msFlg+=5;
              Speed_RealCal();       //2us �ٶȻ�ȡ��ʵ���ٶȼ���  
              if( uPit100msFlg >= 100 )
              {   
                  uPit100msFlg = 0;
                  Speed_PID();//44us     
              }   
              break;           
      case 2:   
              Acc_DataRead();
              break;//692 us (i2c��ʱi=5) 
      case 3: 
              Gyro_DataRead();
              break;//430 us   
      case 4: 
              Sensor_PID();//82us             //�ĺ�δ�� 
              //PWM���
              fSpeedOut += fSpeedOutSuperp;
              Mot_SetLefPWM(fAngleOut,fSpeedOut,fDirOut);
              Mot_SetRigPWM(fAngleOut,fSpeedOut,fDirOut);   
              break; //77us 
      case 5: 
              uPit5msFlg=0;
              w5msCount++;
              break;
      default:break;
    } 
    /****************************���Ĳ���****************************/        
    
}

/*
** ===================================================================
**     Event       :  SCI_OnRxChar (module Events)
**
**     Component   :  SCI [AsynchroSerial]
**     Description :
**         This event is called after a correct character is received.
**         The event is available only when the <Interrupt
**         service/event> property is enabled and either the <Receiver>
**         property is enabled or the <SCI output mode> property (if
**         supported) is set to Single-wire mode.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

//extern int nSpeedSet;
//extern float fDirKp;
//extern float fSpeedOut;
// unsigned char uDFFlg;
/*
void SCI_OnRxChar(void)
{
*/
  /* Write your code here ... */
 /* 
  unsigned char Rece;
  while(SCI_RecvChar(&Rece));
  switch(Rece) 
  {
  */
    /*
    case 'z':nSpeedSet = 0;break;
    case 'a':nSpeedSet = 32;break; 
    case 'b':nSpeedSet = 64;break;
    case 'c':nSpeedSet = 96;break; 
    case 'd':nSpeedSet = 128;break;
    default:break;
    */
    /*
    case 'a':nSpeedSet += 32;break; 
    case 'b':nSpeedSet -= 32;break;
    //case 'c':fSpeedKp += 5;break; 
    //case 'd':fSpeedKi += 0.5;break;
    case 'z':nSpeedSet = 0;fSpeedOut = 0;break;
    case 's':uDFFlg = 1;break;
    default:break;
    
  }
}
*/

/*
** ===================================================================
**     Event       :  HREF_OnInterrupt (module Events)
**
**     Component   :  HREF [ExtInt]
**     Description :
**         This event is called when an active signal edge/level has
**         occurred.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

extern unsigned char uImaCapFlg;
extern unsigned char uCapRow;

void HREF_OnInterrupt(void)
{
  /* place your HREF interrupt procedure body here */
  
  /***********************���жϺ��Ĳ���***********************/
    //�����в�һ��
    if( (uCapRow % 5) == 0 )  FIFOWRITE = 1;//дʹ��(���������)
    else                      FIFOWRITE = 0;//д�ر�(���������)
    if( uCapRow < 3 )         FIFOWRITE = 1; 
    //�ɼ���ɴ���
    if( uCapRow > 200 ) 
    { 
        FIFOWRITE  = 0;  //д�ر�(���������)
        uImaCapFlg = 1;
        HREF_Disable();  //�ر����ж� 
    } 
    //�ɼ����������Լ�;��òɼ���־
    uCapRow++;
    /***********************���жϺ��Ĳ���***********************/

}


/*
** ===================================================================
**     Event       :  VSYNC_OnInterrupt (module Events)
**
**     Component   :  VSYNC [ExtInt]
**     Description :
**         This event is called when an active signal edge/level has
**         occurred.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void VSYNC_OnInterrupt(void)
{
  /* place your VSYNC interrupt procedure body here */
  
  /***********************���жϺ��Ĳ���***********************/
  VSYNC_Disable(); //���жϹر�;
  HREF_Enable();   //���жϿ���;
  FIFOWRST = 0;           //д��λ
  FIFOWRST = 1;    
  /***********************���жϺ��Ĳ���***********************/ 

}


/*
** ===================================================================
**     Event       :  SCI_OnTxChar (module Events)
**
**     Component   :  SCI [AsynchroSerial]
**     Description :
**         This event is called after a character is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void SCI_OnTxChar(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  STOPTIME_OnInterrupt (module Events)
**
**     Component   :  STOPTIME [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void STOPTIME_OnInterrupt(void)
{
  /* Write your code here ... */
}

/* END Events */

/*
** ###################################################################
**
**     This file was created by Processor Expert 3.02 [04.44]
**     for the Freescale HCS12X series of microcontrollers.
**
** ###################################################################
*/
