

#include "main.h"
#include "max7219.h"
#include "snake.h"

extern SPI_HandleTypeDef  hspi3;

void MAX7219_SendData(uint8_t Addr, uint8_t data)
{
	uint16_t writeData = (Addr<<8)|data;
	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, 0);               //enable slave
	HAL_SPI_Transmit(&hspi3, (uint8_t *)&writeData, 1, 100);
	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, 1); 			  //disable slave
}


void matrixInit(void)
{
	// According to the max7219 datasheet
	MAX7219_SendData(0x09, 0); 			// no decoding
	MAX7219_SendData(0x0A, 0x01); 		// 3/32 intensity
	MAX7219_SendData(0x0B, 0x07); 		// scan all 7 columns
	MAX7219_SendData(0x0C, 0x01); 		// normal operation
	MAX7219_SendData(0x0F, 0); 			// no display test
}


