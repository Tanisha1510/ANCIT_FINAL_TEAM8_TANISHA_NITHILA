/*
 * Oled_asci.h
 *
 *  Created on: 13-Oct-2026
 *      Author: acer
 */


#ifndef OLED_ASCI_H_
#define OLED_ASCI_H_

#include <stdint.h>

void OLED_Init(void);
void OLED_Clear(void);
void OLED_Write(uint8_t mode, uint8_t byte);
void OLED_PutString(const char *str, uint8_t page, uint8_t col);


#endif /* OLED_ASCI_H_ */
