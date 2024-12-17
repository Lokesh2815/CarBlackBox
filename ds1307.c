/*
 * File:   ds1307.c
 * Author: lokes
 *
 * Created on 15 November, 2024, 12:56 PM
 */


#include <xc.h>

#include "i2c.h"
#include "ds1307.h"
#include <xc.h>

/* 
 * DS1307 Slave address
 * D0  -  Write Mode
 * D1  -  Read Mode
 */

void init_ds1307(void)
{
    unsigned char dummy;
    
    /* Setting the CH bit of the RTC to Stop the Clock */
	dummy = read_ds1307(SEC_ADDR);
	write_ds1307(SEC_ADDR, dummy | 0x80); 

	/* Setting 12 Hr Format */
	dummy = read_ds1307(HOUR_ADDR);
	write_ds1307(HOUR_ADDR, dummy | 0x00); 

    write_ds1307(CNTL_ADDR, 0x93); 

	/* Clearing the CH bit of the RTC to Start the Clock */
	dummy = read_ds1307(SEC_ADDR);
	write_ds1307(SEC_ADDR, dummy & 0x7F); 

    
}

void write_ds1307(unsigned char address, unsigned char data)
{
	i2c_start();//start the communication
	i2c_write(SLAVE_WRITE);//transmit the unique id
	i2c_write(address);//master send the address and data
	i2c_write(data);
	i2c_stop();//stop the communication
}

unsigned char read_ds1307(unsigned  char address)
{
    unsigned char data;
    //first we have to transmit the unique id
	i2c_start();
	i2c_write(SLAVE_WRITE);
	i2c_write(address);
    //Here we have to read for that we have to change the unique id and r/w bit
    //So we restart from start
	i2c_rep_start();
	i2c_write(SLAVE_READ);
    //read the data and store in a variable
	data = i2c_read();
	i2c_stop();
    //finally return the data
	return data;
}