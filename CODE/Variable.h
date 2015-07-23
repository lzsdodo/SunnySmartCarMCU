#ifndef __Variable_H
#define __Variable_H
/*
	变量定义格式:	fSpeedKp;
	开头:	变量类型 bool,unsigned char,int,word,float,long,int 对应 b,u,n,w,f,ln
	次者:	硬件相关描述 电机Motor 
	后者:	细节描述
	最后:	相关补充
*/ 

//---------------------------------定义---------------------------------//
#define FIFORCK   PORTE_PE6 
#define FIFORRST  PORTE_PE4
#define FIFOWRITE PORTE_PE5        //????????纸上写的FIFOWRITE  对,改回来了
#define FIFOWRST  PORTE_PE7

#define MEASURE0  PORTB_PB0
#define MEASURE1  PORTB_PB1
#define MEASURE2  PORTB_PB2
#define MEASURE3  PORTB_PB3

#define LED0      PTT_PTT4
#define LED1      PTT_PTT6

#define RED         0xF800      //红色
#define PURPLE      0xA0B4      //紫色
#define GREEN       0x07E0      //绿色
#define BLUE        0x001F      //蓝色
#define LIGHTBLUE   0x0CF0      //浅蓝色
#define GREY        0x8410      //灰色
#define WHITECOLOR  0xffff      //白色
#define BLACKCOLOR  0           //黑色

#define THRESHOLD   120         //二值化定义   >130 白 <130 黑
#define BLACK		    0		        //二值化后黑色定义	
#define WHITE	      0xff	      //二值化后白色定义

#define MAXROW		  39	    	  //末尾行	39
#define MINROW		  0		        //起始行	0
#define RANGEROW	  40		      //行范围	40行
#define MAXCOL		  159	        //末尾列	159
#define MINCOL		  0		        //起始列	0
#define RANGECOL	  160		      //列范围	160列

#define STRAIGHT	  0		        //直路标志
#define CORNER	  	1		        //弯路标志
#define PAVEMENT	  2		        //人字路标志


#define ANGLEOFFSET 58.8

//unsigned char Threshold = 0;(程序不好用,暂时先固定不作处理)
//unsigned char Text = 0;


signed char cDirSTRef = 0;    //直道方向辅助判断值
signed char uSTMode[2][2][2] = {0};
//  Lef Mid Rig
//  L+M-R : 小斜率 uSTMode[1][1][0] = -8; uSTMode[0][1][1] = 8;
//  L+M+R : fDirKp=fDirBST=
//  L-M-R : uSTMode[1][0][0] = -12;     -L-M+R :  uSTMode[0][1][1] = 12;


unsigned char uStpTime = 0;
unsigned char uMotFlg  = 0;      //左右电机选择标志
unsigned char uSWValue = 0;      //拨码开关变量

unsigned char uRoadSym = 0;      //道路标志

unsigned char uRoadSymOld = 0;

//人字检测判断
unsigned char uRenJdgSym = 0;

unsigned char uImaFlg = 0;        //观察图像标志
unsigned char uDFFlg  = 0;        //存入DFLASH的标志
unsigned char uStrReady = 1;      //开车标志位
unsigned char uStpReady = 0;      //停车标志位


//float fDirWatch = 0;              //方向控制量的变化
//float fDirOutWatch = 0;           //方向控制输出量的变化

//-------------直立PID参数初始化-------------//
float fAngleKp = 347; //300 330         450//角度比例系数(Proportion)
float fAngleKd = 9;   //6 8             20//角度微分系数(Derivative)
//float fAngleT  = 0.005;               //角度读取计算周期(5ms)

//-------------速度PID参数初始化-------------//
float fSpeedKp = 60;  //45                2//比例系数(Proportion)
float fSpeedKi = 0.25;   //1.5              0.04//积分系数(Integral)
//float fSpeedT  = 0.1;             //采样周期(Period)(100ms,50ms读一个电机)

//-------------方向PID参数初始化-------------//
float fDirKp   = 26;//150               //方向比例系数(Proportion)
float fDirKpPM = 170;
float fDirKpCN = 26;
float fDirKd   = 3;               //方向微分系数(Derivative)
float fAngleOffSet = ANGLEOFFSET;       //直立时角度值

//-------------速度设定变量-------------//
int nSpeedSet = 0;    //速度设定值
int nSpeedAvr = 0;    //左右电机速度平均值作为实际速度
int nSpeedLef = 0;    //左电机速度实际值
int nSpeedRig = 0;    //右电机速度实际值

//-------------各参数归一化赋予电机变量-------------//
float fAngleOut       = 0;
float fSpeedOut       = 0;
float fSpeedIntergral = 0;
float fDirOut         = 0;
float fSpeedOutSuperp = 0;
//int   nSpeedEK        = 0;

//-------------方向控制量-------------//
int   nDirSum = 0;
float fDir    = 0;  //方向
unsigned char uDirZero = 80 ;  

//-------------ms定时器标志-------------//
//unsigned long int uPitms  = 0;    //1ms中断自加标志量
unsigned char uPit5msFlg  = 0;    //1ms内中的5ms标志
unsigned char uPit100msFlg  = 0;    //1ms内中的5ms标志
word  w5msCount = 0;              //5ms计数
word  wImaCount = 0;              //主循环计数  用于计算图像处理一次经过多少次5ms计数



//-------------直立角度相关参数-------------//
float fAngleHB   = ANGLEOFFSET;       //互补滤波后角度计算值
float fAngleATan = ANGLEOFFSET;       //2轴加速度Atan计算值
//float fAngleEK   = 0;               //mG_delta;  互补滤波参数1
//float fAngleTg = 2;                 //Tg=2;       跟随时间

float fXAngleSpeed  = 0;            //陀螺仪计算角速度值
float fYAngleSpeed  = 0;

//-------------MMA8451 & L3G4200D-------------//
unsigned char uMMA8451ID  = 0;  //加速度计ID验证I2C程序
unsigned char uL3g4200dID = 0;  //陀螺仪ID验证I2C程序  
//xGyroOFFSET=10;yGyroOFFSET=11;zGyroOFFSET=3;
int nXGyroDataOFFSET = 10;          //陀螺仪零漂值
int nYGyroDataOFFSET = 11;



//-------------摄像头相关-------------//
unsigned char Image[40][160] = {0};  //图像数组
unsigned char LefEdge[40] = {0};
unsigned char RigEdge[40] = {0};
unsigned char MidLine[40] = {0};

unsigned char uImaCapFlg = 0;        //图像采集标志
unsigned char uImaGetFlg = 0;        //图像获得标志
unsigned char uCapRow  = 0;          //采集行数

unsigned char uDirJdg   = 0;    //向左左为0 ; 向右转为1;

unsigned char uLosNum_L = 0;
unsigned char uLosNum_R = 0;

unsigned char uStrFlg = 0;
unsigned char uStpFlg  = 0;
unsigned char uStrLin = 0;
unsigned char uStpLin_B = 0;
unsigned char uStpLin_T = 0;
unsigned char uStpLin_C = 0;
unsigned char uRenJdgNum = 7;


unsigned char *pRigJdgPM[30] = { 
&Image[10][95],&Image[11][96],&Image[12][97],&Image[13][98],&Image[14][99],
&Image[15][100],&Image[16][101],&Image[17][102],&Image[18][103],&Image[19][104],
&Image[20][105],&Image[21][106],&Image[22][107],&Image[23][108],&Image[24][109],
&Image[25][110],&Image[26][111],&Image[27][112],&Image[28][113],&Image[29][114],
&Image[30][115],&Image[31][116],&Image[32][117],&Image[33][118],&Image[34][119],
&Image[35][120],&Image[36][121],&Image[37][122],&Image[38][123],&Image[39][124]
};
unsigned char *pRigJdgPM2[30] = { 
&Image[10][110],&Image[11][111],&Image[12][112],&Image[13][113],&Image[14][114],
&Image[15][115],&Image[16][116],&Image[17][117],&Image[18][118],&Image[19][119],
&Image[20][120],&Image[21][121],&Image[22][122],&Image[23][123],&Image[24][124],
&Image[25][125],&Image[26][126],&Image[27][127],&Image[28][128],&Image[29][129],
&Image[30][130],&Image[31][131],&Image[32][132],&Image[33][133],&Image[34][134],
&Image[35][135],&Image[36][136],&Image[37][137],&Image[38][138],&Image[39][139]
};

#endif //__Variable_H





