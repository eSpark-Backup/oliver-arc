/**************************************************************************************************
 * \file			lcd.c
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

/**************************************************************************************************
 * \fn 			
 * \brief
 *
 *************************************************************************************************/
void LCD_wait(void)
{
	sleepms(1);
}

/**************************************************************************************************
 * \fn 			
 * \brief
 *
 *************************************************************************************************/
void nopDelay( void )
{
	asm volatile ("nop");
	asm volatile ("nop");
	asm volatile ("nop");
	asm volatile ("nop");
}

/**************************************************************************************************
 * \fn 			
 * \brief
 *
 *************************************************************************************************/
void LCD_putchar(unsigned char data){
//	LCD_wait();  // is it busy?
	LCD_wait();  // is it still busy?
	PORTD |= (1<<LCD_RS);  //RS high for data
//	sleepms(1);
	PORTC &= ~((1<<LCD_DB7)|(1<<LCD_DB6)|(1<<LCD_DB5)|(1<<LCD_DB4));
	if( data & 0x80 ) PORTC |= _BV( LCD_DB7 );
	if( data & 0x40 ) PORTC |= _BV( LCD_DB6 );
	if( data & 0x20 ) PORTC |= _BV( LCD_DB5 );
	if( data & 0x10 ) PORTC |= _BV( LCD_DB4 );
	nopDelay( );
	PORTD |= (1<<LCD_E);    //Enable pin high
	nopDelay( );
	PORTD &= ~(1<<LCD_E);   //Enable Pin Low
	nopDelay( );
	PORTC &= ~((1<<LCD_DB7)|(1<<LCD_DB6)|(1<<LCD_DB5)|(1<<LCD_DB4));
	if( data & 0x08 ) PORTC |= _BV( LCD_DB7 );
	if( data & 0x04 ) PORTC |= _BV( LCD_DB6 );
	if( data & 0x02 ) PORTC |= _BV( LCD_DB5 );
	if( data & 0x01 ) PORTC |= _BV( LCD_DB4 );
	nopDelay( );
	PORTD |= (1<<LCD_E);    //Enable pin high
	nopDelay( );
	PORTD &= ~(1<<LCD_E);   //Enable Pin Low
	nopDelay( );
}

/**************************************************************************************************
 * \fn 			
 * \brief
 *
 *************************************************************************************************/
void LCD_half_command(unsigned char command){

	LCD_wait();  // is it still busy?
	PORTD &= ~(1<<LCD_RS);  //RS low for Command
	PORTC &= ~((1<<LCD_DB7)|(1<<LCD_DB6)|(1<<LCD_DB5)|(1<<LCD_DB4));
	if( command & 0x80 ) PORTC |= _BV( LCD_DB7 );
	if( command & 0x40 ) PORTC |= _BV( LCD_DB6 );
	if( command & 0x20 ) PORTC |= _BV( LCD_DB5 );
	if( command & 0x10 ) PORTC |= _BV( LCD_DB4 );
	nopDelay( );
	PORTD |= (1<<LCD_E);    //Enable pin high
	nopDelay( );
	PORTD &= ~(1<<LCD_E);   //Enable Pin Low
	nopDelay( );
}

/**************************************************************************************************
 * \fn 			
 * \brief
 *
 *************************************************************************************************/
void LCD_command(unsigned char command){

	LCD_wait();  // is it still busy?
	PORTD &= ~(1<<LCD_RS);  //RS low for Command
	PORTC &= ~((1<<LCD_DB7)|(1<<LCD_DB6)|(1<<LCD_DB5)|(1<<LCD_DB4));
	if( command & 0x80 ) PORTC |= _BV( LCD_DB7 );
	if( command & 0x40 ) PORTC |= _BV( LCD_DB6 );
	if( command & 0x20 ) PORTC |= _BV( LCD_DB5 );
	if( command & 0x10 ) PORTC |= _BV( LCD_DB4 );
	nopDelay( );
	PORTD |= (1<<LCD_E);    //Enable pin high
	nopDelay( );
	PORTD &= ~(1<<LCD_E);   //Enable Pin Low
	nopDelay( );
	PORTC &= ~((1<<LCD_DB7)|(1<<LCD_DB6)|(1<<LCD_DB5)|(1<<LCD_DB4));
	if( command & 0x08 ) PORTC |= _BV( LCD_DB7 );
	if( command & 0x04 ) PORTC |= _BV( LCD_DB6 );
	if( command & 0x02 ) PORTC |= _BV( LCD_DB5 );
	if( command & 0x01 ) PORTC |= _BV( LCD_DB4 );
	nopDelay( );
	PORTD |= (1<<LCD_E);    //Enable pin high
	nopDelay( );
	PORTD &= ~(1<<LCD_E);   //Enable Pin Low
	nopDelay( );
}

/**************************************************************************************************
 * \fn 			
 * \brief
 *
 *************************************************************************************************/
void LCD_init(void){
	//setup the LCD control signals on PortC  (RS and Enable as output)
	DDRD |= ((1<<LCD_RS)|(1<<LCD_E));
	PORTD &= ~((1<<LCD_RS)|(1<<LCD_E));// set both low
	DDRC = ((1<<LCD_DB7)|(1<<LCD_DB6)|(1<<LCD_DB5)|(1<<LCD_DB4));
	PORTC &= ~((1<<LCD_DB7)|(1<<LCD_DB6)|(1<<LCD_DB5)|(1<<LCD_DB4));

	sleepms(10);
	LCD_half_command(0x30);
	sleepms(10);
	LCD_half_command(0x30);
	sleepms(10);
	LCD_half_command(0x30);
	sleepms(10);
	LCD_half_command(0x20);
	sleepms(1);
	LCD_command(0x28);
	sleepms(1);
	LCD_command(0x08);
	sleepms(1);
	LCD_command(0x01);
	sleepms(1);
	LCD_command(0x0C);   // on
	sleepms(1);
}

/**************************************************************************************************
 * \fn 			
 * \brief
 *
 *************************************************************************************************/
void LCD_write(char* dstring){

	//is the character pointed at by dstring a zero? If not, write character to LCD
	while(*dstring)
	{
	// write the character from dstring to the LCD, then post-inc the string pointer.
	       LCD_putchar(*dstring++);
	}
}

/* eof */
