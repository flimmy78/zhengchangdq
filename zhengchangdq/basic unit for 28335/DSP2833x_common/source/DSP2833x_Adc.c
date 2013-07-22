// TI File $Revision: /main/5 $
// Checkin $Date: October 23, 2007   13:34:09 $
//###########################################################################
//
// FILE:	DSP2833x_Adc.c
//
// TITLE:	DSP2833x ADC Initialization & Support Functions.
//
//###########################################################################
// $TI Release: DSP2833x/DSP2823x C/C++ Header Files V1.31 $
// $Release Date: August 4, 2009 $
//###########################################################################

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

#define ADC_usDELAY  5000L
#define ADC_CKPS   0x2   // ADC module clock = HSPCLK/2*ADC_CKPS   = 25.0MHz/(1*2) = 12.5MHz
#define ADC_SHCLK  0x5  // S/H width in ADC module periods                        = 16 ADC clocks 

//---------------------------------------------------------------------------
// InitAdc:
//---------------------------------------------------------------------------
// This function initializes ADC to a known state.
//
void InitAdc(void)
{
    extern void DSP28x_usDelay(Uint32 Count);


    // *IMPORTANT*
	// The ADC_cal function, which  copies the ADC calibration values from TI reserved
	// OTP into the ADCREFSEL and ADCOFFTRIM registers, occurs automatically in the
	// Boot ROM. If the boot ROM code is bypassed during the debug process, the
	// following function MUST be called for the ADC to function according
	// to specification. The clocks to the ADC MUST be enabled before calling this
	// function.
	// See the device data manual and/or the ADC Reference
	// Manual for more information.

	    EALLOW;
		SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1;
		ADC_cal();
		EDIS;




    // To powerup the ADC the ADCENCLK bit should be set first to enable
    // clocks, followed by powering up the bandgap, reference circuitry, and ADC core.
    // Before the first conversion is performed a 5ms delay must be observed
	// after power up to give all analog circuits time to power up and settle

    // Please note that for the delay function below to operate correctly the
	// CPU_RATE define statement in the DSP2833x_Examples.h file must
	// contain the correct CPU clock period in nanoseconds.

    AdcRegs.ADCTRL3.all = 0x00E0;  // Power up bandgap/reference/ADC circuits
    DELAY_US(ADC_usDELAY);         // Delay before converting ADC channels




   AdcRegs.ADCTRL1.bit.ACQ_PS = ADC_SHCLK;
   AdcRegs.ADCTRL3.bit.ADCCLKPS = ADC_CKPS;
   AdcRegs.ADCTRL1.bit.SEQ_CASC = 1;
   AdcRegs.ADCCHSELSEQ1.all=0x0000;
   AdcRegs.ADCCHSELSEQ2.all=0x0000;
   AdcRegs.ADCCHSELSEQ3.all=0x0000;
   AdcRegs.ADCCHSELSEQ4.all=0x0000;

   AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 5;    //udc
   AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 4;    //e_AB
   AdcRegs.ADCCHSELSEQ1.bit.CONV02 = 14;   //e_BC
  //dcRegs.ADCCHSELSEQ1.bit.CONV03 = 10;   //U_Cb
   AdcRegs.ADCCHSELSEQ1.bit.CONV03= 13;   //iL1_a
   AdcRegs.ADCCHSELSEQ2.bit.CONV04= 3;    //iL1_b
  //dcRegs.ADCCHSELSEQ2.bit.CONV06 = 12;   //iL2_a
  //dcRegs.ADCCHSELSEQ2.bit.CONV07 = 9;    //iL2_b

/*
   AdcRegs.ADCCHSELSEQ3.bit.CONV08 = 5;    //e_AB
   AdcRegs.ADCCHSELSEQ3.bit.CONV09 = 4;    //e_BC
   AdcRegs.ADCCHSELSEQ3.bit.CONV10 = 14;   //U_Ca
   AdcRegs.ADCCHSELSEQ3.bit.CONV11 = 10;   //U_Cb
   AdcRegs.ADCCHSELSEQ4.bit.CONV12 = 13;   //iL1_a
   AdcRegs.ADCCHSELSEQ4.bit.CONV13 = 3;    //iL1_b
   AdcRegs.ADCCHSELSEQ4.bit.CONV14 = 12;   //iL2_a
   AdcRegs.ADCCHSELSEQ4.bit.CONV15 = 9;    //iL2_b
*/


/*
   AdcRegs.ADCCHSELSEQ1.bit.CONV00=5;    //assigned to u_BA
   AdcRegs.ADCCHSELSEQ1.bit.CONV01=14;  //u_AC
   AdcRegs.ADCCHSELSEQ1.bit.CONV02=4;   //u_dc
   AdcRegs.ADCCHSELSEQ1.bit.CONV03=13;  //iL1_a
   AdcRegs.ADCCHSELSEQ2.bit.CONV04=3;   //iL1_b
   AdcRegs.ADCCHSELSEQ2.bit.CONV05=11;   //idc
   AdcRegs.ADCCHSELSEQ2.bit.CONV06=10;   // e_Cb
    //ADC two times for each channal
   AdcRegs.ADCCHSELSEQ2.bit.CONV07=5;  //u_BA
   AdcRegs.ADCCHSELSEQ3.bit.CONV08=14;  //u_AC
   AdcRegs.ADCCHSELSEQ3.bit.CONV09=4;    //u_dc
   AdcRegs.ADCCHSELSEQ3.bit.CONV10=13;  //iL1_a
   AdcRegs.ADCCHSELSEQ3.bit.CONV11=3;   //iL1_b
   AdcRegs.ADCCHSELSEQ4.bit.CONV12=11;  //idc
   AdcRegs.ADCCHSELSEQ4.bit.CONV13=10;  // 

   */
   AdcRegs.ADCTRL1.bit.CONT_RUN = 0;       // Setup start_stop run!!!
   AdcRegs.ADCMAXCONV.all=4;
   AdcRegs.ADCST.bit.INT_SEQ1_CLR=1;      //
}

//===========================================================================
// End of file.
//===========================================================================
