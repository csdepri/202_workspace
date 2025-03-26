//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  Cole DePriest
//
//       LAB NAME:  LAB6
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
void wait_pb1(void);
void wait_pb2(void);
void start_part(void);
void run_lab6_part1(void);
void run_lab6_part2(void);
void run_lab6_part3(void);
void run_lab6_part4(void);

//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------
#define char_per_line (16)
#define part_2_start_message ("Running part 2")
#define part_2_end_message ("Part 2 Done")
#define part2_value_32bit (1234567890)
#define part2_value_16bit (1234)
#define part2_value_8bit (123)
#define part3_start_message ("Running part 3")
#define part3_done (101)
#define part3_done_flag (102)
#define part3_max_count (100)
#define part3_done_message ("Part 3 done")
#define advance_part_message ("Press PB2")
#define part4_done_message ("Part 4 Done")
#define part4_start_message ("Part 4 starting")

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
    keypad_init();

   
    run_lab6_part1();
    run_lab6_part2();
    run_lab6_part3();
    run_lab6_part4();
 
 
 // Endless loop to prevent program from ending
 while (1);

} /* main */

//-----------------------------------------------------------------------------
// DESCRIPTION:
// Wait for pushbutton 2 to be pressed and then debounce.
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
void wait_pb2(void)
{
    while(is_pb_up(PB2_IDX))
    msec_delay(5);
}

//-----------------------------------------------------------------------------
// DESCRIPTION:
// Wait for pushbutton 1 to be pressed and then debounce.
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
void wait_pb1(void)
{
    while(is_pb_up(PB1_IDX))
    msec_delay(5);   
}

//-----------------------------------------------------------------------------
// DESCRIPTION:
// Setup the to start the next section of the lab. Displays instructions to 
// press PB2 and waits for it to be pressed then clears the display.
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
void start_part(void)
{
    lcd_set_ddram_addr(LCD_LINE1_ADDR);
    lcd_write_string(advance_part_message);
    wait_pb2();
    while (is_pb_down(PB2_IDX)) 
    msec_delay(5);
    lcd_clear();
}

//-----------------------------------------------------------------------------
// DESCRIPTION:
// Waits for user to press PB2 to start function
// Uses a for loop and the lcd_write_char function to display chars A through
// P on the first line of the lcd and Q through Z on the second then waits for 
// PB2 to be pressed then clears the LCD
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
void run_lab6_part1(void)
{
    //Setup
    start_part();
    lcd_set_ddram_addr(LCD_LINE1_ADDR);

    char part1_letter;

    //Advances chars by 1 starting with 'A' and ending with 'Z'
    for(part1_letter = 'A'; part1_letter <= 'Z'; part1_letter++)
    {
        //Moves to line 2 after running our of space in line 1
        if(part1_letter == 'A' + char_per_line)
        {
            lcd_set_ddram_addr(LCD_LINE2_ADDR);
        }
        lcd_write_char(part1_letter);
    }
    //Wait for PB2 to clear LCD
    wait_pb2();
    lcd_clear();

}
//-----------------------------------------------------------------------------
// DESCRIPTION:
// Prompts user to press PB2 to begin section and then waits for PB2 press. 
// When PB2 is pressed Print start message for 1 second and then clear LCD and 
// print 32 bit value centered on line 1 of LCD. Waits for PB2 and then prints 
// 16 bit value on line 2 of LCD. Waits for PB2 and then clears LCD and prints 
// 8 bit value centered on line 1 of LCD. Waits for PB2 and then prints end
// message on line 1 of LCD for one second then clears LCD.
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
void run_lab6_part2(void)
{
    //Setup and start message
    start_part();
    lcd_clear();
    lcd_set_ddram_addr(LCD_LINE1_ADDR);
    lcd_write_string(part_2_start_message);
    msec_delay(1000);
    lcd_clear();
    
    //Prints a 32 bit value on centered on the first line of the lCD
    lcd_set_ddram_addr(LCD_CHAR_POSITION_4);
    lcd_write_quadbyte(part2_value_32bit);
    wait_pb1();
    while(is_pb_down(PB1_IDX));
    
    //Prints a 16 bit value centered on the second line of the LCD
    lcd_set_ddram_addr(LCD_LINE2_ADDR | LCD_CHAR_POSITION_6);
    lcd_write_doublebyte(part2_value_16bit);
    wait_pb1();
    while(is_pb_down(PB1_IDX));
    
    //Clears LCD and prints a 8 bit value centered on the first line of the LCD
    lcd_clear();
    lcd_set_ddram_addr(LCD_LINE1_ADDR | LCD_CHAR_POSITION_8);
    lcd_write_byte(part2_value_8bit);
    wait_pb1();
    while(is_pb_down(PB1_IDX));

    //End Message
    lcd_set_ddram_addr(LCD_LINE1_ADDR);
    lcd_write_string(part_2_end_message);
    msec_delay(1000);
    lcd_clear();
}

//-----------------------------------------------------------------------------
// DESCRIPTION:
// Prompts user to press PB2 to start section and then waits for PB2. Once PB2
// is pressed print start message on first line of LCD for 1 second. Next 
// display a counter centered on line 1 of the LCD that counts down from 100 to 
// 0 displaying every value for .2 seconds. Once the counter reaches 0 roll
// over to 100 and repeat. Pressing PB1 returns the counter to 100 and continues 
// decrementing. PB2 aborts the counter clears the LCD and displays the end 
// message for 1 second and the clears the LCD again.
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
void run_lab6_part3(void)
{
    //setup
    start_part();
    lcd_write_string(part3_start_message);
    msec_delay(1000);
    lcd_clear();

    uint8_t part3_counter;
    
    //count from 100 to 0 unless the count is = to the done value
    for(part3_counter = part3_max_count; (part3_counter >= 0) & (part3_counter != part3_done); --part3_counter)
    {
        //count speed
        msec_delay(200);
        
        //output
        lcd_set_ddram_addr(LCD_LINE1_ADDR | LCD_CHAR_POSITION_7);
        lcd_write_byte(part3_counter);
        
        //repeats counter
        if(part3_counter==0)
        {
            part3_counter = part3_max_count+1;
        }
        
        //sets count == to done flag which aborts count
        if (is_pb_down(PB2_IDX)) 
        {
            part3_counter = part3_done_flag;
        }
        
        //reset counter
        if (is_pb_down(PB1_IDX)) 
        {
            part3_counter = part3_max_count+1;
        }
    }
    
    //End message
    lcd_clear();
    lcd_write_string(part3_done_message);
    msec_delay(1000);
    lcd_clear();
}
//-----------------------------------------------------------------------------
// DESCRIPTION:
// Prompts user to press PB2 to start section and then waits for PB2. Once PB2
// is pressed print start message on first line of LCD for 1 second. Next 
// uses the keypad_scan function to take a value from the keypad and then 
// display it on the LCD filling up the 2 rows and then once the rows are 
// filled clears the rows and starts back at line 1 position 1. PB1 clears the 
// LCD and returns the curson to line 1 positon 1. PB2 aborts the loop and 
// prints the end message.
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
void run_lab6_part4()
{
    //setup
    start_part();
    lcd_write_string(part4_start_message);
    msec_delay(1000);
    lcd_clear();
    lcd_set_ddram_addr(LCD_LINE1_ADDR);
    
    bool done_flag = false;
    uint8_t keypress;
    uint8_t key_count=0;  

    
    while(done_flag == false)
    {
        
        //aborts loop
        if(is_pb_down(PB2_IDX))
        {
            done_flag = true;
            while(is_pb_down(PB2_IDX));
        }
        //reset
        if(is_pb_down(PB1_IDX))
        {
            key_count=32;
            lcd_clear();
        }
        
        //input
        keypress = keypad_scan();
        wait_no_key_pressed();
        
        //executes with a valid input only 
        if(keypress != 0x10)
        {
            //move cursor to line 2 once 1 is filled
            if(key_count == 16)
            {
                lcd_set_ddram_addr(LCD_LINE2_ADDR);
            }
            
            //clear and reset
            else if(key_count == 32)
            {
                lcd_clear();
                lcd_set_ddram_addr(LCD_LINE1_ADDR);
            }
            
            //output
            hex_to_lcd(keypress);
            key_count++;
        }
    }
    //End message
    lcd_set_ddram_addr(LCD_LINE1_ADDR);
    lcd_clear();
    lcd_write_string(part4_done_message);
}
