/*
 * stepmotor.h
 *
 *  Created on: 2018Äê4ÔÂ23ÈÕ
 *      Author: weihaochen
 */

#ifndef STEPPER_H_
#define STEPPER_H_
#include "type.h"
#include "stm32f10x_gpio.h"


struct  stepper_hw_t {

    uint16  in_a_pin;
    GPIO_TypeDef *in_a_port;

    uint16  in_b_pin;
    GPIO_TypeDef *in_b_port;

    uint16  in_c_pin;
    GPIO_TypeDef *in_c_port;

    uint16  in_d_pin;
    GPIO_TypeDef *in_d_port;

};
enum    stepper_direction_t {
    FORWARD, BACKWARD
};
typedef struct  stepper_t {

    struct  stepper_hw_t hw;
    struct  stepper_t   *self;
    uint8   step;
    uint32  ver_length;
    enum stepper_direction_t dir;


    void    (*init)( struct stepper_t* this );
    void    (*pin_set)( struct stepper_t *this );
    void    (*reset)( struct stepper_t *this);
    void    (*forward)( struct stepper_t* this );
    void    (*backward)( struct stepper_t* this );
    void    (*stop)( struct stepper_t *this );
    void    (*enable)( struct stepper_t *self, enum stepper_direction_t dir, uint32 sec);

} STEPPER ;


extern void STEPPER_INIT( STEPPER *self );

void    stepper_init( STEPPER *self );
void    stepper_pin_set( STEPPER *self );
void    stepper_reset( STEPPER *self );
void    stepper_forward( STEPPER *self );
void    stepper_backward( STEPPER *self );
void    stepper_stop( STEPPER *self );
void    stepper_enable( STEPPER *self, enum stepper_direction_t dir, uint32 sec );
#endif /* STEPPER_H_ */
