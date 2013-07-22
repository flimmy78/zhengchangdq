#include "DSP2833x_Device.h"  //一定是这个头文件在前，因为编译时按顺序的，否则DSP2833x_Sci.h中出现的在Device.h中定义的Uint32等编译通不过
#include "DSP2833x_Sci.h"
#include "IQmathLib.h"
#include "Sci_DJ.h"


void send_long(TX_LONG *p)
{
 p->tx[0]=p->tx_data&0x000000FF;
 p->tx[1]=(p->tx_data&0x0000FF00)>>8;
 p->tx[2]=(p->tx_data>>16)&0x000000FF;
 p->tx[3]=((p->tx_data>>16)&0x0000FF00)>>8;
 for(p->tx_counter=0;p->tx_counter<4;p->tx_counter++)
 {
  while(SciaRegs.SCICTL2.bit.TXEMPTY==0)
  {asm(" NOP");}
  SciaRegs.SCITXBUF=p->tx[p->tx_counter];
 }
}

void receive_long(RX_LONG *p)
{
 for(p->rx_counter=0;p->rx_counter<4;p->rx_counter++)
 {
  while(SciaRegs.SCIRXST.bit.RXRDY==0)
  {asm(" NOP");}
  p->rx[p->rx_counter]=SciaRegs.SCIRXBUF.all;
  p->rx[p->rx_counter]=p->rx[p->rx_counter]&0xFF;
 }
 p->rx_data=p->rx[2]+(p->rx[3]<<8);
 p->rx_data=(p->rx_data)<<16;
 p->rx_data=p->rx_data+p->rx[0]+(p->rx[1])<<8;
}
