#include "Sensor.h"

//************L3G4200D初始化*********************************
void L3g4200d_Init(void)
{	
  	Single_Write_Byte(L3G4200_Addr,L3G4200D_CTRL_REG1, 0x0f);
  	Single_Write_Byte(L3G4200_Addr,L3G4200D_CTRL_REG2, 0x00);
  	Single_Write_Byte(L3G4200_Addr,L3G4200D_CTRL_REG3, 0x08);
   	Single_Write_Byte(L3G4200_Addr,L3G4200D_CTRL_REG4, 0x30);
   	Single_Write_Byte(L3G4200_Addr,L3G4200D_CTRL_REG5, 0x00);
   	uL3g4200dID = Single_Read_Byte(L3G4200_Addr,WHO_AM_I);//读L3g4200ID
}	

//************MMA8451初始化*********************************
void MMA845x_Init(void)
{	   
  	//Single_Write_Byte(MMA845x_IIC_ADDRESS,MMA845x_CTRL_REG1,(Single_Read_Byte(MMA845x_CTRL_REG1)& ~FREAD_MASK));
  	Single_Write_Byte(MMA845x_IIC_ADDRESS,MMA845x_CTRL_REG1,ASLP_RATE_20MS+DATA_RATE_5MS);
  	Single_Write_Byte(MMA845x_IIC_ADDRESS,XYZ_DATA_CFG_REG, FULL_SCALE_2G); //2G
   	Single_Write_Byte(MMA845x_IIC_ADDRESS,HP_FILTER_CUTOFF_REG, PULSE_LPF_EN_MASK ); //低通滤波
  	Single_Write_Byte(MMA845x_IIC_ADDRESS,MMA845x_CTRL_REG1, ACTIVE_MASK);          //激活状态
    uMMA8451ID = Single_Read_Byte(MMA845x_IIC_ADDRESS,WHO_AM_I_REG);
}
    

void Acc_DataRead(void)//692 us
{   
    float fAngleATanOld = fAngleATan;
    unsigned char AccData[4]  = {0};
    
  	//(xyz ml)//yMSB=AccData[0];yLSB=AccData[1];zMSB=AccData[2];zLSB=AccData[3]; 	
  	//Multiple_Read_Byte(MMA845x_IIC_ADDRESS,MMA845x_OUT_Y_MSB,AccData,4);
  	Multiple_Read_Byte(MMA845x_IIC_ADDRESS,OUT_Y_MSB_REG,AccData,4);  
  	
    fAngleATan = Acc_DataCal(AccData[0],AccData[1],AccData[2],AccData[3]);

    if( (fAngleATan > 90) || (fAngleATan < 0) )      
        fAngleATan = fAngleATanOld;
    
    fAngleATan = ( fAngleATan+ fAngleATanOld ) / 2;    
}
/*
*/
/*
*/

void Gyro_DataRead(void)//430 us   
{	  
    //加速度可以多字节读取，陀螺仪无效？？？I2C程序问题？？？
    unsigned char GyroData[4]  = {0};
    
    //(xyz lm)//xMSB=GyroData[0];xLSB=GyroData[1];
  	GyroData[0]=Single_Read_Byte(L3G4200_Addr,OUT_X_H);
  	GyroData[1]=Single_Read_Byte(L3G4200_Addr,OUT_X_L);	
  	GyroData[2]=Single_Read_Byte(L3G4200_Addr,OUT_Y_H);
  	GyroData[3]=Single_Read_Byte(L3G4200_Addr,OUT_Y_L);
  	fXAngleSpeed     = Gyro_DataCal(GyroData[0],GyroData[1],nXGyroDataOFFSET);
  	fYAngleSpeed     = Gyro_DataCal(GyroData[2],GyroData[3],nYGyroDataOFFSET); 
  	//(yMSB,yLSB,zMSB,zLSB)
  	//(xMSB,xLSB,xGyroDataOFFSET)
  	
}


void Sensor_PID(void)//82us
{	               
    float AngleOutTmp1;
    float AngleOutTmp2;
    float AngleEK;
    float AngleHBOld = fAngleHB;
    
    AngleEK = ( fAngleATan - fAngleHB ) / 2.0;     //fAngleTg=2.0;
  	fAngleHB +=  ( fXAngleSpeed + AngleEK ) * 0.005;
  	//fAngleHB = (1-fAngleT)fAngleHB + (fXAngleSpeed + fAngleATan) * fAngleT;
  	   
  	//fAngleGyroI = fAngleGyroI + fXAngleSpeed * fAngleT; 
    //fAngleHB = (0.9995)*(fAngleHB+fXAngleSpeed*fAngleT)+(0.005)*(fAngleATan); 

    if( (fAngleHB - AngleHBOld > 3) || (fAngleHB - AngleHBOld < -3) ) 
      fAngleHB = (fAngleHB + AngleHBOld) / 2;
    
    //fAngleOut = fAngleKp*(fAngleOffSet-fAngleHB) - fAngleKd*fXAngleSpeed; 
    AngleOutTmp1 = fAngleKp * ( fAngleOffSet - fAngleHB );
    AngleOutTmp2 = fAngleKd * fXAngleSpeed;
    fAngleOut = AngleOutTmp1 - AngleOutTmp2;
}

float Acc_DataTransform(unsigned char MSB,unsigned char LSB)
{
  	float Acceleration1;
  	int DegreeData1;
  	int Temp;

    DegreeData1=(int)(MSB<<8|LSB);

  	if(MSB>0x7F)
    {
        Temp=(~(DegreeData1>>2)+1)&0X3FFF;
        Acceleration1 = 0 - (((float)Temp) / 0xfff);
    }
    else
    {
        Temp=(DegreeData1>>2)&0X3FFF;
        Acceleration1 = (((float)Temp) / 0xfff);
    }  

    if(Acceleration1 >= 1.0)        Acceleration1 = 1.0; 
    else if(Acceleration1 <= -1.0)  Acceleration1 = -1.0; 

    return Acceleration1;
}

float Acc_DataCal(unsigned char yMSB,unsigned char yLSB,unsigned char zMSB,unsigned char zLSB)
{
  	float AngleATan1;
  	float yAcc1;
  	float zAcc1;

  	yAcc1=Acc_DataTransform(yMSB,yLSB);
  	zAcc1=Acc_DataTransform(zMSB,zLSB);

  if(zAcc1 == 0 ) zAcc1 = 0.001;
	AngleATan1 = -57.296 * ( atan(yAcc1/zAcc1) );// 57.296*
	
	if(AngleATan1 > 90)       AngleATan1 = 90;
	else if(AngleATan1 < -90) AngleATan1 = -90;

  if(zAcc1<0)		AngleATan1 = 90 - AngleATan1;
  else			    AngleATan1 = -90 - AngleATan1; 
  
	return AngleATan1;
}


float Gyro_DataCal(unsigned char MSB,unsigned char LSB,int GyroDataOFFSET)
{
  	int GyrosData2;
  	float GyroSpeed2;

  	GyrosData2 = (int)((MSB<<8)|LSB);
    GyrosData2 = 0 - (GyrosData2 - GyroDataOFFSET);
    GyroSpeed2 = (float)(GyrosData2 * 0.07);        //2000dps 转换*0.07
  	return GyroSpeed2;
}