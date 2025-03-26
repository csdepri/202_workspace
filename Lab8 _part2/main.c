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
#include "adc.h"
#include "lcd1602.h"
#include "ti/devices/msp/peripherals/hw_gpio.h"
#include "ti/devices/msp/m0p/mspm0g350x.h"
#include "clock.h"
#include "LaunchPad.h"
#include "ti/devices/msp/peripherals/m0p/hw_cpuss.h"
//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------
void pb2_interupt_config(void);
void pb1_interupt_config(void);
void run_lab8_part2(void);
void OPA0_init(void);
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
    led_init();
    ADC0_init(ADC12_MEMCTL_VRSEL_INTREF_VSSA);
    pb1_interupt_config();
    
    run_lab8_part2();
 
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
//    This function initializes the operational amplifier (OPA0) on the 
//    MSPM0G3507 microcontroller. The configuration involves resetting the 
//    module, enabling power, setting the gain bandwidth, and configuring 
//    input and output pins. The OPA is then powered up and enabled for use
//
// INPUT PARAMETERS:
//  none
//
// OUTPUT PARAMETERS:
//  none
//
// RETURN:
//  none
// -----------------------------------------------------------------------------
void OPA0_init(void)
{
  OPA0->GPRCM.RSTCTL = (OA_RSTCTL_KEY_UNLOCK_W | OA_RSTCTL_RESETSTKYCLR_CLR |
                        OA_RSTCTL_RESETASSERT_ASSERT);

  OPA0->GPRCM.PWREN = (OA_PWREN_KEY_UNLOCK_W | OA_PWREN_ENABLE_ENABLE);

  // time for OPA to power up
  clock_delay(24); 

  OPA0->CFGBASE &= ~(OA_CFGBASE_RRI_MASK);
  
  OPA0->CFGBASE |= ((uint32_t) OA_CFGBASE_GBW_HIGHGAIN);

  OPA0->CFG |= (OA_CFG_GAIN_MINIMUM | OA_CFG_MSEL_NC | OA_CFG_NSEL_EXTPIN0 | 
                OA_CFG_PSEL_EXTPIN0 | OA_CFG_OUTPIN_ENABLED | OA_CFG_CHOP_OFF);

  // Enable the OPA
  OPA0->CTL |= OA_CTL_ENABLE_ON;

} /* OPA0_init */
//-----------------------------------------------------------------------------
// DESCRIPTION:
//    This function displays the adc value on the first line of the lcd.
//    It uses the potentioemeter as the input to the adc. 
//    This function uses the value of the adc to light up the leds on the 
//    expansion board. The leds 0 through 8 are given a equal range of the  
//    adc value. When the meter is at the maximum rotation all the leds are om.
//    When its at the minimum all the lights are off. Pressing exits the 
//    function.  
//
// INPUT PARAMETERS:
//  none
//
// OUTPUT PARAMETERS:
//  none
//
// RETURN:
//  none
// -----------------------------------------------------------------------------
void run_lab8_part2 (void)
{
    lcd_clear();
    led_enable();
    
    //display static message to lcd
    lcd_set_ddram_addr(LCD_LINE1_ADDR);
    lcd_write_string("ADC = ");

    bool done_flag = false;
    
    while(!done_flag)
    {
        //read channel 7 of the lcd
        uint16_t adc_in = ADC0_in(7);

        //write adc value to lcd
        lcd_set_ddram_addr(LCD_LINE1_ADDR | LCD_CHAR_POSITION_6);
        lcd_write_doublebyte(adc_in);

        //break up adc value onto 9 groups 
        uint8_t adc_groups = adc_in / (4096/9);
        
        uint8_t led_idx;
        
        // turn on all leds in range of adc group value
        for(led_idx = 0; led_idx <= adc_groups; led_idx++)
        {
            led_on(led_idx);
        }
        // turn off all leds not in range of adc group value
        for(led_idx = 9; led_idx > adc_groups; led_idx--)
        {
            led_off(led_idx-1);
        }
        //when pb1 is pressed break while loop and reset pb1 flag
        if(g_PB1_pressed == true)
        {
            done_flag = true;
            g_PB1_pressed = false; 
        }
        
    }
    //shutdown message
    leds_off();
    lcd_clear();
    lcd_set_ddram_addr(LCD_LINE1_ADDR);
    lcd_write_string("Program Done");
    
    //disable interupts
    NVIC_DisableIRQ(GPIOB_INT_IRQn);

}