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
void run_lab7_part4(void);
void pb2_interupt_config(void);
void pb1_interupt_config(void);
#define part3_max_count                                                   (100)
#define pb2_pressed_message "PB2 PRESSED"
#define part4_end_message "Part 4 Done"
//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------
bool g_PB1_pressed = false;
bool g_PB2_pressed = false;
// Define a structure to hold different data types

int main(void)
{
    clock_init_40mhz();
    launchpad_gpio_init();
    I2C_init();
    lcd1602_init();
    dipsw_init();
    pb2_interupt_config();
    pb1_interupt_config();
    run_lab7_part4();
 
 
 // Endless loop to prevent program from ending
 while (1);

} /* main */
//-----------------------------------------------------------------------------
// DESCRIPTION:
// This function configures pb2 for interupts
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
void pb1_interupt_config(void)
void pb2_interupt_config(void)
{
    // Set PB2 to rising edge
    GPIOA->POLARITY15_0 = GPIO_POLARITY15_0_DIO15_RISE;

    // Ensure bit is cleared
    GPIOA->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO15_CLR;

    // Unmask PB2 to allow interupt
    GPIOA->CPU_INT.IMASK = GPIO_CPU_INT_IMASK_DIO15_SET;

    NVIC_SetPriority(GPIOA_INT_IRQn,2);
    NVIC_EnableIRQ(GPIOA_INT_IRQn);
}
//-----------------------------------------------------------------------------
// DESCRIPTION:
// This function configures pb1 for interupts
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
void pb1_interupt_config(void)
{
    // Set PB1 to rising edge
    GPIOB->POLARITY31_16 = GPIO_POLARITY31_16_DIO18_RISE;

    // Ensure bit is cleared
    GPIOB->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO18_CLR;

    // Unmask PB1 to allow interupt
    GPIOB->CPU_INT.IMASK = GPIO_CPU_INT_IMASK_DIO18_SET;

    NVIC_SetPriority(GPIOB_INT_IRQn,2);
    NVIC_EnableIRQ(GPIOB_INT_IRQn);
}
//-----------------------------------------------------------------------------
// DESCRIPTION:
// This function represents the isr for gorup 1 of the IRQ handler. It is 
// called when any of the unmasked interupts that utilize this group are 
// triggered. When pb1 or pb2 are pressed it sets a global variable for that
// button.
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
void pb1_interupt_config(void)
void GROUP1_IRQHandler(void)
{
    uint32_t group_iidx_status;
    uint32_t gpio_mis;

    do 
    {
        group_iidx_status = CPUSS->INT_GROUP[1].IIDX;

        switch(group_iidx_status)
        {
        case (CPUSS_INT_GROUP_IIDX_STAT_INT0):
            gpio_mis = GPIOA->CPU_INT.MIS;
            if((gpio_mis & GPIO_CPU_INT_MIS_DIO15_MASK)==GPIO_CPU_INT_MIS_DIO15_SET)
            {
                g_PB2_pressed = true;

                GPIOA->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO15_CLR;
            }
            break;
        
        case (CPUSS_INT_GROUP_IIDX_STAT_INT1):
            gpio_mis = GPIOB->CPU_INT.MIS;
            if ((gpio_mis & GPIO_CPU_INT_MIS_DIO18_MASK) == GPIO_CPU_INT_MIS_DIO18_SET)
            {
                g_PB1_pressed = true;

                GPIOB->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO18_CLR;
            }
            break;

            defult:
                break;
        }
    
    }while (group_iidx_status != 0);
}
//-----------------------------------------------------------------------------
// DESCRIPTION:
// display a counter centered on line 1 of the LCD that counts from 0 to 
// 99 displaying every value for .2 seconds. Once the counter reaches 99 roll
// over to 0 and repeat. Pressing pb2 to toggle a message on the second line of 
// the lcd. Pressing pb1 aborts the counter loop and displays the end message
// on the lcd
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
void run_lab7_part4(void)
{
    uint8_t counter;
    bool done_flag = false;
    bool pb2_toggle = false;

    while(!done_flag)  
    {  
        //count from 100 to 0 unless the count is = to the done value
        for(counter = part3_max_count; (counter >= 0) & (!done_flag); --counter)
        {
            //count speed
            msec_delay(200);
            
            //output
            lcd_set_ddram_addr(LCD_LINE1_ADDR | LCD_CHAR_POSITION_7);
            lcd_write_byte(counter);
            
            //repeats counter
            if(counter==0)
            {
                counter = part3_max_count+1;
            }
            
            if(g_PB2_pressed)
            {
                if(pb2_toggle == false)
                {
                    lcd_set_ddram_addr(LCD_LINE2_ADDR);
                    lcd_write_string(pb2_pressed_message);
                    pb2_toggle = true;
                }
                else 
                {
                   lcd_set_ddram_addr(LCD_LINE2_ADDR);
                    lcd_write_string("                ");
                   pb2_toggle = false;
                }
                g_PB2_pressed = false;
            }

            if(g_PB1_pressed)
            {
                done_flag = true;
            }

        }
    }
    lcd_clear();
    lcd_set_ddram_addr(LCD_LINE1_ADDR);
    lcd_write_string(part4_end_message);
    
}