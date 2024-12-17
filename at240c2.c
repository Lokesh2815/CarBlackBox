/*
 * File:   at240c2.c
 * Author: lokes
 *
 * Created on 16 November, 2024, 2:14 PM
 */



#include "main.h"
#include "at240c2.h"
#include "i2c.h"
#include <xc.h>

/* 
 * DS1307 Slave address
 * D0  -  Write Mode
 * D1  -  Read Mode
 */



void write_at240c2(unsigned char address, unsigned char data)
{
	i2c_start();
	i2c_write(SLAVE_WRITE1);
	i2c_write(address);
	i2c_write(data);
	i2c_stop();
    for(int wait = 3000;wait--;);
}

unsigned char read_at240c2(unsigned char address)
{
	unsigned char data;

	i2c_start();
	i2c_write(SLAVE_WRITE1);
	i2c_write(address);
	i2c_rep_start();
	i2c_write(SLAVE_READ1);
	data = i2c_read();
	i2c_stop();

	return data;
}