/**************************************************************************************************
 * \file			pwm.c
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

/**************************************************************************************************
 * \fn 			
 * \brief
 *
 *************************************************************************************************/
void PWM_Adjust( unsigned int val, char dir )
{
//
	if ( dir == REEL )
	{
		OCR1B = val;
		OCR1A = 0;
	}
	else if ( dir == SPOOL )
	{
		OCR1A = val;
		OCR1B = 0;
	}
}

/**************************************************************************************************
 * \fn 			
 * \brief
 *
 *************************************************************************************************/
void PWM_Start( unsigned int val, char dir )
{
	if ( dir == REEL )
	{
		OCR1B = val;
		OCR1A = 0;
	}
	else if ( dir == SPOOL )
	{
		OCR1A = val;
		OCR1B = 0;
	}
	else
	{
		return;
	}
	TCCR1B = (1 << CS10)  | (1 << WGM12);
	TCCR1A = (1 << COM1A1)|(1 << COM1B1)| (1 << WGM11);
}

/**************************************************************************************************
 * \fn 			
 * \brief
 *
 *************************************************************************************************/
void PWM_Stop( void )
{

	TCCR1A = 0;
	TCCR1B = 0;
	// PWM PIN NEEDS TO BE LOW HERE
	PORTB &= ~_BV( PB1 );
	PORTB &= ~_BV( PB2 );
}

/* eof */
