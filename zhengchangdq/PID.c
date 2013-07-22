////////////////////////////////////////////////////////////////////////////////
//   Created by DJ.
//   2010-07-02
//////////////////////////////////////////////////////////////////////////////////


#include "DSP2833x_Device.h"
#include "IQmathLib.h"
#include "Pwm_rectifier.h"



void pi(PID *p)
{
 p->err=p->ref-p->fdb;
 p->up=_IQmpy(p->err,p->Kp);
 if (((p->up + p->ui) < p->upper_limit) && ((p->up + p->ui) > p->lower_limit)) {
	p->ui = p->ui+_IQmpy(p->err,p->Ki);
 }
 p->uo=_IQsat(p->up+p->ui,p->upper_limit,p->lower_limit);
}


void pr(PR *p)
{
 p->err=p->ref-p->fdb;
 p->ur_k=_IQmpy(p->KiTs,p->err)-_IQmpy(p->awTs,p->ur_k_1)-_IQmpy(p->Ts,p->ef_k_1)+p->ur_k_1;
 p->up_k=_IQmpy(p->err,p->Kp);
 p->uo=_IQsat(p->up_k+p->ur_k,p->upper_limit,p->lower_limit);

 p->ur_k_1=p->ur_k;
 p->ef_k_1=_IQmpy(p->w2Ts,p->ur_k_1)+p->ef_k_2;
 p->ef_k_2=p->ef_k_1;

}
