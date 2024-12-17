/*
 * File:   dashboard.c
 * Author: lokes
 *
 * Created on 15 November, 2024, 7:05 PM
 */


#include <xc.h>
#include "main.h"
#include "clcd1.h"
#include "blackbox.h"
#include "matrix_keypad.h"
#include "adc.h"
#include "ds1307.h"
#include "at240c2.h"

char *gear[9] = {"ON", "G1", "G2", "G3", "G4", "G5", "GR", "GN", "C "};
int gear_spec = 0;
static num1, num2;
unsigned char clock_reg[3];
unsigned char time[9];
int event_count = 0;
extern unsigned char key;

void get_time(void)
{
	clock_reg[0] = read_ds1307(HOUR_ADDR);
	clock_reg[1] = read_ds1307(MIN_ADDR);
	clock_reg[2] = read_ds1307(SEC_ADDR);

	if (clock_reg[0] & 0x40)
	{
		time[0] = '0' + ((clock_reg[0] >> 4) & 0x01);
		time[1] = '0' + (clock_reg[0] & 0x0F);
	}
	else
	{
		time[0] = '0' + ((clock_reg[0] >> 4) & 0x03);
		time[1] = '0' + (clock_reg[0] & 0x0F);
	}
	time[2] = ':';
	time[3] = '0' + ((clock_reg[1] >> 4) & 0x0F);
	time[4] = '0' + (clock_reg[1] & 0x0F);
	time[5] = ':';
	time[6] = '0' + ((clock_reg[2] >> 4) & 0x0F);
	time[7] = '0' + (clock_reg[2] & 0x0F);
	time[8] = '\0';
}

void speed(void) {

    unsigned long int adc_v = read_adc(CHANNEL4);
    unsigned int adc_vs = adc_v / 10.3;
    num1 = adc_vs % 10;
    num2 = adc_vs / 10;
    num1 = num1 + 48;
    num2 = num2 + 48;
    clcd_putch(num2, LINE2(13));
    clcd_putch(num1, LINE2(14));
}

void store_event(void) {
    unsigned int addr = 0x00;

    // If the event count exceeds 10, shift all events to make space
    if (event_count >= 10) {
        for (int i = 0; i < (10 - 1) * 16; i++) { // Shift the first 9 events
            unsigned char temp = read_at240c2(i + 16); // Read next event's byte
            write_at240c2(i, temp);                    // Write to current position
        }
        addr = (10 - 1) * 16; // Address for the 10th event (overwrite oldest)
    } else {
        // If less than 10 events, add to the next available slot
        addr = event_count * 16;
        event_count++;
    }

    // Write new event data to memory
    write_at240c2(addr++, time[0]);
    write_at240c2(addr++, time[1]);
    write_at240c2(addr++, time[2]);
    write_at240c2(addr++, time[3]);
    write_at240c2(addr++, time[4]);
    write_at240c2(addr++, time[5]);
    write_at240c2(addr++, time[6]);
    write_at240c2(addr++, time[7]);
    write_at240c2(addr++, ' ');  
    write_at240c2(addr++, gear[gear_spec][0]);
    write_at240c2(addr++, gear[gear_spec][1]);
    write_at240c2(addr++, ' '); 
    write_at240c2(addr++, num2);
    write_at240c2(addr++, num1);
}




void dashboard(void) {
    
    get_time();
    speed();
    if (key == MK_SW3) {
        gear_spec = 8;
        store_event();
    } else if (key == MK_SW1 && gear_spec < 7) {
        gear_spec++;
        store_event();
    } else if (key == MK_SW2 && gear_spec > 1 && gear_spec != 8) {
        gear_spec--;
        store_event();
    }
    if (gear_spec == 8 && (key == MK_SW2 || key == MK_SW1)) {
        gear_spec = 0;
    }

    clcd_print("EV", LINE1(10));
    clcd_print(gear[gear_spec], LINE2(10));
    clcd_print("SP", LINE1(13));
    clcd_print("TIME", LINE1(0));
    clcd_print(time, LINE2(0));

}
