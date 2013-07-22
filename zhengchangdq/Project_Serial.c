#include "DSP28x_Project.h" 
#include "Project_Serial.h"  //by ycz for serial communication


/*public varibles*/
Uint16 receiveparams[8];

/*private varibles*/
int receivelock = 0;
int sendlock = 0;
int receiveindex = 0;
int sendindex = 0;
int sendtemp[10];
int receivetemp[32];


void InitialSerialGPIO(void)
{
	EALLOW;

/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled disabled by the user.  
// This will enable the pullups for the specified pins.

	GpioCtrlRegs.GPAPUD.bit.GPIO28 = 0;    // Enable pull-up for GPIO28 (SCIRXDA)
	GpioCtrlRegs.GPAPUD.bit.GPIO29 = 0;	   // Enable pull-up for GPIO29 (SCITXDA)

/* Set qualification for selected pins to asynch only */
// Inputs are synchronized to SYSCLKOUT by default.  
// This will select asynch (no qualification) for the selected pins.

	GpioCtrlRegs.GPAQSEL2.bit.GPIO28 = 3;  // Asynch input GPIO28 (SCIRXDA)

/* Configure SCI-A pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be SCI functional pins.

	GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 1;   // Configure GPIO28 for SCIRXDA operation
	GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 1;   // Configure GPIO29 for SCITXDA operation

	GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 0;
	GpioCtrlRegs.GPADIR.bit.GPIO31 = 1;

	GpioCtrlRegs.GPBMUX2.bit.GPIO55 = 0;
	GpioCtrlRegs.GPBDIR.bit.GPIO55 = 0;

	
	
    EDIS;
	
	InitialSerialVaribles();

}

void InitialSerialConfig(void)
{
	/*diable the old one
	SciaRegs.SCICCR.all =0x0067;   // 1 stop bit,  No loopback
                                  // Even parity,8 char bits,
                                  // async mode, idle-line protocol
	SciaRegs.SCICTL1.all = 0x0003;  // enable TX, RX, internal SCICLK,
                                  // Disable RX ERR, SLEEP, TXWAKE
	SciaRegs.SCICTL2.all = 0x0003;

	//SciaRegs.SCIHBAUD = 0x0001;    //9600
  	//SciaRegs.SCILBAUD = 0x00E7;

	SciaRegs.SCIHBAUD = 0x0000;    //38400
  	SciaRegs.SCILBAUD = 0x0079;

	

	SciaRegs.SCIFFTX.all = 0xE060;  //1110000001100110
	//SciaRegs.SCIFFRX.all = 0xE065;   
	SciaRegs.SCIFFRX.all = 0xE070;//1110000001110000



	SciaRegs.SCICTL1.all =0x0023;     // Relinquish SCI from Reset
	ScibRegs.SCICTL1.bit.SWRESET = 1;
	*/

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

	SciaRegs.SCICCR.all =0x0067;   // 1 stop bit,  No loopback
                                  // No parity,8 char bits,
                                  // async mode, idle-line protocol
	SciaRegs.SCICTL1.all =0x0003;  // enable TX, RX, internal SCICLK,
                                  // Disable RX ERR, SLEEP, TXWAKE
	SciaRegs.SCICTL2.bit.TXINTENA =1;
	SciaRegs.SCICTL2.bit.RXBKINTENA =1;
	SciaRegs.SCIHBAUD = 0x0000;
	SciaRegs.SCILBAUD = 0x0079;   //38400
	//SciaRegs.SCICCR.bit.LOOPBKENA =1; //   Enable loop back
	SciaRegs.SCIFFTX.all=0xC026;  //1100000000100110
	SciaRegs.SCIFFRX.all=0x0030;  //0000000000110000
	SciaRegs.SCIFFCT.all=0x00;

	SciaRegs.SCICTL1.all =0x0023;     // Relinquish SCI from Reset
	SciaRegs.SCIFFTX.bit.TXFIFOXRESET=1;
	SciaRegs.SCIFFRX.bit.RXFIFORESET=1;
}


void InitSerialPIEVectTable(void)
{
	EALLOW;
	   PieVectTable.SCIRXINTA = &SerialReceiveIsr;
   PieVectTable.SCITXINTA = &SerialSendIsr;


	//PieVectTable.SCIRXINTA = &SerialReceiveIsr;
	//PieVectTable.SCITXINTA = &SerialSendIsr;
	EDIS;
}

void InitSerialInterrupts(void)
{
	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   // Enable the PIE block
PieCtrlRegs.PIEIER9.bit.INTx1=1;     // PIE Group 9, int1
   PieCtrlRegs.PIEIER9.bit.INTx2=1;     // PIE Group 9, INT2
   PieCtrlRegs.PIEIER9.bit.INTx3=1;     // PIE Group 9, INT3
   PieCtrlRegs.PIEIER9.bit.INTx4=1;     // PIE Group 9, INT4
	//PieCtrlRegs.PIEIER9.bit.INTx1=1;
	//PieCtrlRegs.PIEIER9.bit.INTx2=1;     // PIE Group 9, INT2
	IER |= 0x100;	// Enable CPU INT
	//PieCtrlRegs.PIEACK.bit.ACK9=1;
}

/*
use #define instead
void EnableSerialSend(void)
{

	GpioDataRegs.GPADAT.bit.GPIO31 = 0;
	//GpioDataRegs.GPADAT.bit.GPIO31 = GpioDataRegs.GPBDAT.bit.GPIO55;
}

void EnableSerialReceive(void)
{

	GpioDataRegs.GPADAT.bit.GPIO31 = 1;
	//GpioDataRegs.GPADAT.bit.GPIO31 = GpioDataRegs.GPBDAT.bit.GPIO55;

}
*/

void checkSendState(void)
{
	if (SciaRegs.SCICTL2.bit.TXEMPTY==1) {
		EnableSerialReceive();
		sendlock = 0;
		sendindex = 0;
	}
}





void SerialSend(Uint16 senddata1, Uint16 senddata2, Uint16 senddata3)
{
	int i=0;
	if (receivelock == 0) {
		
		for (i = 0; i < 20 ; i++) {
			receivetemp[i] = 0;
		}
	
		EnableSerialSend()
		sendlock = 1;
		sendindex = 0;
		sendtemp[0] = 0;
		sendtemp[1] = 255;    //for validation

		sendtemp[2] = senddata1 >> 8;
		sendtemp[3] = senddata1 & 0xff;
		sendtemp[4] = senddata2 >> 8;
		sendtemp[5] = senddata2 & 0xff;
		sendtemp[6] = senddata3 >> 8;
		sendtemp[7] = senddata3 & 0xff;

		sendtemp[8] = 255;
		sendtemp[9] = 0;

		SciaRegs.SCITXBUF=sendtemp[sendindex];
		sendindex++;





		
		while(sendindex < 10) {
			//while(SciaRegs.SCICTL2.bit.TXEMPTY==0)
		  	//{asm(" NOP");}
			SciaRegs.SCITXBUF=sendtemp[sendindex];
			sendindex++;

		}
		//while(SciaRegs.SCICTL2.bit.TXEMPTY==0)
		//{asm(" NOP");}
		//EnableSerialReceive()
		//sendlock = 0;
		//sendindex = 0;

		
	}

	
}

interrupt void SerialSendIsr(void)
{
/*
	while(sendindex < 10) {

		SciaRegs.SCITXBUF=sendtemp[sendindex];
		sendindex++;
	}
*/
	SciaRegs.SCIFFTX.bit.TXFFINTCLR=1;	// Clear SCI Interrupt flag
	PieCtrlRegs.PIEACK.all|=0x100;      // Issue PIE ACK

}



interrupt void SerialReceiveIsr(void)
{

	Uint16 i;

	receivelock = 1;
	for(i=receiveindex;i<receiveindex+16;i++)
	{
	    
	    receivetemp[i]=SciaRegs.SCIRXBUF.all;	 // Read data
	}
	receiveindex = i;
	if (receiveindex == 32) {
		processReceiveParams();
	}

	
	

	

	SciaRegs.SCIFFRX.bit.RXFFOVRCLR=1;  // Clear Overflow flag
	SciaRegs.SCIFFRX.bit.RXFFINTCLR=1; 	// Clear Interrupt flag
	PieCtrlRegs.PIEACK.all |= PIEACK_GROUP9;

	




}


void InitialSerialVaribles(void)
{
	int i;
	for (i = 0; i < 32 ; i++) {
		receivetemp[i] = 0;
	}

	for (i = 0; i < 8 ; i++) {
		receiveparams[i] = 0;
	}
	for (i = 0; i < 10 ; i++) {
		sendtemp[i] = 0;
	}

	receiveparams[0] = 80;//KP
	receiveparams[1] = 1000;//
	receiveparams[2] = 0;
	receiveparams[3] = 0;
	receiveparams[4] = 10;
	receiveparams[5] = 10;
	receiveparams[6] = 50;//IP
	receiveparams[7] = 50;
}
void processReceiveParams(void)
{
	if (receivetemp[0] == 0 && receivetemp[1] == 255 && receivetemp[18] == 255 && receivetemp[19] == 0) {

		receiveparams[0] = receivetemp[2] << 8;
		receiveparams[0]+= receivetemp[3];
		receiveparams[1] = receivetemp[4] << 8;
		receiveparams[1]+= receivetemp[5];
		receiveparams[2] = receivetemp[6] << 8;
		receiveparams[2]+= receivetemp[7];
		receiveparams[3] = receivetemp[8] << 8;
		receiveparams[3]+= receivetemp[9];
		receiveparams[4] = receivetemp[10] << 8;
		receiveparams[4]+= receivetemp[11];
		receiveparams[5] = receivetemp[12] << 8;
		receiveparams[5]+= receivetemp[13];
		
		receiveparams[6] = receivetemp[14] << 8;
		receiveparams[6]+= receivetemp[15];
		receiveparams[7] = receivetemp[16] << 8;
		receiveparams[7]+= receivetemp[17];
		

	}

	receivelock = 0;
	receiveindex = 0;
	


}
