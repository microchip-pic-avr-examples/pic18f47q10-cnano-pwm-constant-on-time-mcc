/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.79.0
        Device            :  PIC18F47Q10
        Driver Version    :  2.00
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "mcc_generated_files/mcc.h"

/* Button is active in low-logic level  */
#define BUTTON_PRESSED               false
/* Time is expressed in ms */
#define BUTTON_DEBOUNCING_TIME        10

#define TIMER_PRESCALER               2
#define FREQUENCY_TO_PR_CONVERT(F)    (uint8_t)(((_XTAL_FREQ)/(4*(F))/(TIMER_PRESCALER))-1)

/* duty cycle is always 1 microsecond */
/* but the period (frequency) is variable, as in the list */
/* where the frequencies are expressed in Hz: */
const uint8_t frequencies_list[] = {                 
    FREQUENCY_TO_PR_CONVERT( 50000UL),
    FREQUENCY_TO_PR_CONVERT(100000UL),
    FREQUENCY_TO_PR_CONVERT(200000UL),
    FREQUENCY_TO_PR_CONVERT(400000UL),
    FREQUENCY_TO_PR_CONVERT(800000UL)
};
#define FREQUENCY_LIST_DIMENSION      (sizeof(frequencies_list)/sizeof(frequencies_list[0]))


typedef enum{
    BT_NOCHANGE,
    BT_PRESS
} button_t;

static button_t ButtonCheck(void);

void main(void)
{
    uint8_t index = 0;
    /*  Initialize the device */
    SYSTEM_Initialize();

    while (1)
    {
        if(ButtonCheck() == BT_PRESS)
        {
            /* When a button press is detected, the index is updated */
            index++;
            if(index >= FREQUENCY_LIST_DIMENSION)
                index = 0;

            /* and the frequency is changed to the next one in the list */
            TMR4_LoadPeriodRegister(frequencies_list[index]);
        }        
    }
}

static button_t ButtonCheck(void)
{
    button_t result = BT_NOCHANGE;
    static bool old_button_state = !BUTTON_PRESSED;
    bool button_state = Button_GetValue();
    /* detecting only the button-press event */
    if( (button_state == BUTTON_PRESSED) && (old_button_state != BUTTON_PRESSED) )
    {
        /*  wait for debouncing time */
        __delay_ms(BUTTON_DEBOUNCING_TIME);
        while(Button_GetValue() == BUTTON_PRESSED)
        {
            /*  then check again */
            /*  and waits in the loop until the button is released */
            result = BT_PRESS;
        }
    }
    old_button_state = button_state;
    return result;
}

