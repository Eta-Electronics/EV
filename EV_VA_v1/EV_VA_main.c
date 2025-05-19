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
//         DAC_setShadowValue(EV_DACA_BASE, dacVal);
//         DEVICE_DELAY_US(2);
    }
}

// System loop
__interrupt void INT_EV_CTRL_CLA1_ISR(void)
{
    test_dac = test_dac_real_value * Vadc / Vadc_V;
    DAC_setShadowValue(EV_DACA_BASE, test_dac);
    test_adc = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER0) + 0.3 * Vadc / Vadc_V;
    test_adc_real_value1 = test_adc * 3.0 / Vadc;
    test_adc_real_value2 = test_adc * Vadc_V / Vadc;

    GPIO_togglePin(EV_LED1);
    CLA_forceTasks(EV_CTRL_CLA_BASE, CLA_TASKFLAG_1);
    asm(" IACK #0x0001");

    // Soft starter
//    Softstart();

    // Error calculation
//    eta_dsp.dsp_ctrl_data.v_err = eta_dsp.dsp_ctrl_data.v_command - dsp_adc_data.Vo_sen;
//    eta_dsp.dsp_ctrl_data.i_err = eta_dsp.dsp_ctrl_data.i_command - dsp_adc_data.Io_sen;

    // Digital compensation
//    Control_loop();

    // Updating PWM
    EPWM_setTimeBasePeriod(EV_PWM_B_main_BASE, SW_PRD);
    EPWM_setCounterCompareValue(EV_PWM_B_main_BASE, EPWM_COUNTER_COMPARE_A, duty_test * SW_PRD);  // Open loop test


    Interrupt_clearACKGroup(INT_EV_CTRL_CLA1_INTERRUPT_ACK_GROUP);
}
