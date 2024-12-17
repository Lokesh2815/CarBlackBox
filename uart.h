#ifndef SCI_H
#define SCI_H

#define RX_PIN					TRISC7
#define TX_PIN					TRISC6

void init_uart(void);
//Transmit 1 byte of data
void putch(unsigned char byte);
//To transmit the string
int puts(const char *s);
//To receive 1 byte data
unsigned char getch(void);
//Receive the 1 byte and transmit the same 1 byte data
unsigned char getche(void);

#endif
