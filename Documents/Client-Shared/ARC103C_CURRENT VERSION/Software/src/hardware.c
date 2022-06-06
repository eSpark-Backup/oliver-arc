/**************************************************************************************************
 * \file			hardware.c
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
#include "hardware.h"
#include "defs.h"
#include "uart.h"
#include "timer.h"

/**************************************************************************************************
 * Globals
 *************************************************************************************************/
extern uint8_t boot_mode;

/**************************************************************************************************
 * \fn 			
 * \brief
 *
 *************************************************************************************************/
void Marker( char mark )
{
	if( mark )
		PORTC |= _BV( PC4 );
	else
		PORTC &= ~_BV( PC4 );

}

/**************************************************************************************************
 * \fn 			
 * \brief
 *
 *************************************************************************************************/
void Led( char led, char mark )
{
	if ( led == RED )
	{
		if( mark )
			PORTC &= ~_BV( PC4 );
		else
			PORTC |= _BV( PC4 );
	}
	else if ( led == YELLOW )
	{
		if( mark )
			PORTC &= ~_BV( PC3 );
		else
			PORTC |= _BV( PC3 );
	}
	else if ( led == GREEN )
	{
		if( mark )
			PORTC &= ~_BV( PC2 );
		else
			PORTC |= _BV( PC2 );
	}
	else
	{
		PORTC |= _BV( PC4 );
		PORTC |= _BV( PC3 );
		PORTC |= _BV( PC2 );
	}
}

/**************************************************************************************************
 * \fn 			
 * \brief
 *
 *************************************************************************************************/
int Blocking_Switch_Wait( void )
{
	while( 1 )
	{
		if( ! (PINB & _BV( PB5 ) )) return REEL_SLOW;
		if( ! (PINB & _BV( PB4 ) )) return REEL_FAST;
	}
    return 0;
}

/**************************************************************************************************
 * \fn 			
 * \brief
 *
 *************************************************************************************************/
int Check_Switchs( void )
{
	if( ! (PINB & _BV( PB5  ) )) return REEL_SLOW;
	if( ! (PINB & _BV( PB4  ) )) return REEL_FAST;
	if( ! (PINB & _BV( PB3  ) )) return REEL_REVERSE;
    return FALSE;
}

/**************************************************************************************************
 * \fn 			
 * \brief
 *
 *************************************************************************************************/
int Check_Mode( void )
{
	if( ! (PIND & _BV( PD6 ) ))
	{
		sleepms(100);
		if( ! (PIND & _BV( PD6 ) )) return TRUE;
	}
    return FALSE;
}

/**************************************************************************************************
 * \fn 			
 * \brief
 *
 *************************************************************************************************/
void Motor_Disengaged_For_Spool( char state )
{
	if( state )
		PORTD &= ~_BV( PD4 );
	else
		PORTD |= _BV( PD4 );

}

/**************************************************************************************************
 * \fn 			
 * \brief
 *
 *************************************************************************************************/
// Opposite side of spool disengage
void Motor_Engage_For_Drive( char state )
{
	if( state )
		PORTD |= _BV( PD5 );
	else
		PORTD &= ~_BV( PD5 );

}

/**************************************************************************************************
 * \fn 			
 * \brief
 *
 *************************************************************************************************/
void Motor( char state )
{
	if( state == REV  )
		PORTB |= _BV( PB5 );
	else
		PORTB &= ~_BV( PB5 );
}

/**************************************************************************************************
 * \fn 			
 * \brief
 *
 *************************************************************************************************/
void Alarm( char state )
{
	if( state == ON  )
		PORTD |= _BV( PD7 );
	else
		PORTD &= ~_BV( PD7 );
}

/**************************************************************************************************
 * \fn 			
 * \brief
 *
 *************************************************************************************************/
void HW_Setup( void )
{
	boot_mode = FALSE;
	sleepms(500);
#ifdef LCD
    sleepms(100);
    LCD_init();
    LCD_write("   GrippaTank    ");

#else
	Led( NONE_ALL_OFF, OFF );
	sleepms(300);
	Led( RED, ON );
	sleepms(300);
	Led( YELLOW, ON );
	sleepms(300);
	Led( GREEN, ON );
	sleepms(300);
	Led( GREEN, OFF );
	sleepms(300);
	Led( YELLOW, OFF );
	sleepms(300);
#endif
    adc_init();
	USART_Init( UBRR_VALUE_38400 );
#ifdef LCD
#else
	Led( RED, ON );
	sleepms(500);
#endif
	boot_mode =  Check_Mode();
#ifdef UART
    serial_write_str("Reel Controller V 1.0.3\n\r");
#endif
#ifdef LCD
    LCD_command( 0xC0 );
    LCD_write(" Version 1.0.3   ");
#endif
    /* Timer for interrupt driven ADC check */
    InitTimer0( 50, 3 );

// 1.0.0  Original code    - no beep
// 1.0.1  Fixed reset and bad compiler    - no  beep
// 1.0.2  Interrupt drive current and back-off    -  long-short

    Alarm( ON );
    sleepms(800);
    Alarm( OFF );
    sleepms(300);
    Alarm( ON );
    sleepms(200);
    Alarm( OFF );
    sleepms( 2000 );
}


/**************************************************************************************************
 * \fn 			void HW_Init( void )
 * \brief
 * \param[in]
 * \return
 *************************************************************************************************/
void HW_Init( void )
{

/* PORTB [7]xtal, [6]xtal, [5]RDF.0, [4]RDF.1, [3]RDF.2, [2]PWM, [1]PWM, [0]RDF.3 */
	DDRB = 0x06;
	PORTB = 0x00;

/* PORTC [7]N.A, [6]Reset, [5]LCD, [4]LCD/Red LED. [3]LCD/Yellow LED, [2]LCD/Green LED, [1]Spool ADC, [0]Current sense ADC  */
	DDRC = 0x1C;
	PORTC = 0x00;

/* PORTD [7]Buzzer, [6]SMod switch, [5]Shutdown PWM, [4]Shutdown PWM, [3]LCD, [2]LCD, [1]UART-tx, [0]UART-rx */
	DDRD = 0xB6;
	PORTD = 0x40;
	/* Enable the ADC */
	ADCSRA |= _BV(ADEN);

	HW_Setup();
}

/* eof */
