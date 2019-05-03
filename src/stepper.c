/*
 * stepmotor.c
 *
 *  Created on: 2018Äê4ÔÂ23ÈÕ
 *      Author: weihaochen
 */


#include "global.h"




void    stepper_init( STEPPER *self )
{
    self->pin_set(self);
    self->step  =   0;
    self->dir   =   FORWARD;
    self->ver_length    =   0;
}
void    stepper_pin_set( STEPPER *self )
{
    GPIO_InitTypeDef io_cfg;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);

    io_cfg.GPIO_Pin     =  self->hw.in_a_pin;
    io_cfg.GPIO_Mode    =   GPIO_Mode_Out_PP;
    io_cfg.GPIO_Speed   =   GPIO_Speed_50MHz;
    GPIO_Init(self->hw.in_a_port, &io_cfg );

    io_cfg.GPIO_Pin     =  self->hw.in_b_pin;
    io_cfg.GPIO_Mode    =   GPIO_Mode_Out_PP;
    io_cfg.GPIO_Speed   =   GPIO_Speed_50MHz;
    GPIO_Init(self->hw.in_b_port, &io_cfg );

    io_cfg.GPIO_Pin     =  self->hw.in_c_pin;
    io_cfg.GPIO_Mode    =   GPIO_Mode_Out_PP;
    io_cfg.GPIO_Speed   =   GPIO_Speed_50MHz;
    GPIO_Init(self->hw.in_c_port, &io_cfg );

    io_cfg.GPIO_Pin     =  self->hw.in_d_pin;
    io_cfg.GPIO_Mode    =   GPIO_Mode_Out_PP;
    io_cfg.GPIO_Speed   =   GPIO_Speed_50MHz;
    GPIO_Init(self->hw.in_d_port, &io_cfg );

}
void    stepper_reset( STEPPER *self )
{

}
void    stepper_forward( STEPPER *self )
{
    if( self->step == 1 ) {
        GPIO_SetBits(self->hw.in_a_port,self->hw.in_a_pin);
        GPIO_ResetBits(self->hw.in_a_port,self->hw.in_b_pin | self->hw.in_c_pin | self->hw.in_d_pin);
    }
    if( self->step == 2 ) {
        GPIO_SetBits(self->hw.in_a_port,self->hw.in_b_pin);
        GPIO_ResetBits(self->hw.in_a_port,self->hw.in_a_pin | self->hw.in_c_pin | self->hw.in_d_pin);
    }
    if( self->step == 3 ) {
        GPIO_SetBits(self->hw.in_a_port,self->hw.in_c_pin);
        GPIO_ResetBits(self->hw.in_a_port,self->hw.in_b_pin | self->hw.in_a_pin | self->hw.in_d_pin);
    }
    if( self->step == 4 ) {
        GPIO_SetBits(self->hw.in_a_port,self->hw.in_d_pin);
        GPIO_ResetBits(self->hw.in_a_port,self->hw.in_b_pin | self->hw.in_c_pin | self->hw.in_a_pin);
    }
}
void    stepper_backward( STEPPER *self )
{
    if( self->step == 4 ) {
        GPIO_SetBits(self->hw.in_a_port,self->hw.in_a_pin);
        GPIO_ResetBits(self->hw.in_a_port,self->hw.in_b_pin | self->hw.in_c_pin | self->hw.in_d_pin);
    }
    if( self->step == 3 ) {
        GPIO_SetBits(self->hw.in_a_port,self->hw.in_b_pin);
        GPIO_ResetBits(self->hw.in_a_port,self->hw.in_a_pin | self->hw.in_c_pin | self->hw.in_d_pin);
    }
    if( self->step == 2 ) {
        GPIO_SetBits(self->hw.in_a_port,self->hw.in_c_pin);
        GPIO_ResetBits(self->hw.in_a_port,self->hw.in_b_pin | self->hw.in_a_pin | self->hw.in_d_pin);
    }
    if( self->step == 1 ) {
        GPIO_SetBits(self->hw.in_a_port,self->hw.in_d_pin);
        GPIO_ResetBits(self->hw.in_a_port,self->hw.in_b_pin | self->hw.in_c_pin | self->hw.in_a_pin);
    }
}
void    stepper_stop( STEPPER *self )
{
    GPIO_ResetBits(self->hw.in_a_port,self->hw.in_a_pin | self->hw.in_b_pin | self->hw.in_c_pin | self->hw.in_d_pin);
}

void    stepper_enable( STEPPER *self, enum stepper_direction_t dir, uint32 sec )
{
    self->dir   =   dir;
    self->ver_length    =   20 * sec * 10;
}

void    STEPPER_INIT( STEPPER *self )
{

    self->init      =   stepper_init;
    self->reset     =   stepper_reset;
    self->pin_set   =   stepper_pin_set;
    self->forward   =   stepper_forward;
    self->backward  =   stepper_backward;
    self->stop      =   stepper_stop;
    self->enable    =   stepper_enable;

    self->hw.in_a_port = self->hw.in_b_port = \
    self->hw.in_c_port = self->hw.in_d_port = GPIOE;

    self->hw.in_a_pin  =   GPIO_Pin_1;
    self->hw.in_c_pin  =   GPIO_Pin_3;
    self->hw.in_d_pin  =   GPIO_Pin_4;
    self->hw.in_b_pin  =   GPIO_Pin_2;

    self->init(self);

}
