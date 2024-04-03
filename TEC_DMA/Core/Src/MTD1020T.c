/*
 * MTD1020T.c
 *
 *  Created on: Feb 13, 2024
 *      Author: Cennet Tugce Turan
 */
 
#include "stm32h7xx_hal.h"
#include "MTD1020T.h"
#include "string.h"
#include "stdio.h"

extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart2_rx;

#define UART huart2
#define DMA hdma_usart2_rx

#define MTD_EN_PORT   GPIOG
#define MTD_EN_PIN    GPIO_PIN_2

uint8_t version[3] = "m?\n";
uint8_t error[3] = "E?\n";
uint8_t reset[3] = "c\n";
uint8_t s_current_lim[7] = "L1000\n";
uint8_t r_current_lim[7] = "L?\n";
uint8_t r_actual_curr[7] = "A?\n";
uint8_t r_actual_vol[7] = "U?\n";
uint8_t s_set_temp[8] = "T35000\n";
uint8_t r_set_temp[7] = "T?\n";
//uint8_t r_actual_T[4] = "Te?\n";
uint8_t s_settempw[7] = "W100\n";
uint8_t r_tempw[7] = "W?\n";
uint8_t s_delay[7] = "d10\n";
uint8_t r_delay[7] = "d?\n";
uint8_t s_cycle[7] = "C86\n";
uint8_t r_cycle[7] = "C?\n";
uint8_t s_P[7] = "P2250\n";
uint8_t r_P[7] = "P?\n";
uint8_t s_I[7] = "I825\n";
uint8_t r_I[7] = "I?\n";
uint8_t s_D[7] = "D1533\n";
uint8_t r_D[7] = "D?\n";
uint8_t s_gain[7] = "G3750\n";
uint8_t r_gain[7] = "G?\n";
uint8_t s_period[7] = "O5454\n";
uint8_t r_period[7] = "O?\n";

#define RxBuf_SIZE 15
#define MainBuf_SIZE 15
uint8_t RxBuf[RxBuf_SIZE];
uint8_t MainBuf[MainBuf_SIZE];

void MTD_Init (void)
{
	memset(RxBuf, '\0', RxBuf_SIZE);
	memset(MainBuf, '\0', MainBuf_SIZE);
	MTD_Disable();

	   MTD_Reset();
	   HAL_Delay(10);
	   HAL_UART_Transmit_DMA(&huart2, s_current_lim,strlen((const char*)s_current_lim));
	   HAL_Delay(10);
	   HAL_UART_Transmit_DMA(&huart2, s_set_temp,strlen((const char*)s_set_temp));
	   HAL_Delay(10);
	   HAL_UART_Transmit_DMA(&huart2, s_settempw,strlen((const char*)s_settempw));
	   HAL_Delay(10);
	   HAL_UART_Transmit_DMA(&huart2, s_delay,strlen((const char*)s_delay));
	   HAL_Delay(10);
	   HAL_UART_Transmit_DMA(&huart2, s_cycle,strlen((const char*)s_cycle));
	   HAL_Delay(10);
	   HAL_UART_Transmit_DMA(&huart2, s_P,strlen((const char*)s_P));
	   HAL_Delay(10);
	   HAL_UART_Transmit_DMA(&huart2, s_I,strlen((const char*)s_I));
	   HAL_Delay(10);
	   HAL_UART_Transmit_DMA(&huart2, s_D,strlen((const char*)s_D));
	   HAL_Delay(10);
	   HAL_UART_Transmit_DMA(&huart2, s_gain,strlen((const char*)s_gain));
	   HAL_Delay(10);
	   HAL_UART_Transmit_DMA(&huart2, s_period,strlen((const char*)s_period));
	   HAL_Delay(10);

	 MTD_Enable();
}

void MTD_Enable (void)
{
	HAL_GPIO_WritePin(MTD_EN_PORT, MTD_EN_PIN, GPIO_PIN_RESET);
}

void MTD_Disable (void)
{
	HAL_GPIO_WritePin(MTD_EN_PORT, MTD_EN_PIN, GPIO_PIN_SET);
}

uint8_t MTD_ReadParam (uint8_t *t_data, uint16_t t_Size, uint16_t r_Size)
{
	uint8_t prevRx[RxBuf_SIZE] ;
	memcpy ((uint8_t *)prevRx, (uint8_t *)RxBuf, RxBuf_SIZE);

	HAL_UART_Transmit_DMA(&UART, t_data, t_Size);
	HAL_UARTEx_ReceiveToIdle_DMA(&huart2, RxBuf , r_Size);

	memcpy ((uint8_t *)MainBuf, (uint8_t *)RxBuf, r_Size);
	uint8_t currRx[RxBuf_SIZE];
	memcpy ((uint8_t *)currRx, (uint8_t *)RxBuf, RxBuf_SIZE);
	memset(RxBuf, '\0', RxBuf_SIZE);
	if ( prevRx == currRx){
		return 0;
	}else{
		return 1;
	}
}

uint8_t MTD_WriteParam(uint8_t *r_data, uint16_t r_Size)
{
	HAL_UART_Transmit_DMA(&UART, r_data, r_Size);
	return 1;
}

void MTD_Reset (void)
{
	HAL_UART_Transmit_DMA(&UART, reset, strlen((const char*)reset));
}

uint8_t checkError(void)
{

	MTD_ReadParam (error, strlen((const char*)error),2);

    switch ((int)RxBuf) {
        case 1:
            printf("Enable pin not set to L");
            return 16;

        case 2:
            printf("Internal temperature too high");
            return 1;

        case 4:
            printf("Thermal Latch-Up");
            return 2;

        case 8:
            printf("Cycling time too small");
            return 3;

        case 16:
            printf("No Sensor detected");
            return 4;

        case 32:
            printf("No TEC detected");
            return 5;

        case 64:
            printf("TEC mispoled");
            return 6;

        case 8192:
            printf("Value out of range");
            return 13;

        case 16384:
            printf("Invalid command");
            return 14 ;

        default:
            printf("No error");
            return 0;
    }

}
