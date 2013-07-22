////////////////////////////////////////////////////////////////////////////////////
//Created by DJ for PLL file 
//2010-07-02
///////////////////////////////////////////////////////////////////////////////////

#include "DSP2833x_Device.h"
#include "IQmathLib.h"
#include "Pwm_rectifier.h"
#include "PLL.h"

/*void PLL(PLLRegs *p,THETA_REGS *q)
{
 p->decouple_cal(&(p->decouple),q);

 p->pi.ref=_IQmpy(p->decouple.Pq_decouple_out,_IQ(10));//100
 p->pi.fdb=0;
 p->PI_cal(&(p->pi));

 p->w=p->w_fedforward+p->pi.uo;

 p->theta=_IQmpy(p->w,_IQ(TPWM))+p->w_integration;

 if(p->decouple.Pd_decouple_out<_IQ(0.0))
 {p->theta=p->theta+_IQ(PI);}

 if((p->theta)>=_IQ(2*PI))
 {p->theta=p->theta-_IQ(2*PI);}

 if((p->theta)<=-_IQ(2*PI))
 {p->theta=p->theta+_IQ(2*PI);}

  p->w_integration=p->theta;
}*/


void PLL(PLLRegs *p,THETA_REGS *q)
{
 p->pllabc2dq_cal(&(p->pllabc2dq),q);

 p->pi.ref=_IQmpy(p->pllabc2dq.q,_IQ(10));//100
 p->pi.fdb=0;
 p->PI_cal(&(p->pi));

 p->w=p->w_fedforward+p->pi.uo;

 p->theta=_IQmpy(p->w,_IQ(TPWM))+p->w_integration;

 if(p->pllabc2dq.d<_IQ(0.0))
 {p->theta=p->theta+_IQ(PI);}

 if((p->theta)>=_IQ(2*PI))
 {p->theta=p->theta-_IQ(2*PI);}

 if((p->theta)<=-_IQ(2*PI))
 {p->theta=p->theta+_IQ(2*PI);}

  p->w_integration=p->theta;
}



void pll_init(PLLRegs *p) 
 {
 //p->decouple.Init(&(p->decouple));
 p->pllabc2dq.d=0;
 p->pllabc2dq.q=0;
 p->pi.Kp=_IQ(5);//50
 p->pi.Ki=_IQmpy(_IQ(50),_IQ(TPWM*10.0));//500-200
 //p->pi.Ki=_IQ(1000);
 p->pi.upper_limit=_IQ(200);
 p->pi.lower_limit=-_IQ(200);
 
 }


