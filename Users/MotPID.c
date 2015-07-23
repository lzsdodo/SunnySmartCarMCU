#include "MotPID.h"

//********************�����ʼ��********************
void  Mot_Init(void)
{
    //�ٶȳ�ʼ��Ϊ0
    Mot_SetLefPWM(0,0,0);
    Mot_SetRigPWM(0,0,0);
}
//********************����ٶ��趨********************
void Mot_SetLefPWM(float AngleOut,float SpeedOut,float DirOut) //��77us ִ��1��39us  
{   
    long int AllOut = 0;
    
      
    //����ٶ�����
    AllOut = (long int)((AngleOut - SpeedOut + DirOut) * 16.4 );
    
    if( AllOut >= 0 ) 
    {
        AllOut += 1500;      //DeadlinePWM=1500   
        if( AllOut > 65530 )  AllOut = 65530; 
        MotLefSetDir_SetVal();//��ת
        while(MotLefPWM_SetRatio16((word)AllOut)); 
    }
    else              
    {
        AllOut += 64040;    //-1500 +65535
        if( AllOut < 0 )    AllOut = 0;
        MotLefSetDir_ClrVal();//��ת
        while(MotLefPWM_SetRatio16((word)AllOut));
    } 
}

void Mot_SetRigPWM(float AngleOut,float SpeedOut,float DirOut) //��77us ִ��1��39us  
{   
    long int AllOut = 0;
    

      
    //����ٶ�����
    AllOut = (long int)((AngleOut - SpeedOut - DirOut) * 16.4 );
    
    if( AllOut >= 0 ) 
    {
        AllOut += 1500;      //DeadlinePWM=1500   
        if( AllOut > 65530 )  AllOut = 65530; 
        MotRigSetDir_SetVal();//��ת
        while(MotRigPWM_SetRatio16((word)AllOut));
    }
    else              
    {
        AllOut += 64040;    //-1500 +65535
        if( AllOut < 0 )    AllOut = 0;
        MotRigSetDir_ClrVal();//��ת        
        while(MotRigPWM_SetRatio16((word)AllOut));
    } 
}
  

//******************������ƺ���****************************************
void Speed_RealCal(void) //2us 
{   
    if( uMotFlg == 0 ) 
    {   //���� 
        //ת���ȡ������������ʵ���ٶ�ֵ
        MotLefGetSpeed_SetVal();
        MotRigGetSpeed_ClrVal();
        if(MotLefGetDir_GetVal())
            nSpeedLef += ( 0 - Pulse_GetCounterValue() );   //PTT_PTT0=0;������ת 
        else                      
            nSpeedLef += Pulse_GetCounterValue();          //T7�ڶ�ȡ������,��ȡ���ת�� 
        Pulse_ResetCounter();       
        uMotFlg = 1;
    } 
    else
    {   //�ҵ��   
        //ת���ȡ������������ʵ���ٶ�ֵ
        MotRigGetSpeed_SetVal();
        MotLefGetSpeed_ClrVal();  
        if(MotRigGetDir_GetVal()) 
            nSpeedRig += Pulse_GetCounterValue();             // PTT_PTT3=1;�ҵ����ת
        else                      
            nSpeedRig += ( 0 - Pulse_GetCounterValue() );  
        Pulse_ResetCounter(); 
        uMotFlg = 0;     
    }
}

void Speed_PID(void)//44us 
{
    float SpeedTmp1;
    //float SpeedTmp2;
    int SpeedEK; 
    //int SpeedEKOld;
    //int SpeedEKDelta;
    
    //�õ��ٶ� ������
    nSpeedAvr = (nSpeedLef + nSpeedRig) >> 1; 
    nSpeedLef = 0; 
    nSpeedRig = 0; 
        

    //PID�㷨 
    //SpeedEKOld = nSpeedEK;
    SpeedEK = nSpeedSet - nSpeedAvr;
    //SpeedEKDelta = nSpeedEK - SpeedEKOld;
    //*************λ��ʽPID*************
    
    SpeedTmp1 = fSpeedKp * SpeedEK;   
    if(SpeedTmp1>1700) 
        SpeedTmp1 = 1700;
    else if(SpeedTmp1<-1700) 
        SpeedTmp1 = -1700;
    
    
    fSpeedIntergral += fSpeedKi * SpeedEK;
    if(fSpeedIntergral > 4000)      
        fSpeedIntergral = 4000;
    else if(fSpeedIntergral < 0) 
        fSpeedIntergral = 0;
 
    fSpeedOutSuperp=( SpeedTmp1 + fSpeedIntergral - fSpeedOut ) / 20.0;
    
    //*************����ʽPID*************
    /*
    //����
    if(SpeedEKDelta > 50)        
        SpeedEKDelta = 50;
    else if(SpeedEKDelta < -50)  
        SpeedEKDelta = -50;  
    //fSpeedOutSuperp =(fSpeedKp*nSpeedEKDelta + fSpeedKi*nSpeedEK)/20.0;
    SpeedTmp1 = fSpeedKp * SpeedEKDelta;
    SpeedTmp2 = fSpeedKi * nSpeedEK; 
    fSpeedOutSuperp = (SpeedTmp1 + SpeedTmp2) / 20.0;
    */

    
}
 
void Dir_PID(void)     //���������Ҹ�
{
    float DirTmp1;
    float DirTmp2;
    //fDirSuperp = fDirKp*fDirection - fDirKd*fYAngleSpeed;  
    DirTmp1 = fDirKp * fDir;
    DirTmp2 = fDirKd * fYAngleSpeed;
    fDirOut  = DirTmp1 - DirTmp2 ;        // 3.5*5msFlg = 1ImaFlg / 3.5; ���Ҿ��� /2; --/7
    
}

