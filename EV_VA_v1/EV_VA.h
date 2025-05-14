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

void initVariables(void);
void initPWM(void);
void initGPIO(void);
void InitCla(void);
void OVP_check(void);
void OCP_check(void);
void OTP_check(void);
inline void Control_loop(void);
void control_initVariables(void);

// Command
extern volatile float32_t   calibration;

// Time variables
extern volatile float32_t   SW_FREQ_SET;
#define SW_FREQ             ((float32_t) SW_FREQ_SET * 1000)
#define SW_PRD              ((float32_t) DEVICE_SYSCLK_FREQ / SW_FREQ / 2)
#define PHASE               ((float32_t) 2)
#define Parallel            ((float32_t) 2)
#define PHASE_PS            ((float32_t) SW_PRD / PHASE * 2)

//extern volatile float32_t   DTP_SET;
//#define DTP_COEFF           ((float32_t) DEVICE_SYSCLK_FREQ / 120 / SW_FREQ)
//#define DTP                 ((float32_t) DTP_SET * (DEVICE_SYSCLK_FREQ / SW_FREQ / DTP_COEFF))

// ADC parameters
extern volatile float32_t   Vo;
#define Vadc                ((float32_t) 4096)
#define Vadc_V              ((float32_t) 3.3)

#define R1_Vi               ((float32_t) 5.0)
#define R2_Vi               ((float32_t) 1000.0 + 1000.0 + 1000.0)
#define R1_Vo               ((float32_t) 5.0)
#define R2_Vo               ((float32_t) 750.0 + 750.0 + 750.0)

#define Vin_max             ((float32_t) 1202.0)
#define Vin_sen_max         ((float32_t) Vin_max * R1_Vi / (R1_Vi + R2_Vi))

#define Vo_max              ((float32_t) 902.0)
#define Vo_sen_max          ((float32_t) Vo_max * R1_Vo / (R1_Vo + R2_Vo))
#define Vo_adc              ((float32_t) Vo_sen_max * Vadc / Vadc_V)
#define Vo_set              ((float32_t) Vo_adc * Vo / Vo_max)

#define I_max               ((float32_t) 50.0)
#define I_sen_max           ((float32_t) IL_max * 0.04)
#define I_adc               ((float32_t) I_sen_max * Vadc / Vadc_V)
#define I_set               ((float32_t) I_adc * Io / I_max * calibration)

// Temp parameters
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
extern volatile float32_t   Vo_ovp;
extern volatile float32_t   IL_ocp;
extern volatile float32_t   Io_ocp;
extern volatile float32_t   EV_otp;
#define Vo_ovp_adc          ((float32_t) Vo_ovp * Vo_sen_max / Vo_max * Vadc / Vadc_V)
#define IL_ocp_adc          ((float32_t) IL_ocp * I_sen_max / I_max * Vadc  / Vadc_V)
#define Io_ocp_adc          ((float32_t) Io_ocp * I_sen_max / I_max * Vadc  / Vadc_V)


// Control variables
extern float32_t gain_gv;

#define EV_gv_b0    ((float32_t)  2.816546 * gain_gv)
#define EV_gv_b1    ((float32_t) -2.613869 * gain_gv)
#define EV_gv_b2    ((float32_t) -2.813550 * gain_gv)
#define EV_gv_b3    ((float32_t)  2.616865 * gain_gv)
#define EV_gv_a1    ((float32_t) -0.397391)
#define EV_gv_a2    ((float32_t)  0.942195)
#define EV_gv_a3    ((float32_t)  0.455196)

extern float32_t gain_gc;
#define EV_gc_b0    ((float32_t)  0.309927 * gain_gc)
#define EV_gc_b1    ((float32_t) -0.286035 * gain_gc)
#define EV_gc_b2    ((float32_t) -0.309516 * gain_gc)
#define EV_gc_b3    ((float32_t)  0.286446 * gain_gc)
#define EV_gc_a1    ((float32_t)  1.107215)
#define EV_gc_a2    ((float32_t)  0.762088)
#define EV_gc_a3    ((float32_t) -0.869304)

#define EV_LFP_b0 ((float32_t)  0.000010)
#define EV_LFP_b1 ((float32_t)  0.000020)
#define EV_LFP_b2 ((float32_t)  0.000010)
#define EV_LFP_a1 ((float32_t)  1.987473)
#define EV_LFP_a2 ((float32_t)  -0.987512)

// Clamp
//#define D_max               ((float32_t) Vo_max / Vin_min)
//#define D_min               ((float32_t) Vo_min / Vin_max)

//#define TX_ctrlMax          ((float32_t) D_max * SW_PRD)
#define TX_ctrlMax          ((float32_t) 0.92 * SW_PRD) // 일시적으로 124V로 막음
#define TX_ctrlMin          ((float32_t) 0)


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
