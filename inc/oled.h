/*
 * oled_v2.h
 *
 *  Created on: 2018年4月14日
 *      Author: Davis
 */

#ifndef INCLUDE_OLED_H_
#define INCLUDE_OLED_H_

#include "stm32f10x_gpio.h"
#include "type.h"
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据
#define OLED_MODE 0

#define             OLED_SCLK_LOW(self)               (GPIO_ResetBits(self->hw.sck_port,self->hw.sck_pin))
#define             OLED_SCLK_HIGH(self)               (GPIO_SetBits(self->hw.sck_port,self->hw.sck_pin))

#define             OLED_SDA_LOW(self)                 (GPIO_ResetBits(self->hw.sda_port,self->hw.sda_pin))
#define             OLED_SDA_HIGH(self)                (GPIO_SetBits(self->hw.sda_port,self->hw.sda_pin))

#define             OLED_RST_LOW(self)                 (GPIO_ResetBits(self->hw.rst_port,self->hw.rst_pin))
#define             OLED_RST_HIGH(self)                (GPIO_SetBits(self->hw.rst_port,self->hw.rst_pin))

#define             OLED_DC_LOW(self)                  (GPIO_ResetBits(self->hw.dc_port,self->hw.dc_pin))
#define             OLED_DC_HIGH(self)                 (GPIO_SetBits(self->hw.dc_port,self->hw.dc_pin))

#define             OLED_CS_LOW(self)                  (GPIO_ResetBits(self->hw.cs_port,self->hw.cs_pin))
#define             OLED_CS_HIGH(self)                 (GPIO_SetBits(self->hw.cs_port,self->hw.cs_pin))



//OLED模式设置
//0:4线串行模式
//1:并行8080模式

#define SIZE 16
#define XLevelL		0x02
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF
#define X_WIDTH 	128
#define Y_WIDTH 	64
//-----------------OLED端口定义----------------

struct	oled_hw_t{

    uint16  sck_pin;
    GPIO_TypeDef *sck_port;
    uint32  sck_periph_clk;

    uint16  sda_pin;
    GPIO_TypeDef *sda_port;
    uint32  sda_periph_clk;

    uint16  rst_pin;
    GPIO_TypeDef *rst_port;
    uint32  rst_periph_clk;

    uint16  dc_pin;
    GPIO_TypeDef *dc_port;
    uint32  dc_periph_clk;

    uint16  cs_pin;
    GPIO_TypeDef *cs_port;
    uint32  cs_periph_clk;
};

typedef struct oled_t{

	struct oled_hw_t hw;
	struct oled_t	*self;

	void	(*writebyte)( struct oled_t *self, u8 dat, u8 cmd);
	void    (*pin_set)( struct oled_t *self );
	void	(*init)( struct oled_t *self);
	void	(*clear)( struct oled_t *self);
	void	(*displayon)( struct oled_t *self);
	void	(*displayoff)( struct oled_t *self);
	void	(*drawpoint)( struct oled_t *self, u8 x, u8 y, u8 t);
	void	(*fill)( struct oled_t *self, u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
	void	(*showchar)( struct oled_t *self, u8 x,u8 y,u8 chr);
	void	(*shownum)( struct oled_t *self, u8 x,u8 y,u32 num,u8 len,u8 size2);
	void	(*showstring)( struct oled_t *self, u8 x,u8 y, u8 *p);
	void	(*setpos)( struct oled_t *self, unsigned char x, unsigned char y);
	void	(*showchinese)( struct oled_t *self, u8 x,u8 y,u8 no);
	void	(*drawbmp)( struct oled_t *self, unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);

} OLED_SPI;

extern void  OLED_INIT( OLED_SPI *p_dev ) ;
//OLED控制用函数
extern void oled_wr_byte(struct oled_t *self, u8 dat, u8 cmd);
extern void	oled_set_pos( struct oled_t *self, unsigned char x, unsigned char y );
extern void	oled_display_on( struct oled_t *self );
extern void	oled_display_off( struct oled_t *self );
extern void	oled_clear( struct oled_t *self );
extern void	oled_showchar( struct oled_t *self, u8 x, u8 y, u8 chr);
extern u32	oled_pow( struct oled_t *self, u8 m, u8 n );
extern void	oled_shownum( struct oled_t *self, u8 x,u8 y,u32 num,u8 len,u8 size2 );
extern void	oled_showstring( struct oled_t *self, u8 x, u8 y, u8 *chr );
extern void	oled_showchinese( struct oled_t *self, u8 x, u8 y, u8 no);
extern void oled_drawbmp( struct oled_t *self, unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[] );
extern void oled_pin_set( struct oled_t *self );
extern void oled_init( struct oled_t *self );





#endif /* INCLUDE_OLED_H_ */
