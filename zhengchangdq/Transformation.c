/////////////////////////////////////////////////////////////////////////////////////
// Created by DJ for Coordinate transformation 
// 2010-07-02
////////////////////////////////////////////////////////////////////////////////////
#include "DSP2833x_Device.h"
#include "IQmathLib.h"
#include "Pwm_rectifier.h"

void cos_sin_cal(THETA_REGS *p)
{
 p->cos_theta=_IQcos(p->theta);
 p->sin_theta=_IQsin(p->theta);
 
 //p->cos_2theta=_IQcos(p->theta<<1);
 //p->sin_2theta=_IQsin(p->theta<<1);

 p->cos_2theta=_IQmpy(p->cos_theta,p->cos_theta)-_IQmpy(p->sin_theta,p->sin_theta);
 p->sin_2theta=_IQmpy(p->sin_theta,p->cos_theta)<<1;

 p->cos_theta_p_120=_IQmpy(p->cos_theta,p->cos_120)-_IQmpy(p->sin_theta,p->sin_120);
 p->cos_theta_m_120=_IQmpy(p->cos_theta,p->cos_120)+_IQmpy(p->sin_theta,p->sin_120);

 p->sin_theta_p_120=_IQmpy(p->sin_theta,p->cos_120)+_IQmpy(p->cos_theta,p->sin_120);
 p->sin_theta_m_120=_IQmpy(p->sin_theta,p->cos_120)-_IQmpy(p->cos_theta,p->sin_120);
 
}

void dq2abc(DQ2ABC *p,THETA_REGS *q)
{
 #define c1 _IQ(0.66666666666666666666667)  //2/3
 p->a=_IQmpy(q->cos_theta,p->d)- _IQmpy(q->sin_theta,p->q);
 p->b=_IQmpy(q->cos_theta_m_120,p->d)- _IQmpy(q->sin_theta_m_120,p->q);
 p->c=_IQmpy(q->cos_theta_p_120,p->d)- _IQmpy(q->sin_theta_p_120,p->q);
}

void dq2abc_N(DQ2ABC *p,THETA_REGS *q)
{
 #define c1 _IQ(0.66666666666666666666667)  //2/3
 p->a=_IQmpy(q->cos_theta,p->d)+ _IQmpy(q->sin_theta,p->q);
 p->b=_IQmpy(q->cos_theta_p_120,p->d)+ _IQmpy(q->sin_theta_p_120,p->q);
 p->c=_IQmpy(q->cos_theta_m_120,p->d)+ _IQmpy(q->sin_theta_m_120,p->q);
}

void abc2dq(ABC2DQ *p,THETA_REGS *q)
{
#define c1 _IQ(0.66666666666666666666667)  //2/3
p->d=_IQmpy(_IQmpy(q->cos_theta,p->a)+_IQmpy(q->cos_theta_m_120,p->b)+_IQmpy(q->cos_theta_p_120,p->c),c1);
p->q=_IQmpy(_IQmpy(-q->sin_theta,p->a)-_IQmpy(q->sin_theta_m_120,p->b)-_IQmpy(q->sin_theta_p_120,p->c),c1);
}

void abc2dq_N(ABC2DQ *p,THETA_REGS *q)
{
#define c1 _IQ(0.66666666666666666666667)  //2/3
p->d=_IQmpy(_IQmpy(q->cos_theta,p->a)+_IQmpy(q->cos_theta_p_120,p->b)+_IQmpy(q->cos_theta_m_120,p->c),c1);
p->q=_IQmpy(_IQmpy(q->sin_theta,p->a)+_IQmpy(q->sin_theta_p_120,p->b)+_IQmpy(q->sin_theta_m_120,p->c),c1);
}

void L_abc2dq(L_ABC2DQ *p,THETA_REGS *q)
{
#define c1 _IQ(0.66666666666666666666667)  //2/3
p->d=_IQmpy(_IQmpy(q->cos_theta,p->ab)-_IQmpy(q->cos_theta_p_120,p->bc),c1);
p->q=_IQmpy(_IQmpy(-q->sin_theta,p->ab)+_IQmpy(q->sin_theta_p_120,p->bc),c1);
}

void L_abc2dq_N(L_ABC2DQ *p,THETA_REGS *q)
{
#define c1 _IQ(0.66666666666666666666667)  //2/3
p->d=_IQmpy(_IQmpy(q->cos_theta,p->ab)-_IQmpy(q->cos_theta_m_120,p->bc),c1);
p->q=_IQmpy(_IQmpy(q->sin_theta,p->ab)+_IQmpy(-q->sin_theta_m_120,p->bc),c1);
}


