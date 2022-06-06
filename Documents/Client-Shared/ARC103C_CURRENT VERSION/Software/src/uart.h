/**************************************************************************************************
 * \file			uart.h
 * \description		
 *
 * \date			
 * \version			1.0.1
 * \brief           
 *
 *************************************************************************************************/

#ifndef UART_H_
#define UART_H_

#if DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif


/* 4.9152   38400 ubbr = 7 */
#define USART_BAUDRATE_38400 38400
#define UBRR_VALUE_38400 (((F_CPU / (USART_BAUDRATE_38400 * 16UL))) - 1)

void USART1SendByte(uint8_t u8Data);
void USART_Init(unsigned int ubrr);
void USARTxEnableInterrupts( void );
void USART_Transmit(unsigned char data );
void serial_write_str(const char* str);

#endif /* UART_H_ */
