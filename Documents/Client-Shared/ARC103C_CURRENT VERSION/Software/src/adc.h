/**************************************************************************************************
 * \file			adc.h
 * \description		
 *
 * \date			
 * \version			1.0.1
 * \brief           
 *
 *************************************************************************************************/

#ifndef ADC_H_
#define ADC_H_

#define ADC_PIN			0

#if WAIT_ADC
uint16_t adc_read(uint8_t adcx);
#endif
void adc_read(uint8_t adcx);
void adc_init( void );

uint16_t ReadADC(uint8_t __channel);

#endif /* ADC_H_ */
