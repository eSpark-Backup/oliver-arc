/**************************************************************************************************
 * \file			defs.h
 * \description		
 *
 * \date			
 * \version			1.0.1
 * \brief           
 *
 *************************************************************************************************/

#ifndef DEFS_H_
#define DEFS_H_

#define TRUE 		1
#define FALSE 		0

#define ADC_THRESHOLD	500

#define CURRENT_148		148
#define CURRENT_17A		170
#define CURRENT_16A		190
#define CURRENT_15A		210
#define CURRENT_14A		233
#define CURRENT_13A		254
#define CURRENT_10A		313
#define CURRENT_9A		340
#define CURRENT_8A		360

#define SLOW_PWM 0xDF

#define _NOP() do { __asm__ __volatile__ ("nop"); } while (0)

#define MV_PER_BT 4883  // mx per bit
#define ACS712_0_AMPS 2540000  // mv

//#define LCD
//define UART

#endif /* DEFS_H_ */
