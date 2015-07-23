#ifndef __Variable_H
#define __Variable_H
/*
	变量定义格式:	fSpeedKp;
	开头:	变量类型 bool,unsigned char,int,word,float,long,int 对应 b,u,n,w,f,ln
	次者:	硬件相关描述 电机Motor 
	后者:	细节描述
	最后:	相关补充
*/ 

#define ANGLEOFFSET 57
//**************************************
unsigned char WhichMotor;              //左右电机选择标志
//*--------拨码开关变量--------*//
unsigned char uSWValue = 0;   
//******** text ********
word  w5msCount = 0;      //5ms计数
word  wImaCount = 0;  //主循环计数  用于计算图像处理一次经过多少次5ms计数

//**************************电机相关****************************************************
//*************速度PID参数初始化*************
//电机控制相关变量
//*************直立PID参数初始化*************
float fAngleKp = 280; //300 330         //角度比例系数(Proportion)
float fAngleKd = 12;   //6 8             //角度微分系数(Derivative)
//float fAngleT  = 0.005;           //角度读取计算周期(5ms)
//*************速度PID参数初始化*************
float fSpeedKp = 40;  //45                //比例系数(Proportion)
float fSpeedKi = 0.2;   //1.5              //积分系数(Integral)
//float fSpeedT  = 0.1;             //采样周期(Period)(100ms,50ms读一个电机)
//*************方向PID参数初始化*************
float fDirKp   = 0;//150               //方向比例系数(Proportion)
float fDirKd   = 24;               //方向微分系数(Derivative)
float fAngleOffSet = ANGLEOFFSET;       //直立时角度值
//存入DFLASH的标志
//unsigned char uDFalshFlg = 0;

//*************速度设定变量*************
int nSpeedSet = 0;    //速度设定值
int nSpeedAvr = 0;    //左右电机速度平均值作为实际速度
int nSpeedLef = 0;    //左电机速度实际值
int nSpeedRig = 0;    //右电机速度实际值

//*************各参数归一化赋予电机变量*************
float fAngleOut = 0;
float fSpeedOut = 0;
float fDirOut   = 0;
float fSpeedOutSuperp   = 0;

//***************
int   nDirSum = 0;
float fDir    = 0;  //方向
float fDirOld = 0;
float fDirDelta = 0;
unsigned char uDirZero = 80 ;  



//*************直立角度相关参数*************
float fAngleHB   = ANGLEOFFSET;        //互补滤波后角度计算值
float fAngleATan = ANGLEOFFSET;       //2轴加速度Atan计算值
float fAngleEK   = 0;               //mG_delta;  互补滤波参数1
//float fAngleTg = 1;             //Tg=2;       跟随时间

float fXAngleSpeed  = 0;            //陀螺仪计算角速度值
//float fYAngleSpeed  = 0; 
float fYAngleSpeedSum = 0;



//-------------MMA8451 & L3G4200D-------------//
unsigned char uMMA8451ID  = 0;  //加速度计ID验证I2C程序
unsigned char uL3g4200dID = 0;  //陀螺仪ID验证I2C程序  
//xGyroOFFSET=10;yGyroOFFSET=11;zGyroOFFSET=3;
int nXGyroDataOFFSET = 10;          //陀螺仪零漂值
int nYGyroDataOFFSET = 11;
//AngleSpeed=GyroData*0.07
//unsigned char AccData[4]  = {0};    //加速度计直接读取数据
//unsigned char GyroData[4] = {0};    //陀螺仪直接读取数据

//-------------ms定时器标志-------------//
unsigned char uPitms     = 0;    //1ms中断自加标志量
unsigned char uPit5msFlg = 0;    //1ms内中的5ms标志

//-------------霍尔传感器数据-------------//
int nSpeedEK    = 0;        //第k次采样时的偏差值
//int nSpeedEKOld = 0;
float fSpeedIntergral = 0;
//-------------摄像头相关-------------//
unsigned char Image[40][160] = {0};  //图像数组
unsigned char LefEdge[40] = {0};
unsigned char RigEdge[40] = {0};
unsigned char MidLine[40] = {0};

unsigned char uImaCapFlg = 0;        //图像采集标志
unsigned char uImaGetFlg = 0;        //图像获得标志
unsigned char uCapRow  = 0;          //采集行数

unsigned char uLosNum_L = 0;
unsigned char uLosNum_R = 0;

unsigned char uStrFlg = 0;
unsigned char uStpFlg  = 0;
unsigned char uStrLin = 0;
unsigned char uStpLin_B = 0;
unsigned char uStpLin_T = 0;
unsigned char uStpLin_C = 0;


unsigned char uDirJdg   = 0;    //向左左为0 ; 向右转为1;
/*
//道路信息识别标志
unsigned char uSmSFlg = 0;
unsigned char uBiSFlg   = 0;
unsigned char uCrsFlg  = 0;
unsigned char uRenFlg    = 0;
*/

        
signed int Run_Times = 0;
/**************************变量声明******************************/
unsigned char i = 0;
unsigned char j = 0;      //循环临时变量
unsigned char *p;

#endif //__Variable_H





