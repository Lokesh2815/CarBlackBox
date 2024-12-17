/*
 * File:   main.c
 * Author: lokes
 *
 * Created on 12 November, 2024, 10:17 AM
 */

#include <xc.h>
#include "main.h"
#include "i2c.h"
#include "ds1307.h"
#include "clcd1.h"
#include "blackbox.h"
#include "matrix_keypad.h"
#include "adc.h"
#include "at240c2.h"
#include "uart.h"


int screen_flag = DASHBOARD;
unsigned char key;
extern unsigned int count = 0;
extern int enter;
int once = 0;
extern unsigned int flag;

void init_config(void) {
    init_i2c();
    init_ds1307();
    init_clcd();
    init_matrix_keypad();
    init_adc();
}

void main(void) {
    init_config();
    while (1) {
        key = read_switches(STATE_CHANGE);


        if (screen_flag == DASHBOARD) {
            dashboard();
            if (key == MK_SW5) {
                screen_flag = MAIN_MENU;
                CLEAR_DISP_SCREEN;
            }
        } else if (screen_flag == MAIN_MENU) {
            main_menu_disp();
        } else if (screen_flag == VIEW_LOG) {
            //CLEAR_DISP_SCREEN;
            view_log();
            if (key == MK_SW6) // Return to the main menu
            {
                CLEAR_DISP_SCREEN;
                enter = 0; // Return to main menu navigation mode
                count = 0;
                screen_flag = MAIN_MENU;
            }
        } else if (screen_flag == DOWNLOAD_LOG) {
            download_log();
           
        } else if (screen_flag == CLEAR_LOG) {
            clear_log();
            if (key == MK_SW6) // Return to the main menu
            {
                CLEAR_DISP_SCREEN;
                enter = 0; // Return to main menu navigation mode
                screen_flag = MAIN_MENU;
            }
        } else if (screen_flag == SET_TIME) {
            set_time();
        }
    }

}



