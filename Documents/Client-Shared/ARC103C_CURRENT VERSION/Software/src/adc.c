/**************************************************************************************************
 * \file			adc.c
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

#if WAIT_ADC
/**************************************************************************************************
 * \fn 			
 * \brief
 *
 *************************************************************************************************/
uint16_t adc_read(uint8_t adcx) {
	/* adcx is the analog pin we want to use.  ADMUX's first few bits are
	 * the binary representations of the numbers of the pins so we can
	 * just 'OR' the pin's number with ADMUX to select that pin.
	 * We first zero the four bits by setting ADMUX equal to its higher
	 * four bits. */
	ADMUX	&=	0xf0;
	ADMUX	|=	adcx;

	/* This starts the conversion. */
	ADCSRA |= _BV(ADSC);

	/* This is an idle loop that just wait around until the conversion
	 * is finished.  It constantly checks ADCSRA's ADSC bit, which we just
	 * set above, to see if it is still set.  This bit is automatically
	 * reset (zeroed) when the conversion is ready so if we do this in
	 * a loop the loop will just go until the conversion is ready. */
//	while ( (ADCSRA & _BV(ADSC)) );

	/* Finally, we return the converted value to the calling function. */
//	return ADC;
}
#endif

/**************************************************************************************************
 * \fn 			
 * \brief
 *
 *************************************************************************************************/
void adc_read(uint8_t adcx) {
	/* adcx is the analog pin we want to use.  ADMUX's first few bits are
	 * the binary representations of the numbers of the pins so we can
	 * just 'OR' the pin's number with ADMUX to select that pin.
	 * We first zero the four bits by setting ADMUX equal to its higher
	 * four bits. */
	ADMUX	&=	0xf0;
	ADMUX	|=	adcx;

	/* This starts the conversion. */
	ADCSRA |= _BV(ADSC);
}

/**************************************************************************************************
 * \fn 			
 * \brief
 *
 *************************************************************************************************/
uint16_t ReadADC(uint8_t __channel)
{
   ADMUX = (ADMUX & 0xf0) | __channel; // Channel selection
   ADCSRA |= _BV(ADSC);                // Start conversion
   while(!bit_is_set(ADCSRA,ADIF));    // Loop until conversion is complete
   ADCSRA |= _BV(ADIF);                // Clear ADIF by writing a 1 (this sets the value to 0)

   return(ADC);
}

/**************************************************************************************************
 * \fn 			
 * \brief
 *
 *************************************************************************************************/
void adc_init( void )
{
	ADMUX = 0b01000000; //AVCC with external capacitor(1) at AREF pin
	ADCSRA = _BV(ADIE) | _BV(ADIE) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0); //Enable ADC and set 128 prescale
	ADCSRA 	|= _BV(ADEN);
//#define ADIE 3
//#define ADIF 4
}

/* eof */
