#ifndef AT240C2_H
#define AT240C2_H

#define SLAVE_READ1		0xA1
#define SLAVE_WRITE1		0xA0


#define SEC_ADDR		0x00
#define MIN_ADDR		0x01
#define HOUR_ADDR		0x02
#define DAY_ADDR		0x03
#define DATE_ADDR		0x04
#define MONTH_ADDR		0x05
#define YEAR_ADDR		0x06
#define CNTL_ADDR		0x07


void write_at240c2(unsigned char address,  unsigned char data);
unsigned char read_at240c2(unsigned char address);

#endif