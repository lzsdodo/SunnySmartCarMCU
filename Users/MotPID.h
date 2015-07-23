#ifndef __MotPID_H
#define __MotPID_H

#include "IO_Map.h"
#include "Pulse.h"
#include "MotLefPWM.h"
#include "MotRigPWM.h"
#include "MotLefSetDir.h"
#include "MotRigSetDir.h"
#include "MotLefGetSpeed.h"
#include "MotRigGetSpeed.h"
#include "MotLefGetDir.h"
#include "MotRigGetDir.h"

extern unsigned char uMotFlg;


extern float fAngleHB; 
extern int nSpeedSet;
extern int nSpeedAvr;
extern int nSpeedLef;
extern int nSpeedRig;

extern float fSpeedKp;              //比例系数(Proportion)
extern float fSpeedKi;              //积分系数(Integral)
extern float fSpeedOut;

extern float fSpeedIntergral; 
extern float fSpeedOutSuperp;
extern int   nSpeedEK;

extern float fDirKp;              //比例系数(Proportion)
extern float fDirKd;              //微分系数(Derivative)
extern float fDir;
extern float fYAngleSpeed;
extern float fDirOut;  



void Mot_Init(void);
void Mot_SetLefPWM(float AngleOut,float SpeedOut,float DirOut);
void Mot_SetRigPWM(float AngleOut,float SpeedOut,float DirOut);
void Speed_RealCal(void);
void Speed_PID(void);
void Dir_PID(void);

#endif //__MotPID_H
