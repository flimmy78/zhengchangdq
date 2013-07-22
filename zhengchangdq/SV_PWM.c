////////////////////////////////////////////////////////////////////////////
// Created by DJ for his SVPWM
///////////////////////////////////////////////////////////////////////////


#include "DSP2833x_Device.h"
#include "IQmathLib.h"
#include "Pwm_rectifier.h"
#include "SV_PWM.h"





void SV_PWM_Init(SVPWM *p)
{
 p->sqrt_3=_IQsqrt(_IQ(3.0));
 p->sqrt_3_1=_IQdiv(_IQ(1.0),p->sqrt_3);
 p->Tpwm=EPWM1_TIMER_TBPRD<<1;
}


void SV_PWM_CAL(SVPWM *p)
{
 //find out the section number
 p->ab=p->a-p->b;
 p->bc=p->b-p->c;
 p->ca=p->c-p->a;

 if((p->ab>=0)&&(p->bc>0)&&(p->ca<0))
 {p->S=1;}
 else if((p->ab<0)&&(p->bc>0)&&(p->ca<=0))
 {p->S=2;}
 else if((p->ab<0)&&(p->bc>=0)&&(p->ca>0))
 {p->S=3;}
 else if((p->ab<=0)&&(p->bc<0)&&(p->ca>0))
 {p->S=4;}
 else if((p->ab>0)&&(p->bc<0)&&(p->ca>=0))
 {p->S=5;}
 else if((p->ab>0)&&(p->bc<=0)&&(p->ca<0))
 {p->S=6;}

 //abc_2_alfa&beta
 p->alfa=p->a;
 p->beta=_IQmpy(p->bc,p->sqrt_3_1);

 //cal X Y Z
 p->Z=_IQmpy(p->alfa,p->sqrt_3); //借Z做下中间变量
 p->X=(p->Z-p->beta)>>1;
 p->Y=(p->Z+p->beta)>>1;
 p->Z=p->beta;

 //lookup table for D1&D2
 switch(p->S)
 {
  case 1:
         p->D1=p->X;
		 p->D2=p->Z;
		 break;
  case 2:
         p->D1=-p->X;
		 p->D2=p->Y;
		 break;
  case 3:
         p->D1=p->Z;
		 p->D2=-p->Y;
		 break;
  case 4:
         p->D1=-p->Z;
		 p->D2=-p->X;
		 break;
  case 5:
         p->D1=-p->Y;
		 p->D2=p->X;
		 break;
  case 6:
         p->D1=p->Y;
		 p->D2=-p->Z;
		 break;

  default:     //err,return 0
         p->D1=0;
		 p->D2=0;
         break;
 }

 //cal T0/4 ,TO/4+T1/2,TO/4+T1/2+T2/2
 p->T1=_IQmpy(p->D1,p->Tpwm);
 p->T2=_IQmpy(p->D2,p->Tpwm);
 p->T0=p->Tpwm-p->T1-p->T2;
 if(p->T0<0)
 {p->T0=0;}

 p->T0_4=p->T0>>2;
 p->T0_4_p_T1_2=p->T0_4+(p->T1>>1);
 p->T0_4_p_T1_2_p_T2_2=p->T0_4_p_T1_2+(p->T2>>1);

}


void SV_PWM_Update(SVPWM *p)
{
  switch(p->S)
 {
  case 1:
         EPwm1Regs.CMPA.half.CMPA=p->T0_4;                 //phase a
		 EPwm2Regs.CMPA.half.CMPA=p->T0_4_p_T1_2;          //phase b
		 EPwm3Regs.CMPA.half.CMPA=p->T0_4_p_T1_2_p_T2_2;   //phase c
		 break;
  case 2:
         EPwm1Regs.CMPA.half.CMPA=p->T0_4_p_T1_2;
		 EPwm2Regs.CMPA.half.CMPA=p->T0_4;
		 EPwm3Regs.CMPA.half.CMPA=p->T0_4_p_T1_2_p_T2_2;
		 break;
  case 3:
         EPwm1Regs.CMPA.half.CMPA=p->T0_4_p_T1_2_p_T2_2;
		 EPwm2Regs.CMPA.half.CMPA=p->T0_4;
		 EPwm3Regs.CMPA.half.CMPA=p->T0_4_p_T1_2;
		 break;
  case 4:
         EPwm1Regs.CMPA.half.CMPA=p->T0_4_p_T1_2_p_T2_2;
		 EPwm2Regs.CMPA.half.CMPA=p->T0_4_p_T1_2;
		 EPwm3Regs.CMPA.half.CMPA=p->T0_4;
		 break;
  case 5:
         EPwm1Regs.CMPA.half.CMPA=p->T0_4_p_T1_2;
		 EPwm2Regs.CMPA.half.CMPA=p->T0_4_p_T1_2_p_T2_2;
		 EPwm3Regs.CMPA.half.CMPA=p->T0_4;
		 break;
  case 6:
         EPwm1Regs.CMPA.half.CMPA=p->T0_4;
		 EPwm2Regs.CMPA.half.CMPA=p->T0_4_p_T1_2_p_T2_2;
		 EPwm3Regs.CMPA.half.CMPA=p->T0_4_p_T1_2;
		 break;

  default:     //err,return 0
         EPwm1Regs.CMPA.half.CMPA=0;
		 EPwm2Regs.CMPA.half.CMPA=0;
		 EPwm3Regs.CMPA.half.CMPA=0;
         break;
 }
 EPwm3Regs.CMPA.half.CMPA=_IQsat(EPwm3Regs.CMPA.half.CMPA,EPWM1_MAX_CMPA,EPWM1_MIN_CMPA);
 EPwm2Regs.CMPA.half.CMPA=_IQsat(EPwm2Regs.CMPA.half.CMPA,EPWM1_MAX_CMPA,EPWM1_MIN_CMPA);
 EPwm1Regs.CMPA.half.CMPA=_IQsat(EPwm1Regs.CMPA.half.CMPA,EPWM1_MAX_CMPA,EPWM1_MIN_CMPA);
}



/*
void SV_PWM_Update(SVPWM *p)
{
  switch(p->S)
 {
  case 1:
         EPwm3Regs.CMPA.half.CMPA=p->T0_4;                 //phase a
		 EPwm2Regs.CMPA.half.CMPA=p->T0_4_p_T1_2;          //phase b
		 EPwm1Regs.CMPA.half.CMPA=p->T0_4_p_T1_2_p_T2_2;   //phase c
		 break;
  case 2:
         EPwm3Regs.CMPA.half.CMPA=p->T0_4_p_T1_2;
		 EPwm2Regs.CMPA.half.CMPA=p->T0_4;
		 EPwm1Regs.CMPA.half.CMPA=p->T0_4_p_T1_2_p_T2_2;
		 break;
  case 3:
         EPwm3Regs.CMPA.half.CMPA=p->T0_4_p_T1_2_p_T2_2;
		 EPwm2Regs.CMPA.half.CMPA=p->T0_4;
		 EPwm1Regs.CMPA.half.CMPA=p->T0_4_p_T1_2;
		 break;
  case 4:
         EPwm3Regs.CMPA.half.CMPA=p->T0_4_p_T1_2_p_T2_2;
		 EPwm2Regs.CMPA.half.CMPA=p->T0_4_p_T1_2;
		 EPwm1Regs.CMPA.half.CMPA=p->T0_4;
		 break;
  case 5:
         EPwm3Regs.CMPA.half.CMPA=p->T0_4_p_T1_2;
		 EPwm2Regs.CMPA.half.CMPA=p->T0_4_p_T1_2_p_T2_2;
		 EPwm1Regs.CMPA.half.CMPA=p->T0_4;
		 break;
  case 6:
         EPwm3Regs.CMPA.half.CMPA=p->T0_4;
		 EPwm2Regs.CMPA.half.CMPA=p->T0_4_p_T1_2_p_T2_2;
		 EPwm1Regs.CMPA.half.CMPA=p->T0_4_p_T1_2;
		 break;

  default:     //err,return 0
         EPwm3Regs.CMPA.half.CMPA=0;
		 EPwm2Regs.CMPA.half.CMPA=0;
		 EPwm1Regs.CMPA.half.CMPA=0;
         break;
 }
 EPwm3Regs.CMPA.half.CMPA=_IQsat(EPwm3Regs.CMPA.half.CMPA,EPWM1_MAX_CMPA,EPWM1_MIN_CMPA);
 EPwm2Regs.CMPA.half.CMPA=_IQsat(EPwm2Regs.CMPA.half.CMPA,EPWM1_MAX_CMPA,EPWM1_MIN_CMPA);
 EPwm1Regs.CMPA.half.CMPA=_IQsat(EPwm1Regs.CMPA.half.CMPA,EPWM1_MAX_CMPA,EPWM1_MIN_CMPA);
}


*/