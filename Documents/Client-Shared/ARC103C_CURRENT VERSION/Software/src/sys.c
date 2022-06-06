/**************************************************************************************************
 * \file			sys.c
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
#include "interrupts.h"
#include "sys.h"

/**************************************************************************************************
 * Globals
 *************************************************************************************************/
volatile uint16_t systick;

/**************************************************************************************************
 * \fn 		void ResetSystick( )
 * \brief   Reset system for count purposes.
 * \param[in]
 * \return
 *************************************************************************************************/
void ResetSystick( )
{
//	systick = 0;
}

/**************************************************************************************************
 * \fn 			uint16_t GetSystick( void )
 * \brief       Get systick for count compare purposes.
 * \param[in]
 * \return
 *************************************************************************************************/
uint16_t GetSystick( void )//
{
	return systick;
}

/* eof */
