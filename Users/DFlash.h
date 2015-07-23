#ifndef _DFlash_H_
#define _DFlash_H_

#include "EEPROM.h"

extern float fAngleKp;
extern float fAngleKd;
extern float fSpeedKp;
extern float fSpeedKi;
extern float fDirKp;
extern float fDirKd;
extern float fAngleOffSet;
extern float fDirKpST;
extern float fDirKpCN;
extern float fDirKpPM;


void EEPROM_SetFloat(EEPROM_TAddress Addr,float Data);
float EEPROM_GetFloat(EEPROM_TAddress Addr);
void DFSaveData(void);
void DFReadData(void);


#endif  /* _DFlash_H_ */