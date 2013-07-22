
//###########################################################################
// DJ used for VSR_SG10K 
//2010-07-02

//###########################################################################


#include "DSP28x_Project.h"    //bingwang实验
#include "IQmathLib.h"
#include "Pwm_rectifier.h"
#include "Grid_ref.h"
#include "PLL.h"
#include "SV_PWM.h"


long theta=0;
long PLL_theta_before=0;
long GlobalQ = GLOBAL_Q;   //这句是为了用IQMATH的gel

void update_compare(PWM *);
void LED_reset(void);
void LED_toggle(void);
void LED_all_on(void);
void LED_all_off(void);
void PWM_disable(void);
void PWM_enable(void);
void adc_calculate(void);
void PDP_process(void);
void scope(long ,long ,long, long ,long ,long);

DQ2ABC iL_loop=DQ2ABC_DEFAULTS;
ABC2DQ  IL=ABC2DQ_DEFAULTS;
DQ i_L={0,0};
IL_LOOP_OUT iL_loop_out={0,0,0};

PWM pwm_ref={0,0,0};
SVPWM svpwm=SVPWM_DEFAULTS;

PDP pdp;
PID pi_iLd=PI_DEFAULTS;
PID pi_iLq=PI_DEFAULTS;

PLLRegs pll=PLL_DEFAULTS;
THETA_REGS Theta_Regs=THETA_REGS_DEFAULTS;

int counter=0;
int counter1=0;
int counter2=0;
//int counter2=0;//test for protect

int d=0;
int b=0;
int c=0;
int PWM_disable_flag=0;
int flag=0;
int PDP_iL_flag=0;
int iL_start_flag=0;

/////////////////////////////
///modi by ycz
/////////////////////////////
long u_Udc=0;
long Udc=0;
long Udc_pu=0;
long e_AB=0; //标幺值
long e_BC=0;
long u_Ca = 0;
long u_Cb = 0;
long iL_b=0;
long iL_c=0;
long iL_a=0;
//add for double current loop
long iL2_a = 0;
long iL2_b = 0;
long iC_a = 0;
long iC_b = 0;
//add for double current loop
long e_a=0;
long e_b=0;
/////////////////////////////
///modi by ycz
/////////////////////////////
long mid_point_e_AB=0;
long mid_point_e_BC=0;
long mid_point_u_Ca = 0;
long mid_point_u_Cb = 0;
long mid_point_iL_a=0;
long mid_point_iL_b=0;
long mid_point_iL_c=0;
long mid_point_iL2_a=0;
long mid_point_iL2_b=0;
long mid_point_u_Udc = 0;

long C_e_AB;
long C_e_BC;
long C_u_Ca;
long C_u_Cb;
long C_iL_a;
long C_iL_b;
long C_iL_c;
long C_iL2_a;
long C_iL2_b;
long C_u_Udc;

/////////////////////////////
///modi by ycz
/////////////////////////////
long Im;
long Im_1;
long Um;
long Um_1;  //表示1/Um
long Um_1_128;//表示1/Um*128
long Um_3;  //表示Um/3
long Im_quarter;   //防止溢出
long Um_quarter;

long Threeperone;

long udc_2; //表示1/(udc/2)
long udc_sqrt_3;//表示1/(udc/sqrt(3))
long temp_feedback_a = 0;
long temp_feedback_b = 0;


#define PIX 3.1415926

void main(void)
{ 
   InitSysCtrl();
   EALLOW;
   SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;//Set TBCLKSYNC = 0. This will stop the time-base clock within any enabled ePWM module.
   EDIS;

   InitEPwm();  //对ePWM进行初始化
   PWM_disable(); //禁用pwm
   
   //设置pwm口gpio
   InitEPwm1Gpio();
   InitEPwm2Gpio();
   InitEPwm3Gpio();
   InitEPwm4Gpio();
   InitEPwm5Gpio();
   InitEPwm6Gpio();
   
   PWM_disable();
  
   /*EALLOW;
   GpioCtrlRegs.GPAPUD.bit.GPIO6 = 0;   // Enable pullup on GPIO6
   GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 0;  // GPIO6 = GPIO6
   GpioCtrlRegs.GPADIR.bit.GPIO6 = 1;   //GPIO6 = output
   EDIS; 
   GpioDataRegs.GPADAT.bit.GPIO6=0;*/
 
   pll.Init(&pll);//pll初始化
   pi_iLd.Kp=_IQ(55);
   pi_iLd.Ki=_IQ(1800*TPWM); 
   
   pi_iLq.Kp=pi_iLd.Kp;
   pi_iLq.Ki=pi_iLd.Ki;


   pdp.iL=_IQ(6);//bingwangbaohu
   pdp.Udc=_IQ(350);
  
   pi_iLd.upper_limit=_IQ(40);//40 for 400V//20 for 200V
   pi_iLd.lower_limit=-_IQ(40);//40
   pi_iLq.upper_limit=pi_iLd.upper_limit;
   pi_iLq.lower_limit=pi_iLd.lower_limit;

   svpwm.Init(&svpwm);
   
/////////////////////////////
///modi by ycz
/////////////////////////////
    C_e_AB=_IQ(0.00040974650349650349650349650349663*1024);//pu
    C_e_BC=_IQ(0.00040974650349650349650349650349663*1024);
    C_u_Ca=_IQ(0.00040974650349650349650349650349663*1024);
    C_u_Cb=_IQ(0.00040974650349650349650349650349663*1024);

    C_u_Udc=_IQ(0.09765625*1024);
   
    C_iL_a=_IQ(0.0032226562500000000000000000000002*1024);
    C_iL_b=_IQ(0.0032226562500000000000000000000002*1024);
	C_iL_c=_IQ(0.0032226562500000000000000000000002*1024);
    C_iL2_a=_IQ(0.0032226562500000000000000000000002*1024);
    C_iL2_b=_IQ(0.0032226562500000000000000000000002*1024);
   
/////////////////////////////
///modi by ycz
/////////////////////////////
   Im=_IQ(7.5757575757575757575757575757576);
   Im_1=_IQ(1.0/7.5757575757575757575757575757576);
   Um=_IQ(220.0);
   Um_1=_IQ(1.0/220.0);
   Um_1_128=_IQ(128.0/220.0);
   Um_3=_IQ(220.0/3.0);
   Threeperone = _IQ(1.0/3.0);
/////////////////////////////
///modi by ycz
/////////////////////////////
   mid_point_e_AB=2104;
   mid_point_e_BC=2194;
   mid_point_u_Ca= 2053;
   mid_point_u_Cb= 2108;
   mid_point_iL_a=2174;
   mid_point_iL_b=2182;
   mid_point_iL_c=2183;
   mid_point_iL2_a=2086;
   mid_point_iL2_b=2074;

   mid_point_u_Udc = 0;
   pi_iLd.ref= _IQ(0.5);//bingwanggeiding
   pi_iLq.ref= _IQ(4);

  
   MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
   MemCopy(&MyfuncsLoadStart, &MyfuncsLoadEnd, &MyfuncsRunStart);

   EALLOW; //LED没进中断前51亮50灭。
   GpioCtrlRegs.GPBMUX2.bit.GPIO50 = 0; 
   GpioCtrlRegs.GPBMUX2.bit.GPIO51 = 0; 
   GpioCtrlRegs.GPBDIR.bit.GPIO50=1;
   GpioCtrlRegs.GPBDIR.bit.GPIO51=1; 
   GpioDataRegs.GPBSET.bit.GPIO50=1;
   GpioDataRegs.GPBCLEAR.bit.GPIO51=1;
   EDIS;  
   
   //TZ2  
   EALLOW;
   GpioCtrlRegs.GPAMUX1.bit.GPIO13=1;
   GpioCtrlRegs.GPAPUD.bit.GPIO13=0;
   EDIS;
  
   EALLOW;//epwm4b,5b,6b
   GpioCtrlRegs.GPADIR.bit.GPIO7=1;
   GpioCtrlRegs.GPADIR.bit.GPIO9=1;
   GpioCtrlRegs.GPADIR.bit.GPIO11=1;
   GpioDataRegs.GPASET.bit.GPIO7=1;
   GpioDataRegs.GPASET.bit.GPIO9=1;
   GpioDataRegs.GPASET.bit.GPIO11=1;
   EDIS;
   
 /////////////////////////////////////////////////////////////////////////////////////
   DINT;
   InitPieCtrl();
   
   IER = 0x0000;
   IFR = 0x0000;
   InitPieVectTable();


   EALLOW; 
   PieVectTable.EPWM3_INT  = &main_isr;   
   EDIS;   
  
   InitAdc(); 
    while(1) {
		if (GpioDataRegs.GPBDAT.bit.GPIO54==0) {

		} else {
			for(d=20;d>0;d--){}
			if (GpioDataRegs.GPBDAT.bit.GPIO54==1) {
				break;
			}
		}
	}
	

   EALLOW;
   SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
   EDIS;
   
   IER |= M_INT3;

   PieCtrlRegs.PIEIER3.bit.INTx3 = 1;
   PieCtrlRegs.PIEACK.bit.ACK3=1;
   
   EPwm3Regs.ETCLR.bit.INT=1;
   EINT; 
   ERTM;  
  
   for(;;)
   {

       asm(" NOP");
	 
   }

} 


interrupt void main_isr(void)
{
 
// GpioDataRegs.GPASET.bit.GPIO6 = 1;
 if(GpioDataRegs.GPBDAT.bit.GPIO54==0)
 {
  for(d=20;d>0;d--){}//防止开关抖动
  if(GpioDataRegs.GPBDAT.bit.GPIO54==0)
  {
   PWM_disable();
  }
 }
 
     
  adc_calculate();
    
 
  PDP_process();
  
  e_a = _IQmpy((e_AB<<1) + e_BC, Threeperone);//line to phase
  e_b = _IQmpy((e_BC - e_AB), Threeperone);

  pll.pllabc2dq.a = e_a;
  pll.pllabc2dq.b = e_b;
  pll.pllabc2dq.c = -e_a-e_b;

// GpioDataRegs.GPASET.bit.GPIO6 = 1;//测量锁相环时间
  PLL(&pll,&Theta_Regs);
// GpioDataRegs.GPACLEAR.bit.GPIO6 = 1;

  Theta_Regs.theta=pll.theta; 


 
  if(pll.theta < PLL_theta_before && iL_start_flag==0 && counter1>=10000 )
  {
     iL_start_flag=1;   //电网相角从2*pi变为0时电流环开始工作
	 
   } else {
   		PLL_theta_before=pll.theta; //存上一次锁相环所得相角
   }

  cos_sin_cal(&Theta_Regs);

  IL.a=iL_a;//桥臂侧电流解耦
  IL.b=iL_b;
 
  IL.c=-iL_a-iL_b;
  abc2dq(&IL,&Theta_Regs);
  i_L.d=IL.d;
  i_L.q=IL.q;


 // IL2.a=iL2_a;//wang侧电流解耦
 // IL2.b=iL2_b;
//  IL2.c=-iL2_a-iL2_b;
//  abc2dq(&IL2,&Theta_Regs);
 // i_L2.d=IL2.d;
 // i_L2.q=IL2.q;
  
  if( iL_start_flag==1 )//锁相环等计算完成后再发波,至少1s钟以后且锁相角首次为0时启动发波。
  {
	if (flag == 0) 
	{
	PWM_enable();
	PWM_disable_flag=0;
	flag = 1;
	GpioDataRegs.GPBSET.bit.GPIO50=1;//保证进入中断并发波时两灯交替亮灭
    GpioDataRegs.GPBCLEAR.bit.GPIO51=1;
    }
  
  } else {
	counter1++;
  }

  if (flag == 1) {

   /*if(b==20000 && c<30000){  //2s后给定由1变为3
    pi_iLd.ref= _IQ(3);
   }else{
        b++;
        }
      if(c==30000){  //3s后给定由3变为1
      pi_iLd.ref= _IQ(3.1);
       }else{
         c++;
            }*/

      pi_iLd.fdb=i_L.d;
	  pi_iLq.fdb=i_L.q;

	  pi(&pi_iLd);
	  pi(&pi_iLq);

	  iL_loop.d=pi_iLd.uo;
	  iL_loop.q=pi_iLq.uo;
	
	  dq2abc(&iL_loop,&Theta_Regs);
	 
      iL_loop_out.a=iL_loop.a; 
	  iL_loop_out.b=iL_loop.b;

     pwm_ref.a=_IQmpy(e_a,Um) + iL_loop_out.a;//并网实验时的调制波
	 pwm_ref.b=_IQmpy(e_b,Um) + iL_loop_out.b;
	 pwm_ref.c = -pwm_ref.a - pwm_ref.b;
  //    pwm_ref.a= iL_loop_out.a;//并网实验时的调制波
    //  pwm_ref.b= iL_loop_out.b;
	//  pwm_ref.c = -pwm_ref.a - pwm_ref.b;


    //   pwm_ref.a=_IQmpy(e_a,Um) ;//qiankui
	 //  pwm_ref.b=_IQmpy(e_b,Um);
	  // pwm_ref.c = -pwm_ref.a - pwm_ref.b;


	
	 //pwm_ref.a=iL_loop_out.a;//短路实验时的调制波
	 // pwm_ref.b=iL_loop_out.b;
	 //pwm_ref.c=-pwm_ref.a-pwm_ref.b;
	 
	 } else {
   	  pwm_ref.a = 0;
	  pwm_ref.b = 0;
	  pwm_ref.c = 0;

  }
  
   // Udc = _IQ(260);//并网实验


  Udc=u_Udc;
  udc_sqrt_3=_IQdiv(svpwm.sqrt_3,Udc);
  svpwm.a=_IQsat(_IQmpy(pwm_ref.a,udc_sqrt_3),_IQ(1),_IQ(-1));
  svpwm.b=_IQsat(_IQmpy(pwm_ref.b,udc_sqrt_3),_IQ(1),_IQ(-1));
  svpwm.c=_IQsat(_IQmpy(pwm_ref.c,udc_sqrt_3),_IQ(1),_IQ(-1));
  svpwm.Cal(&svpwm);
  svpwm.Update(&svpwm);

  scope(pi_iLd.ref>>2,DC,pi_iLd.fdb>>2,DC,e_a,AC);
//  scope(iL_a,AC,iL_b,AC,e_a,AC);
// scope(pll.theta/(2*PIX),DC,e_a,AC,pll.w>>9,DC);//e_a
// scope(pi_iL_Pd.ref>>1,DC,pi_iL_Pd.fdb>>1,DC,a,DC);

  
 if(EPwm2Regs.TZFLG.bit.OST==1)
 {
  PWM_disable_flag=1;
 }

 if(PWM_disable_flag==0)
 { 
 LED_toggle();
 }

 if(PWM_disable_flag==1&&PDP_iL_flag==0)
 {
  LED_all_on();
 }
 
 if(PDP_iL_flag==1)
 { 
 LED_all_off();
 }
 
  //GpioDataRegs.GPACLEAR.bit.GPIO6=1;

 PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
 EPwm3Regs.ETCLR.bit.INT = 1;
 //GpioDataRegs.GPACLEAR.bit.GPIO6=1;


}
/////////////////////////////////////////////////////////////////////////////////////////

void LED_reset(void)
{
  GpioDataRegs.GPBCLEAR.bit.GPIO50=1;
  GpioDataRegs.GPBSET.bit.GPIO51=1;
}

void LED_toggle(void)
{
   if(counter==2000)
    { GpioDataRegs.GPBTOGGLE.bit.GPIO50=1;
      GpioDataRegs.GPBTOGGLE.bit.GPIO51=1;
      counter=0;	
    }
    counter++;
}
void LED_all_on(void)
{
  GpioDataRegs.GPBCLEAR.bit.GPIO50=1;
  GpioDataRegs.GPBCLEAR.bit.GPIO51=1;
}
void LED_all_off(void)
{
  GpioDataRegs.GPBSET.bit.GPIO50=1;
  GpioDataRegs.GPBSET.bit.GPIO51=1;
}
void PWM_disable(void)
{
 EALLOW;
 EPwm1Regs.TZFRC.bit.OST=1;
 EPwm2Regs.TZFRC.bit.OST=1;
 EPwm3Regs.TZFRC.bit.OST=1;
 EDIS;
 
}

void PWM_enable(void)
{
 EALLOW;
 EPwm1Regs.TZCLR.bit.OST=1;
 EPwm2Regs.TZCLR.bit.OST=1;
 EPwm3Regs.TZCLR.bit.OST=1;
 EDIS;
 
}

void adc_calculate(void)
{
	
 AdcRegs.ADCTRL2.bit.SOC_SEQ1=1;//级联时只用这一个即可
 while (AdcRegs.ADCST.bit.INT_SEQ1== 0)
 {asm(" NOP");}
 asm(" RPT #11 || NOP");
 AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;
 AdcRegs.ADCTRL2.bit.RST_SEQ1=1;


 u_Udc=AdcRegs.ADCRESULT0;
 //u_Udc=u_Udc+AdcRegs.ADCRESULT8;
 u_Udc=((u_Udc>>4)-mid_point_u_Udc)<<12;  //2215
 u_Udc=_IQmpy(u_Udc,C_u_Udc); //PU
 u_Udc=u_Udc-_IQ(7.5);


 e_AB=AdcRegs.ADCRESULT1;
 //e_AB=e_AB+AdcRegs.ADCRESULT9;
 e_AB=((e_AB>>4)-mid_point_e_AB)<<12;  //2215
 e_AB=_IQmpy(e_AB,C_e_AB);//PU
 
 e_BC=AdcRegs.ADCRESULT2;
// e_BC=e_BC+AdcRegs.ADCRESULT10;
 e_BC=((e_BC>>4)-mid_point_e_BC)<<12; 
 e_BC=_IQmpy(e_BC,C_e_BC);//PU

// u_Ca=AdcRegs.ADCRESULT2;
 //u_Ca=u_Ca+AdcRegs.ADCRESULT10;
// u_Ca=((u_Ca>>4)-mid_point_u_Ca)<<12;  //2215
 //u_Ca=_IQmpy(u_Ca,C_u_Ca); //PU

/* u_Cb=AdcRegs.ADCRESULT3;
//u_Cb=u_Cb+AdcRegs.ADCRESULT11;
 u_Cb=((u_Cb>>5)-mid_point_u_Cb)<<12;  //2215
 u_Cb=_IQmpy(u_Cb,C_u_Cb); //PU 
*/


 iL_a=AdcRegs.ADCRESULT3;
 //iL_b=iL_b+AdcRegs.ADCRESULT12;
 iL_a=((iL_a>>4)-mid_point_iL_a)<<12; 
 iL_a=-_IQmpy(iL_a,C_iL_a);//PU

 iL_b=AdcRegs.ADCRESULT4;
 //iL_a=iL_a+AdcRegs.ADCRESULT13;
 iL_b=((iL_b>>4)-mid_point_iL_b)<<12; 
 iL_b=-_IQmpy(iL_b,C_iL_b);//PU
 iL_c=-iL_b-iL_a;
// iL2_a=AdcRegs.ADCRESULT6;
 //iL2_a=iL2_a+AdcRegs.ADCRESULT14;
// iL2_a=((iL2_a>>4)-mid_point_iL2_a)<<12; 
// iL2_a=_IQmpy(iL2_a,C_iL2_a);//PU

// iL2_b=AdcRegs.ADCRESULT7;
 //iL2_b=iL2_b+AdcRegs.ADCRESULT15;
// iL2_b=((iL2_b>>4)-mid_point_iL2_b)<<12; 
// iL2_b=_IQmpy(iL2_b,C_iL2_b);//PU

}


void PDP_process(void)
{
/////////////////////////////
///modi by ycz
/////////////////////////////

 if((iL_a>pdp.iL)||(iL_a<-pdp.iL)||(iL_b>pdp.iL)||(iL_b<-pdp.iL)||(iL_a+iL_b>pdp.iL)||(iL_a+iL_b<-pdp.iL)||(u_Udc>pdp.Udc))
 {
  PWM_disable();
  PDP_iL_flag=1;
  PWM_disable_flag=1;
 }

}


/*void delay_ON(DELAY_ON *p)
{
 if(p->counter<p->delay_time)
 {
  PWM_disable();
  p->counter++;
  
 }
 else
 {
  PWM_enable();
  pi_iL_Pd.ui=0;
  pi_iL_Pq.ui=0;
  PDP_iL_flag=0;
  PWM_disable_flag=0;
  LED_reset();
 }
}*/

//===========================================================================
// No more.
//===========================================================================
