//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  TBD
//
//       LAB NAME:  TBD
//
//      FILE NAME:  main.c
//
//-----------------------------------------------------------------------------
//
// DESCRIPTION:
//    This program serves as a ... 
//
//*****************************************************************************
//*****************************************************************************

//-----------------------------------------------------------------------------
// Loads standard C include files
//-----------------------------------------------------------------------------


#include <stdio.h>

//-----------------------------------------------------------------------------
// Loads MSP launchpad board support macros and definitions
//-----------------------------------------------------------------------------
#include <ti/devices/msp/msp.h>
#include "lcd1602.h"
#include "ti/devices/msp/peripherals/hw_gpio.h"
#include "ti/devices/msp/m0p/mspm0g350x.h"
#include "clock.h"
#include "LaunchPad.h"
#include "ti/devices/msp/peripherals/m0p/hw_cpuss.h"


//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------
void run_lab7_part3(void);

#define part3_max_count                                                   (100)
#define MSPM0_ClOCK_FREQUENCY                                            (40E6)
#define SYST_TICK_PERIOD                                             (10.25E-3)
#define SYST_TICK_PERIOD_COUNT       (SYST_TICK_PERIOD * MSPM0_ClOCK_FREQUENCY)
//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------

// Define a structure to hold different data types

int main(void)
{
    clock_init_40mhz();
    launchpad_gpio_init();
    I2C_init();
    lcd1602_init();
    dipsw_init();
    seg7_init();
    
    sys_tick_init(SYST_TICK_PERIOD_COUNT);
    
    run_lab7_part3();
 
 
 // Endless loop to prevent program from ending
 while (1);

} /* main */

void SysTick_Handler(void)
{
    uint8_t display_value = 0; 
    uint8_t idx = 0;
    uint8_t mask = 0x1;

    uint8_t dip_value = dipsw_read();

    for(idx = 0; idx < 4; idx ++)
    {
        if((dip_value & mask) == mask)
        {
            display_value ++;
        }
        mask <<=1;
    }
    seg7_hex(display_value,0);
}   


void run_lab7_part3(void)
{
    uint8_t counter;
    bool part3_done = false;
     
    while(!part3_done)  
    {  
        //count from 0 to 100 unless the count is = to the done value
        for(counter = 0 ; (counter < 100); ++counter)
        {
            //count speed
            msec_delay(200);
            
            //output
            lcd_set_ddram_addr(LCD_LINE1_ADDR | LCD_CHAR_POSITION_7);
            lcd_write_byte(counter);
            
            //repeats counter
            if(counter==99)
            {
                counter = -1 ;
            }
            
        }
    }
}