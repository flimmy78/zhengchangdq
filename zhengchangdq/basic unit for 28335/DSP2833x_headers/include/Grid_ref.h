#ifndef _GRID_REF_H
#define _GRID_REF_H


#define automode 0
#define manualmode 1
#define three_phase_dip 0
#define one_phase_dip 1
#define two_phase_dip 2
#define DQ2ABC_O_DEFAULTS {0,0,0,0,0,0,0}

typedef struct
{
 int  enable;
 int mode;
 int unbalance_dip;
 long dip_begin;//s
 long dip_end;//s
 long begin;//convert value
 long end;
 long dip_to;
 long original_a;
 long original_b;
 long original_c;
 }DIP;
 typedef struct
{
 int enable;
 int mode;
 long shift_value;
 long shift_begin;//s
 long shift_end;
 long begin; //convert value
 long end;
 long wTpwm;
 }F_SHIFT;
typedef struct
{
 int external_positive_ref_enable;
 int negative_seq_enable;
 int zero_seq_enable;
 int harmonic3_enable;
 int harmonic5_enable;
 int harmonic7_enable;
 int harmonic11_enable;
 int harmonicN_enable;
 long N;
}SELECTION;
typedef struct
{
 long phi;
 long amplitude;
 long value;
}ZERO_SEQ;
typedef struct
{
 long negative;
 long zero;
 long harmonic3;
 long harmonic5;
 long harmonic7;
 long harmonic11;
 long harmonicN;
 }PROPORTION;
 
typedef struct
{
 long ea;
 long eb;
 long ec;
}GRID_REF;

typedef struct
{    
     _iq d;
     _iq q;
	 _iq a;
	 _iq b;
	 _iq c;	
	 long phi;
	 long theta;
}DQ2ABC_O;   //常规的dq到abc的变换

extern  PROPORTION proportion={0,0,0,0,0,0,0};
extern  DIP dip={0,0,0,0,0,0,0,0,0,0,0};
extern  F_SHIFT f_shift={0,0,0,0,0,0,0,0};
extern  SELECTION selection={0,0,0,0,0,0,0,0,0};
extern  ZERO_SEQ zero={0,0,0};
extern  GRID_REF grid_ref={0,0,0};
extern  DQ2ABC_O positive= DQ2ABC_O_DEFAULTS;
extern  DQ2ABC_O negative= DQ2ABC_O_DEFAULTS;
extern  DQ2ABC_O harmonic3= DQ2ABC_O_DEFAULTS;
extern  DQ2ABC_O harmonic5= DQ2ABC_O_DEFAULTS;
extern  DQ2ABC_O harmonic7= DQ2ABC_O_DEFAULTS;
extern  DQ2ABC_O harmonic11= DQ2ABC_O_DEFAULTS;
extern  DQ2ABC_O harmonicN= DQ2ABC_O_DEFAULTS;
extern  long t=0;
extern  void grid_ref_calculate(void);
extern  void grid_ref_init(void);
extern  void dq2abc_o(DQ2ABC_O *);
#endif
