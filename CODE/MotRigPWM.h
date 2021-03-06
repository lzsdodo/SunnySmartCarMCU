/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : MotRigPWM.h
**     Project   : Sunny3
**     Processor : MC9S12XS128MAA
**     Component : PWM
**     Version   : Component 02.231, Driver 01.16, CPU db: 3.00.019
**     Compiler  : CodeWarrior HCS12X C Compiler
**     Date/Time : 2014/5/18, 3:19
**     Abstract  :
**         This component implements a pulse-width modulation generator
**         that generates signal with variable duty and fixed cycle. 
**     Settings  :
**         Used output pin             : 
**             ----------------------------------------------------
**                Number (on package)  |    Name
**             ----------------------------------------------------
**                       79            |  PP5_KWP5_PWM5
**             ----------------------------------------------------
**
**         Timer name                  : PWM45 [16-bit]
**         Counter                     : PWMCNT45  [$0310]
**         Mode register               : PWMCTL    [$0305]
**         Run register                : PWME      [$0300]
**         Prescaler                   : PWMPRCLK  [$0303]
**         Compare 1 register          : PWMPER45  [$0318]
**         Compare 2 register          : PWMDTY45  [$0320]
**         Flip-flop 1 register        : PWMPOL    [$0301]
**
**         User handling procedure     : not specified
**
**         Output pin
**
**         Port name                   : P
**         Bit number (in port)        : 5
**         Bit mask of the port        : $0020
**         Port data register          : PTP       [$0258]
**         Port control register       : DDRP      [$025A]
**
**         Runtime setting period      : none
**         Runtime setting ratio       : calculated
**         Initialization:
**              Aligned                : Left
**              Output level           : low
**              Timer                  : Enabled
**              Event                  : Enabled
**         High speed mode
**             Prescaler               : divide-by-1
**             Clock                   : 40000000 Hz
**           Initial value of            period        pulse width (ratio 99.97%)
**             Xtal ticks              : 1600          1600
**             microseconds            : 100           100
**             seconds (real)          : 0.0001        0.000099975
**
**     Contents  :
**         SetRatio16 - byte MotRigPWM_SetRatio16(word Ratio);
**
**     Copyright : 1997 - 2010 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/

#ifndef __MotRigPWM
#define __MotRigPWM

/* MODULE MotRigPWM. */

#include "Cpu.h"

#pragma CODE_SEG MotRigPWM_CODE

#define MotRigPWM_PERIOD_VALUE 0x0FA0UL /* Initial period value in ticks of the timer */
#define MotRigPWM_PERIOD_VALUE_HIGH 0x0FA0UL /* Initial period value in ticks of the timer in high speed mode */

byte MotRigPWM_SetRatio16(word Ratio);
/*
** ===================================================================
**     Method      :  MotRigPWM_SetRatio16 (component PWM)
**
**     Description :
**         This method sets a new duty-cycle ratio. Ratio is expressed
**         as a 16-bit unsigned integer number. Value 0 - 65535 value
**         is proportional to ratio 0 - 100%. 
**         Note: Calculated duty depends on the timer possibilities and
**         on the selected period.
**         The method is available only if method <SetPeriodMode> is
**         not selected.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Ratio           - Ratio to set. 0 - 65535 value is
**                           proportional to ratio 0 - 100%
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/

void MotRigPWM_Init(void);
/*
** ===================================================================
**     Method      :  MotRigPWM_Init (component PWM)
**
**     Description :
**         Initializes the associated peripheral(s) and the component's 
**         internal variables. The method is called automatically as a 
**         part of the application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/

#pragma CODE_SEG DEFAULT

/* END MotRigPWM. */

#endif /* ifndef __MotRigPWM */
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.02 [04.44]
**     for the Freescale HCS12X series of microcontrollers.
**
** ###################################################################
*/
