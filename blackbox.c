/*
 * File:   blackbox.c
 * Author: lokes
 *
 * Created on 12 November, 2024, 10:08 AM
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
            view_log(); // Call the function for "View Log"
        }
        else if (count == 1)
        {
            download_log(); // Call the function for "Download Log"
        }
        else if (count == 2)
        {
            clear_log(); // Call the function for "Clear Log"
        }
        else if (count == 3)
        {
            set_time(); // Call the function for "Set Time"
        }

        // Read user input
        key = read_switches(STATE_CHANGE);

        if (key == MK_SW6) // Return to the main menu
        {
            CLEAR_DISP_SCREEN;
            enter = 0; // Return to main menu navigation mode
        }
    }
}


void view_log()
{
    clcd_print("# VIEW LOG :",LINE1(0));
}
void download_log()
{
    
}
void clear_log()
{
    clcd_print("Clearing Logs....",LINE1(0));
    clcd_print("Just a minute",LINE2(0));
}
void set_time()
{
    clcd_print("HH:MM:SS",LINE1(0));
}

