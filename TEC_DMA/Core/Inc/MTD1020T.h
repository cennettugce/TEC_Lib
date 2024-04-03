/*
 * MTD1020T.h
 *
 *  Created on: Feb 13, 2024
 *      Author: Cennet Tugce Turan
 */

#ifndef INC_MTD1020T_H_
#define INC_MTD1020T_H_

void MTD_Init (void);
void MTD_Enable (void);
void MTD_Disable (void);
uint8_t MTD_ReadParam (uint8_t *t_data, uint16_t t_Size , uint16_t r_Size );
uint8_t MTD_WriteParam(uint8_t *r_data, uint16_t r_Size);
uint8_t checkError(void);
void MTD_Reset (void);
#endif /* INC_MTD1020T_H_ */
