//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  Cole DePriest
//
//       LAB NAME:  LAB5
//
//      FILE NAME:  main.c
//
//-----------------------------------------------------------------------------
//
// DESCRIPTION:
//    This program serves as a demonstration on knowlege of using input devices 
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
#include "ti/devices/msp/peripherals/hw_gpio.h"
#include "ti/devices/msp/m0p/mspm0g350x.h"
#include "clock.h"
#include "LaunchPad.h"
//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------
void run_lab5_part1(void);
void run_lab5_part2(void);
void run_lab5_part3(void);
void run_lab5_part4(void);
//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------
#define part1_max_iterations (6)
#define part2_max_iterations (3)
#define part3_max_iterations (8)
#define part4_max_iterations (4)
#define debounce (5)
#define keypad_empty (0x10)
#define leds7_0 (0xffff)
//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------
enum Read_State
    {
        GET_LOW,
        GET_HIGH,
        DISPlAY    
    };

// Define a structure to hold different data types

int main(void)
{
    clock_init_40mhz();
    launchpad_gpio_init();
    led_init();
    seg7_init();
    dipsw_init();
    led_enable();
    lpsw_init();
    keypad_init();
    
    run_lab5_part1();
    msec_delay(500);
    run_lab5_part2();
    msec_delay(500);
    run_lab5_part3();
    msec_delay(500);
    run_lab5_part4();
 
 // Endless loop to prevent program from ending
 while (1);

} /* main */

//-----------------------------------------------------------------------------
// DESCRIPTION:
// When PB1 is pressed for the first time, the number '3' should be displayed 
// on DIG0 of the seven-segment display. The display of '3' should remain 
// active until PB1 is pressed a second time. On the second press, the display 
// should turn off.You must press PB1 again to make the '3' reappear.   
//
// INPUT PARAMETERS:
//    none
//
// OUTPUT PARAMETERS:
//    none
//
// RETURN:
//    none
// -----------------------------------------------------------------------------
void run_lab5_part1(void)
{
    led_disable();
    bool state_of_display = false;
    uint8_t part1_counter = 0;
    
    while(part1_counter < part1_max_iterations)
    {
        if(is_pb_down(PB1_IDX))
        {
            if(!state_of_display)
            {
                state_of_display = true;
                seg7_hex(0x3, SEG7_DIG0_ENABLE_IDX);
            }
            else 
            {
                state_of_display = false;
                seg7_off();
            }
            
            while(is_pb_down(PB1_IDX));
            msec_delay(debounce);
            part1_counter++;
        }
    }

}
//-----------------------------------------------------------------------------
// DESCRIPTION:
// On the first press of sw2 on the launchpad read the dip switches and
// on the second press of sw2 read dip switches again and combine the 
// values into a word with the first reading as the most signifigant bits.
// On the third press of sw2 display the value on dig 1 of the seg7 display.
// During this stage if pb1 is pressed the value is displayed on dig2 instead.
// When sw2 is pressed again go back to the first stage and wait for sw2 press to 
// read first value. Loop repeats 3 times.    
//
// INPUT PARAMETERS:
//    none
//
// OUTPUT PARAMETERS:
//    none
//
// RETURN:
//    none
// -----------------------------------------------------------------------------
void run_lab5_part2(void)
{
    led_disable();
    uint8_t part2_counter = 0;
    uint8_t dip_first_read;
    uint8_t dip_second_read;
    uint8_t dip_combined_read;
    enum Read_State part2_state = GET_LOW;

    while(part2_counter < part2_max_iterations)    
    {   
        if(is_lpsw_down(LP_SW2_IDX))
        {    
            while(is_lpsw_down(LP_SW2_IDX));
            
            msec_delay(debounce);
            
            switch (part2_state) 
            {
                case GET_LOW:
                    part2_state++;
                    dip_first_read = dipsw_read();
                    break;
                
                case GET_HIGH:
                    part2_state++;
                    dip_second_read = dipsw_read();
                    dip_combined_read = 
                    ((dip_second_read & 0x0F)<<4) | (dip_first_read & 0x0F);
                    break;
                
                case DISPlAY:
                    while(is_lpsw_up(LP_SW2_IDX))
                    {
                        if(is_pb_down(PB1_IDX))
                        {
                            seg7_on(dip_combined_read, SEG7_DIG2_ENABLE_IDX);
                        }
                        else 
                        {
                            seg7_on(dip_combined_read, SEG7_DIG0_ENABLE_IDX);
                        }
                    }
                    while(is_lpsw_down(LP_SW2_IDX))
                    msec_delay(debounce);
                    seg7_off();
                    part2_state = GET_LOW;
                    part2_counter++;
                    break;
            }
        }
    }

}
//-----------------------------------------------------------------------------
// DESCRIPTION:
// Wait for a key press and then light up the corresponding leds to the value
// of the keypress. Once another key is pressed turn off the leds for the 
// old value and display the new ones. Loops 8 times. Seg7 is off.   
//
// INPUT PARAMETERS:
//    none
//
// OUTPUT PARAMETERS:
//    none
//
// RETURN:
//    none
// -----------------------------------------------------------------------------
void run_lab5_part3(void)
{
    leds_off();
    led_enable();
    uint8_t part3_counter = 0;
    
    while(part3_counter < part3_max_iterations)
    {
        leds_on(getkey_pressed());
        wait_no_key_pressed();    
        part3_counter++;
    }
}
//-----------------------------------------------------------------------------
// DESCRIPTION:
// Wait for a keypress and then flash the leds[7:0] times the value of the
// key. Leds flash for .5 seconds on and .5 seconds off. One the leds have 
// flased a new value can be input. Repeats 4 times.
//    
//
// INPUT PARAMETERS:
//    none
//
// OUTPUT PARAMETERS:
//    none
//
// RETURN:
//    none
// -----------------------------------------------------------------------------
void run_lab5_part4(void)
{
    uint8_t part4_counter = 0;
    uint8_t flash_count = 0;

    while(part4_counter < part4_max_iterations)
    {
        while(keypad_scan() != keypad_empty)
        {
            
            for(flash_count = keypad_scan();flash_count >0;flash_count--)
            {
                leds_on(leds7_0);
                msec_delay(500);
                leds_off();
                msec_delay(500);
            }
            part4_counter++;
        }
    }

}

