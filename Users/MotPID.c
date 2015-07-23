#include "MotPID.h"

//********************电机初始化********************
void  Mot_Init(void)
{
    //速度初始化为0
    Mot_SetLefPWM(0,0,0);
    Mot_SetRigPWM(0,0,0);
}
//********************电机速度设定********************
void Mot_SetLefPWM(float AngleOut,float SpeedOut,float DirOut) //共77us 执行1次39us  
{   
    long int AllOut = 0;
    
      
    //电机速度设置
    AllOut = (long int)((AngleOut - SpeedOut + DirOut) * 16.4 );
    
    if( AllOut >= 0 ) 
    {
        AllOut += 1500;      //DeadlinePWM=1500   
        if( AllOut > 65530 )  AllOut = 65530; 
        MotLefSetDir_SetVal();//正转
        while(MotLefPWM_SetRatio16((word)AllOut)); 
    }
    else              
    {
        AllOut += 64040;    //-1500 +65535
        if( AllOut < 0 )    AllOut = 0;
        MotLefSetDir_ClrVal();//反转
        while(MotLefPWM_SetRatio16((word)AllOut));
    } 
}

void Mot_SetRigPWM(float AngleOut,float SpeedOut,float DirOut) //共77us 执行1次39us  
{   
    long int AllOut = 0;
    

      
    //电机速度设置
    AllOut = (long int)((AngleOut - SpeedOut - DirOut) * 16.4 );
    
    if( AllOut >= 0 ) 
    {
        AllOut += 1500;      //DeadlinePWM=1500   
        if( AllOut > 65530 )  AllOut = 65530; 
        MotRigSetDir_SetVal();//正转
        while(MotRigPWM_SetRatio16((word)AllOut));
    }
    else              
    {
        AllOut += 64040;    //-1500 +65535
        if( AllOut < 0 )    AllOut = 0;
        MotRigSetDir_ClrVal();//反转        
        while(MotRigPWM_SetRatio16((word)AllOut));
    } 
}
  

//******************电机控制函数****************************************
void Speed_RealCal(void) //2us 
{   
    if( uMotFlg == 0 ) 
    {   //左电机 
        //转向获取并计算带方向的实际速度值
        MotLefGetSpeed_SetVal();
        MotRigGetSpeed_ClrVal();
        if(MotLefGetDir_GetVal())
            nSpeedLef += ( 0 - Pulse_GetCounterValue() );   //PTT_PTT0=0;左电机正转 
        else                      
            nSpeedLef += Pulse_GetCounterValue();          //T7口读取脉冲数,获取电机转速 
        Pulse_ResetCounter();       
        uMotFlg = 1;
    } 
    else
    {   //右电机   
        //转向获取并计算带方向的实际速度值
        MotRigGetSpeed_SetVal();
        MotLefGetSpeed_ClrVal();  
        if(MotRigGetDir_GetVal()) 
            nSpeedRig += Pulse_GetCounterValue();             // PTT_PTT3=1;右电机正转
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
    
    //得到速度 和清零
    nSpeedAvr = (nSpeedLef + nSpeedRig) >> 1; 
    nSpeedLef = 0; 
    nSpeedRig = 0; 
        

    //PID算法 
    //SpeedEKOld = nSpeedEK;
    SpeedEK = nSpeedSet - nSpeedAvr;
    //SpeedEKDelta = nSpeedEK - SpeedEKOld;
    //*************位置式PID*************
    
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
    
    //*************增量式PID*************
    /*
    //保护
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
 
void Dir_PID(void)     //方向，左正右负
{
    float DirTmp1;
    float DirTmp2;
    //fDirSuperp = fDirKp*fDirection - fDirKd*fYAngleSpeed;  
    DirTmp1 = fDirKp * fDir;
    DirTmp2 = fDirKd * fYAngleSpeed;
    fDirOut  = DirTmp1 - DirTmp2 ;        // 3.5*5msFlg = 1ImaFlg / 3.5; 左右均分 /2; --/7
    
}

