#ifndef __chuankou_H
#define __chuankou_H
#include "SCI.h"

//extern float fAngleOut; 
//extern float fSpeedOut;
//extern int nSpeedSet;
//extern int nSpeedRealAverage;

 
void SCISendInt(int pnum);
void SCI_Send(int real,int set,int other);
//void SCISend_to_PIDDebug();
//void sci_send_char(unsigned char a) ;
#endif //__chuankou_H