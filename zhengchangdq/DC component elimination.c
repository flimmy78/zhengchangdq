////////////////////////////////////////////////////////////////////////////////
//   Created by DJ.
//   2010-07-02
//////////////////////////////////////////////////////////////////////////////////


#include "DSP2833x_Device.h"
#include "IQmathLib.h"
#include "Pwm_rectifier.h"


void DC_component_elimination(DC_ELIMINATION *p)
{
 p->uo_add=p->uo_add+p->ui;
 p->counter++;
 if(p->counter==200)  //10k
 {
  p->counter=0;
  p->uo_offset=_IQmpy(p->uo_add,p->K);
  p->uo_add=0;
  
  p->pi.ref=0;
  p->pi.fdb=p->uo_offset;
  p->PI_cal(&(p->pi));
  
 }
 
}
void DC_component_elimination_Init(DC_ELIMINATION *p)
{
 p->counter=0;
 p->K=_IQ(0.005); //1/200
 p->pi.Kp=_IQ(0.05); //0.05
 p->pi.Ki=_IQ(0.8); //0.8 //Ts=0.02
 p->pi.upper_limit=_IQ(50);
 p->pi.lower_limit=-_IQ(50);
 p->uo_add=0;
}

void DC_component_detection (DC_DETECTION *p)
{
 p->uo_add=p->uo_add+p->ui;
 p->counter++;
 if(p->counter==200)
 {
  p->counter=0;
  p->uo_offset=_IQmpy(p->uo_add,p->K);
  p->uo_add=0;
 }
}
