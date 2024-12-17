/*
 * File:   set_time.c
 * Author: lokes
 *
 * Created on 19 November, 2024, 6:51 PM
 */

#include <xc.h>
#include "clcd1.h"
#include "main.h"
#include "i2c.h"
#include "ds1307.h"
#include "blackbox.h"
#include "matrix_keypad.h"

static int change = 0; // Tracks which field (hours, minutes, seconds) is being edited
extern int once = 0; // Ensures initialization happens only once
extern unsigned char time[9];
unsigned char clock_reg[3]; // Stores time data in BCD format
extern unsigned char key;
extern int enter;
extern int screen_flag;

void set_time() {
    clcd_print("HH:MM:SS", LINE1(0));
    clcd_print(time, LINE2(0));

    // Initialize time display from DS1307 once
    if (once == 0) {
        clock_reg[0] = read_ds1307(HOUR_ADDR);
        clock_reg[1] = read_ds1307(MIN_ADDR);
        clock_reg[2] = read_ds1307(SEC_ADDR);

        time[0] = '0' + ((clock_reg[0] >> 4) & 0x03); // Extract hours (tens)
        time[1] = '0' + (clock_reg[0] & 0x0F); // Extract hours (units)
        time[2] = ':';
        time[3] = '0' + ((clock_reg[1] >> 4) & 0x0F); // Extract minutes (tens)
        time[4] = '0' + (clock_reg[1] & 0x0F); // Extract minutes (units)
        time[5] = ':';
        time[6] = '0' + ((clock_reg[2] >> 4) & 0x0F); // Extract seconds (tens)
        time[7] = '0' + (clock_reg[2] & 0x0F); // Extract seconds (units)
        time[8] = '\0'; // Null-terminate string
        once = 1; // Mark initialization as complete
    }

    // Handle key presses for editing fields
    if (key == MK_SW2) // Move to the next field (hours -> minutes -> seconds)
    {
        change++;
        if (change > 2) {
            change = 0; // Wrap around to hours after seconds
        }
        key = 0; // Clear the key
    }

    if (key == MK_SW1) // Increment the selected field
    {
        if (change == 0) // Increment hours
        {
            time[1]++;
            if (time[1] > '9') {
                time[1] = '0';
                time[0]++;
            }
            if (time[0] == '2' && time[1] == '4') // Reset to 00 after 23
            {
                time[0] = '0';
                time[1] = '0';
            }
        } else if (change == 1) // Increment minutes
        {
            time[4]++;
            if (time[4] > '9') {
                time[4] = '0';
                time[3]++;
            }
            if (time[3] == '6') // Reset to 00 after 59
            {
                time[3] = '0';
            }
        } else if (change == 2) // Increment seconds
        {
            time[7]++;
            if (time[7] > '9') {
                time[7] = '0';
                time[6]++;
            }
            if (time[6] == '6') // Reset to 00 after 59
            {
                time[6] = '0';
            }
        }
        key = 0; // Clear the key
    }

    // Blinking logic for selected field
    static int toggle = 0;
    toggle++;
    if (toggle < 250) {
        if (change == 0) // Blink hours
        {
            clcd_putch(time[0], LINE2(0));
            clcd_putch(time[1], LINE2(1));
        } else if (change == 1) // Blink minutes
        {
            clcd_putch(time[3], LINE2(3));
            clcd_putch(time[4], LINE2(4));
        } else if (change == 2) // Blink seconds
        {
            clcd_putch(time[6], LINE2(6));
            clcd_putch(time[7], LINE2(7));
        }
    } else if (toggle < 500) {
        if (change == 0) {
            clcd_print("  ", LINE2(0)); // Blank hours
        } else if (change == 1) {
            clcd_print("  ", LINE2(3)); // Blank minutes
        } else if (change == 2) {
            clcd_print("  ", LINE2(6)); // Blank seconds
        }
    } else {
        toggle = 0; // Reset toggle
    }

    // Save the updated time and exit
    if (key == MK_SW5) // Save and exit
    {
        CLEAR_DISP_SCREEN;
        clcd_print("Time Updated",LINE1(0));
        __delay_ms(3000);
        // Convert time to BCD and write to DS1307
        clock_reg[0] = ((time[0] - '0') << 4) | (time[1] - '0'); // Hours
        clock_reg[1] = ((time[3] - '0') << 4) | (time[4] - '0'); // Minutes
        clock_reg[2] = ((time[6] - '0') << 4) | (time[7] - '0'); // Seconds

        write_ds1307(HOUR_ADDR, clock_reg[0]);
        write_ds1307(MIN_ADDR, clock_reg[1]);
        write_ds1307(SEC_ADDR, clock_reg[2]);
        CLEAR_DISP_SCREEN;
        screen_flag = DASHBOARD; // Return to main menu
        once = 0; // Reset initialization flag
        enter = 0;
        for (unsigned long int wait = 10000; wait--;);
    }
    if (key == MK_SW6) // Return to the main menu
    {
        CLEAR_DISP_SCREEN;
        enter = 0; // Return to main menu navigation mode
        screen_flag = MAIN_MENU;
    }
}


