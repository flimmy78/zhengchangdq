#ifndef PROJECT_SERIAL_H
#define PROJECT_SERIAL_H


/*public functions*/
void InitialSerialGPIO(void);
void InitialSerialConfig(void);
void InitSerialPIEVectTable(void);
void InitSerialInterrupts(void);
//void EnableSerialSend(void);
//void EnableSerialReceive(void);
void SerialSend(Uint16 senddata1, Uint16 senddata2, Uint16 senddata3);
#pragma CODE_SECTION(SerialSend, "myfuncs");
void checkSendState(void);
#pragma CODE_SECTION(checkSendState, "myfuncs");
interrupt void SerialSendIsr(void);
#pragma CODE_SECTION(SerialSendIsr, "myfuncs");
interrupt void SerialReceiveIsr(void);

#define EnableSerialSend() GpioDataRegs.GPADAT.bit.GPIO31 = 0;

#define EnableSerialReceive() GpioDataRegs.GPADAT.bit.GPIO31 = 1;



/*private functions*/
void InitialSerialVaribles(void);
void processReceiveParams(void); 
#pragma CODE_SECTION(processReceiveParams, "myfuncs");


#endif  // end of PROJECT_SERIAL_H definition/*public varibles*/


