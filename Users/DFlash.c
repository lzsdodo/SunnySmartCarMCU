#include "DFlash.h"

void EEPROM_SetFloat(EEPROM_TAddress Addr,float Data)
{
  union
  {
    float Temp_Data;
    unsigned long Data_Part;
  }_Float_;
  _Float_.Temp_Data = Data;
  while(EEPROM_SetLong( Addr ,_Float_.Data_Part));

}

float EEPROM_GetFloat(EEPROM_TAddress Addr)
{
  union
  {
    float Temp_Data;
    unsigned long Data_Part;
  }_Float_;
  
  while(EEPROM_GetLong( Addr ,&_Float_.Data_Part));
  return _Float_.Temp_Data;
  
}

//---保存数据到DFlash---//
void DFSaveData(void)
{
    EEPROM_SetFloat( EEPROM_AREA_START   , fAngleKp);
    EEPROM_SetFloat( EEPROM_AREA_START+4 , fAngleKd);
    EEPROM_SetFloat( EEPROM_AREA_START+8 , fSpeedKp);
    EEPROM_SetFloat( EEPROM_AREA_START+12, fSpeedKi);
    EEPROM_SetFloat( EEPROM_AREA_START+16, fDirKp);
    EEPROM_SetFloat( EEPROM_AREA_START+20, fDirKd);
    EEPROM_SetFloat( EEPROM_AREA_START+24, fAngleOffSet); 
    EEPROM_SetFloat( EEPROM_AREA_START+28, fDirKpCN); 
    EEPROM_SetFloat( EEPROM_AREA_START+32, fDirKpPM); 
}
//---保存数据到DFlash---//


//---读取DFlash的数据---//
void DFReadData(void)
{
    fAngleKp = EEPROM_GetFloat(EEPROM_AREA_START);
    fAngleKd = EEPROM_GetFloat(EEPROM_AREA_START+4);
    fSpeedKp = EEPROM_GetFloat(EEPROM_AREA_START+8);
    fSpeedKi = EEPROM_GetFloat(EEPROM_AREA_START+12);
    fDirKp   = EEPROM_GetFloat(EEPROM_AREA_START+16);
    fDirKd   = EEPROM_GetFloat(EEPROM_AREA_START+20);
    fAngleOffSet = EEPROM_GetFloat(EEPROM_AREA_START+24);
    fDirKpCN = EEPROM_GetFloat(EEPROM_AREA_START+28);
    fDirKpPM = EEPROM_GetFloat(EEPROM_AREA_START+32);
}
//---读取DFlash的数据---//


