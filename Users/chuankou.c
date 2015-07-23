#include "chuankou.h"

void SCISendInt(int pnum)
{

 /**/
    if(pnum<0)
    {
      pnum = -pnum;
      while(SCI_SendChar('-'));
    } 

    if(pnum<10)
    {
        while(SCI_SendChar(pnum+'0'));
    }
    else 
    {
        if(pnum<=999)
        {
            while(SCI_SendChar((unsigned char)(pnum/100+'0')));
            while(SCI_SendChar((unsigned char)(pnum/10%10+'0')));
            while(SCI_SendChar((unsigned char)(pnum%10+'0')));
        } 
        else
        {
            if(pnum<=9999)
            {
                while(SCI_SendChar(pnum/1000+'0'));
                while(SCI_SendChar(pnum/100%10+'0'));
                while(SCI_SendChar(pnum/10%10+'0'));
                while(SCI_SendChar(pnum%10+'0'));  
            } 
            else
            {
                while(SCI_SendChar(pnum/10000+48));
                while(SCI_SendChar(pnum/1000%10+48));
                while(SCI_SendChar(pnum/100%10+48));
                while(SCI_SendChar(pnum/10%10+48)); 
                while(SCI_SendChar(pnum%10+48));  
            }
        }
    }
}

void SCI_Send(int real,int set,int other) 
{
	/*=======与算法无关，为了在串口中看数据而做的缩放=======================*/
	/*====================SCI=====================*/
	while(SCI_SendChar('I'));
	SCISendInt((int)(real));						//red
	while(SCI_SendChar('|'));
	SCISendInt((int)(set));	 				//blue
	while(SCI_SendChar('|'));
	SCISendInt((int)(other));	   					//light blue
	while(SCI_SendChar('\r'));
	while(SCI_SendChar('\n'));      
}

/*
void SCISend_to_PIDDebug()
{ 	
    //1.所用数据
    static unsigned char i,j;
    static unsigned int send_data[3][8]={{0},{0},{0}};//3幅图，每幅最多8条曲线，一条曲线是1个数
    static unsigned char send_num[3]={2,0,0}; //每幅图多少条曲线
    
    //2.将想发的变量赋值给send1_data，send2_data，send3_data,就是把10,20,30这些数字改成变量就行。不发的数据要注释（省时间)
    send_data[0][0]=(unsigned int)(fAngleATan);
    send_data[0][1]=(unsigned int)(fAngleHB);
    send_data[0][2]=(unsigned int)(30);
    */
    /*send_data[0][3]=(unsigned short int)(40);
    send_data[0][4]=(unsigned short int)(-10);
    send_data[0][5]=(unsigned short int)(-20);
    send_data[0][6]=(unsigned short int)(-30);
    send_data[0][7]=(unsigned short int)(-40);

    send_data[1][0]=(unsigned short int)(100);
    send_data[1][1]=(unsigned short int)(200);
    send_data[1][2]=(unsigned short int)(300);
    send_data[1][3]=(unsigned short int)(400);
    send_data[1][4]=(unsigned short int)(-100);
    send_data[1][5]=(unsigned short int)(-200);
    send_data[1][6]=(unsigned short int)(-300);
    send_data[1][7]=(unsigned short int)(-400);
    
    send_data[2][0]=(unsigned short int)(10000);
    send_data[2][1]=(unsigned short int)(20000);
    send_data[2][2]=(unsigned short int)(30000);
    send_data[2][3]=(unsigned short int)(40000);
    send_data[2][4]=(unsigned short int)(-10000);
    send_data[2][5]=(unsigned short int)(-20000);
    send_data[2][6]=(unsigned short int)(-30000);
    send_data[2][7]=(unsigned short int)(-40000);*/
    /*
    //3.这三幅图的字头,start的前两个字母
    //理论上误码率约为1/65536=0.0000152587890625，应该这么算吧，就是这么个意思，不准确
    //如果是处女座有强迫症可以在单片机上加上过滤，或者找我在软件上加上更加严格的字头
    sci_send_char('S');
    sci_send_char('T');
    
    //4.发送数据
    for(i=0;i < 3;i++)
      for(j=0;j < send_num[i];j++)
      {
          sci_send_char(send_data[i][j]);
          sci_send_char(send_data[i][j]>>8u);
      }
}

void sci_send_char(unsigned char a) 
{
  while(!(SCI0SR1&0x80))
  SCI0DRH=0;
  SCI0DRH=a;
}
*/