//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  Cole DePriest
//
//       LAB NAME:  LAB4
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
#include "ti/devices/msp/peripherals/hw_gpio.h"
#include "ti/devices/msp/m0p/mspm0g350x.h"
#include "clock.h"
#include "LaunchPad.h"
//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------
void run_lab4_part1(void);
void run_lab4_part2(void);
void run_lab4_part3(void);
void run_lab4_part4(void);
void run_lab4_part5(void);
void run_lab4_part6(void);
//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------
#define part1_leds_mask (0x3C)
#define part1_number_of_repeats (5)
#define part3_number_of_repeats (2)
#define seg7_letter_L (0x38)
#define part5_number_of_repeats (2)
#define part5_display_charachter (4)
#define part6_number_of_repeats (200)
//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------


// Define a structure to hold different data types

int main(void)
{
    clock_init_40mhz();
    launchpad_gpio_init();
    led_init();
    seg7_init();
    led_enable();
    
    run_lab4_part1();
    msec_delay(1000);
    
    
    run_lab4_part2();
    msec_delay(1000);

    run_lab4_part3();
    msec_delay(1000);

    run_lab4_part4();
    msec_delay(1000);

    run_lab4_part5();
    msec_delay(1000);

    run_lab4_part6();

    
    
 
 // Endless loop to prevent program from ending
 while (1);

} /* main */

void run_lab4_part1(void)
{
    leds_on(part1_leds_mask);
}

void run_lab4_part2(void)
{
    leds_off();

    uint8_t repeat_counter = 0;
    uint8_t led_index;
    
    while(repeat_counter < part1_number_of_repeats)
    {
        for(led_index = 0; led_index<7; led_index++)
        {
            led_on(led_index);
            msec_delay(25);
            led_off(led_index);
        }
        for(led_index = 6; led_index>=0; led_index--)
        {
            led_on(led_index);
            msec_delay(25);
            led_off(led_index);
        }
    msec_delay(100);
    repeat_counter++;
    }    
}

void run_lab4_part3(void)
{
    leds_off();
    uint8_t repeat_counter;
    int leds_on_index;

    while(repeat_counter < part3_number_of_repeats)
    {
        for(leds_on_index = 0; leds_on_index < 256; leds_on_index++)
        {
            leds_on(leds_on_index);
            msec_delay(10);
            leds_off();
        }
        repeat_counter++;
    }

}

void run_lab4_part4(void)
{
    led_disable();
    seg7_on(seg7_letter_L, SEG7_DIG0_ENABLE_IDX);

}

void run_lab4_part5(void)
{
    uint8_t counter;

    while(counter < part5_number_of_repeats)
    {
        seg7_hex(part5_display_charachter, SEG7_DIG2_ENABLE_IDX);
        msec_delay(3000);
        seg7_off();
        msec_delay(2000);

        counter++;
    }
}

void run_lab4_part6(void)
{
    uint8_t counter = 0;
    led_disable();
    while(counter < part6_number_of_repeats)
    {
        seg7_hex(0xc, SEG7_DIG0_ENABLE_IDX);
        msec_delay(10);
        seg7_hex(0xa, SEG7_DIG1_ENABLE_IDX);
        msec_delay(10);
        seg7_hex(0xf, SEG7_DIG2_ENABLE_IDX);
        msec_delay(10);
        seg7_hex(0xe, SEG7_DIG3_ENABLE_IDX);
        msec_delay(10);

        counter++;
    }
}