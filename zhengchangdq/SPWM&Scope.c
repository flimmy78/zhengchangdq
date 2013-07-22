////////////////////////////////////////////////////////////////////////////////
//   Created by DJ.
//   2010-07-02
//////////////////////////////////////////////////////////////////////////////////


#include "DSP2833x_Device.h"
#include "IQmathLib.h"
#include "Pwm_rectifier.h"



void update_compare(PWM *p)
{ 
 EPwm3Regs.CMPA.half.CMPA=_IQsat(_IQmpy(_IQ(0.5)-(p->c>>1),EPWM1_TIMER_TBPRD),EPWM1_MAX_CMPA,EPWM1_MIN_CMPA);
 EPwm2Regs.CMPA.half.CMPA=_IQsat(_IQmpy(_IQ(0.5)-(p->b>>1),EPWM1_TIMER_TBPRD),EPWM1_MAX_CMPA,EPWM1_MIN_CMPA);
 EPwm1Regs.CMPA.half.CMPA=_IQsat(_IQmpy(_IQ(0.5)-(p->a>>1),EPWM1_TIMER_TBPRD),EPWM1_MAX_CMPA,EPWM1_MIN_CMPA);
}
  


void scope(long ch1,long mode1,long ch2,long mode2,long ch3,long mode3)
{
 if(mode1==DC)
 {EPwm4Regs.CMPA.half.CMPA=_IQsat(_IQmpy(_IQ(1)-ch1,EPWM1_TIMER_TBPRD),EPWM1_MAX_CMPA,EPWM1_MIN_CMPA);}
 else
 {EPwm4Regs.CMPA.half.CMPA=_IQsat(_IQmpy(_IQ(0.5)-(ch1>>1),EPWM1_TIMER_TBPRD),EPWM1_MAX_CMPA,EPWM1_MIN_CMPA);}
 if(mode2==DC)
 {EPwm5Regs.CMPA.half.CMPA=_IQsat(_IQmpy(_IQ(1)-ch2,EPWM1_TIMER_TBPRD),EPWM1_MAX_CMPA,EPWM1_MIN_CMPA);}
 else
 {EPwm5Regs.CMPA.half.CMPA=_IQsat(_IQmpy(_IQ(0.5)-(ch2>>1),EPWM1_TIMER_TBPRD),EPWM1_MAX_CMPA,EPWM1_MIN_CMPA);}
 if(mode3==DC)
 {EPwm6Regs.CMPA.half.CMPA=_IQsat(_IQmpy(_IQ(1)-ch3,EPWM1_TIMER_TBPRD),EPWM1_MAX_CMPA,EPWM1_MIN_CMPA);}
 else
 {EPwm6Regs.CMPA.half.CMPA=_IQsat(_IQmpy(_IQ(0.5)-(ch3>>1),EPWM1_TIMER_TBPRD),EPWM1_MAX_CMPA,EPWM1_MIN_CMPA);}

}
