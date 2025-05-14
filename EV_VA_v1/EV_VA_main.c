//#############################################################################
//
// FILE:   EV_VA_main.c
//
// Written by D. Kim
// Date: 20250512
// Purpose: EV_VA_main code
//
//#############################################################################

// Included Files
#include <EV_VA.h>
#include "board.h"
#include "DCLF32.h"
#include <string.h>

#include <stdio.h>

//#ifdef FLASH
//#pragma CODE_SECTION(INT_EV_CTRL_CLA1_ISR,".TI.ramfunc");
//#endif

//void initPWM(void);
//void initGPIO(void);

// Main
void main(void)
{///////
    // Initialization
    Device_init();
    Device_initGPIO();
    Interrupt_initModule();
    Interrupt_initVectorTable();
    Board_init();
    initVariables();
    initPWM();
    initGPIO();
    InitCla();
    control_initVariables();

    EINT;
    ERTM;

     while(1)
    {
    }
}

// System loop
__interrupt void INT_EV_CTRL_CLA1_ISR(void)
{

//    CLA_forceTasks(TX_CTRL_CLA_BASE, CLA_TASKFLAG_1);
    asm(" IACK #0x0001");


    // Updating PWM
//    EPWM_setCounterCompareValue(TX_1_PH_BASE, EPWM_COUNTER_COMPARE_A, eta_dsp.dsp_ctrl_data.vi_ctrl_DCL);


    Interrupt_clearACKGroup(INT_EV_CTRL_CLA1_INTERRUPT_ACK_GROUP);
}
