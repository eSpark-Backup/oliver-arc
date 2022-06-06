/**************************************************************************************************
 * \file			pwm.h
 * \description		
 *
 * \date			
 * \version			1.0.1
 * \brief           
 *
 *************************************************************************************************/

#ifndef PWM_H_
#define PWM_H_

#define ADC_RAMP_VALUE_005H 0x05
#define ADC_RAMP_START_VALUE_010H 0x10
#define PWM_MAX_VALUE 505
#define ADC_BACK_OFF_VALUE_010H 0x10
#define ADC_BACK_OFF_VALUE_030H 0x30

#define SPOOL 1
#define REEL 2

void PWM_Stop( void );
void PWM_Adjust( unsigned int val, char dir );
void PWM_Start( unsigned int val, char dir );

#endif /* PWM_H_ */
