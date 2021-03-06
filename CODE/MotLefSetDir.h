/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : MotLefSetDir.h
**     Project   : Sunny3
**     Processor : MC9S12XS128MAA
**     Component : BitIO
**     Version   : Component 02.075, Driver 03.14, CPU db: 3.00.019
**     Compiler  : CodeWarrior HCS12X C Compiler
**     Date/Time : 2014/5/18, 3:19
**     Abstract  :
**         This component "BitIO" implements an one-bit input/output.
**         It uses one bit/pin of a port.
**         Note: This component is set to work in Output direction only.
**         Methods of this component are mostly implemented as a macros
**         (if supported by target language and compiler).
**     Settings  :
**         Used pin                    :
**             ----------------------------------------------------
**                Number (on package)  |    Name
**             ----------------------------------------------------
**                       7             |  PT2_IOC2
**             ----------------------------------------------------
**
**         Port name                   : T
**
**         Bit number (in port)        : 2
**         Bit mask of the port        : $0004
**
**         Initial direction           : Output (direction cannot be changed)
**         Initial output value        : 0
**         Initial pull option         : up
**
**         Port data register          : PTT       [$0240]
**         Port control register       : DDRT      [$0242]
**
**         Optimization for            : speed
**     Contents  :
**         ClrVal - void MotLefSetDir_ClrVal(void);
**         SetVal - void MotLefSetDir_SetVal(void);
**
**     Copyright : 1997 - 2010 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/

#ifndef MotLefSetDir_H_
#define MotLefSetDir_H_

/* MODULE MotLefSetDir. */

  /* Including shared modules, which are used in the whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Cpu.h"

#pragma CODE_SEG MotLefSetDir_CODE
/*
** ===================================================================
**     Method      :  MotLefSetDir_ClrVal (component BitIO)
**
**     Description :
**         This method clears (sets to zero) the output value.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
#define MotLefSetDir_ClrVal() ( \
    (void)clrReg8Bits(PTT, 0x04U)      /* PTT2=0x00U */ \
  )

/*
** ===================================================================
**     Method      :  MotLefSetDir_SetVal (component BitIO)
**
**     Description :
**         This method sets (sets to one) the output value.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
#define MotLefSetDir_SetVal() ( \
    (void)setReg8Bits(PTT, 0x04U)      /* PTT2=0x01U */ \
  )

#pragma CODE_SEG DEFAULT

/* END MotLefSetDir. */
#endif /* #ifndef __MotLefSetDir_H_ */
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.02 [04.44]
**     for the Freescale HCS12X series of microcontrollers.
**
** ###################################################################
*/
