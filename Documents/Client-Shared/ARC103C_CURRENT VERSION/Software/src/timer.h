/**************************************************************************************************
 * \file			timer.h
 * \description		
 *
 * \date			
 * \version			1.0.1
 * \brief           
 *
 *************************************************************************************************/

#ifndef TIMER_H_
#define TIMER_H_

void InitTimer0( uint8_t countto, uint8_t divider );
void StopTimer0( void );
void InitTimer2( uint8_t countto, uint8_t divider );
void StopTimer2( void );

#endif /* TIMER_H_ */
