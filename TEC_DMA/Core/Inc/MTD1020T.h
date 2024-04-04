/*
 * MTD1020T.h
 *
 *  Created on: Feb 13, 2024
 *      Author: Cennet Tugce Turan
 */

#ifndef INC_MTD1020T_H_
#define INC_MTD1020T_H_

typedef struct TEC
{
	uint8_t *m_Buf;
}TEC;

void MTD_Init (void);
void MTD_Enable (void);
void MTD_Disable (void);
uint8_t MTD_ReadParam (uint8_t *t_data, uint16_t t_Size , uint8_t *r_data, uint16_t r_Size );
uint8_t MTD_WriteParam(uint8_t *r_data, uint16_t r_Size);
void MTD_r_SetTemp(uint8_t *r_data);
void MTD_r_ActualTemp(uint8_t *r_data);
uint8_t checkError(uint8_t *r_data, uint16_t r_Size);
void MTD_Reset (void);
#endif /* INC_MTD1020T_H_ */
