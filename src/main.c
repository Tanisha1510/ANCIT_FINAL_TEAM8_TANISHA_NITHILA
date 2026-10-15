/*!
** Copyright 2020 NXP
** @file main.c
** @brief
**         Main module.
**         This module contains user's application code.
*/
/*!
**  @addtogroup main_module main module documentation
**  @{
*/
/* MODULE main */
#include "sdk_project_config.h"
#include "Nvm.h"
#include "EEPROM.h"
#include "Oled_asci.h"
#include "OLED_Display.h"
#include "Nvm_Test.h"

lpi2c_master_state_t lpi2c1MasterState;

int main(void)
{
    CLOCK_DRV_Init(&clockMan1_InitConfig0);
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);
    LPI2C_DRV_MasterInit(0U, &lpi2c0_MasterConfig0, &lpi2c1MasterState);
    OLED_Init();
    OLED_Clear();
    NvM_Init();
    OLED_PutString("INITIALIZED", 2U, 0U);
    OSIF_TimeDelay(1000U);
    Nvm_RunDeferredTest();
    Nvm_RunImmediateTest();
    OLED_Clear();
    OLED_PutString("TESTS FINISHED", 0U, 0U);
}

