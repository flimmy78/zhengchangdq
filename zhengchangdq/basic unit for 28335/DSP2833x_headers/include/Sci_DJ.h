#ifndef SCI_DJ
#define SCI_DJ

#define RX_ENABLE()   GpioDataRegs.GPASET.bit.GPIO31=1
#define TX_ENABLE()   GpioDataRegs.GPACLEAR.bit.GPIO31=1

#define Synchronization 0x55

typedef struct
{
 long tx_data;
 int tx[4];
 int tx_counter;
 int counter;
}TX_LONG;
typedef struct
{
 long rx_data;
 int rx[4];
 int rx_counter;
}RX_LONG;


extern void send_long(TX_LONG *);
extern void receive_long(RX_LONG *);
#endif

//===========================================================================
// End of file.
///===========================================================================
