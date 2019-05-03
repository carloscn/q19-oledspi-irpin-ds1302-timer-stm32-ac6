/***
 *
 *                                ,%%%%%%%%,
 *                              ,%%/\%%%%/\%%
 *                             ,%%%\c "" J/%%%
 *                    %.       %%%%/ o  o \%%%
 *                     `%%.     %%%%    _  |%%%
 *                     `%%     `%%%%(__Y__)%%'
 *                     //       ;%%%%`\-/%%%'
 *                    ((       /  `%%%%%%%'
 *                     \\    .'          |
 *                      \\  /       \  | |
 *                       \\/         ) | |
 *                        \         /_ | |__
 *                        (___________))))))) Carlos Lopez. (Wei Haochen)
***/
// STM32   Project.                                            			GitLab
/****************************************************************************/
/*                                                                          */
/*  @file       : uart.h	                     	                        */
/*  @Copyright  : Powered by MULTIBEANS ORG rights reserved.                */
/*  @Revision   : Ver 1.0.                                                  */
/*  @Data       : 2017.09.25 Release.                                       */
/*  @Belong     : EDU class.                                                */
/*  @Git        :        */
/*  **code : (GB2312/GBK) in Windows 10 x64. Keil   Vision4 platform.	    */
/****************************************************************************/
/*  @Attention:                                                             */
/*  ---------------------------------------------------------------------   */
/*  |    Data    |  Behavior |     Offer      |          Content         |  */
/*  | 2017.09.25 |   create  |Carlos Lopez(M) | ceate the document.      |  */
/*  ---------------------------------------------------------------------   */
/*  Email: carlos@mltbns.top                                  MULTIBEANS.   */
/****************************************************************************/

#ifndef _UART_H
#define _UART_H


#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"

#define							UART_RECV_BUFFER_MAX			256

enum uart_id_t {

    UART_ID_0,UART_ID_1,UART_ID_2,UART_ID_3, UART_ID_4, UART_ID_5

};

struct uart_hw_t{
	
	uint16	rx_pin;
	GPIO_TypeDef	*rx_port;
	uint16	tx_pin;
	GPIO_TypeDef	*tx_port;
	
	uint32			io_clk;
	
} ;

struct uart_config_t {
	

	USART_TypeDef *usart_num;
	enum uart_id_t uart_id;
	uint32	baud_rate;
	uint16	word_length;
	uint16	stop_bits;
	uint16 	parity;
	uint16	hardware_flow_ctrl;
	uint16 	mode;
	
};

typedef struct	uart_t	{

	struct uart_t 			*self;
	struct uart_hw_t 		hw;
	struct uart_config_t 	uart_cfg;
	uint16 uart_rx_count;
	BYTE uart_buffer[UART_RECV_BUFFER_MAX];
	
	void (*init)			(struct uart_t *self);
	void (*write_string)	(struct uart_t *self, BYTE *bytes, int length );
	void (*write_byte)		(struct uart_t *self, BYTE byte);
	void (*read_byte)		(struct uart_t *self, BYTE *byte );
	void (*pin_set)			(struct uart_t *self);
	//void (*rx_interrupt)	(struct uart_t *self);
	void (*clear_buffer)	(struct uart_t *self);
	void (*fetch_data)      (struct uart_t *self, int length );
	
} UART_T;

extern void    UART1_INIT( UART_T* self, uint32 baud );
extern void    UART2_INIT( UART_T* self, uint32 baud );
extern void    UART3_INIT( UART_T* self , uint32 baud);

void		uart_clear_buffer		( struct uart_t *self );
void 	    uart_write_string		( struct uart_t *self, BYTE *bytes, int lenght );
void		uart_write_byte			( struct uart_t *self, BYTE byte );
void		uart_init				( struct uart_t *self );
void 	    uart_pin_set			( struct uart_t *self );
void        uart_fetch_data         ( struct uart_t *self,  int );


void USART3_IRQHandler( void );
void USART2_IRQHandler( void );
void USART1_IRQHandler( void );

#endif
