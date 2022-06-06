/**************************************************************************************************
 * \file			uart.c
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
#include "uart.h"

/**************************************************************************************************
 * \fn 			void USARTxEnableInterrupts( void
 * \brief
 * \param[in]
 * \return
 *************************************************************************************************/
void USARTxEnableInterrupts( void )
{
//	UCSRB |= (1<<RXCIE);
//	sei();
}

/**************************************************************************************************
 * \fn 			void USART1Init(void)
 * \brief
 * \param[in]
 * \return
 *************************************************************************************************/

void USART_Init(unsigned int ubrr)
{
 /*Set baud rate */
  UBRR0H = (unsigned char)(ubrr>>8);
  UBRR0L = (unsigned char)ubrr;
  /*Enable receiver and transmitter */
  UCSR0B = (1<<RXEN0)|(1<<TXEN0);
  /* Set frame format: 8data, 1 stop bit */
  UCSR0C = (1<<UCSZ00) | (1 << UCSZ01);
}

/**************************************************************************************************
 * \fn 			void USART1SendByte(uint8_t u8Data)
 * \brief
 * \param[in]
 * \return
 *************************************************************************************************/
void USART1SendByte(uint8_t u8Data)
{
//	uint8_t dump = UDR;
//	dump = UDR;
//
//	/* Wait while previous byte is completed */
//	while(!(UCSRA&(1<<UDRE))){};
//	/* Transmit data */
//	UDR = u8Data;
//	while(!(UCSRA&(1<<UDRE))){};
}

/**************************************************************************************************
 * \fn 			
 * \brief
 *
 *************************************************************************************************/
void USART_Transmit(unsigned char data )
{
  /* Wait for empty transmit buffer */
  while ( !( UCSR0A & (1<<UDRE0)) )
    ;
  /* Put data into buffer, sends the data */
  UDR0 = data;
}

/**************************************************************************************************
 * \fn 			
 * \brief
 *
 *************************************************************************************************/
// write null terminated string
void serial_write_str(const char* str)
{
  int len = strlen(str);
  int i;
  for (i = 0; i < len; i++) {
    USART_Transmit(str[i]);
  }
}

/* eof */
