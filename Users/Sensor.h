#ifndef _Sensor_H_
#define _Sensor_H_

#include "math.h"
#include "IO_Map.h"
#include "I2C.h"
#include "L3G4200D.h"
#include "mma845x.h"

extern  unsigned char uMMA8451ID;
extern  unsigned char uL3g4200dID;
//extern	unsigned char	AccData[4];
//extern	unsigned char	GyroData[4];

extern	int   nXGyroDataOFFSET;
extern	int   nYGyroDataOFFSET;
extern	float fXAngleSpeed;
//extern	float fYAngleSpeed;
extern	float fYAngleSpeed;

extern  float fAngleOffSet; 
extern	float fAngleATan;
extern	float fAngleEK;
extern  float fAngleHB;

extern  float fAngleKp;
extern  float fAngleKd;

extern  float fAngleOut;

void L3g4200d_Init(void);
void MMA845x_Init(void);

void Acc_DataRead(void);
void Gyro_DataRead(void);
void Sensor_PID(void);

float Acc_DataTransform(unsigned char MSB,unsigned char LSB);
float Acc_DataCal(unsigned char yMSB,unsigned char yLSB,unsigned char zMSB,unsigned char zLSB);

float Gyro_DataCal(unsigned char MSB,unsigned char LSB,int GyroDataOFFSET);

#endif  /* _I2C_H_ */

