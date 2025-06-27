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

#ifdef FLASH
#pragma CODE_SECTION(INT_EV_CTRL_CLA1_ISR,".TI.ramfunc");
#endif

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
    Init_variables();
    Control_initVariables();
    Init_PWM();
    Init_GPIO();
    Init_cla();

    EINT;
    ERTM;

     while(1)
    {

    }
}

// System loop
__interrupt void INT_EV_CTRL_CLA1_ISR(void)
{
//    GPIO_writePin(TEST1, 1);

    //#############################################################################
    // DAC test
    test_dac = test_dac_real_value * Vadc / Vadc_V; // 2.674
    DAC_setShadowValue(EV_DACA_BASE, test_dac);
    //#############################################################################

//    GPIO_togglePin(EV_LED1);
//    CLA_forceTasks(EV_CTRL_CLA_BASE, CLA_TASKFLAG_1);
    asm(" IACK #0x0001");

    // Soft starter
    Softstart();

    // Error calculation
//    DSP.DSP_ctrl.v_command = Vo_set;
    DSP.DSP_ctrl.v_err = DSP.DSP_ctrl.v_command - DSP_ADC_RAW.Vo_sen;

    // Digital compensation
    Control_loop();

    // Updating PWM
//    EPWM_setCounterCompareValue(EV_PWM_B_main_BASE, EPWM_COUNTER_COMPARE_A, duty_test * SW_PRD);  // Open loop test
    EPWM_setCounterCompareValue(EV_PWM_B_main_BASE, EPWM_COUNTER_COMPARE_A, DSP.DSP_ctrl.v_ctrl);  // Closed loop test
//    GPIO_togglePin(TEST1);
//    GPIO_writePin(TEST1, 0);

    Interrupt_clearACKGroup(INT_EV_CTRL_CLA1_INTERRUPT_ACK_GROUP);
}
