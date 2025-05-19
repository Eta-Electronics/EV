//#############################################################################
//
// FILE:   EV_VA.h
//
// Written by D. Kim
// Date: 20250512
// Purpose: EV_VA header code
//
//#############################################################################

#ifdef __cplusplus
extern "C" {
#endif

// Included Files
#include "driverlib.h"
#include "device.h"
#include "board.h"

void Init_variables(void);
void Control_initVariables(void);
void Init_PWM(void);
void Init_GPIO(void);
void Init_cla(void);
void OVP_check(void);
void OCP_check(void);
void OTP_check(void);
void Softstart(void);
void Control_loop(void);

extern volatile float32_t   duty_test;
extern volatile float32_t   test_adc;

// Time variables
#define SW_FREQ             ((float32_t) DSP.DSP_command.SW_FREQ_SET * 1000)
#define SW_PRD              ((float32_t) DEVICE_SYSCLK_FREQ / SW_FREQ)
#define PHASE               ((float32_t) 2)
#define Parallel            ((float32_t) 2)
#define PHASE_PS            ((float32_t) SW_PRD / PHASE * 2)

//extern volatile float32_t   DTP_SET;
//#define DTP_COEFF           ((float32_t) DEVICE_SYSCLK_FREQ / 120 / SW_FREQ)
//#define DTP                 ((float32_t) DTP_SET * (DEVICE_SYSCLK_FREQ / SW_FREQ / DTP_COEFF))

// ADC parameters
#define Vadc                ((float32_t) 4096)
#define Vadc_V              ((float32_t) 3.0)

#define R1_Vi               ((float32_t) 5.0)
#define R2_Vi               ((float32_t) 1000.0 + 1000.0 + 1000.0)
#define R1_Vo               ((float32_t) 5.0)
#define R2_Vo               ((float32_t) 750.0 + 750.0 + 750.0)

#define Vi_max              ((float32_t) 1202.0)
#define Vi_sen_max          ((float32_t) Vi_max * R1_Vi / (R1_Vi + R2_Vi))

#define Vo_max              ((float32_t) 902.0)
#define Vo_sen_max          ((float32_t) Vo_max * R1_Vo / (R1_Vo + R2_Vo))
#define Vo_adc              ((float32_t) Vo_sen_max * Vadc / Vadc_V)
#define Vo_set              ((float32_t) Vo_adc * DSP.DSP_command.Vo / Vo_max)

#define I_max               ((float32_t) 50.0)
#define I_sen_max           ((float32_t) I_max * 0.04)
#define I_adc               ((float32_t) I_sen_max * Vadc / Vadc_V)
#define Ii_set              ((float32_t) I_adc / I_max * DSP.DSP_command.Po / DSP.DSP_command.Vi / DSP.DSP_command.eff * DSP.DSP_command.Calibration)

// Temperature parameters
#define KT                  ((float32_t) 273)
#define beta                ((float32_t) 3435)
#define T0                  ((float32_t) 25 + KT)

//#define Buck_FET_Rth        ((float32_t) 0.1)
//#define Buck_FET_cal        ((float32_t) 1.3)
//#define FB_FET_Rth          ((float32_t) 2.5)
//#define FB_FET_cal          ((float32_t) 0.85)

//#define FET_Rds_on          ((float32_t) 55  * 0.001)
//#define FET_tr              ((float32_t) 44  * 0.000000001)
//#define FET_tf              ((float32_t) 102 * 0.000000001)
//#define FET_tft             ((float32_t) 15  * 6.8 * 0.000000001)
//#define FET_Irr             ((float32_t) 8.4)
//#define FET_trr             ((float32_t) 184 * 0.000000001)

// Protection
#define Vo_OVP_ADC          ((float32_t) DSP.DSP_protection.Vo_OVP * Vo_sen_max / Vo_max * Vadc / Vadc_V)
#define IL_OCP_ADC          ((float32_t) DSP.DSP_protection.IL_OCP * I_sen_max / I_max * Vadc / Vadc_V)
#define Io_OCP_ADC          ((float32_t) DSP.DSP_protection.Io_OCP * I_sen_max / I_max * Vadc / Vadc_V)


// Control variables
#define EV_gv_b0    ((float32_t)   8.011535 * DSP.DSP_ctrl.gain_gv)
#define EV_gv_b1    ((float32_t)  -7.547092 * DSP.DSP_ctrl.gain_gv)
#define EV_gv_b2    ((float32_t)  -8.004804 * DSP.DSP_ctrl.gain_gv)
#define EV_gv_b3    ((float32_t)   7.553824 * DSP.DSP_ctrl.gain_gv)
#define EV_gv_a1    ((float32_t)   -0.705606)
#define EV_gv_a2    ((float32_t)   0.978333)
#define EV_gv_a3    ((float32_t)   0.727273)

//#define EV_gv_b0    ((float32_t)  2.816546 * gain_gv)
//#define EV_gv_b1    ((float32_t) -2.613869 * gain_gv)
//#define EV_gv_b2    ((float32_t) -2.813550 * gain_gv)
//#define EV_gv_b3    ((float32_t)  2.616865 * gain_gv)
//#define EV_gv_a1    ((float32_t) -0.397391)
//#define EV_gv_a2    ((float32_t)  0.942195)
//#define EV_gv_a3    ((float32_t)  0.455196)

#define EV_gc_b0    ((float32_t)  0.309927 * DSP.DSP_ctrl.gain_gc)
#define EV_gc_b1    ((float32_t) -0.286035 * DSP.DSP_ctrl.gain_gc)
#define EV_gc_b2    ((float32_t) -0.309516 * DSP.DSP_ctrl.gain_gc)
#define EV_gc_b3    ((float32_t)  0.286446 * DSP.DSP_ctrl.gain_gc)
#define EV_gc_a1    ((float32_t)  1.107215)
#define EV_gc_a2    ((float32_t)  0.762088)
#define EV_gc_a3    ((float32_t) -0.869304)

#define EV_LFP_b0   ((float32_t)  0.000010)
#define EV_LFP_b1   ((float32_t)  0.000020)
#define EV_LFP_b2   ((float32_t)  0.000010)
#define EV_LFP_a1   ((float32_t)  1.987473)
#define EV_LFP_a2   ((float32_t) -0.987512)

// Clamp
//#define D_max               ((float32_t) Vo_max / Vi_min)
//#define D_min               ((float32_t) Vo_min / Vi_max)

#define EV_ctrlMax          ((float32_t) 0.5 * SW_PRD)
#define EV_ctrlMin          ((float32_t) 0)

//CLA C Tasks defined in Cla1Tasks_C.cla
__attribute__((interrupt))  void Cla1Task1();
__attribute__((interrupt))  void Cla1Task2();
__attribute__((interrupt))  void Cla1Task3();
__attribute__((interrupt))  void Cla1Task4();
__attribute__((interrupt))  void Cla1Task5();
__attribute__((interrupt))  void Cla1Task6();
__attribute__((interrupt))  void Cla1Task7();
__attribute__((interrupt))  void Cla1Task8();

#ifdef __cplusplus
}
#endif // extern "C"

// Struct

// Command parameters
typedef struct
{
    float   Calibration;
    float   SW_FREQ_SET;
    float   Po, eff, Vi, Vo;
}DSP_command_DATA;

// Protection parameters
typedef struct
{
    float   Vo_OVP, IL_OCP, Io_OCP, EV_OTP;
    int     EV_OVP_flag, EV_IL1_OCP_flag, EV_IL2_OCP_flag, EV_Io_OCP_flag;
    int     EV_Qm_OTP_flag, EV_Qs_OTP_flag, EV_L_OTP_flag, EV_BDG_OTP_flag, EV_DSP_OTP_flag;
}DSP_protection_DATA;

// Control parameters
typedef struct
{
    float   v_command, i_command, v_err, i_err;
    float   v_ctrl, i_ctrl, vi_ctrl;
    float   gain_gv, gain_gc;
}DSP_ctrl_DATA;

// ADC parameters
typedef struct
{
    float   Va_sen, Vb_sen, Vo_sen;
    float   IL1_sen, IL2_sen, Io_sen;
    float   Temp_Q1_sen, Temp_Q2_sen, Temp_Qs1_sen, Temp_Qs2_sen, Temp_L_sen, Temp_BDG_D_sen;
    float   DACA_sen, DACB_sen;
}DSP_ADC_DATA;

extern DSP_ADC_DATA DSP_ADC_RAW;

// Monitoring parameters
typedef struct
{
    float   Va_mon, Vb_mon, Vo_mon;
    float   IL1_mon, IL2_mon, Io_mon;
    float   Temp_Q1_mon, Temp_Q2_mon, Temp_Qs1_mon, Temp_Qs2_mon, Temp_L_mon, Temp_BDG_D_mon;
    float   DACA_mon, DACB_mon;
}DSP_ADC_MON_DATA;

extern DSP_ADC_MON_DATA DSP_ADC_MON;

typedef struct
{
    DSP_command_DATA DSP_command;
    DSP_protection_DATA DSP_protection;
    DSP_ctrl_DATA DSP_ctrl;
//    DSP_ADC_DATA DSP_ADC_RAW;
//    DSP_ADC_MON_DATA DSP_ADC_MON;
}DSP_DATA;
extern DSP_DATA DSP;
