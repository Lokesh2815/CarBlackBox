#include <xc.h>
#include "uart.h"

void init_uart(void)
{
	/* Serial initialization */
	RX_PIN = 1;
	TX_PIN = 0;

	/* TXSTA:- Transmitor Status and control Register */
	/* 9bit TX enable or disable bit */ 
	TX9 = 0;
	/* UART Tarsmition enable bit */
	TXEN = 1;
	/* Synchronous or Asynchronous mode selection */
	/* Asynchronous */
	SYNC = 0;
	
	/* Low or High baud rate selection bit */
	/* High Baud Rate */
	BRGH = 1;

	/* RCSTA :- Recepition Status and control Register */
	/* TX/RC7 and RX/RC6 act as serial port */ 
	SPEN = 1;
	/* 9bit RX enable or disable bit */
	RX9 = 0;
	/* Continous reception enable or disable */ 
	CREN = 1;

	/* BAUDCTL:- Baud rate control register */

	/* 16bit baud generate bit */ 
	BRG16 = 0;
	

	/* Baud Rate Setting Register */
	/* Set to 10 for 115200, 64 for 19200 and 129 for 9600 */
	SPBRG = 129;

    //ABDEN = 0;
	/* TX interrupt flag bit */
	TXIF = 0;

	/* RX interrupt enable bit */
	RCIF = 0;
}

void putch(unsigned char byte) 
{
	/* Output one byte */
	/* Set when register is empty */
	while(!TXIF)
	{
		continue;
	}
    //clear the transmit flag
	TXIF = 0;
    //load the data to the transmit register
	TXREG = byte;
} 

int puts(const char *s)
{
    //we cannot load the data into register at a time so we load byte by byte
	while(*s)		
	{
        //load the data 
		putch(*s++);	
	}
	return 0;
}

unsigned char getch(void)
{
	/* Retrieve one byte */
	/* Set when register is not empty */
    //check the receiver register is empty or full
    //RCIF=0-->empty RCIF = 1-->full
	while(!RCIF)
	{
		continue;
	}
    //we read the value from the buffer to the register
	RCIF = 0;
	return RCREG;
}

unsigned char getche(void)
{
	unsigned char c;
    //getch will receive the 1 byte of data and transmit same 1 byte data
	putch(c = getch());

	return (c);
}
