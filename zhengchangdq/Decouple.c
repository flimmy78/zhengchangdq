////////////////////////////////////////////////////////////////////////////////
//   Created by DJ.
//   2010-07-02
//////////////////////////////////////////////////////////////////////////////////


#include "DSP2833x_Device.h"
#include "IQmathLib.h"
#include "Pwm_rectifier.h"
#include "PLL.h"

void decouple(DECOUPLE *p,THETA_REGS *q)
{
 if(p->phase_or_line_in_selection==1)
 {
  p->L_abc2dq.ab=p->ab;
  p->L_abc2dq.bc=p->bc;
  p->L_abc2dq_transformation_P(&(p->L_abc2dq),q);

  p->Pd_coupled=p->L_abc2dq.d;
  p->Pq_coupled=p->L_abc2dq.q;
 
  p->L_abc2dq_transformation_N(&(p->L_abc2dq),q);
  p->Nd_coupled=p->L_abc2dq.d;
  p->Nq_coupled=p->L_abc2dq.q;
 }
 if(p->phase_or_line_in_selection==0)
 {
  p->abc2dq.a=p->a;
  p->abc2dq.b=p->b;
  p->abc2dq.c=p->c;
  p->abc2dq_transformation_P(&(p->abc2dq),q);

  p->Pd_coupled=p->abc2dq.d;
  p->Pq_coupled=p->abc2dq.q;
 
  p->abc2dq_transformation_N(&(p->abc2dq),q);
  p->Nd_coupled=p->abc2dq.d;
  p->Nq_coupled=p->abc2dq.q;
 }

 p->Nd_decouple_out=p->Nd_coupled-_IQmpy(p->Pd,q->cos_2theta)+_IQmpy(p->Pq,q->sin_2theta);
 p->Nq_decouple_out=p->Nq_coupled-_IQmpy(p->Pd,q->sin_2theta)-_IQmpy(p->Pq,q->cos_2theta);
 
 p->filter_Nd.ui_k=p->Nd_decouple_out;
 p->filter_Nq.ui_k=p->Nq_decouple_out;
 p->first_order_filter(&(p->filter_Nd));
 p->first_order_filter(&(p->filter_Nq));
 p->Nd=p->filter_Nd.uo_k;
 p->Nq=p->filter_Nq.uo_k;
 
 p->Pd_decouple_out=p->Pd_coupled-_IQmpy(p->Nd,q->cos_2theta)-_IQmpy(p->Nq,q->sin_2theta);
 p->Pq_decouple_out=p->Pq_coupled-_IQmpy(p->Nq,q->cos_2theta)+_IQmpy(p->Nd,q->sin_2theta);

 p->filter_Pd.ui_k=p->Pd_decouple_out;
 p->filter_Pq.ui_k=p->Pq_decouple_out;
 p->first_order_filter(&(p->filter_Pd));
 p->first_order_filter(&(p->filter_Pq));
 p->Pd=p->filter_Pd.uo_k;
 p->Pq=p->filter_Pq.uo_k;

}

void decouple_init(DECOUPLE *p)
{
 p->filter_Pd.wb=167;//设定低通滤波器截至频率
 p->filter_Pd.wbT=_IQ((p->filter_Pd.wb)*TPWM);
 p->filter_Pq.wb=p->filter_Pd.wb;
 p->filter_Nq.wb=p->filter_Pd.wb;
 p->filter_Nd.wb=p->filter_Pd.wb;
 p->filter_Pq.wbT=p->filter_Pd.wbT;
 p->filter_Nq.wbT=p->filter_Pd.wbT;
 p->filter_Nd.wbT=p->filter_Pd.wbT;
 p->filter_Pd.wbT_p_1__1=_IQdiv(_IQ(1),_IQ(1)+p->filter_Pd.wbT);
 p->filter_Pq.wbT_p_1__1=p->filter_Pd.wbT_p_1__1;
 p->filter_Nd.wbT_p_1__1=p->filter_Pd.wbT_p_1__1;
 p->filter_Nq.wbT_p_1__1=p->filter_Pd.wbT_p_1__1;

}
