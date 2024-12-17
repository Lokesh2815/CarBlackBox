/*
 * File:   download_log.c
 * Author: lokes
 *
 * Created on 19 November, 2024, 1:24 PM
 */

#include <xc.h>
#include "at240c2.h"
#include "main.h"
#include "blackbox.h"
#include "matrix_keypad.h"
#include "clcd1.h"
#include "ds1307.h"
#include "i2c.h"
#include "uart.h"

extern int event_count;
extern int screen_flag;
unsigned int flag = 0;
extern int enter;
extern unsigned char key;

void download_log(void) {
    init_uart(); // Initialize UART communication
    unsigned char arr[17]; // Buffer to hold one event (16 bytes + null terminator)

    // Display success message or no logs message on the CLCD
    if (flag == 0 && event_count != 0) {
        clcd_print("DOWNLOAD SUCCESS", LINE1(0));
        puts("\n\r");

        // Send headers over UART
        puts("SL TIME     G  SP"); // Header: Serial, Time, Gear, Speed
        puts("\n\r");

        // Loop through each event and send data over UART
        for (int i = 0; i < event_count; i++) {
            unsigned int addr = i * 16; // Calculate the address for each event

            for (int j = 0; j < 16; j++) {
                arr[j] = read_at240c2(addr + j); // Read 16 bytes of data
            }
            arr[14] = '\0'; // Null-terminate the string
            puts("\n\r");

            // Send the event data
            putch(i + '0'); // Serial number
            puts("  "); // Space after serial number
            puts(arr); // Print event data
            puts("\n\r");
        }

        flag = 1; // Mark as completed
    }
    else if (flag == 0 && event_count == 0) {
        // No logs available
        clcd_print("NO logs to", LINE1(0));
        clcd_print("Download", LINE2(3));
        puts("\n\rLogs are not available\n\r");
        flag = 1;
    }

    // Check if SW6 is pressed to return to the main menu
    if (key == MK_SW6) {
        CLEAR_DISP_SCREEN;  // Clear the CLCD screen
        enter = 0;          // Reset the main menu navigation mode
        screen_flag = MAIN_MENU; // Return to the main menu
        flag = 0;           // Reset the flag for the next use
    }
}


