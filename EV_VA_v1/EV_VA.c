//#############################################################################
//
// FILE:   EV_VA.c
//
// Written by D. Kim
// Date: 20250512
// Purpose: EV_VA code
//
//#############################################################################

// Included Files
#include <EV_VA.h>
#include "DCLF32.h"

#include <stdio.h>
#include <string.h>


DSP_ADC_DATA DSP_ADC_RAW;
#pragma DATA_SECTION(DSP_ADC_RAW, "Cla1ToCpuMsgRAM");
DSP_ADC_MON_DATA DSP_ADC_MON;
#pragma DATA_SECTION(DSP_ADC_MON, "Cla1ToCpuMsgRAM");
DSP_DATA DSP;

volatile float32_t  duty_test = 0.5f;
volatile float32_t test_adc = 0.0f;

// Time variables
//volatile float32_t  DTP_SET = 0.15;

// Control variables
DCL_DF13 EV_gv;
DCL_DF13 EV_gc;

// Initializing buffer
void Init_variables(void)
{
    // Command variables
    DSP.DSP_command.Calibration   = 1.0f;
    DSP.DSP_command.SW_FREQ_SET   = 85.0f;
    DSP.DSP_command.Po            = 100.0f;
    DSP.DSP_command.eff           = 0.95f;
    DSP.DSP_command.Vi            = 400.0f;
    DSP.DSP_command.Vo            = 800.0f;
    DSP.DSP_protection.Vo_OVP        = 850.0f;
    DSP.DSP_protection.IL_OCP        = 32.0f; // 11 kW, 45.0f for 22 kW
    DSP.DSP_protection.Io_OCP        = 15.0f; // 11 kW, 30.0f for 22 kW
    DSP.DSP_protection.EV_OTP        = 85.0f;

    // Control variables
    memset((void*)&DSP.DSP_ctrl, 0, sizeof(DSP.DSP_ctrl));
    DSP.DSP_ctrl.gain_gv    = 0.12;
    DSP.DSP_ctrl.gain_gc    = 0.01;
    DCL_resetDF13(&EV_gv);
    DCL_resetDF13(&EV_gc);
}

void Control_initVariables(void)
{
    EV_gv.a1 = -EV_gv_a1;
    EV_gv.a2 = -EV_gv_a2;
    EV_gv.a3 = -EV_gv_a3;
    EV_gv.b0 = EV_gv_b0;
    EV_gv.b1 = EV_gv_b1;
    EV_gv.b2 = EV_gv_b2;
    EV_gv.b3 = EV_gv_b3;

    EV_gc.a1 = -EV_gc_a1;
    EV_gc.a2 = -EV_gc_a2;
    EV_gc.a3 = -EV_gc_a3;
    EV_gc.b0 = EV_gc_b0;
    EV_gc.b1 = EV_gc_b1;
    EV_gc.b2 = EV_gc_b2;
    EV_gc.b3 = EV_gc_b3;
}

// Initializing PWM
void Init_PWM(void)
{
    // PHASE B
    EPWM_setTimeBasePeriod(EV_PWM_B_main_BASE, SW_PRD);
}

void Init_GPIO(void)
{
    GPIO_writePin(EV_GD_EN_seed, 0);
    GPIO_writePin(EV_LED1, 1);
    GPIO_writePin(EV_LED2, 1);
    GPIO_writePin(EV_LED3, 1);
}

void Init_cla(void)
{
    extern uint32_t CLA1mathTablesRunStart, CLA1mathTablesLoadStart, CLA1mathTablesLoadSize;

    asm(" EALLOW");

    memcpy((uint32_t *)&CLA1mathTablesRunStart, (uint32_t *)&CLA1mathTablesLoadStart,
            (uint32_t)&CLA1mathTablesLoadSize);
}

// OVP
void OVP_check(void)
{
    if(DSP_ADC_RAW.Vo_sen >= Vo_OVP_ADC)
    {
        DSP.DSP_protection.EV_OVP_flag = 1;
    }
}

// OCP
void OCP_check(void)
{
    if((DSP_ADC_RAW.IL1_sen >= IL_OCP_ADC) || (DSP_ADC_RAW.IL2_sen >= IL_OCP_ADC)
        || (DSP_ADC_RAW.Io_sen >= Io_OCP_ADC))
    {
        if(DSP_ADC_RAW.IL1_sen >= IL_OCP_ADC)
        {
            DSP.DSP_protection.EV_IL1_OCP_flag = 1;
        }
        if(DSP_ADC_RAW.IL2_sen >= IL_OCP_ADC)
        {
            DSP.DSP_protection.EV_IL2_OCP_flag = 1;
        }
        if(DSP_ADC_RAW.Io_sen >= Io_OCP_ADC)
        {
            DSP.DSP_protection.EV_Io_OCP_flag = 1;
        }
    }
}

// OTP
void OTP_check(void)
{
    if((DSP_ADC_MON.Temp_Q1_mon >= DSP.DSP_protection.EV_OTP)
        || (DSP_ADC_MON.Temp_Q2_mon >= DSP.DSP_protection.EV_OTP)
        || (DSP_ADC_MON.Temp_Qs1_mon >= DSP.DSP_protection.EV_OTP)
        || (DSP_ADC_MON.Temp_Qs2_mon >= DSP.DSP_protection.EV_OTP)
        || (DSP_ADC_MON.Temp_L_mon >= DSP.DSP_protection.EV_OTP)
        || (DSP_ADC_MON.Temp_BDG_D_mon >= DSP.DSP_protection.EV_OTP))
    {
        if((DSP_ADC_MON.Temp_Q1_mon >= DSP.DSP_protection.EV_OTP)
            || (DSP_ADC_MON.Temp_Q2_mon >= DSP.DSP_protection.EV_OTP))
        {
            DSP.DSP_protection.EV_Qm_OTP_flag = 1;
        }
        if((DSP_ADC_MON.Temp_Qs1_mon >= DSP.DSP_protection.EV_OTP)
            || (DSP_ADC_MON.Temp_Temp_Qs2_monQ2_mon >= DSP.DSP_protection.EV_OTP))
        {
            DSP.DSP_protection.EV_Qs_OTP_flag = 1;
        }
        if(DSP_ADC_MON.Temp_L_mon >= DSP.DSP_protection.EV_OTP)
        {
            DSP.DSP_protection.EV_L_OTP_flag = 1;
        }
    }
}

void Softstart(void)
{
//    // Volt.
//    if(fabsf(Vo_set - eta_dsp.dsp_ctrl_data.v_command) > 0.05f)
//    {
//        if(Vo_set > eta_dsp.dsp_ctrl_data.v_command)
//        {
//            eta_dsp.dsp_ctrl_data.v_command = eta_dsp.dsp_ctrl_data.v_command
//                                              + 0.02f;
//        }
//        else
//        {
//            eta_dsp.dsp_ctrl_data.v_command = eta_dsp.dsp_ctrl_data.v_command
//                                              - 0.02f;
//        }
//    }
//    else if(fabsf(Vo_set - eta_dsp.dsp_ctrl_data.v_command) > 0.005f)
//    {
//        if(Vo_set > eta_dsp.dsp_ctrl_data.v_command)
//        {
//            eta_dsp.dsp_ctrl_data.v_command = eta_dsp.dsp_ctrl_data.v_command
//                                              + 0.002f;
//        }
//        else
//        {
//            eta_dsp.dsp_ctrl_data.v_command = eta_dsp.dsp_ctrl_data.v_command
//                                              - 0.002f;
//        }
//    }
//    else
//    {
//        eta_dsp.dsp_ctrl_data.v_command = Vo_set;
//    }
//
//    // Current
//    if(fabsf(Io_set - eta_dsp.dsp_ctrl_data.i_command) > 0.05f)
//    {
//        if(Io_set > eta_dsp.dsp_ctrl_data.i_command)
//        {
//            eta_dsp.dsp_ctrl_data.i_command = eta_dsp.dsp_ctrl_data.i_command
//                                              + 0.02f;
//        }
//        else
//        {
//            eta_dsp.dsp_ctrl_data.i_command = eta_dsp.dsp_ctrl_data.i_command
//                                              - 0.02f;
//        }
//    }
//    else if(fabsf(Io_set - eta_dsp.dsp_ctrl_data.i_command) > 0.005f)
//    {
//        if(Io_set > eta_dsp.dsp_ctrl_data.i_command)
//        {
//            eta_dsp.dsp_ctrl_data.i_command = eta_dsp.dsp_ctrl_data.i_command
//                                              + 0.002f;
//        }
//        else
//        {
//            eta_dsp.dsp_ctrl_data.i_command = eta_dsp.dsp_ctrl_data.i_command
//                                              - 0.002f;
//        }
//    }
//    else
//    {
//        eta_dsp.dsp_ctrl_data.i_command = Io_set;
//    }
}

// Digital compensation
void Control_loop(void)
{
//    // DCL_DF13 compensation
//    eta_dsp.dsp_ctrl_data.v_ctrl_DCL = DCL_runDF13_C5(&Tx_gv,
//                                                      eta_dsp.dsp_ctrl_data.v_err,
//                                                      eta_dsp.dsp_ctrl_data.v_ctrl_pc);
//    if(eta_dsp.dsp_ctrl_data.v_ctrl_DCL > TX_ctrlMax)
//    {
//        eta_dsp.dsp_ctrl_data.v_ctrl_DCL = TX_ctrlMax;
//    }
//    else if(eta_dsp.dsp_ctrl_data.v_ctrl_DCL < -10.0)
//    {
//        eta_dsp.dsp_ctrl_data.v_ctrl_DCL = -10.0;
//    }
//    eta_dsp.dsp_ctrl_data.v_ctrl_pc = DCL_runDF13_C6(&Tx_gv,
//                                                     eta_dsp.dsp_ctrl_data.v_err,
//                                                     eta_dsp.dsp_ctrl_data.v_ctrl_DCL);
//
//    eta_dsp.dsp_ctrl_data.i_ctrl_DCL = DCL_runDF13_C5(&Tx_gc,
//                                                      eta_dsp.dsp_ctrl_data.i_err,
//                                                      eta_dsp.dsp_ctrl_data.i_ctrl_pc);
//    if(eta_dsp.dsp_ctrl_data.i_ctrl_DCL > TX_ctrlMax)
//    {
//        eta_dsp.dsp_ctrl_data.i_ctrl_DCL = TX_ctrlMax;
//    }
//    else if(eta_dsp.dsp_ctrl_data.i_ctrl_DCL < -10.0)
//    {
//        eta_dsp.dsp_ctrl_data.i_ctrl_DCL = -10.0;
//    }
//    eta_dsp.dsp_ctrl_data.i_ctrl_pc = DCL_runDF13_C6(&Tx_gc,
//                                                     eta_dsp.dsp_ctrl_data.i_err,
//                                                     eta_dsp.dsp_ctrl_data.i_ctrl_DCL);
//
//    if(eta_dsp.dsp_ctrl_data.i_ctrl_DCL >= eta_dsp.dsp_ctrl_data.v_ctrl_DCL)
//    {
//        eta_dsp.dsp_ctrl_data.vi_ctrl_DCL = eta_dsp.dsp_ctrl_data.v_ctrl_DCL;
//    }
//    else if(eta_dsp.dsp_ctrl_data.i_ctrl_DCL < eta_dsp.dsp_ctrl_data.v_ctrl_DCL)
//    {
//        eta_dsp.dsp_ctrl_data.vi_ctrl_DCL = eta_dsp.dsp_ctrl_data.i_ctrl_DCL;
//    }
}
