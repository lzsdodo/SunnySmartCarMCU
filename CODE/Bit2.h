/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : Bit2.h
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
**                       20            |  PB4
**             ----------------------------------------------------
**
**         Port name                   : B
**
**         Bit number (in port)        : 4
**         Bit mask of the port        : $0010
**
**         Initial direction           : Output (direction cannot be changed)
**         Initial output value        : 0
**         Initial pull option         : up
**
**         Port data register          : PORTB     [$0001]
**         Port control register       : DDRB      [$0003]
**
**         Optimization for            : speed
**     Contents  :
**         GetVal - bool Bit2_GetVal(void);
**         PutVal - void Bit2_PutVal(bool Val);
**         ClrVal - void Bit2_ClrVal(void);
**         SetVal - void Bit2_SetVal(void);
**
**     Copyright : 1997 - 2010 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/

#ifndef Bit2_H_
#define Bit2_H_

/* MODULE Bit2. */

  /* Including shared modules, which are used in the whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Cpu.h"

#pragma CODE_SEG Bit2_CODE
/*
** ===================================================================
**     Method      :  Bit2_GetVal (component BitIO)
**
**     Description :
**         This method returns an input value.
**           a) direction = Input  : reads the input value from the
**                                   pin and returns it
**           b) direction = Output : returns the last written value
**         Note: This component is set to work in Output direction only.
**     Parameters  : None
**     Returns     :
**         ---             - Input value. Possible values:
**                           FALSE - logical "0" (Low level)
**                           TRUE - logical "1" (High level)

** ===================================================================
*/
#define Bit2_GetVal() ( \
    (bool)((getReg8(PORTB) & 0x10U))   /* Return port data */ \
  )

/*
** ===================================================================
**     Method      :  Bit2_PutVal (component BitIO)
**
**     Description :
**         This method writes the new output value.
**     Parameters  :
**         NAME       - DESCRIPTION
**         Val             - Output value. Possible values:
**                           FALSE - logical "0" (Low level)
**                           TRUE - logical "1" (High level)
**     Returns     : Nothing
** ===================================================================
*/
void Bit2_PutVal(bool Val);

/*
** ===================================================================
**     Method      :  Bit2_ClrVal (component BitIO)
**
**     Description :
**         This method clears (sets to zero) the output value.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
#define Bit2_ClrVal() ( \
    (void)clrReg8Bits(PORTB, 0x10U)    /* PB4=0x00U */ \
  )

/*
** ===================================================================
**     Method      :  Bit2_SetVal (component BitIO)
**
**     Description :
**         This method sets (sets to one) the output value.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
#define Bit2_SetVal() ( \
    (void)setReg8Bits(PORTB, 0x10U)    /* PB4=0x01U */ \
  )

#pragma CODE_SEG DEFAULT

/* END Bit2. */
#endif /* #ifndef __Bit2_H_ */
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.02 [04.44]
**     for the Freescale HCS12X series of microcontrollers.
**
** ###################################################################
*/
