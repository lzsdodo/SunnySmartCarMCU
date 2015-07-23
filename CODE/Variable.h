#ifndef __Variable_H
#define __Variable_H
/*
	���������ʽ:	fSpeedKp;
	��ͷ:	�������� bool,unsigned char,int,word,float,long,int ��Ӧ b,u,n,w,f,ln
	����:	Ӳ��������� ���Motor 
	����:	ϸ������
	���:	��ز���
*/ 

//---------------------------------����---------------------------------//
#define FIFORCK   PORTE_PE6 
#define FIFORRST  PORTE_PE4
#define FIFOWRITE PORTE_PE5        //????????ֽ��д��FIFOWRITE  ��,�Ļ�����
#define FIFOWRST  PORTE_PE7

#define MEASURE0  PORTB_PB0
#define MEASURE1  PORTB_PB1
#define MEASURE2  PORTB_PB2
#define MEASURE3  PORTB_PB3

#define LED0      PTT_PTT4
#define LED1      PTT_PTT6

#define RED         0xF800      //��ɫ
#define PURPLE      0xA0B4      //��ɫ
#define GREEN       0x07E0      //��ɫ
#define BLUE        0x001F      //��ɫ
#define LIGHTBLUE   0x0CF0      //ǳ��ɫ
#define GREY        0x8410      //��ɫ
#define WHITECOLOR  0xffff      //��ɫ
#define BLACKCOLOR  0           //��ɫ

#define THRESHOLD   120         //��ֵ������   >130 �� <130 ��
#define BLACK		    0		        //��ֵ�����ɫ����	
#define WHITE	      0xff	      //��ֵ�����ɫ����

#define MAXROW		  39	    	  //ĩβ��	39
#define MINROW		  0		        //��ʼ��	0
#define RANGEROW	  40		      //�з�Χ	40��
#define MAXCOL		  159	        //ĩβ��	159
#define MINCOL		  0		        //��ʼ��	0
#define RANGECOL	  160		      //�з�Χ	160��

#define STRAIGHT	  0		        //ֱ·��־
#define CORNER	  	1		        //��·��־
#define PAVEMENT	  2		        //����·��־


#define ANGLEOFFSET 58.8

//unsigned char Threshold = 0;(���򲻺���,��ʱ�ȹ̶���������)
//unsigned char Text = 0;


signed char cDirSTRef = 0;    //ֱ���������ж�ֵ
signed char uSTMode[2][2][2] = {0};
//  Lef Mid Rig
//  L+M-R : Сб�� uSTMode[1][1][0] = -8; uSTMode[0][1][1] = 8;
//  L+M+R : fDirKp=fDirBST=
//  L-M-R : uSTMode[1][0][0] = -12;     -L-M+R :  uSTMode[0][1][1] = 12;


unsigned char uStpTime = 0;
unsigned char uMotFlg  = 0;      //���ҵ��ѡ���־
unsigned char uSWValue = 0;      //���뿪�ر���

unsigned char uRoadSym = 0;      //��·��־

unsigned char uRoadSymOld = 0;

//���ּ���ж�
unsigned char uRenJdgSym = 0;

unsigned char uImaFlg = 0;        //�۲�ͼ���־
unsigned char uDFFlg  = 0;        //����DFLASH�ı�־
unsigned char uStrReady = 1;      //������־λ
unsigned char uStpReady = 0;      //ͣ����־λ


//float fDirWatch = 0;              //����������ı仯
//float fDirOutWatch = 0;           //�������������ı仯

//-------------ֱ��PID������ʼ��-------------//
float fAngleKp = 347; //300 330         450//�Ƕȱ���ϵ��(Proportion)
float fAngleKd = 9;   //6 8             20//�Ƕ�΢��ϵ��(Derivative)
//float fAngleT  = 0.005;               //�Ƕȶ�ȡ��������(5ms)

//-------------�ٶ�PID������ʼ��-------------//
float fSpeedKp = 60;  //45                2//����ϵ��(Proportion)
float fSpeedKi = 0.25;   //1.5              0.04//����ϵ��(Integral)
//float fSpeedT  = 0.1;             //��������(Period)(100ms,50ms��һ�����)

//-------------����PID������ʼ��-------------//
float fDirKp   = 26;//150               //�������ϵ��(Proportion)
float fDirKpPM = 170;
float fDirKpCN = 26;
float fDirKd   = 3;               //����΢��ϵ��(Derivative)
float fAngleOffSet = ANGLEOFFSET;       //ֱ��ʱ�Ƕ�ֵ

//-------------�ٶ��趨����-------------//
int nSpeedSet = 0;    //�ٶ��趨ֵ
int nSpeedAvr = 0;    //���ҵ���ٶ�ƽ��ֵ��Ϊʵ���ٶ�
int nSpeedLef = 0;    //�����ٶ�ʵ��ֵ
int nSpeedRig = 0;    //�ҵ���ٶ�ʵ��ֵ

//-------------��������һ������������-------------//
float fAngleOut       = 0;
float fSpeedOut       = 0;
float fSpeedIntergral = 0;
float fDirOut         = 0;
float fSpeedOutSuperp = 0;
//int   nSpeedEK        = 0;

//-------------���������-------------//
int   nDirSum = 0;
float fDir    = 0;  //����
unsigned char uDirZero = 80 ;  

//-------------ms��ʱ����־-------------//
//unsigned long int uPitms  = 0;    //1ms�ж��Լӱ�־��
unsigned char uPit5msFlg  = 0;    //1ms���е�5ms��־
unsigned char uPit100msFlg  = 0;    //1ms���е�5ms��־
word  w5msCount = 0;              //5ms����
word  wImaCount = 0;              //��ѭ������  ���ڼ���ͼ����һ�ξ������ٴ�5ms����



//-------------ֱ���Ƕ���ز���-------------//
float fAngleHB   = ANGLEOFFSET;       //�����˲���Ƕȼ���ֵ
float fAngleATan = ANGLEOFFSET;       //2����ٶ�Atan����ֵ
//float fAngleEK   = 0;               //mG_delta;  �����˲�����1
//float fAngleTg = 2;                 //Tg=2;       ����ʱ��

float fXAngleSpeed  = 0;            //�����Ǽ�����ٶ�ֵ
float fYAngleSpeed  = 0;

//-------------MMA8451 & L3G4200D-------------//
unsigned char uMMA8451ID  = 0;  //���ٶȼ�ID��֤I2C����
unsigned char uL3g4200dID = 0;  //������ID��֤I2C����  
//xGyroOFFSET=10;yGyroOFFSET=11;zGyroOFFSET=3;
int nXGyroDataOFFSET = 10;          //��������Ưֵ
int nYGyroDataOFFSET = 11;



//-------------����ͷ���-------------//
unsigned char Image[40][160] = {0};  //ͼ������
unsigned char LefEdge[40] = {0};
unsigned char RigEdge[40] = {0};
unsigned char MidLine[40] = {0};

unsigned char uImaCapFlg = 0;        //ͼ��ɼ���־
unsigned char uImaGetFlg = 0;        //ͼ���ñ�־
unsigned char uCapRow  = 0;          //�ɼ�����

unsigned char uDirJdg   = 0;    //������Ϊ0 ; ����תΪ1;

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





