////////////////////////////////////////////////////////////////////////////////
//   Created by DJ.
//   2010-07-02
//////////////////////////////////////////////////////////////////////////////////


#include "DSP2833x_Device.h"
#include "IQmathLib.h"
#include "Pwm_rectifier.h"

void first_order_filter(FIRST_ORDER_FILTER *p)
{
 p->uo_k=_IQmpy(_IQmpy(p->wbT,p->ui_k)+p->uo_k_1,p->wbT_p_1__1);
 p->uo_k_1=p->uo_k;
}  

void notch_filter(NOTCH_FILTER *p)
{
 p->uo_k=p->ui_k-p->fed1_k_1;

 p->w0_Ts_Q=_IQmpy(p->Ts_Q,p->w0)<<2;
 p->w0_Ts__Q=_IQmpy(p->Ts__Q,p->w0)<<2;

 p->fed1_k=p->fed1_k_1+_IQmpy(p->w0_Ts__Q,p->uo_k-p->fed2_k_1);
 p->fed2_k=p->fed2_k_1+_IQmpy(p->fed1_k,p->w0_Ts_Q);

 p->fed1_k_1=p->fed1_k;
 p->fed2_k_1=p->fed2_k;

}
void notch_filter_init(NOTCH_FILTER *p)
{
 p->Ts__Q=_IQdiv(p->Ts,p->Q);
 p->Ts_Q=_IQmpy(p->Ts,p->Q);
}
