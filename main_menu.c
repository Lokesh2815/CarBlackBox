/*
 * File:   main_menu.c
 * Author: lokes
 *
 * Created on 18 November, 2024, 1:55 PM
 */


#include <xc.h>
#include "main.h"
#include "clcd1.h"
#include "blackbox.h"
#include "matrix_keypad.h"
#include "adc.h"
#include "ds1307.h"
#include "at240c2.h"

char *main_menu[]={"view log      ","download log     ","clear log    ","set time     "};
static unsigned int count=0;
int enter=0;
extern int screen_flag;
extern unsigned char key;


void main_menu_disp(void)
{
    if (enter == 0) // Main menu navigation mode
    {
        // Display the menu options with a selection indicator
        if (count % 2 == 0)
        {
            clcd_print("*", LINE1(0));                         // Selection indicator for the first line
            clcd_print(main_menu[count], LINE1(3));           // Display current menu option
            clcd_print(main_menu[count + 1], LINE2(3));       // Display next menu option
        }
        else
        {
            clcd_print("*", LINE2(0));                        // Selection indicator for the second line
            clcd_print(main_menu[count - 1], LINE1(3));       // Display previous menu option
            clcd_print(main_menu[count], LINE2(3));           // Display current menu option
        }

        // Read user input
        key = read_switches(STATE_CHANGE);

        if (key == MK_SW1) // Navigate down
        {
            if (count < 3)
            {
                CLEAR_DISP_SCREEN;
                count++;
            }
        }
        else if (key == MK_SW2) // Navigate up
        {
            if (count > 0)
            {
                CLEAR_DISP_SCREEN;
                count--;
            }
        }
        else if (key == MK_SW6) // Exit main menu
        {
            count = 0;
            CLEAR_DISP_SCREEN;
            screen_flag = DASHBOARD; // Return to the dashboard
        }
        else if (key == MK_SW5) // Select an option
        {
            CLEAR_DISP_SCREEN;
            enter = 1; 
        }
    }
    else if (enter == 1) 
    {
        
        if (count == 0)
        {
            screen_flag = VIEW_LOG;
        }
        else if (count == 1)
        {
            screen_flag = DOWNLOAD_LOG; // Call the function for "Download Log"
        }
        else if (count == 2)
        {
             screen_flag = CLEAR_LOG; // Call the function for "Clear Log"
        }
        else if (count == 3)
        {
             screen_flag = SET_TIME; // Call the function for "Set Time"
        }

        
    }
}
