/*
 * DS1302.h
 *
 *  Created on: Nov 28, 2023
 *      Author: glgan
 */

#ifndef INC_DS1302_H_
#define INC_DS1302_H_
#include <stdint.h>
#include "main.h"

#define SDA_Read 1
#define SDA_Write 0

#define SDA_PORT GPIOB
#define SDA_PIN GPIO_PIN_3

#define CE_PORT GPIOB
#define CE_PIN GPIO_PIN_5

#define SCLK_PORT GPIOB
#define SCLK_PIN GPIO_PIN_4

#define read_Sec 0x81
#define read_Min 0x83
#define read_Hour 0x85
#define REG_BURST 0xBE


void DS1302_init();
void setDat_PinMode(uint8_t mode);
void sendCmd(uint8_t cmd);
uint8_t readCmd();


#endif /* INC_DS1302_H_ */
