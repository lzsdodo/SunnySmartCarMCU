/** ###################################################################
**     Filename  : Events.h
**     Project   : Sunny3
**     Processor : MC9S12XS128MAA
**     Component : Events
**     Version   : Driver 01.04
**     Compiler  : CodeWarrior HCS12X C Compiler
**     Date/Time : 2014/5/11, 20:53
**     Abstract  :
**         This is user's event module.
**         Put your event handler code here.
**     Settings  :
**     Contents  :
**         No public methods
**
** ###################################################################*/

#ifndef __Events_H
#define __Events_H
/* MODULE Events */

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "DigitalRST.h"
#include "FIFORCK.h"
#include "FIFOWRST.h"
#include "FIFOWRITE.h"
#include "FIFORRST.h"
#include "PA.h"
#include "PB3.h"
#include "PB2.h"
#include "PB1.h"
#include "PB0.h"
#include "LED1.h"
#include "LED0.h"
#include "VSYNC.h"
#include "HREF.h"
#include "Pulse.h"
#include "Pitms.h"
#include "MotRigPWM.h"
#include "MotLefSetDir.h"
#include "MotRigSetDir.h"
#include "MotLefGetSpeed.h"
#include "MotRigGetSpeed.h"
#include "MotLefGetDir.h"
#include "MotRigGetDir.h"
#include "MotLefPWM.h"
#include "SW.h"
#include "Bit1.h"
#include "Bit2.h"
#include "Bit3.h"
#include "Bit4.h"

#pragma CODE_SEG DEFAULT


void Pitms_OnInterrupt(void);
/*
** ===================================================================
**     Event       :  Pitms_OnInterrupt (module Events)
**
**     Component   :  Pitms [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void SCI_OnRxChar(void);
/*
** ===================================================================
**     Event       :  SCI_OnRxChar (module Events)
**
**     Component   :  SCI [AsynchroSerial]
**     Description :
**         This event is called after a correct character is received.
**         The event is available only when the <Interrupt
**         service/event> property is enabled and either the <Receiver>
**         property is enabled or the <SCI output mode> property (if
**         supported) is set to Single-wire mode.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void HREF_OnInterrupt(void);
/*
** ===================================================================
**     Event       :  HREF_OnInterrupt (module Events)
**
**     Component   :  HREF [ExtInt]
**     Description :
**         This event is called when an active signal edge/level has
**         occurred.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void VSYNC_OnInterrupt(void);
/*
** ===================================================================
**     Event       :  VSYNC_OnInterrupt (module Events)
**
**     Component   :  VSYNC [ExtInt]
**     Description :
**         This event is called when an active signal edge/level has
**         occurred.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void SCI_OnTxChar(void);
/*
** ===================================================================
**     Event       :  SCI_OnTxChar (module Events)
**
**     Component   :  SCI [AsynchroSerial]
**     Description :
**         This event is called after a character is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void STOPTIME_OnInterrupt(void);
/*
** ===================================================================
**     Event       :  STOPTIME_OnInterrupt (module Events)
**
**     Component   :  STOPTIME [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

/* END Events */
#endif /* __Events_H*/

/*
** ###################################################################
**
**     This file was created by Processor Expert 3.02 [04.44]
**     for the Freescale HCS12X series of microcontrollers.
**
** ###################################################################
*/
