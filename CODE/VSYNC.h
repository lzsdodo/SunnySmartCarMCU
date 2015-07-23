/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : VSYNC.h
**     Project   : Sunny3
**     Processor : MC9S12XS128MAA
**     Component : ExtInt
**     Version   : Component 02.094, Driver 01.20, CPU db: 3.00.019
**     Compiler  : CodeWarrior HCS12X C Compiler
**     Date/Time : 2014/5/18, 3:19
**     Abstract  :
**         This component "ExtInt" implements an external 
**         interrupt, its control methods and interrupt/event 
**         handling procedure.
**         The component uses one pin which generates interrupt on 
**         selected edge.
**     Settings  :
**         Interrupt name              : Vportj
**         User handling procedure     : VSYNC_OnInterrupt
**
**         Used pin                    :
**             ----------------------------------------------------
**                Number (on package)  |    Name
**             ----------------------------------------------------
**                       68            |  PJ7_KWJ7
**             ----------------------------------------------------
**
**         Port name                   : J
**
**         Bit number (in port)        : 7
**         Bit mask of the port        : $0080
**
**         Signal edge/level           : falling
**         Priority                    : 7
**         Pull option                 : off
**         Initial state               : Enabled
**
**         Edge register               : PPSJ      [$026D]
**         Enable register             : PIEJ      [$026E]
**         Request register            : PIFJ      [$026F]
**
**         Port data register          : PTJ       [$0268]
**         Port control register       : DDRJ      [$026A]
**     Contents  :
**         Enable  - void VSYNC_Enable(void);
**         Disable - void VSYNC_Disable(void);
**
**     Copyright : 1997 - 2010 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/

#ifndef __VSYNC_H
#define __VSYNC_H

/* MODULE VSYNC. */

/*Including shared modules, which are used in the whole project*/
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Events.h"
#include "Cpu.h"

#pragma CODE_SEG VSYNC_CODE


void VSYNC_Enable(void);
/*
** ===================================================================
**     Method      :  VSYNC_Enable (component ExtInt)
**
**     Description :
**         Enable the component - the external events are accepted.
**         This method is available only if HW module allows
**         enable/disable of the interrupt.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

#define VSYNC_Disable() (PIEJ_PIEJ7 = 0U) /* Disable interrupt "Vportj" */
/*
** ===================================================================
**     Method      :  VSYNC_Disable (component ExtInt)
**
**     Description :
**         Disable the component - the external events are not accepted.
**         This method is available only if HW module allows
**         enable/disable of the interrupt.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/



#pragma CODE_SEG DEFAULT

/* END VSYNC. */

#endif /* __VSYNC_H*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.02 [04.44]
**     for the Freescale HCS12X series of microcontrollers.
**
** ###################################################################
*/
