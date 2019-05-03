/**
 ******************************************************************************
 * @file    main.c
 * @author  Ac6
 * @version V1.0
 * @date    01-December-2013
 * @brief   Default main function.
 ******************************************************************************
 */

/*
 * OLED 接线：
 * --------------------------------
 * 1. SCK   -    PD12
 * 2. SDA   -    PD13
 * 3. RES   -    PD10
 * 4. DC    -    PD11
 * 5. CS    -    PD08
 *
 * 步进电机 接线：
 * --------------------------------
 * 1. INT1  -    PE01
 * 2. INT2  -    PE02
 * 3. INT3  -    PE03
 * 4. INT4  -    PE04
 *
 * 实时时钟：
 * --------------------------------
 * 1. SCK   -    PC13
 * 2. DAT   -    PE06
 * 3. RST   -    PE05
 *
 * 蓝牙：
 * --------------------------------
 * UART1
 * 1. RX    -    PA9
 * 2. TX    -    PA10
 *
 * 调试串口：
 * --------------------------------
 * UART2：
 * 1. RX    -    PA2
 * 2. TX    -    PA3
 * --------------------------------
 *
 * 红外对射管：
 * 1. DAT   -    PC00
 * --------------------------------
 *
 *
 * */

#include "global.h"
#include "main.h"
//#define             SET_TIME

UART_T      bluetooth, *bluetooth_p, uartdebug, *uartdebug_p;
OLED_SPI    oled, *oled_p;
DS1302      ds1302, *ds1302_p;
STEPPER     stepper, *stepper_p;
IRPIN       irpin, *irpin_p;
uint32      timer_counter = 0;
const   BYTE    password[4] = {'2','0','1','8'};

uint16  d_hour;
uint16  d_min;
uint16  d_sec;
uint16  s_hour = 0,s_min = 0,s_sec = 0, last_sec = 0;
char time_str[40];
bool    standby_count_flag = false;
uint32  standby_count = 0;
bool    time_read_flag = true;
int main(void)
{

    bluetooth_p     =   &bluetooth;
    uartdebug_p     =   &uartdebug;
    oled_p          =   &oled;
    ds1302_p        =   &ds1302;
    stepper_p       =   &stepper;
    irpin_p         =   &irpin;

    SystemInit();
    delay_init();
    RCC_Configuration();
    OLED_INIT( oled_p );
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    DS1302_INIT( ds1302_p );
    STEPPER_INIT( stepper_p );
    IRPIN_INIT( irpin_p );

    UART1_INIT( bluetooth_p, 115200 );
    UART2_INIT( uartdebug_p, 115200 );

    TIM_SetInterval(1,4000);//50ms
    stepper_p->enable( stepper_p, FORWARD, 10 );
#ifdef SET_TIME
    ds1302_p->time.year     =   0x18;
    ds1302_p->time.month    =   0x04;
    ds1302_p->time.day      =   0x27;
    ds1302_p->time.hour     =   0x20;
    ds1302_p->time.min      =   0x30;
    ds1302_p->time.sec      =   0x00;
    ds1302_p->time.week     =   0x05;
    ds1302_p->write_time( ds1302_p );
#endif

    for(;;) {

        // refresh the irpin state; the state information save in irpin_p->is_close boolean value.
        irpin_p->read_state(irpin_p);
        ds1302_p->read_time( ds1302_p );
        if( timer_counter == 50 ) {
            oled_p->showstring( oled_p, 0,0, (u8*)ds1302_p->time.all_time  );
            if( irpin_p->is_close == true ) {

                if( standby_count_flag == false ) {

                    standby_count_flag = true;
                }
                // remember the current time;
                d_hour  =   ds1302_p->time.hour;
                d_min   =   ds1302_p->time.min;
                d_sec   =   ds1302_p->time.sec;
                oled_p->showstring( oled_p, 0,4, (u8*)time_str);
                time_read_flag = true;
            }else {
                // remember the current time;
                standby_count_flag = false;
                standby_count = 0;
                s_hour = s_min = s_sec = 0;
                oled_p->showstring( oled_p, 0,4, (u8*)("no detecked car")  );
            }
            uartdebug_p->write_string(uartdebug_p, ds1302_p->time.all_time  ,-1);
        }
    }

}

void TIM1_IRQHandler(void)   //TIM中断
{


    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET){  //检查TIM更新中断发生与否
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update  );  //清除TIMx更新中断标志


        // motor use timer source.
        if( stepper_p->ver_length > 1 ) {

            if( stepper_p->dir == FORWARD ) {
                stepper_p->step ++;
                if( stepper_p->step >= 5 ) {
                    stepper_p->step = 0;
                }
                stepper_p->forward( stepper_p );
            }else {
                stepper_p->step ++;
                if( stepper_p->step >= 5 ) {
                    stepper_p->step = 0;
                }
                stepper_p->backward( stepper_p );
            }
            stepper_p->ver_length   --;
        }else {
            stepper_p->stop( stepper_p );
        }

        // irpin use timer source
        if ( irpin_p->is_close   ==     false ) {

            // count;
            if( time_read_flag == true ) {
                irpin_p->hour   =   ds1302_p->time.hour;
                irpin_p->mini   =   ds1302_p->time.min;
                irpin_p->sec    =   ds1302_p->time.sec;
                time_read_flag = false;
            }
            // judge time;
            // if time hour is equal.

            if ( irpin_p->hour != ds1302_p->time.hour ) {

                if ( (60 - irpin_p->sec) + ds1302_p->time.sec >= 30 ) {
                    stepper_p->enable( stepper_p, BACKWARD, 10 );
                    time_read_flag = true;
                }
            }else {

                if ( irpin_p->mini != ds1302_p->time.min  ) {

                    if ( (60 - irpin_p->sec) + ds1302_p->time.sec >= 30 ) {
                        stepper_p->enable( stepper_p, BACKWARD, 10 );
                        time_read_flag = true;
                    }

                }else {

                    if( irpin_p->sec - ds1302_p->time.sec >=  30 ) {
                        stepper_p->enable( stepper_p, BACKWARD, 10 );
                        time_read_flag = true;
                    }
                }
            }

            // display standby time.
        }else {
            if( last_sec != d_sec ) {

                s_sec = standby_count ++;
                if( standby_count == 60 ) {
                    standby_count = 0;
                    s_min ++;
                    if( s_min == 60 ) {
                        s_min = 0;
                        s_hour ++;
                        if( s_hour == 60 ) {
                            s_hour = 0;
                        }
                    }
                }
                sprintf( time_str, "Count: %d:%d:%d", s_hour,s_min,s_sec );
                last_sec = d_sec;
            }


        }


        // while loop in main function, use timer source.
        timer_counter++;
        if( timer_counter > 50 ) {
            timer_counter  = 0;
        }


    }
}
uint16  rx_buffer[10];
uint16  rx_count = 0;
void    bluetooth_message_deal( BYTE byte_data )
{

    rx_buffer[rx_count ++ ] = (uint16) byte_data;

    if( rx_buffer[0] != 0xAA ) {
        rx_count = 0;
        return;
    }

    if (rx_count < 3)
        return;

    if( rx_buffer[1] == 0x11 ) {
        stepper_p->enable( stepper_p, FORWARD, 10 );
    }else if( rx_buffer[1] == 0x22 ) {
        stepper_p->enable( stepper_p, BACKWARD, 10 );
    }
    rx_count = 0;
}

void RCC_Configuration(void)
{
    /* Enable system clocks ------------------------------------------------*/
    ErrorStatus HSEStartUpStatus;
    RCC_DeInit();                                                                                   // System clock reset.
    RCC_HSEConfig( RCC_HSE_ON );                                                                    // Open the HSE clock.
    HSEStartUpStatus = RCC_WaitForHSEStartUp();                                                     // Wait for HSE clock.
#if 1
    if( HSEStartUpStatus == SUCCESS ) {                                                             // If the HSE time consuming normal.

        FLASH_PrefetchBufferCmd( FLASH_PrefetchBuffer_Enable );                                     // Enable flash reader buffer.
        FLASH_SetLatency( FLASH_Latency_2 );                                                        // Flash wait state.
        RCC_HCLKConfig( RCC_SYSCLK_Div1 );                                                          // HCLK = SYSCLK = 72.0MHz
        RCC_PCLK2Config( RCC_HCLK_Div1 );                                                           // PCLK2 = HCLK = 72.0MHz
        RCC_PCLK1Config( RCC_HCLK_Div2 );                                                           // PCLK1 = HCLK/2 = 36.0MHz

        RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);
        RCC_PLLCmd(ENABLE);                                                                         // Enable PLL

        while( RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET );                                       // Wait till PLL is ready.
        RCC_SYSCLKConfig( RCC_SYSCLKSource_PLLCLK );                                                // Select PLL as system clock source.
        while( RCC_GetSYSCLKSource() != 0x08 );                                                     // Wait till PLL is used as system clock source.
    }
    // notice :
    // If there is as "RCC_ADCCLKConfig( RCC_PCLK2_Div6 )" peripheral clock.
    // So, the ADCLK = PCLK2 / 6 = 12MHz.
#endif
    /* Enable peripheral clocks ------------------------------------------------*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD, ENABLE);
    /* Enable DMA1 and DMA2 clocks */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 | RCC_AHBPeriph_DMA2, ENABLE);
    /* Enable ADC1, ADC2, ADC3 and GPIOC clocks */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC2 |
                           RCC_APB2Periph_ADC3 | RCC_APB2Periph_GPIOC, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1   , ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOD ,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
}


