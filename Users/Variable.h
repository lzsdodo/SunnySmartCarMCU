#ifndef __Variable_H
#define __Variable_H
/*
	���������ʽ:	fSpeedKp;
	��ͷ:	�������� bool,unsigned char,int,word,float,long,int ��Ӧ b,u,n,w,f,ln
	����:	Ӳ��������� ���Motor 
	����:	ϸ������
	���:	��ز���
*/ 

#define ANGLEOFFSET 57
//**************************************
unsigned char WhichMotor;              //���ҵ��ѡ���־
//*--------���뿪�ر���--------*//
unsigned char uSWValue = 0;   
//******** text ********
word  w5msCount = 0;      //5ms����
word  wImaCount = 0;  //��ѭ������  ���ڼ���ͼ����һ�ξ������ٴ�5ms����

//**************************������****************************************************
//*************�ٶ�PID������ʼ��*************
//���������ر���
//*************ֱ��PID������ʼ��*************
float fAngleKp = 280; //300 330         //�Ƕȱ���ϵ��(Proportion)
float fAngleKd = 12;   //6 8             //�Ƕ�΢��ϵ��(Derivative)
//float fAngleT  = 0.005;           //�Ƕȶ�ȡ��������(5ms)
//*************�ٶ�PID������ʼ��*************
float fSpeedKp = 40;  //45                //����ϵ��(Proportion)
float fSpeedKi = 0.2;   //1.5              //����ϵ��(Integral)
//float fSpeedT  = 0.1;             //��������(Period)(100ms,50ms��һ�����)
//*************����PID������ʼ��*************
float fDirKp   = 0;//150               //�������ϵ��(Proportion)
float fDirKd   = 24;               //����΢��ϵ��(Derivative)
float fAngleOffSet = ANGLEOFFSET;       //ֱ��ʱ�Ƕ�ֵ
//����DFLASH�ı�־
//unsigned char uDFalshFlg = 0;

//*************�ٶ��趨����*************
int nSpeedSet = 0;    //�ٶ��趨ֵ
int nSpeedAvr = 0;    //���ҵ���ٶ�ƽ��ֵ��Ϊʵ���ٶ�
int nSpeedLef = 0;    //�����ٶ�ʵ��ֵ
int nSpeedRig = 0;    //�ҵ���ٶ�ʵ��ֵ

//*************��������һ������������*************
float fAngleOut = 0;
float fSpeedOut = 0;
float fDirOut   = 0;
float fSpeedOutSuperp   = 0;

//***************
int   nDirSum = 0;
float fDir    = 0;  //����
float fDirOld = 0;
float fDirDelta = 0;
unsigned char uDirZero = 80 ;  



//*************ֱ���Ƕ���ز���*************
float fAngleHB   = ANGLEOFFSET;        //�����˲���Ƕȼ���ֵ
float fAngleATan = ANGLEOFFSET;       //2����ٶ�Atan����ֵ
float fAngleEK   = 0;               //mG_delta;  �����˲�����1
//float fAngleTg = 1;             //Tg=2;       ����ʱ��

float fXAngleSpeed  = 0;            //�����Ǽ�����ٶ�ֵ
//float fYAngleSpeed  = 0; 
float fYAngleSpeedSum = 0;



//-------------MMA8451 & L3G4200D-------------//
unsigned char uMMA8451ID  = 0;  //���ٶȼ�ID��֤I2C����
unsigned char uL3g4200dID = 0;  //������ID��֤I2C����  
//xGyroOFFSET=10;yGyroOFFSET=11;zGyroOFFSET=3;
int nXGyroDataOFFSET = 10;          //��������Ưֵ
int nYGyroDataOFFSET = 11;
//AngleSpeed=GyroData*0.07
//unsigned char AccData[4]  = {0};    //���ٶȼ�ֱ�Ӷ�ȡ����
//unsigned char GyroData[4] = {0};    //������ֱ�Ӷ�ȡ����

//-------------ms��ʱ����־-------------//
unsigned char uPitms     = 0;    //1ms�ж��Լӱ�־��
unsigned char uPit5msFlg = 0;    //1ms���е�5ms��־

//-------------��������������-------------//
int nSpeedEK    = 0;        //��k�β���ʱ��ƫ��ֵ
//int nSpeedEKOld = 0;
float fSpeedIntergral = 0;
//-------------����ͷ���-------------//
unsigned char Image[40][160] = {0};  //ͼ������
unsigned char LefEdge[40] = {0};
unsigned char RigEdge[40] = {0};
unsigned char MidLine[40] = {0};

unsigned char uImaCapFlg = 0;        //ͼ��ɼ���־
unsigned char uImaGetFlg = 0;        //ͼ���ñ�־
unsigned char uCapRow  = 0;          //�ɼ�����

unsigned char uLosNum_L = 0;
unsigned char uLosNum_R = 0;

unsigned char uStrFlg = 0;
unsigned char uStpFlg  = 0;
unsigned char uStrLin = 0;
unsigned char uStpLin_B = 0;
unsigned char uStpLin_T = 0;
unsigned char uStpLin_C = 0;


unsigned char uDirJdg   = 0;    //������Ϊ0 ; ����תΪ1;
/*
//��·��Ϣʶ���־
unsigned char uSmSFlg = 0;
unsigned char uBiSFlg   = 0;
unsigned char uCrsFlg  = 0;
unsigned char uRenFlg    = 0;
*/

        
signed int Run_Times = 0;
/**************************��������******************************/
unsigned char i = 0;
unsigned char j = 0;      //ѭ����ʱ����
unsigned char *p;

#endif //__Variable_H





