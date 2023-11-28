/*
 * DDS1302.c
 *
 *  Created on: Nov 28, 2023
 *      Author: glgan
 */
#include "DS1302.h"
#include "stm32l0xx_hal.h"


void DS1302_init(){
	HAL_GPIO_WritePin(CE_PORT, CE_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SCLK_PORT, SCLK_PIN, GPIO_PIN_RESET);

}

void setDat_PinMode(uint8_t mode){
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	switch(mode){
		case 1:
			  GPIO_InitStruct.Pin = GPIO_PIN_3;
			  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
			  GPIO_InitStruct.Pull = GPIO_NOPULL;
			  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
			break;
		case 0:
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
		    GPIO_InitStruct.Pin = GPIO_PIN_3;
		    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		    GPIO_InitStruct.Pull = GPIO_NOPULL;
		    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
			break;
	}
}

void sendCmd(uint8_t cmd){
	HAL_GPIO_WritePin(CE_PORT, CE_PIN, GPIO_PIN_SET);
	setDat_PinMode(SDA_Write);
	uint8_t bit_index =0;
	uint8_t state = 0;
	uint32_t time_stamp =0;
	while(bit_index<8){
		if(time_stamp<HAL_GetTick()){
			switch(state){
				case 0:
					HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, (cmd&1) ?  GPIO_PIN_SET :  GPIO_PIN_RESET);
					HAL_GPIO_WritePin(SCLK_PORT, SCLK_PIN, GPIO_PIN_SET);
					state = 1;
					break;
				case 1:
					HAL_GPIO_WritePin(SCLK_PORT, SCLK_PIN, GPIO_PIN_RESET);
					state = 0;
					bit_index++;
					cmd >>= 1;
					break;
			}
			time_stamp=HAL_GetTick()+10;
		}
	}
}

uint8_t readCmd(){
	setDat_PinMode(SDA_Read);
	uint8_t ReadData = 0;
	uint8_t bit_index =0;
	uint8_t state = 0;
	uint32_t time_stamp =0;
	while(bit_index<8){
		if(time_stamp<HAL_GetTick()){
			switch(state){
			case 0:
				if (HAL_GPIO_ReadPin(SDA_PORT, SDA_PIN) == 1) ReadData |= 0x01 << bit_index;
//				rawReadData[bit_index+1] = HAL_GPIO_ReadPin(SDA_PORT, SDA_PIN);
				HAL_GPIO_WritePin(SCLK_PORT, SCLK_PIN, GPIO_PIN_SET);
				state =1;
				break;
			case 1:
				HAL_GPIO_WritePin(SCLK_PORT, SCLK_PIN, GPIO_PIN_RESET);
				bit_index++;
				state = 0;
				break;
			}
			time_stamp=HAL_GetTick()+10;
		}
	}
	HAL_GPIO_WritePin(CE_PORT, CE_PIN, GPIO_PIN_RESET);
	ReadData = ReadData&0b01111111;
//	ReadData = bcd2dec(ReadData);
	ReadData=((ReadData/16*10)+(ReadData%16 ));
	return ReadData;
}




