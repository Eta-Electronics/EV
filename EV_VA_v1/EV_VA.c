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

// Command

volatile float32_t  calibration = 1.0f;

// Time variables
volatile float32_t  SW_FREQ_SET = 85.0f;
//volatile float32_t  DTP_SET = 0.15;

// Parameters
volatile float32_t  Vo = 800.0f;

// Protection
volatile float32_t  Vo_ovp = 850.0f;

// Control variables
float32_t   gain_gv     = 0.12;
float32_t   gain_gc     = 0.01;
DCL_DF13 Tx_gv;
DCL_DF13 Tx_gc;

// Initializing buffer
void initVariables(void)
{
//    DCL_resetDF13(&Tx_gv);
//    DCL_resetDF13(&Tx_gc);
}

// Initializing PWM
void initPWM(void)
{
    // PHASE B
    EPWM_setTimeBasePeriod(EV_PWM_B_main_BASE, SW_PRD);
}

void initGPIO(void)
{
    GPIO_writePin(EV_GD_EN_seed, 0);
    GPIO_writePin(EV_LED1, 1);
    GPIO_writePin(EV_LED2, 1);
    GPIO_writePin(EV_LED3, 1);
}

void InitCla(void)
{
    extern uint32_t CLA1mathTablesRunStart, CLA1mathTablesLoadStart, CLA1mathTablesLoadSize;

    asm(" EALLOW");

    memcpy((uint32_t *)&CLA1mathTablesRunStart, (uint32_t *)&CLA1mathTablesLoadStart,
            (uint32_t)&CLA1mathTablesLoadSize);
}

// OVP
void OVP_check(void)
{
//    if(dsp_adc_data.Vo_sen >= Vo_ovp_adc)
//    {
//        TX_OVP_flag = 1;
//    }
}

// OCP
void OCP_check(void)
{
//    if((dsp_adc_data.IL1_sen >= Io_ocp_adc) || (dsp_adc_data.IL2_sen >= Io_ocp_adc))
//    {
//        if(dsp_adc_data.IL1_sen >= Io_ocp_adc)
//        {
//            TX_OCP_flag1 = 1;
//        }
//        if(dsp_adc_data.IL2_sen >= Io_ocp_adc)
//        {
//            TX_OCP_flag2 = 1;
//        }
//    }
}

// OTP
void OTP_check(void)
{
//    if( (cla_monitor.Buck_T_mon >= TX_OTP_set) || (eta_dsp.dsp_data.DB.cla_mon.DSP_T_mon >= TX_OTP_set) )
//    {
//        if(cla_monitor.Buck_T_mon >= TX_OTP_set)
//        {
//            TX_FET_OTP_flag = 1;
//        }
//
//        if(eta_dsp.dsp_data.DB.cla_mon.DSP_T_mon >= TX_OTP_set)
//        {
//            TX_DSP_OTP_flag = 1;
//        }
//    }
}

// Digital compensation
inline void Control_loop(void)
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

void control_initVariables(void)
{
//    Tx_gv.a1 = -TX_gv_a1;
//    Tx_gv.a2 = -TX_gv_a2;
//    Tx_gv.a3 = -TX_gv_a3;
//    Tx_gv.b0 = TX_gv_b0;
//    Tx_gv.b1 = TX_gv_b1;
//    Tx_gv.b2 = TX_gv_b2;
//    Tx_gv.b3 = TX_gv_b3;
//
//    Tx_gc.a1 = -TX_gc_a1;
//    Tx_gc.a2 = -TX_gc_a2;
//    Tx_gc.a3 = -TX_gc_a3;
//    Tx_gc.b0 = TX_gc_b0;
//    Tx_gc.b1 = TX_gc_b1;
//    Tx_gc.b2 = TX_gc_b2;
//    Tx_gc.b3 = TX_gc_b3;
}
