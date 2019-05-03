/*
 * irpin.c
 *
 *  Created on: 2018Äê4ÔÂ24ÈÕ
 *      Author: weihaochen
 */


#include "global.h"

void    irpin_init( IRPIN *this )
{
    this->pin_set( this );
    this->timer_counter = 0;
    if( this->read_state(this) ) {
        this->is_close  =   false;
    }else {
        this->is_close  =   true;
    }
}
void    irpin_reset( IRPIN *this )
{

}
void    irpin_pin_set( IRPIN *this )
{
    GPIO_InitTypeDef io_cfg;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

    io_cfg.GPIO_Pin     =   this->hw.state_pin;
    io_cfg.GPIO_Mode    =   GPIO_Mode_IN_FLOATING;
    io_cfg.GPIO_Speed   =   GPIO_Speed_50MHz;
    GPIO_Init(this->hw.state_port, &io_cfg );

}
uint8    irpin_read_state( IRPIN *this )
{

    if (GPIO_ReadInputDataBit(this->hw.state_port, this->hw.state_pin) ) {

        this->is_close  =   false;
        return 1;
    } else {
        this->is_close  =   true;
        return 0;
    }

}

void    IRPIN_INIT( IRPIN *this )
{
    this->init  =   &irpin_init;
    this->reset =   &irpin_reset;
    this->pin_set   =   &irpin_pin_set;
    this->read_state    =   &irpin_read_state;

    this->hw.state_pin  =   GPIO_Pin_0;
    this->hw.state_port =   GPIOC;
    this->init(this);
}



