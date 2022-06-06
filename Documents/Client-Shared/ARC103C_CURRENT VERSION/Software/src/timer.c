/**************************************************************************************************
 * \file			timer.c
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
#include "timer.h"

/**************************************************************************************************
 * \fn 			void InitTimer0( void )
 * \brief       Set timer0 for stepper speed control. Timer compare mode.
 * \param[in]
 * \return
 *************************************************************************************************/
void InitTimer0( uint8_t countto, uint8_t divider )
{
	/* Count to */
	OCR0A = countto;
	/* Divider */
	TCCR0B = divider;

	TCCR0A = _BV(WGM01);  // Mode = CTC
	TIMSK0 = _BV(OCIE0A);  // Enable Interrupt TimerCounter0 Compare Match A (SIG_OUTPUT_COMPARE0A)
}

/**************************************************************************************************
 * \fn 		void StopTimer0( void )
 * \brief   Stop timer interrupt therefore stopping stepper.
 * \param[in]
 * \return
 *************************************************************************************************/
void StopTimer0( void )
{
	/* TC0 compare match A interrupt disable */
	TIMSK0 &= ~0x01;
	TCCR1B = 0;
}

/**************************************************************************************************
 * \fn 			
 * \brief
 *
 *************************************************************************************************/
void InitTimer2( uint8_t countto, uint8_t divider )
{
	/* Count to */
	OCR2A = countto;
	/* Divider */
	TCCR2B = divider;
	TCCR2A = _BV(WGM21);  // Mode = CTC
	TIMSK2 = _BV(OCIE2A);  // Enable Interrupt TimerCounter0 Compare Match A (SIG_OUTPUT_COMPARE0A)
}

/**************************************************************************************************
 * \fn 			
 * \brief
 *
 *************************************************************************************************/
void StopTimer2( void )
{
	TCCR2B = 0;
}

/* eof */
