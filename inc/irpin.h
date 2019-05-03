/*
 * irpin.h
 *
 *  Created on: 2018Äê4ÔÂ24ÈÕ
 *      Author: weihaochen
 */

#ifndef IRPIN_H_
#define IRPIN_H_


#include "type.h"
#include "stm32f10x_gpio.h"

//#define             IRPIN_READ(self)               (GPIO_ReadInputDataBit(self->hw.sda_port,self->hw.sda_pin))

struct  irpin_hw_t {

    uint16  state_pin;
    GPIO_TypeDef *state_port;
};

typedef struct  irpin_t {

    struct  irpin_hw_t  hw;
    struct  irpin_t *self;
    bool    is_close;
    uint32  timer_counter;
    uint16  hour;
    uint16  mini;
    uint16  sec;
    void    (*init)( struct irpin_t *this );
    void    (*reset)( struct irpin_t *this );
    void    (*pin_set)( struct irpin_t *this );

    uint8    (*read_state)( struct   irpin_t *this );

}  IRPIN;


extern  void IRPIN_INIT( IRPIN *self  );

void    irpin_init( IRPIN *this );
void    irpin_reset( IRPIN *this );
void    irpin_pin_set( IRPIN *this );
uint8   irpin_read_state( IRPIN *this );


#endif /* IRPIN_H_ */
