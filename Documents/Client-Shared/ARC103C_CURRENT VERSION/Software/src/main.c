/**************************************************************************************************
 * \file			main.c
 * \description		Main
 *
 * \date			04/12/2016
 * \version			1.0.1
 * \brief           ADC is interrupt driven and kills motor if needed
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

/**************************************************************************************************
 * Globals
 *************************************************************************************************/
extern volatile uint8_t kill;
volatile uint16_t ramp;
volatile uint8_t result;
volatile uint16_t adc_target;
extern volatile uint16_t adc_result_copy;
extern volatile uint16_t adc_result_copy_battery;
uint8_t boot_mode;

const int spool_table_PWM [] =               { 250,275,300,325,350,375,400,425,450,475,500,505 };
const int spool_table_ADC_read_at_spool [] = { 295,334,404,434,466,503,569,607,634,668,737,738 };

/**************************************************************************************************
 * Prototypes
 *************************************************************************************************/


/**************************************************************************************************
 * \fn 			
 * \brief
 *
 *************************************************************************************************/
void Display_Battery_Voltage( uint16_t adc_result, uint8_t dis )
{
	uint32_t calc;
	float human_value;
    char buffer[50];

	calc = (uint32_t) MV_PER_BT * (uint32_t)adc_result;  // 4.883mv per bit but 1000
	//calc = (uint32_t)ACS712_0_AMPS - calc;
	human_value = (float)calc / 1000;
	human_value *= 3.1;
	human_value /= 1000;
	//sprintf( buffer, "FAST => %i ADC\n\r", adc_result );
	//serial_write_str( buffer );
	if ( human_value > 14.9 )
	{
		LCD_command( 0x80 );
		LCD_write("    WARNING        ");
		LCD_command( 0xC0 );
		LCD_write("  Over Voltage!    ");
		return;
	}
	else if ( dis == REEL_FAST )
	{
		LCD_command( 0x80 );
		LCD_write("   Fast Wind    ");

	}
	else if ( dis == REEL_SLOW )
	{
		LCD_command( 0x80 );
		LCD_write("   Slow Wind    ");
	}
	else if ( dis == REEL_REVERSE )
	{
		LCD_command( 0x80 );
		LCD_write("  Reverse Wind  ");
	}
	else
	{
		LCD_command( 0x80 );
		LCD_write("   GrippaTank    ");
	}

	sprintf( buffer, " Battery %0.2fv  ", human_value );
	LCD_command( 0xC0 );
	LCD_write(buffer);
}

/**************************************************************************************************
 * \fn 			
 * \brief
 *
 *************************************************************************************************/
void Display_Battery_Voltage_Only( uint16_t adc_result )
{
	uint32_t calc;
	float human_value;
    char buffer[50];

	calc = (uint32_t) MV_PER_BT * (uint32_t)adc_result;  // 4.883mv per bit but 1000
	//calc = (uint32_t)ACS712_0_AMPS - calc;
	human_value = (float)calc / 1000;
	human_value *= 3.1;
	human_value /= 1000;
	//sprintf( buffer, "FAST => %i ADC\n\r", adc_result );
	//serial_write_str( buffer );
	if ( human_value > 14.9 )
	{
		LCD_command( 0x80 );
		LCD_write("    WARNING        ");
		LCD_command( 0xC0 );
		LCD_write("  Over Voltage!    ");
		return;
	}

	sprintf( buffer, " Battery %0.2fv  ", human_value );
	LCD_command( 0xC0 );
	LCD_write(buffer);
}

/**************************************************************************************************
 * \fn 			int main(void)
 * \brief
 *
 * 1.0.0  Original code    - no beep
 * 1.0.1  Fixed reset and bad compiler    - no  beep
 * 1.0.2  Interrupt drive current and back-off    -  long-short
 *
 *************************************************************************************************/
int main(void)
{
	uint16_t adc_result;
	uint32_t calc;
	float human_value;
    char buffer[50];
    int sum[20];
    uint8_t n;
    int8_t i;
    uint16_t x;
    uint16_t time_on, time_off;
    uint8_t pulled;
    uint16_t tick;
    HW_Init();

    Motor_Disengaged_For_Spool( TRUE );
    Motor_Engage_For_Drive( FALSE );

    pulled = FALSE;

    tick = 0;

    while(1)
    {

#ifdef FIXED_TEST
    	if ( Check_Mode() )
    	{
#ifdef UART
    		sprintf( buffer, "Mode switch pressed\n\r" );
        	serial_write_str( buffer );
#endif
        	ramp = ADC_RAMP_START_VALUE_010H;
        	// 505, 450, 400
        	adc_target = 505;
    		Motor_Engage_For_Drive( TRUE );
    		Motor_Disengaged_For_Spool( FALSE );
			PWM_Start( ramp, SPOOL );
            n = 0;
            there = 0;
			while( n < 20 )
			{
				if ( ramp < adc_target )
				{
		    		sleepms(10);
					PWM_Adjust( ramp, SPOOL );
					ramp+= 20;
				}
				else
				{
		    		Motor_Engage_For_Drive( TRUE );
		    		Motor_Disengaged_For_Spool( FALSE );
					if( ! there )
					{
						PWM_Adjust( ramp, adc_target );
						there = 1;
					}
					sleepms(2000);
		   			Motor_Disengaged_For_Spool( TRUE );
		   			Motor_Engage_For_Drive( FALSE );
					sleepms(50);
					adc_result = ReadADC(1);
					sum[ n++ ] = adc_result;
#ifdef UART
	    			sprintf( buffer, "%i,", adc_result );
	    		   	serial_write_str( buffer );
#endif
				}
			}
   			PWM_Stop();
   			Motor_Disengaged_For_Spool( TRUE );
   			Motor_Engage_For_Drive( FALSE );

   			calc = 0;
   			for( n=0; n < 20; n++)
   			{
   				calc += ( uint32_t )sum[ n ];
   			}
   			calc = calc / 20;
#ifdef UART
			sprintf( buffer, "\n\rAverage over 20 was %i\n\r", ( uint16_t )calc  );
		   	serial_write_str( buffer );
#endif
			Mandatory_Wait( 1 );
    	}
#endif
#ifdef RAMP_TEST
    	if ( Check_Mode() )
    	{
#ifdef UART
    		sprintf( buffer, "Mode switch pressed\n\r" );
        	serial_write_str( buffer );
#endif
        	ramp = ADC_RAMP_START_VALUE_010H;
        	adc_target = PWM_MAX_VALUE;
    		//Motor_Engage_For_Drive( TRUE );
    		//Motor_Disengaged_For_Spool( FALSE );
			PWM_Start( ramp, SPOOL );

			while( 1 )
			{
				if ( ramp < adc_target )
				{
		    		Motor_Engage_For_Drive( TRUE );
		    		Motor_Disengaged_For_Spool( FALSE );
		    		sleepms(1);
					PWM_Adjust( ramp, SPOOL );
					sleepms(300);
		   			Motor_Disengaged_For_Spool( TRUE );
		   			Motor_Engage_For_Drive( FALSE );
					sleepms(5);
					adc_result = ReadADC(1);
#ifdef UART
	    			sprintf( buffer, "%i,", adc_result );
	    		   	serial_write_str( buffer );
#endif
					ramp++;
				}
				else
				{
					break;
				}
			}
   			PWM_Stop();
   			Motor_Disengaged_For_Spool( TRUE );
   			Motor_Engage_For_Drive( FALSE );
   			Mandatory_Wait( 1 );
    	}
#endif

    	result = Check_Switchs();

    	if ( result )
    	{
    		Motor_Engage_For_Drive( TRUE );
    		Motor_Disengaged_For_Spool( FALSE );
    		adc_result_copy = ReadADC(6);

    		if( result == REEL_SLOW )
    		{
#ifndef LCD
    			Led( YELLOW, ON );
#else
    			Display_Battery_Voltage( adc_result_copy, REEL_SLOW );
#endif
    			adc_target = SLOW_PWM;
#ifdef UART
        		sprintf( buffer, "SLOW => Speed Control ADC => %i\n\r", adc_target );
#endif
    		}
    		else if ( result == REEL_REVERSE )
    		{
#ifndef LCD
    			Led( GREEN, ON );
#else
    			Display_Battery_Voltage( adc_result_copy, REEL_REVERSE );
#endif
    			adc_target = SLOW_PWM;
#ifdef UART
        		sprintf( buffer, "REVERSE SLOW => Speed Control ADC => %i\n\r", adc_target );
#endif
    		}
    		else
    		{
#ifndef LCD
    			Led( YELLOW, ON );
#else
    			Display_Battery_Voltage( adc_result_copy, REEL_FAST );
#endif
    			adc_target = ReadADC(7);
    			/* Back off value of pot set too high, need a few percent below
    			 100% PWM for low side MOSFET charge pump */
        		if( adc_target > PWM_MAX_VALUE )
        		{
        			adc_target = PWM_MAX_VALUE;
        		}
#ifdef UART
    			sprintf( buffer, "FAST => Speed Control ADC => %i\n\r", adc_target );
#endif
    		}
#ifdef UART
        	serial_write_str( buffer );
#endif
        	/* Interrupts and ADC current process */

        	ramp = ADC_RAMP_START_VALUE_010H;
			if ( result == REEL_REVERSE )
			{
				PWM_Start( ramp, SPOOL );
			}
			else
			{
				PWM_Start( ramp, REEL );
			}
        	BackMonitor( START );
    		/* While switches are pressed run motor */
        	/* Mode speed/direction on LCD for 2 seconds initially */
        	i=100;
    		while( Check_Switchs() && !kill )
    		{
    			sleepms(20);
    			if( i-- ) continue;
    			/* Print every 1/2 second */
    			i=25;
		        adc_result = adc_result_copy;
        		calc = (uint32_t) MV_PER_BT * (uint32_t)adc_result;
        		calc = (uint32_t)ACS712_0_AMPS - calc;
        		human_value = (float)calc / 100000;
#ifdef UART
        		sprintf( buffer, "FAST => %i ADC\n\r", adc_result );
            	serial_write_str( buffer );
        		sprintf( buffer, "FAST => %0.2f Amps\n\r", human_value );
            	serial_write_str( buffer );
#endif
#ifdef LCD
        		sprintf( buffer, " Current %0.2fA ", human_value );
        		LCD_command( 0x80 );
        		LCD_write(buffer);

        		adc_result = adc_result_copy_battery;
        		Display_Battery_Voltage_Only( adc_result );
#endif
#if 0
    			if ( ramp < adc_result )
    			{
    				if ( result == REEL_REVERSE )
    				{
    					PWM_Adjust( ramp, SPOOL );
    				}
    				else
    				{
    					PWM_Adjust( ramp, REEL );
    				}
    				if ( ( ramp + ADC_RAMP_VALUE_005H ) < adc_result )
    				{
    					ramp += ADC_RAMP_VALUE_005H;
    				}
    			}
#endif
    		}
    		/* Stopped pressing or interrupted over current */
        	BackMonitor( STOP );
   			PWM_Stop();
#ifndef LCD
   			Led( YELLOW, OFF );
   			Led( GREEN, OFF );
#endif
    		Alarm( OFF );

   			Motor_Disengaged_For_Spool( TRUE );
   			Motor_Engage_For_Drive( FALSE );
   			/* From interrupt */
   			if ( kill )
   			{
#ifdef UART
   	    		sprintf( buffer, "FAST/SLOW/REVERSE => Killed\n\r" );
   	        	serial_write_str( buffer );
#endif
   				while( Check_Switchs() )
   				{
   					sleepms( 20 );
   		    		Alarm( ON );
   					sleepms( 20 );
   		    		Alarm( OFF );
   				}
   			}
#ifdef UART
	    	sprintf( buffer, "FAST/SLOW/REVERSE => Done\n\r" );
	        serial_write_str( buffer );
#endif
	        /* Prevent Spool kick */
			if ( result == REEL_REVERSE )
			{
		    	adc_result = ReadADC(1);
		    	while ( adc_result >= 250 )
		    	{
		    		sleepms(100);
		    		adc_result = ReadADC(1);
		    	}
			}
			else
			{
   				Mandatory_Wait( 1 );
			}
// TODO: might stop motor, wait then release to avoid tripping spool detect
    	}
    	/* Initial spool detect - blocking ADEC read */
    	adc_result = ReadADC(1);
    	if( adc_result >= 250 )
    	{
#ifndef LCD
    		Led( GREEN, ON );
#endif
#ifdef UART
	    	sprintf( buffer, "SPOOL pull result %i\n\r", adc_result );
	        serial_write_str( buffer );
#endif
	        for( n=11 ; n ; n-- )
	        {
        		x = spool_table_ADC_read_at_spool[ n ];
	        	if( x > adc_result )
	        	{
	        		continue;
	        	}
	        	else break;
	        }
#ifdef UART
    		sprintf( buffer, "Motor on - SPOOL Table %i-  N was %i\n\r", x, n );
    		serial_write_str( buffer );
#endif
    		if( boot_mode )
    		{
    			Alarm( ON );
    		}
    		Motor_Engage_For_Drive( TRUE );
    		Motor_Disengaged_For_Spool( FALSE );

    		switch( n )
    		{

    		case 0 :
        		x = spool_table_PWM[ n ];
        		time_on = 100;
        		time_off = 500;
    			break;
    		case 1 :
        		x = spool_table_PWM[ n ];
        		time_on = 100;
        		time_off = 300;
    			break;
    		case 2 :
        		x = spool_table_PWM[ n ];
        		time_on = 100;
        		time_off = 300;
    			break;
    		case 3 :
        		x = spool_table_PWM[ n ];
        		time_on = 100;
        		time_off = 300;
    			break;
    		case 4 :
        		x = spool_table_PWM[ n ];
        		time_on = 100;
        		time_off = 300;
    			break;
    		case 5 :
        		x = spool_table_PWM[ n ];
        		time_on = 100;
        		time_off = 300;
    			break;
    		case 6 :
        		x = spool_table_PWM[ n ];
        		time_on = 100;
        		time_off = 300;
    			break;
    		case 7 :
        		x = spool_table_PWM[ n ];
        		time_on = 100;
        		time_off = 300;
    			break;
    		case 8 :
        		x = spool_table_PWM[ n ];
        		time_on = 100;
        		time_off = 300;
    			break;
    		case 9 :
        		x = spool_table_PWM[ n ];
        		time_on = 100;
        		time_off = 300;
    			break;
    		case 10 :
        		x = spool_table_PWM[ n ];
        		time_on = 100;
        		time_off = 300;
    			break;
    		case 11 :
    			x = spool_table_PWM[ n ];
        		time_on = 100;
        		time_off = 300;
    			break;
    		}

			PWM_Start( x , SPOOL );
			sleepms( time_on );
   			Motor_Disengaged_For_Spool( TRUE );
   			Motor_Engage_For_Drive( FALSE );
   			Alarm( OFF );
#ifndef LCD
   			Led( GREEN, OFF );
#endif
   			sleepms( time_off );
    	}
    	else
    	{
    		PWM_Stop();
    		pulled = FALSE;

#ifdef LCD
    		if( tick == 0 )
    		{
        		adc_result_copy = ReadADC(6);
        		Display_Battery_Voltage( adc_result_copy, 0 );
				tick = 10000;
    		}
#endif
    	}
    	tick--;

    }/* End of while(1) */
}

/* eof */
