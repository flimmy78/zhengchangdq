#ifndef SV_pwm
#define SV_pwm



typedef struct
{
 long a;
 long b;
 long c;
 long ab;
 long bc;
 long ca;
 long S;
 long alfa;
 long beta;
 long D1;
 long D2;
 long X;
 long Y;
 long Z;
 long sqrt_3;
 long sqrt_3_1;
 long Tpwm;
 long T1;
 long T2;
 long T0;
 long T0_4;
 long T0_4_p_T1_2;
 long T0_4_p_T1_2_p_T2_2;
 void (*Init)();
 void (*Cal)();
 void (*Update)();
}SVPWM;
#define SVPWM_DEFAULTS {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,(void (*)(Uint32))SV_PWM_Init,(void (*)(Uint32))SV_PWM_CAL,(void (*)(Uint32))SV_PWM_Update}




extern void SV_PWM_CAL(SVPWM *);
extern void SV_PWM_Init(SVPWM *);
extern void SV_PWM_Update(SVPWM *);
#pragma CODE_SECTION(SV_PWM_CAL, "ramfuncs");






#endif
