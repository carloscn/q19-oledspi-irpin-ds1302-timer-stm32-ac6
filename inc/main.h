/*
 * main.h
 *
 *  Created on: 2018��4��22��
 *      Author: weihaochen
 */

#ifndef MAIN_H_
#define MAIN_H_

void CopeSerialData(unsigned char ucData);
void RCC_Configuration(void);
extern void TIM_SetInterval(u8 TIM_Index,u32 nus);

#endif /* MAIN_H_ */
