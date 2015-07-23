/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : Cpu.c
**     Project   : Sunny3
**     Processor : MC9S12XS128MAA
**     Component : MC9S12XS256_80
**     Version   : Component 01.003, Driver 02.06, CPU db: 3.00.019
**     Datasheet : MC9S12XS256RMV1 Rev. 1.03 06/2008
**     Compiler  : CodeWarrior HCS12X C Compiler
**     Date/Time : 2014/5/18, 7:07
**     Abstract  :
**         This component "MC9S12XS256_80" implements properties, methods,
**         and events of the CPU.
**     Settings  :
**
**     Contents  :
**         EnableInt  - void Cpu_EnableInt(void);
**         DisableInt - void Cpu_DisableInt(void);
**         Delay100US - void Cpu_Delay100US(word us100);
**
**     Copyright : 1997 - 2010 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/

/* MODULE Cpu. */

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
#include "MotLefPWM.h"
#include "MotRigPWM.h"
#include "MotLefSetDir.h"
#include "MotRigSetDir.h"
#include "MotLefGetSpeed.h"
#include "MotRigGetSpeed.h"
#include "MotLefGetDir.h"
#include "MotRigGetDir.h"
#include "VSYNC.h"
#include "HREF.h"
#include "Pulse.h"
#include "Pitms.h"
#include "SW.h"
#include "Bit1.h"
#include "Bit2.h"
#include "Bit3.h"
#include "Bit4.h"
#include "Events.h"
#include "Cpu.h"

#define CGM_DELAY  0x13FFU

#pragma DATA_SEG DEFAULT               /* Select data segment "DEFAULT" */
#pragma CODE_SEG DEFAULT


/* Global variables */
volatile byte CCR_reg;                 /* Current CCR reegister */

/*Definition of global shadow variables*/
byte Shadow_B;
byte Shadow_E;

#pragma CODE_SEG __NEAR_SEG NON_BANKED


/*
** ===================================================================
**     Method      :  Cpu_Cpu_Interrupt (component MC9S12XS256_80)
**
**     Description :
**         The method services unhandled interrupt vectors.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
ISR(Cpu_Interrupt)
{
  /*lint -save -e950 Disable MISRA rule (1.1) checking. */
  asm(BGND);
  /*lint -restore Enable MISRA rule (1.1) checking. */
}


/*
** ===================================================================
**     Method      :  Cpu_Delay100US (component MC9S12XS256_80)
**
**     Description :
**         This method realizes software delay. The length of delay
**         is at least 100 microsecond multiply input parameter
**         [us100]. As the delay implementation is not based on real
**         clock, the delay time may be increased by interrupt
**         service routines processed during the delay. The method
**         is independent on selected speed mode.
**     Parameters  :
**         NAME            - DESCRIPTION
**         us100           - Number of 100 us delay repetitions.
**                         - The value of zero results in maximal 
**                           delay of approx. 6.5 seconds.
**     Returns     : Nothing
** ===================================================================
*/
#pragma NO_ENTRY                       /* Suppress generation of entry code in a function */
#pragma NO_EXIT                        /* Suppress generation of exit from a function */
#pragma MESSAGE DISABLE C5703          /*Disable C5703: Parameter �<Parameter>� declared in function �<Function>� but not referenced */
void Cpu_Delay100US(word us100)
{
  /* irremovable overhead (ignored): 13 cycles */
  /* ldd:  2 cycles overhead (load parameter into register) */
  /* jsr:  4 cycles overhead (call this function) */
  /* rts:  7 cycles overhead (return from this function) */

  /* irremovable overhead for each 100us cycle (counted): 13 cycles */
  /* dbne:  3 cycles overhead (return from this function) */

  /*lint -save  -e950 -e522 Disable MISRA rule (1.1,14.2) checking. */
  asm {
    loop:
    /* 100 us delay block begin */
    /*
     * Delay
     *   - requested                  : 100 us @ 40MHz,
     *   - possible                   : 4000 c, 100000 ns
     *   - without removable overhead : 3997 c, 99925 ns
     */
    pshd                               /* (2 c: 50 ns) backup D */
    ldd #$0532                         /* (2 c: 50 ns) number of iterations */
    label0:
    dbne d, label0                     /* (3 c: 75 ns) repeat 1330x */
    puld                               /* (3 c: 75 ns) restore D */
    /* 100 us delay block end */
    dbne d, loop                       /* us100 parameter is passed via D register */
    rts                                /* return from subroutine */
  };
  /*lint -restore Enable MISRA rule (1.1,14.2) checking. */
}

#pragma CODE_SEG DEFAULT

/*
** ===================================================================
**     Method      :  Cpu__ivVportj (component MC9S12XS256_80)
**
**     Description :
**         The method services the ivVportj interrupt, which is shared by 
**         several components.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
#pragma CODE_SEG __NEAR_SEG NON_BANKED
ISR(Cpu__ivVportj)
{
  if (PIFJ_PIFJ7 != 0U) {              /* Is the device enabled and an interrupt flag is set? */
    if (PIEJ_PIEJ7 != 0U) {
      PIFJ = PIFJ_PIFJ7_MASK;          /* If yes then clear an interrupt flag */
      VSYNC_OnInterrupt();
    }
  }
  if (PIFJ_PIFJ6 != 0U) {              /* Is the device enabled and an interrupt flag is set? */
    if (PIEJ_PIEJ6 != 0U) {
      PIFJ = PIFJ_PIFJ6_MASK;          /* If yes then clear an interrupt flag */
      HREF_OnInterrupt();
    }
  }
}

#pragma CODE_SEG DEFAULT
/*
** ===================================================================
**     Method      :  Cpu_DisableInt (component MC9S12XS256_80)
**
**     Description :
**         Disable maskable interrupts
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/*
void Cpu_DisableInt(void)

**      This method is implemented as macro in the header module. **
*/

/*
** ===================================================================
**     Method      :  Cpu_EnableInt (component MC9S12XS256_80)
**
**     Description :
**         Enable maskable interrupts
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/*
void Cpu_EnableInt(void)

**      This method is implemented as macro in the header module. **
*/

/*
** ===================================================================
**     Method      :  PE_low_level_init (component MC9S12XS256_80)
**
**     Description :
**         Initializes components and provides common register 
**         initialization. The method is called automatically as a part 
**         of the application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void PE_low_level_init(void)
{
  /* Int. priority initialization */
  /*                                        No. Address Pri XGATE Name           Description */
  setReg8(INT_CFADDR, 0x70U);           
  setReg8(INT_CFDATA5, 0x04U);         /*  0x3D  0xFF7A   4   no   ivVpit0        used by PE */ 
  setReg8(INT_CFADDR, 0xC0U);           
  setReg8(INT_CFDATA7, 0x07U);         /*  0x67  0xFFCE   7   no   ivVportj       used by PE */ 
  /* Common initialization of the CPU registers */
  /* PORTB: PB6=0,PB5=0,PB4=0,PB3=0,PB2=0,PB1=0,PB0=0 */
  clrReg8Bits(PORTB, 0x7FU);            
  /* PUCR: PUPEE=1,PUPBE=1,PUPAE=1 */
  setReg8Bits(PUCR, 0x13U);             
  /* DDRB: DDRB6=1,DDRB5=1,DDRB4=1,DDRB3=1,DDRB2=1,DDRB1=1,DDRB0=1 */
  setReg8Bits(DDRB, 0x7FU);             
  /* PORTE: PE7=0,PE6=0,PE5=1,PE4=1,PE3=0,PE2=0 */
  clrSetReg8Bits(PORTE, 0xCCU, 0x30U);  
  /* DDRE: DDRE7=1,DDRE6=1,DDRE5=1,DDRE4=1,DDRE3=1,DDRE2=1 */
  setReg8Bits(DDRE, 0xFCU);             
  /* ECLKCTL: NECLK=1,NCLKX2=1 */
  setReg8Bits(ECLKCTL, 0xC0U);          
  /* DDRA: DDRA7=0,DDRA6=0,DDRA5=0,DDRA4=0,DDRA3=0,DDRA2=0,DDRA1=0,DDRA0=0 */
  setReg8(DDRA, 0x00U);                 
  /* PTT: PTT6=0,PTT4=0,PTT2=0,PTT1=1 */
  clrSetReg8Bits(PTT, 0x54U, 0x02U);    
  /* PPST: PPST3=0,PPST2=0,PPST1=0,PPST0=0 */
  clrReg8Bits(PPST, 0x0FU);             
  /* PERT: PERT7=0,PERT6=0,PERT4=0,PERT3=1,PERT2=1,PERT1=1,PERT0=1 */
  clrSetReg8Bits(PERT, 0xD0U, 0x0FU);   
  /* DDRT: DDRT6=1,DDRT4=1,DDRT3=0,DDRT2=1,DDRT1=1,DDRT0=0 */
  clrSetReg8Bits(DDRT, 0x09U, 0x56U);   
  /* PWME: PWME7=0,PWME6=0,PWME5=0,PWME4=0,PWME3=0,PWME2=0,PWME1=0,PWME0=0 */
  setReg8(PWME, 0x00U);                 
  /* PWMCTL: CON45=1,CON01=1,PSWAI=0,PFRZ=0 */
  clrSetReg8Bits(PWMCTL, 0x0CU, 0x50U); 
  /* PWMCAE: CAE5=0,CAE1=0 */
  clrReg8Bits(PWMCAE, 0x22U);           
  /* PWMPOL: PPOL5=0,PPOL1=0 */
  clrReg8Bits(PWMPOL, 0x22U);           
  /* PIEP: PIEP3=0,PIEP2=0 */
  clrReg8Bits(PIEP, 0x0CU);             
  /* PTP: PTP5=0,PTP3=0,PTP2=1,PTP1=0 */
  clrSetReg8Bits(PTP, 0x2AU, 0x04U);    
  /* DDRP: DDRP6=1,DDRP5=1,DDRP3=1,DDRP2=1,DDRP1=1 */
  setReg8Bits(DDRP, 0x6EU);             
  /* PTTRR: PTTRR5=0 */
  clrReg8Bits(PTTRR, 0x20U);            
  /* PPSP: PPSP3=0,PPSP2=0 */
  clrReg8Bits(PPSP, 0x0CU);             
  /* PERP: PERP3=1,PERP2=1 */
  setReg8Bits(PERP, 0x0CU);             
  /* PPSJ: PPSJ7=0,PPSJ6=1 */
  clrSetReg8Bits(PPSJ, 0x80U, 0x40U);   
  /* PERJ: PERJ7=0,PERJ6=0 */
  clrReg8Bits(PERJ, 0xC0U);             
  /* DDRJ: DDRJ7=0,DDRJ6=0,DDRJ1=1,DDRJ0=1 */
  clrSetReg8Bits(DDRJ, 0xC0U, 0x03U);   
  /* PACTL: PAEN=0 */
  clrReg8Bits(PACTL, 0x40U);            
  /* TSCR1: TEN=0,TSWAI=0,TSFRZ=0,TFFCA=0,PRNT=0,??=0,??=0,??=0 */
  setReg8(TSCR1, 0x00U);                
  /* TCTL1: OM7=0,OL7=0 */
  clrReg8Bits(TCTL1, 0xC0U);            
  /* OC7M: OC7M7=0 */
  clrReg8Bits(OC7M, 0x80U);             
  /* PITCFLMT: PITE=0 */
  clrReg8Bits(PITCFLMT, 0x80U);         
  /* PITCE: PCE0=0 */
  clrReg8Bits(PITCE, 0x01U);            
  /* PITTF: PTF0=1 */
  setReg8Bits(PITTF, 0x01U);            
  /* PITINTE: PINTE0=1 */
  setReg8Bits(PITINTE, 0x01U);          
  /* PITMTLD0: PMTLD7=0,PMTLD6=0,PMTLD5=0,PMTLD4=0,PMTLD3=0,PMTLD2=0,PMTLD1=0,PMTLD0=0 */
  setReg8(PITMTLD0, 0x00U);             
  /* PITMUX: PMUX0=0 */
  clrReg8Bits(PITMUX, 0x01U);           
  /* ATD0DIENL: IEN7=1,IEN6=1,IEN5=1,IEN4=1,IEN3=1,IEN2=1,IEN1=1,IEN0=1 */
  setReg8(ATD0DIENL, 0xFFU);            
  /* PER1AD0: PER1AD07=1,PER1AD06=1,PER1AD05=1,PER1AD04=1,PER1AD03=1,PER1AD02=1,PER1AD01=1,PER1AD00=1 */
  setReg8(PER1AD0, 0xFFU);              
  /* DDR1AD0: DDR1AD07=0,DDR1AD06=0,DDR1AD05=0,DDR1AD04=0,DDR1AD03=0,DDR1AD02=0,DDR1AD01=0,DDR1AD00=0 */
  setReg8(DDR1AD0, 0x00U);              
  /* CRGINT: LOCKIE=0,SCMIE=0 */
  clrReg8Bits(CRGINT, 0x12U);           
  /* VREGCTRL: LVIE=0 */
  clrReg8Bits(VREGCTRL, 0x02U);         
  /* COPCTL: WCOP=0,RSBCK=0,WRTMASK=0,??=0,??=0,CR2=0,CR1=0,CR0=0 */
  setReg8(COPCTL, 0x00U);               
  /* DDR0AD0: DDR0AD07=1,DDR0AD06=1,DDR0AD05=1,DDR0AD04=1,DDR0AD03=1,DDR0AD02=1,DDR0AD01=1,DDR0AD00=1 */
  setReg8(DDR0AD0, 0xFFU);              
  /* DDRH: DDRH7=1,DDRH6=1,DDRH5=1,DDRH4=1,DDRH3=1,DDRH2=1,DDRH1=1,DDRH0=1 */
  setReg8(DDRH, 0xFFU);                 
  /* DDRK: DDRK7=1,DDRK5=1,DDRK4=1,DDRK3=1,DDRK2=1,DDRK1=1,DDRK0=1 */
  setReg8Bits(DDRK, 0xBFU);             
  /* DDRM: DDRM7=1,DDRM6=1 */
  setReg8Bits(DDRM, 0xC0U);             
  /* DDRS: DDRS7=1,DDRS6=1,DDRS5=1,DDRS4=1 */
  setReg8Bits(DDRS, 0xF0U);             
  /* RDRIV: RDPE=0,RDPB=0,RDPA=0 */
  clrReg8Bits(RDRIV, 0x13U);            
  /* RDRJ: RDRJ7=0,RDRJ6=0 */
  clrReg8Bits(RDRJ, 0xC0U);             
  /* RDRM: RDRM5=0,RDRM4=0,RDRM3=0,RDRM2=0,RDRM1=0,RDRM0=0 */
  clrReg8Bits(RDRM, 0x3FU);             
  /* RDRP: RDRP7=0,RDRP5=0,RDRP4=0,RDRP3=0,RDRP2=0,RDRP1=0,RDRP0=0 */
  clrReg8Bits(RDRP, 0xBFU);             
  /* RDRS: RDRS3=0,RDRS2=0,RDRS1=0,RDRS0=0 */
  clrReg8Bits(RDRS, 0x0FU);             
  /* RDRT: RDRT7=0,RDRT6=0,RDRT5=0,RDRT4=0,RDRT3=0,RDRT2=0,RDRT1=0,RDRT0=0 */
  setReg8(RDRT, 0x00U);                 
  /* RDR1AD0: RDR1AD07=0,RDR1AD06=0,RDR1AD05=0,RDR1AD04=0,RDR1AD03=0,RDR1AD02=0,RDR1AD01=0,RDR1AD00=0 */
  setReg8(RDR1AD0, 0x00U);              
  /* IRQCR: IRQEN=0 */
  clrReg8Bits(IRQCR, 0x40U);            
  /* PACNT: BIT15=0,BIT14=0,BIT13=0,BIT12=0,BIT11=0,BIT10=0,BIT9=0,BIT8=0,BIT7=0,BIT6=0,BIT5=0,BIT4=0,BIT3=0,BIT2=0,BIT1=0,BIT0=0 */
  setReg16(PACNT, 0x00U);               
  /* ### MC9S12XS256_80 "Cpu" init code ... */
  /* ### BitIO "DigitalRST" init code ... */
  /* ### BitIO "FIFORCK" init code ... */
  /* ### BitIO "FIFOWRST" init code ... */
  /* ### BitIO "FIFOWRITE" init code ... */
  /* ### BitIO "FIFORRST" init code ... */
  /* ### ByteIO "PA" init code ... */
  /* ### BitIO "PB3" init code ... */
  /* ### BitIO "PB2" init code ... */
  /* ### BitIO "PB1" init code ... */
  /* ### BitIO "PB0" init code ... */
  /* ### BitIO "LED1" init code ... */
  /* ### BitIO "LED0" init code ... */
  /* ### Programable pulse generation "MotLefPWM" init code ... */
  MotLefPWM_Init();
  /* ### Programable pulse generation "MotRigPWM" init code ... */
  MotRigPWM_Init();
  /* ### BitIO "MotLefSetDir" init code ... */
  /* ### BitIO "MotRigSetDir" init code ... */
  /* ### BitIO "MotLefGetSpeed" init code ... */
  /* ### BitIO "MotRigGetSpeed" init code ... */
  /* ### BitIO "MotLefGetDir" init code ... */
  /* ### BitIO "MotRigGetDir" init code ... */
  /* ### External interrupt "VSYNC" init code ... */
  PIFJ = 0x80U;                         /* Clear flag */
  PIEJ_PIEJ7 = 1U;                     /* Enable interrupt */
  /* ### External interrupt "HREF" init code ... */
  PIFJ = 0x40U;                         /* Clear flag */
  PIEJ_PIEJ6 = 1U;                     /* Enable interrupt */
  /* ### PulseAccumulator "Pulse" init code ... */
  Pulse_Init();
  /* ### TimerInt "Pitms" init code ... */
  /* PITLD0: PLD15=1,PLD14=0,PLD13=0,PLD12=1,PLD11=1,PLD10=1,PLD9=0,PLD8=0,PLD7=0,PLD6=1,PLD5=0,PLD4=0,PLD3=0,PLD2=0,PLD1=0,PLD0=0 */
  setReg16(PITLD0, 0x9C40U);           /* Store given value to the compare register */ 
  /* PITCE: PCE0=1 */
  setReg8Bits(PITCE, 0x01U);           /* Enable Timer */ 
  /* ### ByteIO "SW" init code ... */
  /* ### BitIO "Bit1" init code ... */
  Shadow_B &= 0xDFU;                   /* Initialize pin shadow variable bit */
  /* ### BitIO "Bit2" init code ... */
  /* ### BitIO "Bit3" init code ... */
  Shadow_E &= 0xF7U;                   /* Initialize pin shadow variable bit */
  /* ### BitIO "Bit4" init code ... */
  /* Common peripheral initialization - ENABLE */
  /* PITCFLMT: PITE=1 */
  setReg8Bits(PITCFLMT, 0x80U);         
  __EI();                              /* Enable interrupts */
}

/*lint -save  -e950 Disable MISRA rule (1.1) checking. */
/* Initialization of the CPU registers in FLASH */
/*lint -restore Enable MISRA rule (1.1) checking. */

#pragma CODE_SEG __NEAR_SEG NON_BANKED

/*
** ===================================================================
**     Method      :  _EntryPoint (component MC9S12XS256_80)
**
**     Description :
**         Initializes the whole system like timing and so on. At the end 
**         of this function, the C startup is invoked to initialize stack,
**         memory areas and so on.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
extern void _Startup(void);            /* Forward declaration of external startup function declared in file Start12.c */
#pragma OPTION ADD "-Onf"              /* 'Create Sub-Functions with Common Code optimalization' must be disabled because stack is not yet initialized */

#pragma NO_FRAME                       /* Suppress generation of frame code */
#pragma NO_EXIT                        /* Suppress generation of exit from a function */
void _EntryPoint(void)
{
  /* ### MC9S12XS256_80 "Cpu" init code ... */
  /*  PE initialization code after reset */
  /* MMCCTL1: MGRAMON=0,??=0,DFIFRON=0,PGMIFRON=0,??=0,??=0,??=0,??=0 */
  setReg8(MMCCTL1, 0x00U);              
  /* DIRECT: DP15=0,DP14=0,DP13=0,DP12=0,DP11=0,DP10=0,DP9=0,DP8=0 */
  setReg8(DIRECT, 0x00U);               
  /* IVBR: IVB_ADDR=0xFF */
  setReg8(IVBR, 0xFFU);                 
  /* ECLKCTL: NECLK=1,NCLKX2=1,DIV16=0,EDIV4=0,EDIV3=0,EDIV2=0,EDIV1=0,EDIV0=0 */
  setReg8(ECLKCTL, 0xC0U);              
  /*  System clock initialization */
  /* CLKSEL: PLLSEL=0,PSTP=0,XCLKS=0,??=0,PLLWAI=0,??=0,RTIWAI=0,COPWAI=0 */
  setReg8(CLKSEL, 0x00U);              /* Select clock source from XTAL and set bits in CLKSEL reg. */ 
  /* PLLCTL: CME=1,PLLON=0,FM1=0,FM0=0,FSTWKP=0,PRE=0,PCE=0,SCME=1 */
  setReg8(PLLCTL, 0x81U);              /* Disable the PLL */ 
  /* SYNR: VCOFRQ1=0,VCOFRQ0=1,SYNDIV5=0,SYNDIV4=0,SYNDIV3=0,SYNDIV2=1,SYNDIV1=0,SYNDIV0=0 */
  setReg8(SYNR, 0x44U);                /* Set the multiplier register */ 
  /* REFDV: REFFRQ1=1,REFFRQ0=0,REFDIV5=0,REFDIV4=0,REFDIV3=0,REFDIV2=0,REFDIV1=0,REFDIV0=1 */
  setReg8(REFDV, 0x81U);               /* Set the divider register */ 
  /* POSTDIV: ??=0,??=0,??=0,POSTDIV4=0,POSTDIV3=0,POSTDIV2=0,POSTDIV1=0,POSTDIV0=0 */
  setReg8(POSTDIV, 0x00U);             /* Set the post divider register */ 
  /* PLLCTL: CME=1,PLLON=1,FM1=0,FM0=0,FSTWKP=0,PRE=0,PCE=0,SCME=1 */
  setReg8(PLLCTL, 0xC1U);               
  while(CRGFLG_LOCK == 0U) {           /* Wait until the PLL is within the desired tolerance of the target frequency */
  }
  /* CLKSEL: PLLSEL=1 */
  setReg8Bits(CLKSEL, 0x80U);          /* Select clock source from PLL */ 
  /* VREGHTCL: ??=0,??=0,VSEL=0,VAE=1,HTEN=0,HTDS=0,HTIE=0,HTIF=0 */
  setReg8(VREGHTCL, 0x10U);             
  /*** End of PE initialization code after reset ***/
  /*lint -save  -e950 Disable MISRA rule (1.1) checking. */
  __asm("jmp _Startup");               /* Jump to C startup code */
  /*lint -restore Enable MISRA rule (1.1) checking. */
}

#pragma CODE_SEG DEFAULT
/* END Cpu. */

/*
** ###################################################################
**
**     This file was created by Processor Expert 3.02 [04.44]
**     for the Freescale HCS12X series of microcontrollers.
**
** ###################################################################
*/
