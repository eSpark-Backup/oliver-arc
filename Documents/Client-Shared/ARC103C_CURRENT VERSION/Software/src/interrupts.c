/**************************************************************************************************
 * \file			interrupts.c
 * \description		
 *
 * \date			
 * \version			1.0.1
 * \brief           
 *
 *************************************************************************************************/

#include <string.h>
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "lcd.h"
#include "utils.h"
#include "adc.h"
#include "timer.h"
#include "hardware.h"
#include "uart.h"
#include "defs.h"
#include "pwm.h"

#define CURRENT_5A 407

#define ALERT_OFF 0
#define ALERT_NOW 1
#define ALERT_DONE 2

/**************************************************************************************************
 * Globals
 *************************************************************************************************/
volatile uint8_t kill = 0;
extern volatile uint16_t ramp;
extern volatile uint8_t result;
extern uint16_t adc_target;
uint8_t int_count;
volatile uint16_t adc_result_copy;
volatile uint16_t adc_result_copy_battery;
uint8_t current_warning;
uint8_t beep_kill;
uint32_t fault;
int16_t sample_battery;

/**************************************************************************************************
 * \fn 			ISR (SIG_TIMER0_COMPA)
 * \brief       This comes around ~ 180us, 1x PWM cycle is 40us
 *              Checks the current - too high - reduces the driver
 *                                 - low - ramps up driver
 *                                 - OK - left alone
 *
 *              Alarm - sets timer 2 on which drives the alarm piezo pins
 *                      timer 2 counts n-cycles and auto-cancels the alert after a second or so
 *                      This is good at max current the driver will "hunt" and this mech smoothes
 *                      out alarm glitches if done but a count variable.
 *
 *              int_count - just to give some noticeable ramping of the motor
 *
 *************************************************************************************************/
ISR(ADC_vect)
{
	uint16_t adc_result = ADC;

	if( sample_battery == 0 )
	{
		adc_result_copy_battery = adc_result;
		sample_battery = 4000;
		return;
	}
	/* For main printf current debug */
    adc_result_copy = adc_result;
    /* If we're over current back off the motor */
	if( adc_result <= CURRENT_17A )
	{
		if( current_warning == ALERT_OFF )
		{
			current_warning = ALERT_DONE;
			beep_kill = 25;
		    /* Roughly 20ms */
		    InitTimer2( 250, 7 );
		}
		/* Below motor run loop is in a constant ramp and will always try
		 * to ramp it back and will do once the load reduces */
		if (( ramp + 5 ) > ADC_RAMP_START_VALUE_010H )
		{
			ramp -= 5;
		}
		if ( result == REEL_REVERSE )
		{
			PWM_Adjust( ramp, SPOOL );
		}
		else
		{
			PWM_Adjust( ramp, REEL );
		}
	}
	/* Still ramping or recovering from an overload */
	else if( ! int_count-- )
	{
		if ( ramp < adc_target )
		{
			if ( ( ramp + 1 ) < adc_target )
			{
				ramp += 1;
			}

			if ( result == REEL_REVERSE )
			{
				PWM_Adjust( ramp, SPOOL );
			}
			else
			{
				PWM_Adjust( ramp, REEL );
			}
		}
		int_count = 20;
		return;
	}

#if 0
		TCCR1A = 0;
		TCCR1B = 0;
		// PWM PIN NEEDS TO BE LOW HERE
		PORTB &= ~_BV( PB1 );
		PORTB &= ~_BV( PB2 );

		kill = 1;
#endif

}

/**************************************************************************************************
 * \fn 			ISR (SIG_TIMER0_COMPA)
 * \brief       This timer trigger the ADC read and it's interrupt
 *************************************************************************************************/
ISR (TIMER0_COMPA_vect)
{
	fault++;
	if( fault >= 1100000 )
	{
		kill = TRUE;
	}
	else
	{
		/* Every 4000 ticks ~ one second sample the battery */
		if( ! --sample_battery )
		{
			adc_read( 6 );
		}
		else
		{
			adc_read( 0 );
		}
	}

}

/**************************************************************************************************
 * \fn 			ISR (SIG_TIMER0_COMPA)
 * \brief       Mainly smooth buzzer operation and a process that can be started/stopped simply
 *              Once kicked by over current the beep_kill will decrement and stop the piezo.
 *              If still over current the ADC interrupt will simply restart it.
 *
 *************************************************************************************************/
uint8_t beep_off;
uint8_t beep_on;

/* Roughly 20ms per tick */
ISR (TIMER2_COMPA_vect)
{
	if( beep_off  )
	{
		beep_off--;
		PORTD &= ~_BV( PD7 );
		beep_kill--;
		if ( !beep_kill )
		{
			current_warning = ALERT_OFF;
			StopTimer2();
		}
	}
	else
	{
		/* beep_off=0 - do beep on */
		if( beep_on )
		{
			beep_on--;
			PORTD |= _BV( PD7 );
		}
		else
		{
			beep_on = 5;
			beep_off = 5;
		}
	}
}

/**************************************************************************************************
 * \fn
 * \brief
 *************************************************************************************************/
void BackMonitor( uint8_t state )
{
	if ( state )
	{
		fault = FALSE;
		sample_battery = 4000;
		int_count = 2;
		kill = 0;
		beep_off = 5;
		beep_on = 5;
		current_warning = ALERT_OFF;
		sei();
	}
	else
	{
		cli();
		StopTimer2();
	}
}

/* eof */
