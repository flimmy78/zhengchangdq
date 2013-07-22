////////////////////////////////////////////////////////////////////////////////
//   Created by DJ.
//   2010-07-02
//////////////////////////////////////////////////////////////////////////////////


#include "DSP2833x_Device.h"
#include "IQmathLib.h"
#include "Pwm_rectifier.h"
#include "PLL.h"


void negative_seq_ref_cal(N_SEQ_REF *p,DECOUPLE *i,DECOUPLE *u)
{
 if(p->enable==0)
 {
  p->counter++;
  p->id_N_ref=0;
  p->iq_N_ref=0;

  if(p->counter==p->delay_on_time)
  {p->enable=1;}
 }
 else
 {
  p->d_d_p_q_q=_IQmpy(u->Pd,u->Pd)+_IQmpy(u->Pq,u->Pq);
  if(p->d_d_p_q_q<p->d_d_p_q_q_limit)
  {p->d_d_p_q_q=p->d_d_p_q_q_limit;}

  p->d_d_p_q_q__1=_IQdiv(_IQ(1),p->d_d_p_q_q);

  p->id_N_ref=_IQmpy(_IQmpy(u->Nq,u->Pq)-_IQmpy(u->Nd,u->Pd),i->Pd)-_IQmpy(_IQmpy(u->Nq,u->Pd)+_IQmpy(u->Nd,u->Pq),i->Pq);
  p->id_N_ref=_IQsat(_IQmpy(p->id_N_ref,p->d_d_p_q_q__1),p->ref_upper_limit,p->ref_lower_limit);

  p->iq_N_ref=-_IQmpy(_IQmpy(u->Nq,u->Pd)+_IQmpy(u->Nd,u->Pq),i->Pd)+_IQmpy(_IQmpy(u->Nd,u->Pd)-_IQmpy(u->Nq,u->Pq),i->Pq);
  p->iq_N_ref=_IQsat(_IQmpy(p->iq_N_ref,p->d_d_p_q_q__1),p->ref_upper_limit,p->ref_lower_limit);

 }

}


void negative_seq_ref_init(N_SEQ_REF *p)
{
 p->counter=0;
 p->delay_on_time=80*40;
 p->enable=0;

 p->d_d_p_q_q_limit=_IQ(0.0001);
 p->ref_upper_limit=_IQ(0.5);
 p->ref_lower_limit=-_IQ(0.5);

}
