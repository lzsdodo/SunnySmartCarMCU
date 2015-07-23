/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : FIFORCK.c
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
**                       25            |  PE6
**             ----------------------------------------------------
**
**         Port name                   : E
**
**         Bit number (in port)        : 6
**         Bit mask of the port        : $0040
**
**         Initial direction           : Output (direction cannot be changed)
**         Initial output value        : 0
**         Initial pull option         : up
**
**         Port data register          : PORTE     [$0008]
**         Port control register       : DDRE      [$0009]
**         Port function register      : IRQCR     [$001E]
**
**         Optimization for            : speed
**     Contents  :
**         No public methods
**
**     Copyright : 1997 - 2010 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/

/* MODULE FIFORCK. */

#include "FIFORCK.h"
  /* Including shared modules, which are used in the whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Cpu.h"

#pragma DATA_SEG FIFORCK_DATA          /* Select data segment "FIFORCK_DATA" */
#pragma CODE_SEG FIFORCK_CODE
#pragma CONST_SEG FIFORCK_CONST        /* Constant section for this module */

/* END FIFORCK. */
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.02 [04.44]
**     for the Freescale HCS12X series of microcontrollers.
**
** ###################################################################
*/
