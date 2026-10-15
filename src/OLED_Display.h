#ifndef OLED_DISPLAY_H_
#define OLED_DISPLAY_H_

#include <stdint.h>
#include "Nvm.h"

void OLED_DisplayInit(void);
void OLED_DisplayReady(void);
void OLED_DisplayWriteStatus(uint8_t blockId,uint8_t mode,uint8_t status);
void OLED_DisplayComplete(uint8_t blockId);
void OLED_DisplayState(NvM_State state);

#endif
