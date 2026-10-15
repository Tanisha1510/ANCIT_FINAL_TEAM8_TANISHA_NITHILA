/*
 * OLED_Display.c
 *
 *  Created on: 15-Oct-2026
 *      Author: acer
 */
#include "OLED_Display.h"
#include "Oled_asci.h"
#include "Nvm.h"

void OLED_DisplayInit(void)
{
    OLED_Init();
    OLED_Clear();
}

void OLED_DisplayReady(void)
{
    OLED_Clear();
    OLED_PutString("Exercise 18", 0U, 0U);
    OLED_PutString("NvM Ready", 2U, 0U);
}

void OLED_DisplayWriteStatus(uint8_t blockId,uint8_t mode,uint8_t status)
{
    (void)blockId;

    OLED_Clear();
    OLED_PutString("NvM Write", 0U, 0U);
    if (mode == DEFERRED){
        OLED_PutString("Mode: DEFERRED", 2U, 0U);
        OSIF_TimeDelay(1000U);}
    else{
        OLED_PutString("Mode: IMMEDIATE", 2U, 0U);
        OSIF_TimeDelay(1000U);}
    switch (status)
    {
        case NVM_STATUS_PENDING:
            OLED_PutString("PENDING", 4U, 0U);
            OSIF_TimeDelay(1000U);
            break;
        case NVM_STATUS_BUSY:
            OLED_PutString("BUSY", 4U, 0U);
            OSIF_TimeDelay(1000U);
            break;

        case NVM_STATUS_DONE:
            OLED_PutString("DONE", 4U, 0U);
            OSIF_TimeDelay(1000U);
            break;

        default:
            OLED_PutString("ERROR", 4U, 0U);
            OSIF_TimeDelay(1000U);
            break;
    }
}

void OLED_DisplayComplete(uint8_t blockId)
{
    OLED_Clear();
    OLED_PutString("Write Complete", 0U, 0U);

    if (blockId < NVM_MAX_BLOCKS)
    {
        OLED_PutString("Block:", 2U, 0U);
        OLED_PutString((blockId == 0U) ? "0" :
                       (blockId == 1U) ? "1" : "2", 2U, 48U);
        OSIF_TimeDelay(1000U);
    }
}
void OLED_DisplayState(NvM_State state)
{
    OLED_Clear();
    OLED_PutString("NvM STATE:", 0U, 0U);
    OSIF_TimeDelay(1000U);
    switch (state)
    {
        case IDLE:
            OLED_PutString("IDLE", 2U, 0U);
            OSIF_TimeDelay(1000U);
            break;
        case WRITE_PENDING:
            OLED_PutString("WRITE PENDING", 2U, 0U);
            OSIF_TimeDelay(1000U);
            break;
        case WRITE_BUSY:
            OLED_PutString("WRITE BUSY", 2U, 0U);
            OSIF_TimeDelay(1000U);
            break;
        case DONE:
            OLED_PutString("DONE", 2U, 0U);
            OSIF_TimeDelay(1000U);
            break;
        default:
            OLED_PutString("UNKNOWN", 2U, 0U);
            OSIF_TimeDelay(1000U);
            break;
    }
}

