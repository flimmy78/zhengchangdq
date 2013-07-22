#ifndef PLL_H
#define PLL_H
#include "Pwm_rectifier.h"
typedef struct
{
 int phase_or_line_in_selection;

 long a;
 long b;
 long c;
 long ab;
 long bc;
 
 long Pd_coupled;
 long Pq_coupled;
 long Nd_coupled;
 long Nq_coupled;
 long Pd_decouple_out;
 long Pq_decouple_out;
 long Nd_decouple_out;
 long Nq_decouple_out;
 long Pd;
 long Pq;
 long Nd;
 long Nq;

 ABC2DQ abc2dq;
 L_ABC2DQ L_abc2dq;
 
 FIRST_ORDER_FILTER filter_Pd;
 FIRST_ORDER_FILTER filter_Pq;
 FIRST_ORDER_FILTER filter_Nd;
 FIRST_ORDER_FILTER filter_Nq;

 void (*abc2dq_transformation_P)();
 void (*abc2dq_transformation_N)();
 void (*L_abc2dq_transformation_P)();
 void (*L_abc2dq_transformation_N)();
 void (*first_order_filter)();
 void (*Init)();
}DECOUPLE;

typedef struct
{
 long theta;
 long w_fedforward;
 long w;   //w_fedforward+PI_OUT
 long w_integration;
 //DECOUPLE decouple;
 ABC2DQ  pllabc2dq;
 PID pi;       
 void (*PI_cal)();
 void (*Init)();
// void (*decouple_cal)();
 void (*pllabc2dq_cal)();
}PLLRegs;

#define DECOUPLE_DEFAULTS{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,ABC2DQ_DEFAULTS,L_ABC2DQ_DEFAULTS,FIRST_ORDER_FILTER_DEFAULTS,FIRST_ORDER_FILTER_DEFAULTS,FIRST_ORDER_FILTER_DEFAULTS,FIRST_ORDER_FILTER_DEFAULTS,(void (*)(Uint32))abc2dq,(void (*)(Uint32))abc2dq_N,(void (*)(Uint32))L_abc2dq,(void (*)(Uint32))L_abc2dq_N,(void (*)(Uint32))first_order_filter,(void (*)(Uint32))decouple_init}
//#define PLL_DEFAULTS {0,_IQ(314.15926),0,0,DECOUPLE_DEFAULTS,PI_DEFAULTS,(void (*)(Uint32))pi,(void (*)(Uint32))pll_init,(void (*)(Uint32))decouple}
#define PLL_DEFAULTS {0,_IQ(314.15926),0,0,ABC2DQ_DEFAULTS,PI_DEFAULTS,(void (*)(Uint32))pi,(void (*)(Uint32))pll_init,(void (*)(Uint32))abc2dq}
extern void decouple(DECOUPLE *,THETA_REGS *);
extern void decouple_init(DECOUPLE *);
extern void abc2dq(ABC2DQ *,THETA_REGS *);
extern void PLL(PLLRegs *,THETA_REGS *);
extern void pll_init(PLLRegs *);
#pragma CODE_SECTION(PLL, "myfuncs");
#pragma CODE_SECTION(decouple, "myfuncs");
#endif
