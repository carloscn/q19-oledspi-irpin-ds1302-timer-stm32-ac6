
#include "global.h"
#include "oledfont.h"

//              ----------------------------------------------------------------
//              GND    电源地

//              VCC  接5V或3.3v电源
//              D0   接GPIOA0（SCL）
//              D1   接GPIOA1（SDA）
//              RES  接GPIOA2
//              DC   接GPIOA3
//              CS   接GPIOA4
//              ----------------------------------------------------------------


//向SSD1306写入一个字节。
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;
void	oled_wr_byte(OLED_SPI *self, u8 dat, u8 cmd) {

    u8 i;
    if(cmd)
        OLED_DC_HIGH( self );
    else
        OLED_DC_LOW( self );
    OLED_CS_LOW( self );
    for (i = 0; i < 8; ++i) {
        OLED_SCLK_LOW( self );
        if (dat&0x80) {
            OLED_SDA_HIGH( self );
        } else {
            OLED_SDA_LOW( self );
        }
        OLED_SCLK_HIGH( self );
        dat <<= 1;
    }
    OLED_CS_HIGH( self );
    OLED_DC_HIGH( self );
}

void	oled_set_pos( OLED_SPI *self, unsigned char x, unsigned char y) {

    self->writebyte( self, 0xb0 + y, OLED_CMD);
    self->writebyte( self, ( (x & 0xf0) >> 4) | 0x10, OLED_CMD );
    self->writebyte( self, ( (x & 0x0f) | 0x01), OLED_CMD);
}

void	oled_display_on( OLED_SPI *self ) {

    self->writebyte( self, 0x8d, OLED_CMD );
    self->writebyte( self, 0x14, OLED_CMD );
    self->writebyte( self, 0xaf, OLED_CMD );
}

void	oled_display_off( OLED_SPI *self ) {

    self->writebyte( self, 0x8d, OLED_CMD );
    self->writebyte( self, 0x10, OLED_CMD );
    self->writebyte( self, 0xae, OLED_CMD );
}

void	oled_clear( OLED_SPI *self ) {

    u8 i, n;
    for (i = 0; i < 8; ++i) {
        self->writebyte( self, 0xb0 + i, OLED_CMD );
        self->writebyte( self, 0x00, OLED_CMD );
        self->writebyte( self, 0x10, OLED_CMD );
        for (n = 0; n < 128; ++n) {
            self->writebyte( self, 0, OLED_DATA);
        }//Update Display
    }
}

void	oled_showchar( OLED_SPI *self, u8 x, u8 y, u8 chr) {

    unsigned char c = 0, i = 0;
    c = chr - ' ';//得到偏移后的值
    if ( x > Max_Column - 1 ) {	x = 0; y = y + 2; }
    if ( SIZE == 16 ) {
        self->setpos( self, x, y );
        for (i = 0; i < 8; ++i) {
            self->writebyte( self, F8X16[c*16+i], OLED_DATA );
        }
        self->setpos( self, x, y + 1);
        for (i = 0; i < 8; ++i) {
            self->writebyte( self, F8X16[c*16+i+8],OLED_DATA );
        }
    } else {
        self->setpos( self, x, y+1 );
        for (i = 0; i < 6; ++i) {
            self->writebyte( self, F6x8[c][i],OLED_DATA );
        }
    }
}

//m^n函数
u32		oled_pow( OLED_SPI *self, u8 m, u8 n ) {

    u32 result = 1;
    while( n-- ) result *= m;
    return result;
}

//显示2个数字
//x,y :起点坐标
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);
void	oled_shownum( OLED_SPI *self, u8 x,u8 y,u32 num,u8 len,u8 size2 ) {

    u8 t, temp;
    u8 enshow = 0;
    for (t = 0; t < len; ++t) {
        temp = ( num/oled_pow( self, 10, len-t-1 ) ) % 10;
        if ( enshow == 0 && t < ( len - 1 )) {
            if ( temp == 0 ) {
                self->showchar( self, x+(size2/2)*t, y, ' ' );
                continue;
            } else {
                enshow = 1;
            }
        }
        self->showchar( self, x+(size2/2)*t, y, temp + '0');
    }
}

//显示一个字符号串
void	oled_showstring( OLED_SPI *self, u8 x, u8 y, u8 *chr ) {

    unsigned char j = 0;
    while( chr[j] != '\0' ) {
        self->showchar( self, x, y, chr[j] );
        x += 8;
        if( x > 120 ) {	x = 0; y += 2;	}
        j++;
    }
}

//显示汉字
void	oled_showchinese( OLED_SPI *self, u8 x, u8 y, u8 no) {

    u8 t, adder = 0;
    self->setpos( self, x, y );
    for (t = 0; t < 16; ++t) {
        self->writebyte( self, Hzk[2*no][t],OLED_DATA );
        adder += 1;
    }
    self->setpos( self, x, y+1);
    for(t = 0; t < 16; t++) {
        self->writebyte( self, Hzk[2*no+1][t],OLED_DATA );
        adder+=1;
    }
}

/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void oled_drawbmp( OLED_SPI *self, unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[] ) {

    unsigned int j=0;
    unsigned char x,y;

    if(y1 % 8 == 0) y = y1 / 8;
    else y = y1 / 8 + 1;
    for (y = y0; y < y1; ++y) {
        self->setpos( self, x0, y );
        for (x = x0; x < x1; ++x) {
            self->writebyte( self, BMP[j++],OLED_DATA );
        }
    }
}

void oled_init( OLED_SPI *self ) {


    self->pin_set( self );

    DELAY_MS(100);
    OLED_RST_HIGH( self );
    DELAY_MS(100);
    OLED_RST_LOW( self );
    DELAY_MS(100);
    OLED_RST_HIGH( self );
    self->clear( self );
    self->writebyte( self, 0xAE, OLED_CMD);//--turn off oled panel
    self->writebyte( self, 0x00, OLED_CMD);//---set low column address
    self->writebyte( self, 0x10, OLED_CMD);//---set high column address
    self->writebyte( self, 0x40, OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    self->writebyte( self, 0x81, OLED_CMD);//--set contrast control register
    self->writebyte( self, 0xCF, OLED_CMD); // Set SEG Output Current Brightness
    self->writebyte( self, 0xA1, OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    self->writebyte( self, 0xC8, OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
    self->writebyte( self, 0xA6, OLED_CMD);//--set normal display
    self->writebyte( self, 0xA8, OLED_CMD);//--set multiplex ratio(1 to 64)
    self->writebyte( self, 0x3f, OLED_CMD);//--1/64 duty
    self->writebyte( self, 0xD3, OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
    self->writebyte( self, 0x00, OLED_CMD);//-not offset
    self->writebyte( self, 0xd5, OLED_CMD);//--set display clock divide ratio/oscillator frequency
    self->writebyte( self, 0x80, OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
    self->writebyte( self, 0xD9, OLED_CMD);//--set pre-charge period
    self->writebyte( self, 0xF1, OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    self->writebyte( self, 0xDA, OLED_CMD);//--set com pins hardware configuration
    self->writebyte( self, 0x12, OLED_CMD);
    self->writebyte( self, 0xDB, OLED_CMD);//--set vcomh
    self->writebyte( self, 0x40, OLED_CMD);//Set VCOM Deselect Level
    self->writebyte( self, 0x20, OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
    self->writebyte( self, 0x02, OLED_CMD);//
    self->writebyte( self, 0x8D, OLED_CMD);//--set Charge Pump enable/disable
    self->writebyte( self, 0x14, OLED_CMD);//--set(0x10) disable

    self->writebyte( self, 0xA4, OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
    self->writebyte( self, 0xA6, OLED_CMD);// Disable Inverse Display On (0xa6/a7)
    self->writebyte( self, 0xAF, OLED_CMD);//--turn on oled panel
    self->clear( self );
    self->setpos( self, 0, 0 );

    DELAY_MS(10);

}

void    oled_pin_set( OLED_SPI *this )
{
    GPIO_InitTypeDef io_cfg;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

    io_cfg.GPIO_Pin     =   this->hw.sck_pin | this->hw.sda_pin | this->hw.rst_pin | this->hw.cs_pin  |  this->hw.dc_pin;
    io_cfg.GPIO_Mode    =   GPIO_Mode_Out_PP;
    io_cfg.GPIO_Speed   =   GPIO_Speed_50MHz;
    GPIO_Init(this->hw.sck_port, &io_cfg );

}
/*
 * OLED 接线：
 * --------------------------------
 * 1. SCK   -    PB14
 * 2. SDA   -    PB15
 * 3. RES   -    PB12
 * 4. DC    -    PB13
 * 5. CS    -    PB10
 *
 **/
void	OLED_INIT( OLED_SPI *p_dev ) {

    p_dev->init = 	&oled_init;
    p_dev->clear = 	&oled_clear;
    p_dev->displayoff = 	&oled_display_off;
    p_dev->displayon =		&oled_display_on;
    p_dev->drawbmp =		&oled_drawbmp;
    p_dev->pin_set =        &oled_pin_set;
    p_dev->setpos = 		&oled_set_pos;
    p_dev->showchar = 		&oled_showchar;
    p_dev->showchinese = 	&oled_showchinese;
    p_dev->shownum = 		&oled_shownum;
    p_dev->showstring = 	&oled_showstring;
    p_dev->writebyte = 		&oled_wr_byte;

    p_dev->hw.dc_pin    =   GPIO_Pin_13;
    p_dev->hw.sck_pin   =   GPIO_Pin_14;
    p_dev->hw.sda_pin   =   GPIO_Pin_15;
    p_dev->hw.rst_pin   =   GPIO_Pin_12;
    p_dev->hw.cs_pin    =   GPIO_Pin_10;

    p_dev->hw.cs_port = p_dev->hw.dc_port = \
            p_dev->hw.rst_port = p_dev->hw.sda_port = \
            p_dev->hw.sck_port = GPIOB;

    p_dev->hw.cs_periph_clk = p_dev->hw.dc_periph_clk = \
            p_dev->hw.rst_periph_clk = p_dev->hw.sda_periph_clk = \
            p_dev->hw.sck_periph_clk = GPIO_Speed_50MHz;

    p_dev->init(p_dev);

}















