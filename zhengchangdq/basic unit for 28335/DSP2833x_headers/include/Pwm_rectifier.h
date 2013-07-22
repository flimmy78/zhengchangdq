#ifndef PWM_RECTIFIER
#define PWM_RECTIFIER


#define ENABLE 1
#define DISABLE 0

#define EPWM1_TIMER_TBPRD  7500  // Period register,for:SYSCLOK=50MHz,fPWM=10KHz
#define EPWM1_MAX_CMPA     7450
#define EPWM1_MIN_CMPA       50//最下脉宽限制
#define EPWM1_MAX_CMPB     7450
#define EPWM1_MIN_CMPB       50
#define PI 3.14159262180328369140625
#define PI2 6.2831852436065673828125
#define WTPWM _IQ(0.0314159262180328369140625)
#define TPWM 0.0001



#define DC 0
#define AC 1
#define PHASE 0;
#define LINE 1;

typedef struct
{
 long ref;
 long fdb;
 long uo;
 long w;
 long a;
 long Kp;
 long Ki;
 long err;
 long up_k;
 long ur_k;
 long ur_k_1;
 long e_k;
 long ef_k_1;
 long ef_k_2;
 long Ts;
 long KiTs;
 long awTs;
 long w2Ts;
 long upper_limit;
 long lower_limit;
}PR;

typedef struct
{    
     _iq d;
     _iq q;
	 _iq a;
	 _iq b;
	 _iq c;	
}DQ2ABC ;

typedef struct
{    
     long d;
     long q;
	 long a;
	 long b;
	 long c;
 }ABC2DQ;

 typedef struct
 {
  long ab;
  long bc;
  long d;
  long q;
 }L_ABC2DQ;

 typedef struct
 {
  long theta;
  long cos_theta;
  long sin_theta;
  long cos_2theta;
  long sin_2theta;
  long cos_theta_p_120;
  long cos_theta_m_120;
  long sin_theta_p_120;
  long sin_theta_m_120;
  long cos_120;
  long sin_120;
 }THETA_REGS;

 typedef struct
{
 long ui_k;
 long uo_k_1;   //ui(k-1)
 long uo_k;
 long wb;
 long wbT; //wb*T
 long wbT_p_1__1;
}FIRST_ORDER_FILTER;


 typedef struct
{
 long Kp; 
 long Ki;
 long ref;
 long fdb;
 long err;
 long ui;
 long up;
 long uo;
 long upper_limit;
 long lower_limit;
}PID;  //reason for using PID not PI is PI has already been used in “#define PI 3.14159262180328369140625”


typedef struct
{
 long uo;
 long err;
 long Kp;
 long upper_limit;
 long lower_limit;
}P;

typedef struct
{
 long a;
 long b;
 long c;
}PWM;

typedef struct
{
 long counter;
 long delay_on_time;
 int enable;
 long id_N_ref;
 long iq_N_ref;
 long ref_upper_limit;
 long ref_lower_limit;
 long d_d_p_q_q;  //ud2+uq2
 long d_d_p_q_q_limit;
 long d_d_p_q_q__1; //1/(ud2+uq2)
 void (*Init)();
 void (*cal)();
}N_SEQ_REF;

typedef struct
{
 long w0;
 long Q;
 long Ts;
 long uo_k;
 long ui_k;
 long fed1_k_1;
 long fed1_k;
 long fed2_k;
 long fed2_k_1;
 long Ts_Q;  //Ts*Q
 long Ts__Q; //Ts/Q
 long w0_Ts_Q;  //w0*Ts*Q
 long w0_Ts__Q; //w0*Ts/Q
 void (*Init)();
 void (*cal)();
}NOTCH_FILTER;

typedef struct
{
 int counter;
 long ui;
 long uo_add;
 long K;
 long uo_offset;
 PID pi;
 void (*PI_cal)(); 
 void (*cal)();
 void (*Init)();
}DC_ELIMINATION;

typedef struct
{
 long start_value;
 long end_value;
 long delta_value;
 long out;
 long start2end_time;   
 long counter;
 int first_on_flag;
 int start_flag;
 void (*cal)();
}RAMP_START;

typedef struct
{
 int counter;
 int action_value;
 long Ts;
 long ref;
 long out;
 long upper_limit;
 long lower_limit;
 FIRST_ORDER_FILTER filter;
 NOTCH_FILTER Notch_filter;
 void (*filter_cal)();
}UDC_LOOP;

typedef struct
{
 long Udc;
 long iL;
 long idc;
}PDP;

typedef struct
{
 long original;
 long step_value;
 long ref;
}STEP;

typedef struct
{
 int counter;
 long ui;
 long uo_add;
 long K;
 long uo_offset;
}DC_DETECTION;

typedef struct
{
 long delay_time;
 long soft_start_time;
 long counter;
 int start_flag;
}DELAY_ON;

typedef struct
{
 int Kp_external_enable;
 int Ki_external_enable;
 int K_external_enable;
}DEBUG;

typedef struct
{
 long a;
 long b;
 long c;
}IL_LOOP_OUT;
////123////
typedef struct
{
 long d;
 long q;
}DQ;

////123////
#define PI_DEFAULTS {0,0,0,0,0,0,0,0,2147483647,-2147483647}
#define PR_DEFAULTS {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2147483647,-2147483647}
#define FIRST_ORDER_FILTER_DEFAULTS {0,0,0,0,0,0}
#define P_DEFAULTS {0,0,0,2147483647,-2147483647}
#define DQ2ABC_DEFAULTS {0,0,0,0,0 }
#define ABC2DQ_DEFAULTS {0,0,0,0,0}
#define L_ABC2DQ_DEFAULTS{0,0,0,0}
#define THETA_REGS_DEFAULTS{0,1,0,1,0,_IQ(-0.5),_IQ(-0.5),_IQ(0.86602540378443864676372317075294),-_IQ(0.86602540378443864676372317075294),_IQ(-0.5),_IQ(0.86602540378443864676372317075294)}
#define UDC_LOOP_DEFAULTS {0,0,0,0,0,0,0,FIRST_ORDER_FILTER_DEFAULTS,NOTCH_FILTER_DEFAULTS,(void (*)(Uint32))first_order_filter}
#define DC_ELIMINATION_DEFAULTS {0,0,0,0,0,PI_DEFAULTS,(void (*)(Uint32))pi,(void (*)(Uint32))DC_component_elimination,(void (*)(Uint32))DC_component_elimination_Init}
#define RAMP_START_DEFAULTS{0,0,0,0,0,0,0,0,(void (*)(Uint32))ramp_start}
#define NOTCH_FILTER_DEFAULTS {0,0,0,0,0,0,0,0,0,0,0,0,0,(void (*)(Uint32))notch_filter_init,(void (*)(Uint32))notch_filter}
#define STEP_DEFAULTS {0,0,0}
#define N_SEQ_REF_DEFAULTS{0,0,0,0,0,0,0,0,0,0,(void (*)(Uint32))negative_seq_ref_init,(void (*)(Uint32))negative_seq_ref_cal}


extern  interrupt void main_isr(void);


extern void dq2abc(DQ2ABC *,THETA_REGS *);
extern void dq2abc_N(DQ2ABC *,THETA_REGS *);
extern void abc2dq(ABC2DQ *,THETA_REGS *);
extern void abc2dq_N(ABC2DQ *,THETA_REGS *);
extern void L_abc2dq(L_ABC2DQ *,THETA_REGS *);
extern void L_abc2dq_N(L_ABC2DQ *,THETA_REGS *);
extern void pi(PID *);
extern void pr(PR *);
extern void notch_filter(NOTCH_FILTER *);
extern void first_order_filter(FIRST_ORDER_FILTER *);
extern void cos_sin_cal(THETA_REGS *);

#pragma CODE_SECTION(dq2abc, "ramfuncs");
#pragma CODE_SECTION(dq2abc_N, "ramfuncs");
#pragma CODE_SECTION(abc2dq, "ramfuncs");
#pragma CODE_SECTION(abc2dq_N, "ramfuncs");
#pragma CODE_SECTION(L_abc2dq, "ramfuncs");
#pragma CODE_SECTION(L_abc2dq_N, "ramfuncs");
#pragma CODE_SECTION(main_isr, "ramfuncs");
#pragma CODE_SECTION(pi, "ramfuncs");
//#pragma CODE_SECTION(pr, "ramfuncs");
#pragma CODE_SECTION(notch_filter, "ramfuncs");
#pragma CODE_SECTION(first_order_filter, "ramfuncs");
#pragma CODE_SECTION(cos_sin_cal, "myfuncs");



#endif
