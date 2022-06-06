/**************************************************************************************************
 * \file			hardware.h
 * \description		
 *
 * \date			
 * \version			1.0.1
 * \brief           
 *
 *************************************************************************************************/

#ifndef HARDWARE_H_
#define HARDWARE_H_

#define NONE_ALL_OFF 0
#define RED 4
#define YELLOW 3
#define GREEN 2


#define HIGH 1
#define LOW 0
#define ON 1
#define OFF 0
#define FWD 1
#define REV 2
#define REEL_SLOW 3
#define REEL_FAST 4
#define REEL_REVERSE 5

// [4]LCD/Red LED. [3]LCD/Yellow LED, [2]LCD/Green LED,
void Led( char led, char mark );
void  	Marker( char mark );
int Blocking_Switch_Wait( void );
int Check_Mode( void );
int Check_Switchs( void );
void Motor_Disengaged_For_Spool( char state );
void Motor_Engage_For_Drive( char state );
void Motor( char state );
void Alarm( char state );
void HW_Init( void );

#endif /* HARDWARE_H_ */
