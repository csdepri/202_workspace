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
//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------
void run_lab7_part1(void);
void run_lab7_part2(void);
#define LCD_MAX_VISIBLE_ADRR            (LCD_LINE2_ADDR | LCD_CHAR_POSITION_16)
#define MSPM0_ClOCK_FREQUENCY                                            (40E6)
#define SYST_TICK_PERIOD                                             (10.25E-3)
#define SYST_TICK_PERIOD_COUNT       (SYST_TICK_PERIOD * MSPM0_ClOCK_FREQUENCY)

//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------
#define part_1_message                              "Microcontrollers are fun."
#define part_2_message    "Microcontrollers are fun. I love programming in MSPM0+ assembly code!!!"

#define NUM_STATES                                                           18

// Defines the seven segment display for SOS
const uint8_t seg7_letter_code [] =
{
    0x6D, 0x00, 0x6D, 0x00, 0x6D, 0x00, //s
    0x3F, 0x00, 0x3F, 0x00, 0x3F, 0x00, //o
    0x6D, 0x00, 0x6D, 0x00, 0x6D, 0x00  //s
};

const uint8_t delay_count[] =
{
    0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x18, //dots
    0x24, 0x24, 0x24, 0x24, 0x24, 0x24, //dash
    0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x18 //dots
};
 

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

    
    
    run_lab7_part1();
    run_lab7_part2();
    sys_tick_disable();
    seg7_off();
 
 // Endless loop to prevent program from ending
 while (1);

} /* main */

//-----------------------------------------------------------------------------
// DESCRIPTION:
// This function represents the isr for the systick timer. It is called at 
// regular intervals based on the configed systick period. This isr is 
// responsioble for the timing and display of of the Morse code for "SOS" on 
// the seven segment display. s = (...)  and o = (---). Sequence repeats 
// indefinetly. 
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
void SysTick_Handler(void)
{
    static uint16_t delay_time = 1;
    static uint16_t code_index = 0;

    delay_time--;
    if(delay_time == 0)
    {
        //delay time expires so move on to next letter to display
        seg7_on(seg7_letter_code[code_index], SEG7_DIG0_ENABLE_IDX);

        //get next delay time
        delay_time = delay_count[code_index];
        code_index++;

        if(code_index == NUM_STATES)
        {
            //we went through SOS message so restart index
            code_index = 0;
        }
    }
}

//-----------------------------------------------------------------------------
// DESCRIPTION:
// This function parses a string and outputs a given amount of chars 
// in the string to the lcd 
//    
//
// INPUT PARAMETERS:
//    char* string to parse
//    uint8_t lcd ddram adress to output first char
//    uint8_t lcd ddram adress to output the last char
//
// OUTPUT PARAMETERS:
//    none
//
// RETURN:
//    none
// -----------------------------------------------------------------------------
void lcd_parser(char* string, uint8_t start_lcd_addr, uint8_t max_lcd_addr)
{
    //find the range of the string
    uint8_t range = max_lcd_addr - start_lcd_addr;
    uint8_t letter_in_string_idx = 0;

    //output until reaching the end of the string or max range of string
    while ((string[letter_in_string_idx] != '\0') 
           && (letter_in_string_idx <= range))
    {
        lcd_write_char(string[letter_in_string_idx]);
        letter_in_string_idx++;
    } 
}

//-----------------------------------------------------------------------------
// DESCRIPTION:
// This function scrolls a message across the bottom line of the lcd. When the 
// first char hits the left side of the lcd it begins slicing the string to 
// conmtinue scrolling. When there are no more chars to display its starts 
// again at the right side of the lcd. 
//    
//
// INPUT PARAMETERS:
//    
//
// OUTPUT PARAMETERS:
//    none
//
// RETURN:
//    none
// -----------------------------------------------------------------------------
void run_lab7_part1(void)
{
    bool done_flag = false;
    uint16_t lcd_adrress;
    
    while(!done_flag)
    {
        // walk the string across until it hits left side of lcd or 
        // pb1 is pressed
        for(lcd_adrress = CD_MAX_VISIBLE_ADRR ; 
        lcd_adrress > LCD_LINE2_ADDR && !done_flag; lcd_adrress--)
        {
            lcd_clear();
            lcd_set_ddram_addr(lcd_adrress);
            lcd_write_string(part_1_message);
            msec_delay(100);
            
            if(is_pb_down(PB1_IDX))
            {
                done_flag = true;
                while (is_pb_down(PB1_IDX));
            }
        }
        
        uint8_t string_index = 1;
        
        // slice string to scroll it off left side of lcd until reaching the
        // end of string or pb1 is pressed 
        while((part_1_message[string_index] != '\0') && !done_flag)
        {
            lcd_clear();
            lcd_set_ddram_addr(LCD_LINE2_ADDR);
            lcd_write_string(part_1_message + string_index);
            msec_delay(100);
            string_index ++;

            if(is_pb_down(PB1_IDX))
            {
                done_flag = true;
                while (is_pb_down(PB1_IDX));
            }
        }
    }
}
//-----------------------------------------------------------------------------
// DESCRIPTION:
// This function scrolls a message across the bottom line of the lcd. When the 
// first char hits the left side of the lcd it begins slicing the string to 
// conmtinue scrolling. When there are no more chars to display its starts 
// again at the right side of the lcd.
//
// Since the message in this part is to long and wraps around in the lcd
// memory we must use the string parser function to display only the chars
// that will fit on the second line of the lcd    
//
// INPUT PARAMETERS:
//    
//
// OUTPUT PARAMETERS:
//    none
//
// RETURN:
//    none
// -----------------------------------------------------------------------------
void run_lab7_part2(void)
{
    lcd_clear();
    
    bool done_flag = false;
    uint16_t lcd_adress;
    
    while(!done_flag)
    {
        // walk the string across until it hits left side of lcd or 
        // pb1 is pressed
        for(lcd_adress = LCD_MAX_VISIBLE_ADRR; 
        lcd_adress > LCD_LINE2_ADDR && (!done_flag); lcd_adress--)
        {
            lcd_clear();
            lcd_set_ddram_addr(lcd_adress);
            lcd_parser(part_2_message, lcd_adress, LCD_MAX_VISIBLE_ADRR);
            msec_delay(100);

            if(is_pb_down(PB1_IDX))
            {
                done_flag = true;
                while (is_pb_down(PB1_IDX));
            }
        }
        // slice string to scroll it off left side of lcd until reaching the
        // end of string or pb1 is pressed 
        uint8_t string_idx = 1;
        while((part_2_message[string_idx] != '\0') && !done_flag)
        {
            lcd_clear();
            lcd_set_ddram_addr(LCD_LINE2_ADDR);
            lcd_parser((part_2_message + string_idx), LCD_LINE2_ADDR, 
            LCD_MAX_VISIBLE_ADRR);
            msec_delay(100);
            
            string_idx ++;

            if(is_pb_down(PB1_IDX))
            {
                done_flag = true;
                while (is_pb_down(PB1_IDX));
            }
        }
    }
    
    lcd_clear();
    lcd_write_string("Program Done");
}