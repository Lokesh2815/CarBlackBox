/*
 * File:   clear_log.c
 * Author: lokes
 *
 * Created on 19 November, 2024, 6:45 PM
 */


#include <xc.h>
#include "clcd1.h"
#include "main.h"
#include "blackbox.h"
#include "at240c2.h"

extern int event_count;
void clear_log()
{
    clcd_print("Clearing Logs...",LINE1(0));
    clcd_print("Just a minute",LINE2(0));
    
    event_count = 0;
}
