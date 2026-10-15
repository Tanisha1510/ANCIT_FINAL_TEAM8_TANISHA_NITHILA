
#include "sdk_project_config.h"
#include "Nvm.h"
#include "EEPROM.h"
#include "OLED_Display.h"
#include "Nvm_Test.h"
#include "Oled_asci.h"

#include <stdint.h>
#include <string.h>

volatile uint8_t deferredResult  = 0U;
volatile uint8_t immediateResult = 0U;
volatile uint8_t deferredRequest0 = 0U;
volatile uint8_t deferredRequest1 = 0U;
volatile uint8_t immediateRequest = 0U;
volatile uint8_t activeTest  = 0U;
volatile uint8_t activeBlock = 0U;
volatile uint8_t activeMode  = 0U;
volatile uint8_t block0Address = 0x10U;
volatile uint8_t block1Address = 0x20U;
volatile uint8_t block2Address = 0x30U;

uint8_t deferredData0[NVM_BLOCK_SIZE] = "QUEUE001";
uint8_t deferredData1[NVM_BLOCK_SIZE] = "QUEUE002";
uint8_t immediateData[NVM_BLOCK_SIZE] = "IMMED001";

uint8_t deferredRead0[NVM_BLOCK_SIZE] = {0U};
uint8_t deferredRead1[NVM_BLOCK_SIZE] = {0U};
uint8_t immediateRead[NVM_BLOCK_SIZE] = {0U};

#define NVM_TEST_TIMEOUT 1000000U

void Nvm_RunDeferredTest(void)
{
    uint32_t timeout = 0U;
    activeTest = 1U;
    activeMode = (uint8_t)DEFERRED;
    activeBlock = 0U;
    deferredResult = 0U;
    deferredRequest0 = 0U;
    deferredRequest1 = 0U;
    memset(deferredRead0, 0, NVM_BLOCK_SIZE);
    memset(deferredRead1, 0, NVM_BLOCK_SIZE);

    if ((NvM_SetBlockMode(0U, DEFERRED) == 0U) ||(NvM_SetBlockMode(1U, DEFERRED) == 0U))
    {
        OLED_DisplayWriteStatus(0U, DEFERRED, NVM_STATUS_ERROR);
        return;
    }

    OLED_DisplayWriteStatus(0U, DEFERRED, NVM_STATUS_PENDING);
    deferredRequest0 = NvM_WriteBlock(0U, deferredData0);
    deferredRequest1 = NvM_WriteBlock(1U, deferredData1);
    if ((deferredRequest0 == 0U) || (deferredRequest1 == 0U))
    {
        OLED_DisplayWriteStatus(0U, DEFERRED, NVM_STATUS_ERROR);
        return;
    }
    while ((NvM_GetBlockStatus(0U) != NVM_STATUS_DONE) ||(NvM_GetBlockStatus(1U) != NVM_STATUS_DONE))
    {
        NvM_MainFunction();
        OLED_DisplayState(NvM_GetState());
        if ((NvM_GetBlockStatus(0U) == NVM_STATUS_ERROR) ||
            (NvM_GetBlockStatus(1U) == NVM_STATUS_ERROR))
        {
            OLED_DisplayWriteStatus(activeBlock, DEFERRED,NVM_STATUS_ERROR);
            return;
        }
        if (++timeout >= NVM_TEST_TIMEOUT)
        {
            OLED_DisplayWriteStatus(activeBlock, DEFERRED,NVM_STATUS_ERROR);
            return;
        }
    }
    EEPROM_Read(block0Address, deferredRead0, NVM_BLOCK_SIZE);
    EEPROM_Read(block1Address, deferredRead1, NVM_BLOCK_SIZE);
    if ((memcmp(deferredData0, deferredRead0, NVM_BLOCK_SIZE) == 0) && (memcmp(deferredData1, deferredRead1, NVM_BLOCK_SIZE) == 0))
    {
        deferredResult = 1U;
        OLED_DisplayWriteStatus(0U, DEFERRED, NVM_STATUS_DONE);
        OLED_DisplayComplete(0U);
    }
    else
        OLED_DisplayWriteStatus(0U, DEFERRED, NVM_STATUS_ERROR);
}
void Nvm_RunImmediateTest(void)
{
    uint32_t timeout = 0U;
    activeTest = 2U;
    activeMode = (uint8_t)IMMEDIATE;
    activeBlock = 2U;
    immediateResult = 0U;
    immediateRequest = 0U;
    memset(immediateRead, 0, NVM_BLOCK_SIZE);
    if (NvM_SetBlockMode(2U, IMMEDIATE) == 0U)
    {
        OLED_DisplayWriteStatus(2U, IMMEDIATE, NVM_STATUS_ERROR);
        return;
    }
    OLED_DisplayWriteStatus(2U, IMMEDIATE, NVM_STATUS_PENDING);
    immediateRequest = NvM_WriteBlock(2U, immediateData);
    if (immediateRequest == 0U)
    {
        OLED_DisplayWriteStatus(2U, IMMEDIATE, NVM_STATUS_ERROR);
        return;
    }
    while (NvM_GetBlockStatus(2U) != NVM_STATUS_DONE)
    {
        NvM_MainFunction();
        OLED_DisplayState(NvM_GetState());
        if (NvM_GetBlockStatus(2U) == NVM_STATUS_ERROR)
        {
            OLED_DisplayWriteStatus(2U, IMMEDIATE, NVM_STATUS_ERROR);
            return;
        }
        if (++timeout >= NVM_TEST_TIMEOUT)
        {
            OLED_DisplayWriteStatus(2U, IMMEDIATE, NVM_STATUS_ERROR);
            return;
        }
    }
    EEPROM_Read(block2Address, immediateRead, NVM_BLOCK_SIZE);
    if (memcmp(immediateData, immediateRead, NVM_BLOCK_SIZE) == 0)
    {
        immediateResult = 1U;
        OLED_DisplayWriteStatus(2U, IMMEDIATE, NVM_STATUS_DONE);
        OLED_DisplayComplete(2U);
    }
    else
        OLED_DisplayWriteStatus(2U, IMMEDIATE, NVM_STATUS_ERROR);
}
