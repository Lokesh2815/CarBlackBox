/*
 * File:   view_log.c
 * Author: lokes
 *
 * Created on 18 November, 2024, 1:57 PM
 */


#include <xc.h>
#include "main.h"
#include "clcd1.h"
#include "blackbox.h"
#include "matrix_keypad.h"
#include "adc.h"
#include "ds1307.h"
#include "at240c2.h"

extern int event_count;
extern int screen_flag;
unsigned int count = 0;
extern unsigned char key;

void view_log() {
    char arr[16];         
    static unsigned int addr;    
    unsigned char i;

    if(event_count == 0)
    {
        //CLEAR_DISP_SCREEN;
        clcd_print("No logs to view",LINE1(0));
        return;
    }
    if (key == MK_SW1) {  
        if (count < (event_count - 1)) { 
            count++;
            CLEAR_DISP_SCREEN;
        }
    } else if (key == MK_SW2) { 
        if (count > 0) {  
            count--;
            CLEAR_DISP_SCREEN;
        }
    }

    
    addr = count * 16;

    // Read the current log page from EEPROM
    for (i = 0; i < 16; i++) {
        arr[i] = read_at240c2(addr + i);
    }

    // Display the log entry on the CLCD
    clcd_print("# VIEW LOG:", LINE1(0));   // Header
    clcd_putch(count + '0', LINE2(0)); 

    // Display formatted log data (HH:MM:SS, Gear, Speed)
    clcd_putch(arr[0], LINE2(2));  // Hour tens
    clcd_putch(arr[1], LINE2(3));  // Hour units
    clcd_putch(arr[2], LINE2(4)); 
    clcd_putch(arr[3], LINE2(5));  // Minute tens
    clcd_putch(arr[4], LINE2(6));  // Minute units
    clcd_putch(arr[5], LINE2(7));
    clcd_putch(arr[6], LINE2(8));  // Second tens
    clcd_putch(arr[7], LINE2(9));  // Second units
    clcd_putch(arr[8], LINE2(10)); 
    clcd_putch(arr[9], LINE2(11)); // Gear tens
    clcd_putch(arr[10], LINE2(12)); // Gear units
    clcd_putch(arr[11], LINE2(13)); 
    clcd_putch(arr[12], LINE2(14)); // Speed tens
    clcd_putch(arr[13], LINE2(15)); // Speed units
}


    