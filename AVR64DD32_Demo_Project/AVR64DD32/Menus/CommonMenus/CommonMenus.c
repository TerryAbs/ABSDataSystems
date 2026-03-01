/*
 * CommonMenus.c
 *
 * Created: 11/28/2025 9:23:25 AM
 *  Author: terry
 */
#include <avr/pgmspace.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "CommonMenus.h"
#include "CommandList/CommandList.h"
#include "Common/DataStructures.h"
#include "CommonStrings/CommonStrings.h"
#include "Menu_Report_Processor/Themes/Themes.h"
#include "Menus/USART/Menu_USART.h"
#include "DAC/DAC.h"
#include "Events/Events.h"
#include "Timers/Timers.h"

static struct EnterValueByHand enterValue;

// ADC Specific

static const MenuHeader ADCPreScaleHeader[] PROGMEM = {
	{  4, 20, themeRedWhite,	msg_EQUALLONG, msg_EQUAL_LEN},
	{  5, 25, themeBlueWhite,	msg_ADCPresScaleMenu, msg_ADCPresScaleMenu_LEN},
	{  6, 20, themeRedWhite,	msg_EQUALLONG, msg_EQUAL_LEN},
};

static const MenuBody ADCPreScaleBody[] PROGMEM = {
	{  8, 22, themeBlueWhite,	msg_LTR_A,		msg_LTR_A_LEN,  msg_ADC_PRESC_DIV2_gc,	NULL},
	{  9, 22, themeBlueWhite,	msg_LTR_B,		msg_LTR_B_LEN,  msg_ADC_PRESC_DIV4_gc,	NULL},
	{  10, 22, themeBlueWhite,	msg_LTR_C,		msg_LTR_C_LEN,  msg_ADC_PRESC_DIV8_gc,	NULL},
	{  11, 22, themeBlueWhite,	msg_LTR_D,		msg_LTR_D_LEN,  msg_ADC_PRESC_DIV12_gc,	NULL},
	{  12, 22, themeBlueWhite,	msg_LTR_E,		msg_LTR_E_LEN,  msg_ADC_PRESC_DIV16_gc,	NULL},
	{  13, 22,themeBlueWhite,	msg_LTR_F,		msg_LTR_F_LEN,  msg_ADC_PRESC_DIV20_gc,	NULL},
	{  14, 22,themeBlueWhite,	msg_LTR_G,		msg_LTR_G_LEN,  msg_ADC_PRESC_DIV24_gc,	NULL},
	{  15, 22,themeBlueWhite,	msg_LTR_H,		msg_LTR_H_LEN,  msg_ADC_PRESC_DIV28_gc,	NULL},
	{  16, 22,themeBlueWhite,	msg_LTR_I,		msg_LTR_I_LEN,  msg_ADC_PRESC_DIV32_gc,	NULL},
	{  17, 22,themeBlueWhite,	msg_LTR_J,		msg_LTR_J_LEN,  msg_ADC_PRESC_DIV48_gc,	NULL},
	{  18, 22,themeBlueWhite,	msg_LTR_K,		msg_LTR_K_LEN,  msg_ADC_PRESC_DIV64_gc,	NULL},
	{  19, 22,themeBlueWhite,	msg_LTR_L,		msg_LTR_L_LEN,  msg_ADC_PRESC_DIV96_gc,	NULL},
	{  20, 22,themeBlueWhite,	msg_LTR_M,		msg_LTR_M_LEN,  msg_ADC_PRESC_DIV128_gc,NULL},
	{  21, 22,themeBlueWhite,	msg_LTR_N,		msg_LTR_N_LEN,  msg_ADC_PRESC_DIV256_gc,NULL},
	{  22, 22, themeBlueWhite,	msg_LTR_X,		msg_LTR_X_LEN,	msg_ExitMenu,  NULL},
};

const DataEntry ADCPreScaleDataEntry[] PROGMEM = {
	{  8, 22, themeBlueWhite,	ADC_PRESC_DIV2_gc,	FIELDTYPE_UINT8,		NULL, NULL},
	{  9, 22, themeBlueWhite,	ADC_PRESC_DIV4_gc,	FIELDTYPE_UINT8,		NULL, NULL},
	{  10, 22, themeBlueWhite,	ADC_PRESC_DIV8_gc,	FIELDTYPE_UINT8,		NULL, NULL},
	{  11, 22, themeBlueWhite,	ADC_PRESC_DIV12_gc,	FIELDTYPE_UINT8,		NULL, NULL},
	{  12, 22, themeBlueWhite,	ADC_PRESC_DIV16_gc,	FIELDTYPE_UINT8,		NULL, NULL},
	{  13, 22, themeBlueWhite,	ADC_PRESC_DIV20_gc,	FIELDTYPE_UINT8,		NULL, NULL},
	{  14, 22, themeBlueWhite,	ADC_PRESC_DIV24_gc,	FIELDTYPE_UINT8,		NULL, NULL},
	{  15, 22, themeBlueWhite,	ADC_PRESC_DIV28_gc,	FIELDTYPE_UINT8,		NULL, NULL},
	{  16, 22, themeBlueWhite,	ADC_PRESC_DIV32_gc,	FIELDTYPE_UINT8,		NULL, NULL},
	{  17, 22, themeBlueWhite,	ADC_PRESC_DIV48_gc,	FIELDTYPE_UINT8,		NULL, NULL},
	{  18, 22, themeBlueWhite,	ADC_PRESC_DIV64_gc,	FIELDTYPE_UINT8,		NULL, NULL},
	{  19, 22, themeBlueWhite,	ADC_PRESC_DIV96_gc,	FIELDTYPE_UINT8,		NULL, NULL},
	{  20, 22, themeBlueWhite,	ADC_PRESC_DIV128_gc,FIELDTYPE_UINT8,		NULL, NULL},
	{  21, 22, themeBlueWhite,	ADC_PRESC_DIV256_gc,FIELDTYPE_UINT8,		NULL, NULL}
};

MenuSystem ADCPreScaleMainMenu = {
	.menuHeader = ADCPreScaleHeader,
	.menuHeaderSize = sizeof(ADCPreScaleHeader) / sizeof(MenuHeader),
	.menuBody = ADCPreScaleBody,
	.menuBodySize = sizeof(ADCPreScaleBody) / sizeof(MenuBody),
	.dataEntry = ADCPreScaleDataEntry,
	.dataEntrySize = sizeof(ADCPreScaleDataEntry) / sizeof(DataEntry),
	.dataptr = NULL,
	.dataEntryMode = DATAENTRYMODE_DROPDOWN,
	.screenArea = {  3, 17, 25, 50, themeBlueWhite},
	.callingMenu = NULL,
	.saveConfig = {0, NULL}
};

void ADC_PreScaleMainMenu(const char *msg)
{
	ADCPreScaleMainMenu.dataptr = GetDataPtrForMenuItem(msg);
	ApplySaveConfigFromParentMenu(&ADCPreScaleMainMenu);
	SetActiveMenu(&ADCPreScaleMainMenu);
}

static const MenuHeader ADCSampleDelayHeader[] PROGMEM = {
	{  4, 20, themeRedWhite,	msg_EQUALLONG, msg_EQUAL_LEN},
	{  5, 20, themeBlueWhite,	msg_ADCSampleDelayMenu, msg_ADCSampleDelayMenu_LEN},
	{  6, 20, themeRedWhite,	msg_EQUALLONG, msg_EQUAL_LEN},
};

static const MenuBody ADCSampleDelayBody[] PROGMEM = {
	{  8, 22, themeBlueWhite,	msg_LTR_A,		msg_LTR_A_LEN, msg_ADC_SAMPDLY_DLY0_gc,		NULL},
	{  9, 22, themeBlueWhite,	msg_LTR_B,		msg_LTR_B_LEN, msg_ADC_SAMPDLY_DLY1_gc,		NULL},
	{  10, 22, themeBlueWhite,	msg_LTR_C,		msg_LTR_C_LEN, msg_ADC_SAMPDLY_DLY2_gc,		NULL},
	{  11, 22, themeBlueWhite,	msg_LTR_D,		msg_LTR_D_LEN, msg_ADC_SAMPDLY_DLY3_gc,		NULL},
	{  12, 22, themeBlueWhite,	msg_LTR_E,		msg_LTR_E_LEN, msg_ADC_SAMPDLY_DLY4_gc,		NULL},
	{  13, 22, themeBlueWhite,	msg_LTR_F,		msg_LTR_F_LEN, msg_ADC_SAMPDLY_DLY5_gc,		NULL},
	{  14, 22, themeBlueWhite,	msg_LTR_G,		msg_LTR_G_LEN, msg_ADC_SAMPDLY_DLY6_gc,		NULL},
	{  15, 22, themeBlueWhite,	msg_LTR_H,		msg_LTR_H_LEN, msg_ADC_SAMPDLY_DLY7_gc,		NULL},
	{  16, 22, themeBlueWhite,	msg_LTR_I,		msg_LTR_I_LEN, msg_ADC_SAMPDLY_DLY8_gc,		NULL},
	{  17, 22, themeBlueWhite,	msg_LTR_J,		msg_LTR_J_LEN, msg_ADC_SAMPDLY_DLY9_gc,		NULL},
	{  18, 22, themeBlueWhite,	msg_LTR_K,		msg_LTR_K_LEN, msg_ADC_SAMPDLY_DLY10_gc,	NULL},
	{  19, 22, themeBlueWhite,	msg_LTR_L,		msg_LTR_L_LEN, msg_ADC_SAMPDLY_DLY11_gc,	NULL},
	{  20, 22, themeBlueWhite,	msg_LTR_M,		msg_LTR_M_LEN, msg_ADC_SAMPDLY_DLY12_gc,	NULL},
	{  21, 22, themeBlueWhite,	msg_LTR_N,		msg_LTR_N_LEN, msg_ADC_SAMPDLY_DLY13_gc,	NULL},
	{  22, 22, themeBlueWhite,	msg_LTR_O,		msg_LTR_N_LEN, msg_ADC_SAMPDLY_DLY14_gc,	NULL},
	{  23, 22, themeBlueWhite,	msg_LTR_P,		msg_LTR_N_LEN, msg_ADC_SAMPDLY_DLY15_gc,	NULL},
	{  24, 22, themeBlueWhite,	msg_LTR_X,		msg_LTR_X_LEN, msg_ExitMenu,  NULL},
};

static const DataEntry ADCSampleDelayDataEntry[] PROGMEM = {
	{  8, 22, themeBlueWhite,	ADC_SAMPDLY_DLY0_gc,	FIELDTYPE_UINT8,		NULL, NULL},
	{  9, 22, themeBlueWhite,	ADC_SAMPDLY_DLY1_gc,	FIELDTYPE_UINT8,		NULL, NULL},
	{  10, 22, themeBlueWhite,	ADC_SAMPDLY_DLY2_gc,	FIELDTYPE_UINT8,		NULL, NULL},
	{  11, 22, themeBlueWhite,	ADC_SAMPDLY_DLY3_gc,	FIELDTYPE_UINT8,		NULL, NULL},
	{  12, 22, themeBlueWhite,	ADC_SAMPDLY_DLY4_gc,	FIELDTYPE_UINT8,		NULL, NULL},
	{  13, 22, themeBlueWhite,	ADC_SAMPDLY_DLY5_gc,	FIELDTYPE_UINT8,		NULL, NULL},
	{  14, 22, themeBlueWhite,	ADC_SAMPDLY_DLY6_gc,	FIELDTYPE_UINT8,		NULL, NULL},
	{  15, 22, themeBlueWhite,	ADC_SAMPDLY_DLY7_gc,	FIELDTYPE_UINT8,		NULL, NULL},
	{  16, 22, themeBlueWhite,	ADC_SAMPDLY_DLY8_gc,	FIELDTYPE_UINT8,		NULL, NULL},
	{  17, 22, themeBlueWhite,	ADC_SAMPDLY_DLY9_gc,	FIELDTYPE_UINT8,		NULL, NULL},
	{  18, 22, themeBlueWhite,	ADC_SAMPDLY_DLY10_gc,	FIELDTYPE_UINT8,		NULL, NULL},
	{  19, 22, themeBlueWhite,	ADC_SAMPDLY_DLY11_gc,	FIELDTYPE_UINT8,		NULL, NULL},
	{  20, 22, themeBlueWhite,	ADC_SAMPDLY_DLY12_gc,	FIELDTYPE_UINT8,		NULL, NULL},
	{  21, 22, themeBlueWhite,	ADC_SAMPDLY_DLY13_gc,	FIELDTYPE_UINT8,		NULL, NULL},
	{  22, 22, themeBlueWhite,	ADC_SAMPDLY_DLY14_gc,	FIELDTYPE_UINT8,		NULL, NULL},
	{  23, 22, themeBlueWhite,	ADC_SAMPDLY_DLY15_gc,	FIELDTYPE_UINT8,		NULL, NULL},
};

MenuSystem ADCSampleDelayMainMenu = {
	.menuHeader = ADCSampleDelayHeader,
	.menuHeaderSize = sizeof(ADCSampleDelayHeader) / sizeof(MenuHeader),
	.menuBody = ADCSampleDelayBody,
	.menuBodySize = sizeof(ADCSampleDelayBody) / sizeof(MenuBody),
	.dataEntry = ADCSampleDelayDataEntry,
	.dataEntrySize = sizeof(ADCSampleDelayDataEntry) / sizeof(DataEntry),
	.dataptr = NULL,
	.dataEntryMode = DATAENTRYMODE_DROPDOWN,
	.screenArea = {  3, 17, 27, 50, themeBlueWhite},
	.callingMenu = NULL,
	.saveConfig = {0, NULL}
};

void ADC_SampleDelayMainMenu(const char *msg)
{
	ADCSampleDelayMainMenu.dataptr = GetDataPtrForMenuItem(msg);
	ApplySaveConfigFromParentMenu(&ADCSampleDelayMainMenu);
	SetActiveMenu(&ADCSampleDelayMainMenu);
}

static const MenuHeader ADCDataBitsHeader[] PROGMEM = {
	{  4, 20, themeRedWhite,	msg_EQUALLONG, msg_EQUAL_LEN},
	{  5, 20, themeBlueWhite,	msg_ADC_DataBitsMenu, msg_ADC_DataBitsMenu_LEN},
	{  6, 20, themeRedWhite,	msg_EQUALLONG, msg_EQUAL_LEN},
};

static const MenuBody ADCDataBitsBody[] PROGMEM = {
	{  9, 22, themeBlueWhite,	msg_LTR_A,		msg_LTR_A_LEN, msg_ADC_RESSEL_10BIT_gc,	 NULL},
	{  10, 22, themeBlueWhite,	msg_LTR_B,		msg_LTR_B_LEN, msg_ADC_RESSEL_12BIT_gc,  NULL},
	{  11, 22, themeBlueWhite,	msg_LTR_X,		msg_LTR_X_LEN, msg_ExitMenu,  NULL},
};

static const DataEntry ADCDataBitsDataEntry[] PROGMEM = {
	{  9, 22, themeRedBlack,	ADC_RESSEL_10BIT_gc,	FIELDTYPE_UINT8,		NULL, NULL},
	{  10, 22, themeRedBlack,	ADC_RESSEL_12BIT_gc, 	FIELDTYPE_UINT8,		NULL, NULL},
};

MenuSystem ADCDataBitsMainMenu = {
	.menuHeader = ADCDataBitsHeader,
	.menuHeaderSize = sizeof(ADCDataBitsHeader) / sizeof(MenuHeader),
	.menuBody = ADCDataBitsBody,
	.menuBodySize = sizeof(ADCDataBitsBody) / sizeof(MenuBody),
	.dataEntry = ADCDataBitsDataEntry,
	.dataEntrySize = sizeof(ADCDataBitsDataEntry) / sizeof(DataEntry),
	.dataptr = NULL,
	.dataEntryMode = DATAENTRYMODE_DROPDOWN,
	.screenArea = {  3, 17, 14, 50, themeBlueWhite},
	.callingMenu = NULL,
	.saveConfig = {0, NULL}
};

void ADC_DataBitsMainMenu(const char *msg)
{
	ADCDataBitsMainMenu.dataptr = GetDataPtrForMenuItem(msg);
	ApplySaveConfigFromParentMenu(&ADCDataBitsMainMenu);
	SetActiveMenu(&ADCDataBitsMainMenu);
}

static const MenuHeader ADCSampleNumberHeader[] PROGMEM = {
	{  4, 20, themeRedWhite,	msg_EQUALLONG, msg_EQUAL_LEN},
	{  5, 20, themeBlueWhite,	msg_ADCSampleNumberMenu, msg_ADCSampleNumberMenu_LEN},
	{  6, 20, themeRedWhite,	msg_EQUALLONG, msg_EQUAL_LEN},
};

static const MenuBody ADCSampleNumberBody[] PROGMEM = {
	{  8, 22, themeBlueWhite,	msg_LTR_A,		msg_LTR_A_LEN,	msg_ADC_SAMPNUM_NONE_gc,	NULL},
	{  9, 22, themeBlueWhite,	msg_LTR_B,		msg_LTR_B_LEN,	msg_ADC_SAMPNUM_ACC2_gc,	NULL},
	{  10, 22, themeBlueWhite,	msg_LTR_C,		msg_LTR_C_LEN,	msg_ADC_SAMPNUM_ACC4_gc,	NULL},
	{  11, 22, themeBlueWhite,	msg_LTR_D,		msg_LTR_D_LEN,	msg_ADC_SAMPNUM_ACC8_gc,	NULL},
	{  12, 22, themeBlueWhite,	msg_LTR_E,		msg_LTR_E_LEN,	msg_ADC_SAMPNUM_ACC16_gc,	NULL},
	{  13, 22, themeBlueWhite,	msg_LTR_F,		msg_LTR_F_LEN,	msg_ADC_SAMPNUM_ACC32_gc,	NULL},
	{  14, 22, themeBlueWhite,	msg_LTR_G,		msg_LTR_G_LEN,	msg_ADC_SAMPNUM_ACC64_gc,	NULL},
	{  15, 22, themeBlueWhite,	msg_LTR_H,		msg_LTR_H_LEN,	msg_ADC_SAMPNUM_ACC128_gc,	NULL},
	{  16, 22, themeBlueWhite,	msg_LTR_X,		msg_LTR_X_LEN,	msg_ExitMenu,  NULL},
};

static const DataEntry ADCSampleNumberDataEntry[] PROGMEM = {
	{  8, 22, themeBlueWhite,	ADC_SAMPNUM_NONE_gc,	 	FIELDTYPE_UINT8,		NULL, NULL},
	{  9, 22, themeBlueWhite,	ADC_SAMPNUM_ACC2_gc,	 	FIELDTYPE_UINT8,		NULL, NULL},
	{  10, 22, themeBlueWhite,	ADC_SAMPNUM_ACC4_gc,	 	FIELDTYPE_UINT8,		NULL, NULL},
	{  11, 22, themeBlueWhite,	ADC_SAMPNUM_ACC8_gc,	 	FIELDTYPE_UINT8,		NULL, NULL},
	{  12, 22, themeBlueWhite,	ADC_SAMPNUM_ACC16_gc,	 	FIELDTYPE_UINT8,		NULL, NULL},
	{  13, 22, themeBlueWhite,	ADC_SAMPNUM_ACC32_gc,	 	FIELDTYPE_UINT8,		NULL, NULL},
	{  14, 22, themeBlueWhite,	ADC_SAMPNUM_ACC64_gc,	 	FIELDTYPE_UINT8,		NULL, NULL},
	{  15, 22, themeBlueWhite,	ADC_SAMPNUM_ACC128_gc,	 	FIELDTYPE_UINT8,		NULL, NULL},
};

MenuSystem ADCSampleNumberMainMenu = {
	.menuHeader = ADCSampleNumberHeader,
	.menuHeaderSize = sizeof(ADCSampleNumberHeader) / sizeof(MenuHeader),
	.menuBody = ADCSampleNumberBody,
	.menuBodySize = sizeof(ADCSampleNumberBody) / sizeof(MenuBody),
	.dataEntry = ADCSampleNumberDataEntry,
	.dataEntrySize = sizeof(ADCSampleNumberDataEntry) / sizeof(DataEntry),
	.dataptr = NULL,
	.dataEntryMode = DATAENTRYMODE_DROPDOWN,
	.screenArea = {  3, 17, 19, 50, themeBlueWhite},
	.callingMenu = NULL,
	.saveConfig = {0, NULL}
};

void ADC_SampleNumberMainMenu(const char *msg)
{
	ADCSampleNumberMainMenu.dataptr = GetDataPtrForMenuItem(msg);
	ApplySaveConfigFromParentMenu(&ADCSampleNumberMainMenu);
	SetActiveMenu(&ADCSampleNumberMainMenu);
}

// Generic Menus

static const MenuHeader EnableDisabledHeader[] PROGMEM = {
	{  4, 20, themeRedWhite,	msg_EQUALLONG, msg_EQUAL_LEN},
	{  5, 25, themeBlueWhite,	msg_ENABLEDMenu, msg_ENABLEDMenu_LEN},
	{  6, 20, themeRedWhite,	msg_EQUALLONG, msg_EQUAL_LEN}
};

static const MenuBody EnableDisabledBody[] PROGMEM = {
	{  8, 22, themeBlueWhite,	msg_Num_1,		msg_Num_1_LEN, msg_ENABLE,	 NULL},
	{  9, 22, themeBlueWhite,	msg_Num_2,		msg_Num_2_LEN, msg_DISABLED, NULL},
	{  10, 22, themeBlueWhite,	msg_LTR_X,		msg_LTR_X_LEN, msg_ExitMenu,  NULL},
};

static const DataEntry EnableDisabledDataEntry[] PROGMEM = {
	{  8, 22, themeBlueWhite,	1,	FIELDTYPE_BOOL,		NULL, NULL},
	{  9, 22, themeBlueWhite,	0,	FIELDTYPE_BOOL,		NULL, NULL},
};

MenuSystem EnableDisabledMainMenuSystem = {
	.menuHeader = EnableDisabledHeader,
	.menuHeaderSize = sizeof(EnableDisabledHeader) / sizeof(MenuHeader),
	.menuBody = EnableDisabledBody,
	.menuBodySize = sizeof(EnableDisabledBody) / sizeof(MenuBody),
	.dataEntry = EnableDisabledDataEntry,
	.dataEntrySize = sizeof(EnableDisabledDataEntry) / sizeof(DataEntry),
	.dataptr = NULL,
	.dataEntryMode = DATAENTRYMODE_DROPDOWN,
	.screenArea = {  3, 17, 13, 50, themeBlueWhite},
	.callingMenu = NULL,
	.saveConfig = {0, NULL}
};

void EnableDisabledMainMenu(const char *msg)
{
	EnableDisabledMainMenuSystem.dataptr = GetDataPtrForMenuItem(msg);
	ApplySaveConfigFromParentMenu(&EnableDisabledMainMenuSystem);
	SetActiveMenu(&EnableDisabledMainMenuSystem);
}


static const MenuHeader PortDirectionHeader[] PROGMEM = {
	{  4, 20, themeRedWhite,	msg_EQUALLONG, msg_EQUAL_LEN},
	{  5, 25, themeBlueWhite,	msg_PortDirectionMenu, msg_PortDirectionMenu_LEN},
	{  6, 20, themeRedWhite,	msg_EQUALLONG, msg_EQUAL_LEN}
};

static const MenuBody PortDirectionBody[] PROGMEM = {
	{  8, 22, themeBlueWhite,	msg_Num_1,		msg_Num_1_LEN, msg_PortDirection_Output,	 NULL},
	{  9, 22, themeBlueWhite,	msg_Num_2,		msg_Num_2_LEN, msg_PortDirection_Input, NULL},
	{  10, 22, themeBlueWhite,	msg_LTR_X,		msg_LTR_X_LEN, msg_ExitMenu,  NULL},
};

static const DataEntry PortDirectionDataEntry[] PROGMEM = {
	{  8, 22, themeBlueWhite,	1,	FIELDTYPE_BOOL,		NULL, NULL},
	{  9, 22, themeBlueWhite,	0,	FIELDTYPE_BOOL,		NULL, NULL},
};

MenuSystem PortDirectionMainMenuSystem = {
	.menuHeader = PortDirectionHeader,
	.menuHeaderSize = sizeof(PortDirectionHeader) / sizeof(MenuHeader),
	.menuBody = PortDirectionBody,
	.menuBodySize = sizeof(PortDirectionBody) / sizeof(MenuBody),
	.dataEntry = PortDirectionDataEntry,
	.dataEntrySize = sizeof(PortDirectionDataEntry) / sizeof(DataEntry),
	.dataptr = NULL,
	.dataEntryMode = DATAENTRYMODE_DROPDOWN,
	.screenArea = {  3, 17, 12, 50, themeBlueWhite},
	.callingMenu = NULL,
	.saveConfig = {0, NULL}
};

void PortDirectionMainMenu(const char *msg)
{
	PortDirectionMainMenuSystem.dataptr = GetDataPtrForMenuItem(msg);
	ApplySaveConfigFromParentMenu(&PortDirectionMainMenuSystem);
	SetActiveMenu(&PortDirectionMainMenuSystem);
}


static const MenuHeader PortInvertedHeader[] PROGMEM = {
	{  4, 20, themeRedWhite,	msg_EQUALLONG, msg_EQUAL_LEN},
	{  5, 25, themeBlueWhite,	msg_PortInvertedMenu, msg_PortInvertedMenu_LEN},
	{  6, 20, themeRedWhite,	msg_EQUALLONG, msg_EQUAL_LEN}
};

static const MenuBody PortInvertedBody[] PROGMEM = {
	{  8, 22, themeBlueWhite,	msg_Num_1,		msg_Num_1_LEN, msg_PortInvertedNotInveted, NULL},
	{  9, 22, themeBlueWhite,	msg_Num_2,		msg_Num_2_LEN, msg_PortInvertedInverted,	 NULL},
	{  10, 22, themeBlueWhite,	msg_LTR_X,		msg_LTR_X_LEN, msg_ExitMenu,  NULL},
};

static const DataEntry PortInvertedDataEntry[] PROGMEM = {
	{  8, 22, themeBlueWhite,	0,	FIELDTYPE_BOOL,		NULL, NULL},
	{  9, 22, themeBlueWhite,	1,	FIELDTYPE_BOOL,		NULL, NULL},
};

MenuSystem PortInvertedMainMenuSystem = {
	.menuHeader = PortInvertedHeader,
	.menuHeaderSize = sizeof(PortInvertedHeader) / sizeof(MenuHeader),
	.menuBody = PortInvertedBody,
	.menuBodySize = sizeof(PortInvertedBody) / sizeof(MenuBody),
	.dataEntry = PortInvertedDataEntry,
	.dataEntrySize = sizeof(PortInvertedDataEntry) / sizeof(DataEntry),
	.dataptr = NULL,
	.dataEntryMode = DATAENTRYMODE_DROPDOWN,
	.screenArea = {  3, 17, 12, 50, themeBlueWhite},
	.callingMenu = NULL,
	.saveConfig = {0, NULL}
};

void PortInvertedMainMenu(const char *msg)
{
	PortInvertedMainMenuSystem.dataptr = GetDataPtrForMenuItem(msg);
	ApplySaveConfigFromParentMenu(&PortInvertedMainMenuSystem);
	SetActiveMenu(&PortInvertedMainMenuSystem);
}


static const MenuHeader PortResistorsHeader[] PROGMEM = {
	{  4, 20, themeRedWhite,	msg_EQUALLONG, msg_EQUAL_LEN},
	{  5, 25, themeBlueWhite,	msg_PortPullUpResistorMenu, msg_PortPullUpResistorMenu_LEN},
	{  6, 20, themeRedWhite,	msg_EQUALLONG, msg_EQUAL_LEN}
};

static const MenuBody PortResistorsBody[] PROGMEM = {
	{  8, 22, themeBlueWhite,	msg_Num_1,		msg_Num_1_LEN, msg_PortPullUpResistorPullUpDisabled,	 NULL},
	{  9, 22, themeBlueWhite,	msg_Num_2,		msg_Num_2_LEN, msg_PortPullUpResistorPullUpEnabled, NULL},
	{  10, 22, themeBlueWhite,	msg_LTR_X,		msg_LTR_X_LEN, msg_ExitMenu,  NULL},
};

static const DataEntry PortResistorsDataEntry[] PROGMEM = {
	{  8, 22, themeBlueWhite,	0,	FIELDTYPE_BOOL,		NULL, NULL},
	{  9, 22, themeBlueWhite,	1,	FIELDTYPE_BOOL,		NULL, NULL},
};

MenuSystem PortResistorsMainMenuSystem = {
	.menuHeader = PortResistorsHeader,
	.menuHeaderSize = sizeof(PortResistorsHeader) / sizeof(MenuHeader),
	.menuBody = PortResistorsBody,
	.menuBodySize = sizeof(PortResistorsBody) / sizeof(MenuBody),
	.dataEntry = PortResistorsDataEntry,
	.dataEntrySize = sizeof(PortResistorsDataEntry) / sizeof(DataEntry),
	.dataptr = NULL,
	.dataEntryMode = DATAENTRYMODE_DROPDOWN,
	.screenArea = {  3, 17, 12, 50, themeBlueWhite},
	.callingMenu = NULL,
	.saveConfig = {0, NULL}
};

void PortResistorsMainMenu(const char *msg)
{
	PortResistorsMainMenuSystem.dataptr = GetDataPtrForMenuItem(msg);
	ApplySaveConfigFromParentMenu(&PortResistorsMainMenuSystem);
	SetActiveMenu(&PortResistorsMainMenuSystem);
}

static const MenuHeader OutputHighLowHeader[] PROGMEM = {
	{  4, 20, themeRedWhite,	msg_EQUALLONG, msg_EQUAL_LEN},
	{  5, 25, themeBlueWhite,	msg_OutputLevelMenu, msg_OutputLevelMenu_LEN},
	{  6, 20, themeRedWhite,	msg_EQUALLONG, msg_EQUAL_LEN}
};

static const MenuBody OutputHighLowBody[] PROGMEM = {
	{  8, 22, themeBlueWhite,	msg_Num_1,		msg_Num_1_LEN, msg_OutputLevel_HIGH,	 NULL},
	{  9, 22, themeBlueWhite,	msg_Num_2,		msg_Num_2_LEN, msg_OutputLevel_LOW, NULL},
	{  10, 22, themeBlueWhite,	msg_LTR_X,		msg_LTR_X_LEN, msg_ExitMenu,  NULL},
};

static const DataEntry OutputHighLowDataEntry[] PROGMEM = {
	{  8, 22, themeBlueWhite,	1,	FIELDTYPE_BOOL,		NULL, NULL},
	{  9, 22, themeBlueWhite,	0,	FIELDTYPE_BOOL,		NULL, NULL},
};

MenuSystem OutputHighLowMainMenuSystem = {
	.menuHeader = OutputHighLowHeader,
	.menuHeaderSize = sizeof(OutputHighLowHeader) / sizeof(MenuHeader),
	.menuBody = OutputHighLowBody,
	.menuBodySize = sizeof(OutputHighLowBody) / sizeof(MenuBody),
	.dataEntry = OutputHighLowDataEntry,
	.dataEntrySize = sizeof(OutputHighLowDataEntry) / sizeof(DataEntry),
	.dataptr = NULL,
	.dataEntryMode = DATAENTRYMODE_DROPDOWN,
	.screenArea = {  3, 17, 12, 50, themeBlueWhite},
	.callingMenu = NULL,
	.saveConfig = {0, NULL}
};

void OutputHighLowMainMenu(const char *msg)
{
	OutputHighLowMainMenuSystem.dataptr = GetDataPtrForMenuItem(msg);
	ApplySaveConfigFromParentMenu(&OutputHighLowMainMenuSystem);
	SetActiveMenu(&OutputHighLowMainMenuSystem);
}
static const MenuHeader PortInputLevelHeader[] PROGMEM = {
	{  4, 20, themeRedWhite,	msg_EQUALLONG, msg_EQUAL_LEN},
	{  5, 25, themeBlueWhite,	msg_PortInputLevelMenu, msg_PortInputLevelMenu_LEN},
	{  6, 20, themeRedWhite,	msg_EQUALLONG, msg_EQUAL_LEN}
};

static const MenuBody PortInputLevelBody[] PROGMEM = {
	{  8, 22, themeBlueWhite,	msg_Num_1,		msg_Num_1_LEN, msg_PortInputLevelTTL,	 NULL},
	{  9, 22, themeBlueWhite,	msg_Num_2,		msg_Num_2_LEN, msg_PortInputLevelSchmitt, NULL},
	{  10, 22, themeBlueWhite,	msg_LTR_X,		msg_LTR_X_LEN, msg_ExitMenu,  NULL},
};

static const DataEntry PortInputLevelDataEntry[] PROGMEM = {
	{  8, 22, themeBlueWhite,	1,	FIELDTYPE_BOOL,		NULL, NULL},
	{  9, 22, themeBlueWhite,	0,	FIELDTYPE_BOOL,		NULL, NULL},
};

MenuSystem PortInputLevelMainMenuSystem = {
	.menuHeader = PortInputLevelHeader,
	.menuHeaderSize = sizeof(PortInputLevelHeader) / sizeof(MenuHeader),
	.menuBody = PortInputLevelBody,
	.menuBodySize = sizeof(PortInputLevelBody) / sizeof(MenuBody),
	.dataEntry = PortInputLevelDataEntry,
	.dataEntrySize = sizeof(PortInputLevelDataEntry) / sizeof(DataEntry),
	.dataptr = NULL,
	.dataEntryMode = DATAENTRYMODE_DROPDOWN,
	.screenArea = {  3, 17, 12, 50, themeBlueWhite},
	.callingMenu = NULL,
	.saveConfig = {0, NULL}
};

void PortInputLevelMainMenu(const char *msg)
{
	PortInputLevelMainMenuSystem.dataptr = GetDataPtrForMenuItem(msg);
	ApplySaveConfigFromParentMenu(&PortInputLevelMainMenuSystem);
	SetActiveMenu(&PortInputLevelMainMenuSystem);
}

static const MenuHeader VRefHeader[] PROGMEM = {
	{  4, 20, themeRedWhite,	msg_EQUALLONG, msg_EQUAL_LEN},
	{  5, 20, themeBlueWhite,	msg_VrefMenu, msg_VrefMenu_LEN},
	{  6, 20, themeRedWhite,	msg_EQUALLONG, msg_EQUAL_LEN},
};

static const MenuBody VRefBody[] PROGMEM = {
	{  8, 22, themeBlueWhite,	msg_LTR_A,		msg_LTR_A_LEN,  msg_VREF_REFSEL_1V024_gc,	NULL},
	{  9, 22, themeBlueWhite,	msg_LTR_B,		msg_LTR_B_LEN,  msg_VREF_REFSEL_2V048_gc,	NULL},
	{  10, 22, themeBlueWhite,	msg_LTR_C,		msg_LTR_C_LEN,  msg_VREF_REFSEL_4V096_gc,	NULL},
	{  11, 22, themeBlueWhite,	msg_LTR_D,		msg_LTR_D_LEN,  msg_VREF_REFSEL_2V500_gc,	NULL},
	{  12, 22, themeBlueWhite,	msg_LTR_E,		msg_LTR_E_LEN,  msg_VREF_REFSEL_VDD_gc,		NULL},
	{  13, 22, themeBlueWhite,	msg_LTR_F,		msg_LTR_F_LEN,  msg_VREF_REFSEL_VREFA_gc,	NULL},
	{  14, 22, themeBlueWhite,	msg_LTR_X,		msg_LTR_X_LEN,	msg_ExitMenu,  NULL},
};

static const DataEntry VRefDataEntry[] PROGMEM = {
	{  8, 22, themeBlueWhite,	VREF_REFSEL_1V024_gc, 	FIELDTYPE_UINT8,		NULL, NULL},
	{  9, 22, themeBlueWhite,	VREF_REFSEL_2V048_gc, 	FIELDTYPE_UINT8,		NULL, NULL},
	{  10, 22, themeBlueWhite,	VREF_REFSEL_4V096_gc, 	FIELDTYPE_UINT8,		NULL, NULL},
	{  11, 22, themeBlueWhite,	VREF_REFSEL_2V500_gc, 	FIELDTYPE_UINT8,		NULL, NULL},
	{  12, 22, themeBlueWhite,	VREF_REFSEL_VDD_gc,	 	FIELDTYPE_UINT8,		NULL, NULL},
	{  13, 22, themeBlueWhite,	VREF_REFSEL_VREFA_gc, 	FIELDTYPE_UINT8,		NULL, NULL},
};

MenuSystem VRefMainMenuSystem = {
	.menuHeader = VRefHeader,
	.menuHeaderSize = sizeof(VRefHeader) / sizeof(MenuHeader),
	.menuBody = VRefBody,
	.menuBodySize = sizeof(VRefBody) / sizeof(MenuBody),
	.dataEntry = VRefDataEntry,
	.dataEntrySize = sizeof(VRefDataEntry) / sizeof(DataEntry),
	.dataptr = NULL,
	.dataEntryMode = DATAENTRYMODE_DROPDOWN,
	.screenArea = {  3, 17, 17, 50, themeBlueWhite},
	.callingMenu = NULL,
	.saveConfig = {0, NULL}
};

void VRefMainMenu(const char *msg)
{
	VRefMainMenuSystem.dataptr = GetDataPtrForMenuItem(msg);
	ApplySaveConfigFromParentMenu(&VRefMainMenuSystem);
	SetActiveMenu(&VRefMainMenuSystem);
}

// DAC Specific

static const MenuHeader DACModeHeader[] PROGMEM = {
	{  4, 20, themeRedWhite,	msg_EQUALLONG, msg_EQUAL_LEN},
	{  5, 26, themeBlueWhite,	msg_DACMODEMenu, msg_DACMODEMenu_LEN},
	{  6, 20, themeRedWhite,	msg_EQUALLONG, msg_EQUAL_LEN},
};

static const MenuBody DACModesBody[] PROGMEM = {
	{  9, 22, themeBlueWhite,	msg_LTR_A,		msg_LTR_A_LEN, msg_SETVALUE,	 NULL},
	{  10, 22, themeBlueWhite,	msg_LTR_B,		msg_LTR_B_LEN, msg_SINEWAVE,  NULL},
	{  11, 22, themeBlueWhite,	msg_LTR_C,		msg_LTR_C_LEN, msg_SAWTOOHWAVE,	 NULL},
	{  12, 22, themeBlueWhite,	msg_LTR_D,		msg_LTR_D_LEN, msg_TRIANGLEWAVE,  NULL},
	{  13, 22, themeBlueWhite,	msg_LTR_X,		msg_LTR_X_LEN, msg_ExitMenu,  NULL},
};

static const DataEntry DACModesDataEntry[] PROGMEM = {
	{  9, 22, themeRedBlack,	DAC_DISPLAY_MODES_VALUE,		FIELDTYPE_UINT8,		NULL, NULL},
	{  10, 22, themeRedBlack,	DAC_DISPLAY_MODES_SINEWAVE, 	FIELDTYPE_UINT8,		NULL, NULL},
	{  11, 22, themeRedBlack,	DAC_DISPLAY_MODES_SAWTOOTH, 	FIELDTYPE_UINT8,		NULL, NULL},
	{  12, 22, themeRedBlack,	DAC_DISPLAY_MODES_TRIANGLE, 	FIELDTYPE_UINT8,		NULL, NULL}
};

MenuSystem DACModeMainMenuSystem = {
	.menuHeader = DACModeHeader,
	.menuHeaderSize = sizeof(DACModeHeader) / sizeof(MenuHeader),
	.menuBody = DACModesBody,
	.menuBodySize = sizeof(DACModesBody) / sizeof(MenuBody),
	.dataEntry = DACModesDataEntry,
	.dataEntrySize = sizeof(DACModesDataEntry) / sizeof(DataEntry),
	.dataptr = NULL,
	.dataEntryMode = DATAENTRYMODE_DROPDOWN,
	.screenArea = {  3, 17, 16, 50, themeBlueWhite},
	.callingMenu = NULL,
	.saveConfig = {0, NULL}
};

void DACModeMainMenu(const char *msg)
{
	DACModeMainMenuSystem.dataptr = GetDataPtrForMenuItem(msg);
	ApplySaveConfigFromParentMenu(&DACModeMainMenuSystem);
	SetActiveMenu(&DACModeMainMenuSystem);
}

static const MenuHeader CCLInputSourceHeader[] PROGMEM = {
	{  4, 20, themeRedWhite,	msg_EQUALLONG, msg_EQUAL_LEN},
	{  5, 20, themeBlueWhite,	msg_CCL_INPUT, msg_CCL_INPUT_LEN},
	{  6, 20, themeRedWhite,	msg_EQUALLONG, msg_EQUAL_LEN},
};

static const MenuBody CCLInputSourceBody[] PROGMEM = {
	{  8, 22, themeBlueWhite,	msg_LTR_A,		msg_LTR_A_LEN,  msg_CCL_INPUT_A,	NULL},
	{  9, 22, themeBlueWhite,	msg_LTR_B,		msg_LTR_B_LEN,  msg_CCL_INPUT_A_B,	NULL},
	{  10, 22, themeBlueWhite,	msg_LTR_C,		msg_LTR_C_LEN,  msg_CCL_INPUT_A_B_C,	NULL},
	{  11, 22, themeBlueWhite,	msg_LTR_X,		msg_LTR_X_LEN,	msg_ExitMenu,  NULL},
};

static const DataEntry CCLInputSourceDataEntry[] PROGMEM = {
	{  8, 22, themeBlueWhite,	VREF_REFSEL_1V024_gc, 	FIELDTYPE_UINT8,		NULL, NULL},
	{  9, 22, themeBlueWhite,	VREF_REFSEL_2V048_gc, 	FIELDTYPE_UINT8,		NULL, NULL},
	{  10, 22, themeBlueWhite,	VREF_REFSEL_4V096_gc, 	FIELDTYPE_UINT8,		NULL, NULL},
};

MenuSystem CCLInputSourceMainMenuSystem = {
	.menuHeader = CCLInputSourceHeader,
	.menuHeaderSize = sizeof(CCLInputSourceHeader) / sizeof(MenuHeader),
	.menuBody = CCLInputSourceBody,
	.menuBodySize = sizeof(CCLInputSourceBody) / sizeof(MenuBody),
	.dataEntry = CCLInputSourceDataEntry,
	.dataEntrySize = sizeof(CCLInputSourceDataEntry) / sizeof(DataEntry),
	.dataptr = NULL,
	.dataEntryMode = DATAENTRYMODE_DROPDOWN,
	.screenArea = {  3, 17, 17, 50, themeBlueWhite},
	.callingMenu = NULL,
	.saveConfig = {0, NULL}
};

void CCLInputSourceMainMenu(const char *msg)
{
	CCLInputSourceMainMenuSystem.dataptr = GetDataPtrForMenuItem(msg);
	ApplySaveConfigFromParentMenu(&CCLInputSourceMainMenuSystem);
	SetActiveMenu(&CCLInputSourceMainMenuSystem);
}

static const MenuHeader ClkSelectHeader[] PROGMEM = {
	{  4, 20, themeRedWhite,	msg_EQUALLONG, msg_EQUAL_LEN},
	{  5, 20, themeBlueWhite,	msg_ClkSelMenu, msg_ClkSelMenu_LEN},
	{  6, 20, themeRedWhite,	msg_EQUALLONG, msg_EQUAL_LEN},
};

static const MenuBody ClkSelectBody[] PROGMEM = {
	{  8, 22, themeBlueWhite,	msg_LTR_A,		msg_LTR_A_LEN,  msg_ClkSEL_DIV1_gc,		NULL},
	{  9, 22, themeBlueWhite,	msg_LTR_B,		msg_LTR_B_LEN,  msg_ClkSEL_DIV2_gc,		NULL},
	{  10, 22, themeBlueWhite,	msg_LTR_C,		msg_LTR_C_LEN,  msg_ClkSEL_DIV4_gc,		NULL},
	{  11, 22, themeBlueWhite,	msg_LTR_D,		msg_LTR_D_LEN,  msg_ClkSEL_DIV8_gc,		NULL},
	{  12, 22, themeBlueWhite,	msg_LTR_E,		msg_LTR_E_LEN,  msg_ClkSEL_DIV16_gc,	NULL},
	{  13, 22, themeBlueWhite,	msg_LTR_F,		msg_LTR_F_LEN,  msg_ClkSEL_DIV64_gc,	NULL},
	{  14, 22, themeBlueWhite,	msg_LTR_G,		msg_LTR_G_LEN,  msg_ClkSEL_DIV256_gc,	NULL},
	{  15, 22, themeBlueWhite,	msg_LTR_H,		msg_LTR_H_LEN,  msg_ClkSEL_DIV1024_gc,	NULL},
	{  16, 22, themeBlueWhite,	msg_LTR_X,		msg_LTR_X_LEN,	msg_ExitMenu,			NULL},
};

static const DataEntry ClkSelectDataEntry[] PROGMEM = {
	{  8, 22, themeBlueWhite,	TCA_SINGLE_CLKSEL_DIV1_gc, 		FIELDTYPE_UINT8,		NULL, NULL},
	{  9, 22, themeBlueWhite,	TCA_SINGLE_CLKSEL_DIV2_gc, 		FIELDTYPE_UINT8,		NULL, NULL},
	{  10, 22, themeBlueWhite,	TCA_SINGLE_CLKSEL_DIV4_gc, 		FIELDTYPE_UINT8,		NULL, NULL},
	{  11, 22, themeBlueWhite,	TCA_SINGLE_CLKSEL_DIV8_gc, 		FIELDTYPE_UINT8,		NULL, NULL},
	{  12, 22, themeBlueWhite,	TCA_SINGLE_CLKSEL_DIV16_gc, 	FIELDTYPE_UINT8,		NULL, NULL},
	{  13, 22, themeBlueWhite,	TCA_SINGLE_CLKSEL_DIV64_gc, 	FIELDTYPE_UINT8,		NULL, NULL},
	{  14, 22, themeBlueWhite,	TCA_SINGLE_CLKSEL_DIV256_gc, 	FIELDTYPE_UINT8,		NULL, NULL},
	{  15, 22, themeBlueWhite,	TCA_SINGLE_CLKSEL_DIV1024_gc, 	FIELDTYPE_UINT8,		NULL, NULL},
};

MenuSystem ClkSelectMainMenuSystem = {
	.menuHeader = ClkSelectHeader,
	.menuHeaderSize = sizeof(ClkSelectHeader) / sizeof(MenuHeader),
	.menuBody = ClkSelectBody,
	.menuBodySize = sizeof(ClkSelectBody) / sizeof(MenuBody),
	.dataEntry = ClkSelectDataEntry,
	.dataEntrySize = sizeof(ClkSelectDataEntry) / sizeof(DataEntry),
	.dataptr = NULL,
	.dataEntryMode = DATAENTRYMODE_DROPDOWN,
	.screenArea = {  3, 17, 19, 50, themeBlueWhite},
	.callingMenu = NULL,
	.saveConfig = {0, NULL}
};

void ClkSelectMainMenu(const char *msg)
{
	ClkSelectMainMenuSystem.dataptr = GetDataPtrForMenuItem(msg);
	ApplySaveConfigFromParentMenu(&ClkSelectMainMenuSystem);
	SetActiveMenu(&ClkSelectMainMenuSystem);
}


static const MenuHeader WaveformHeader[] PROGMEM = {
	{  4, 20, themeRedWhite,	msg_EQUALLONG, msg_EQUAL_LEN},
	{  5, 22, themeBlueWhite,	msg_WaveformMenu, msg_WaveformMenu_LEN},
	{  6, 20, themeRedWhite,	msg_EQUALLONG, msg_EQUAL_LEN},
};

static const MenuBody WaveformBody[] PROGMEM = {
	{  8, 20, themeBlueWhite,	msg_LTR_A,		msg_LTR_A_LEN,  msg_Waveform_Normal,		NULL},
	{  9, 20, themeBlueWhite,	msg_LTR_B,		msg_LTR_B_LEN,  msg_Waveform_Frequency,		NULL},
	{  10, 20, themeBlueWhite,	msg_LTR_C,		msg_LTR_C_LEN,  msg_Waveform_SingleSlope,		NULL},
	{  11, 20, themeBlueWhite,	msg_LTR_D,		msg_LTR_D_LEN,  msg_Waveform_DualPWMTop,		NULL},
	{  12, 20, themeBlueWhite,	msg_LTR_E,		msg_LTR_E_LEN,  msg_Waveform_DualPWMTopBottom,	NULL},
	{  13, 20, themeBlueWhite,	msg_LTR_F,		msg_LTR_F_LEN,  msg_Waveform_DualPWMBottom,	NULL},
	{  14, 20, themeBlueWhite,	msg_LTR_X,		msg_LTR_X_LEN,	msg_ExitMenu,			NULL},
};

static const DataEntry WaveformDataEntry[] PROGMEM = {
	{  8, 20, themeBlueWhite,	TCA_SINGLE_WGMODE_NORMAL_gc, 		FIELDTYPE_UINT8,		NULL, NULL},
	{  9, 20, themeBlueWhite,	TCA_SINGLE_WGMODE_FRQ_gc, 			FIELDTYPE_UINT8,		NULL, NULL},
	{  10, 20, themeBlueWhite,	TCA_SINGLE_WGMODE_SINGLESLOPE_gc, 	FIELDTYPE_UINT8,		NULL, NULL},
	{  11, 20, themeBlueWhite,	TCA_SINGLE_WGMODE_DSTOP_gc, 		FIELDTYPE_UINT8,		NULL, NULL},
	{  12, 20, themeBlueWhite,	TCA_SINGLE_WGMODE_DSBOTH_gc, 		FIELDTYPE_UINT8,		NULL, NULL},
	{  13, 20, themeBlueWhite,	TCA_SINGLE_WGMODE_DSBOTTOM_gc, 		FIELDTYPE_UINT8,		NULL, NULL},
};

MenuSystem WaveformMainMenuSystem = {
	.menuHeader = WaveformHeader,
	.menuHeaderSize = sizeof(WaveformHeader) / sizeof(MenuHeader),
	.menuBody = WaveformBody,
	.menuBodySize = sizeof(WaveformBody) / sizeof(MenuBody),
	.dataEntry = WaveformDataEntry,
	.dataEntrySize = sizeof(WaveformDataEntry) / sizeof(DataEntry),
	.dataptr = NULL,
	.dataEntryMode = DATAENTRYMODE_DROPDOWN,
	.screenArea = {  3, 17, 16, 55, themeBlueWhite},
	.callingMenu = NULL,
	.saveConfig = {0, NULL}
};

void WaveformMainMenu(const char *msg)
{
	WaveformMainMenuSystem.dataptr = GetDataPtrForMenuItem(msg);
	ApplySaveConfigFromParentMenu(&WaveformMainMenuSystem);
	SetActiveMenu(&WaveformMainMenuSystem);
}

static const MenuHeader DutyCycleHeader[] PROGMEM = {
	{  4, 20, themeRedWhite,	msg_EQUALLONG, msg_EQUAL_LEN},
	{  5, 22, themeBlueWhite,	msg_DutyCycleMenu, msg_DutyCycleMenu_LEN},
	{  6, 20, themeRedWhite,	msg_EQUALLONG, msg_EQUAL_LEN},
};

static const MenuBody DutyCycleBody[] PROGMEM = {
	{  8, 20, themeBlueWhite,	msg_LTR_A,		msg_LTR_A_LEN,  msg_DutyCycleList_10,	NULL},
	{  9, 20, themeBlueWhite,	msg_LTR_B,		msg_LTR_B_LEN,  msg_DutyCycleList_20,	NULL},
	{  10, 20, themeBlueWhite,	msg_LTR_C,		msg_LTR_C_LEN,  msg_DutyCycleList_30,	NULL},
	{  11, 20, themeBlueWhite,	msg_LTR_D,		msg_LTR_D_LEN,  msg_DutyCycleList_40,	NULL},
	{  12, 20, themeBlueWhite,	msg_LTR_E,		msg_LTR_E_LEN,  msg_DutyCycleList_50,	NULL},
	{  13, 20, themeBlueWhite,	msg_LTR_F,		msg_LTR_F_LEN,  msg_DutyCycleList_60,	NULL},
	{  14, 20, themeBlueWhite,	msg_LTR_G,		msg_LTR_G_LEN,  msg_DutyCycleList_70,	NULL},
	{  15, 20, themeBlueWhite,	msg_LTR_H,		msg_LTR_H_LEN,  msg_DutyCycleList_80,	NULL},
	{  16, 20, themeBlueWhite,	msg_LTR_I,		msg_LTR_I_LEN,  msg_DutyCycleList_90,	NULL},
	{  17, 20, themeBlueWhite,	msg_LTR_X,		msg_LTR_X_LEN,	msg_ExitMenu,			NULL},
};

static const DataEntry DutyCycleDataEntry[] PROGMEM = {
	{  8, 20, themeBlueWhite,	DUTYCYCLELIST_10, 	FIELDTYPE_UINT8,		NULL, NULL},
	{  9, 20, themeBlueWhite,	DUTYCYCLELIST_20, 	FIELDTYPE_UINT8,		NULL, NULL},
	{  10, 20, themeBlueWhite,	DUTYCYCLELIST_30, 	FIELDTYPE_UINT8,		NULL, NULL},
	{  11, 20, themeBlueWhite,	DUTYCYCLELIST_40, 	FIELDTYPE_UINT8,		NULL, NULL},
	{  12, 20, themeBlueWhite,	DUTYCYCLELIST_50, 	FIELDTYPE_UINT8,		NULL, NULL},
	{  13, 20, themeBlueWhite,	DUTYCYCLELIST_60, 	FIELDTYPE_UINT8,		NULL, NULL},
	{  14, 20, themeBlueWhite,	DUTYCYCLELIST_70, 	FIELDTYPE_UINT8,		NULL, NULL},
	{  15, 20, themeBlueWhite,	DUTYCYCLELIST_80, 	FIELDTYPE_UINT8,		NULL, NULL},
	{  16, 20, themeBlueWhite,	DUTYCYCLELIST_90,	FIELDTYPE_UINT8,		NULL, NULL},		
};

MenuSystem DutyCycleMainMenuSystem = {
	.menuHeader = DutyCycleHeader,
	.menuHeaderSize = sizeof(DutyCycleHeader) / sizeof(MenuHeader),
	.menuBody = DutyCycleBody,
	.menuBodySize = sizeof(DutyCycleBody) / sizeof(MenuBody),
	.dataEntry = DutyCycleDataEntry,
	.dataEntrySize = sizeof(DutyCycleDataEntry) / sizeof(DataEntry),
	.dataptr = NULL,
	.dataEntryMode = DATAENTRYMODE_DROPDOWN,
	.screenArea = {  3, 17, 20, 55, themeBlueWhite},
	.callingMenu = NULL,
	.saveConfig = {0, NULL}
};

void DutyCycleMainMenu(const char *msg)
{
	DutyCycleMainMenuSystem.dataptr = GetDataPtrForMenuItem(msg);
	ApplySaveConfigFromParentMenu(&DutyCycleMainMenuSystem);
	SetActiveMenu(&DutyCycleMainMenuSystem);
}

static const MenuHeader InterruptEdgeHeader[] PROGMEM = {
	{  4, 20, themeRedWhite,	msg_EQUALLONG, msg_EQUAL_LEN},
	{  5, 22, themeBlueWhite,	msg_InterruptEdgeMenu, msg_InterruptEdgeMenu_LEN},
	{  6, 20, themeRedWhite,	msg_EQUALLONG, msg_EQUAL_LEN},
};

static const MenuBody InterruptEdgeBody[] PROGMEM = {
	{  8, 20, themeBlueWhite,	msg_LTR_A,		msg_LTR_A_LEN,  msg_EdgesRising,	NULL},
	{  9, 20, themeBlueWhite,	msg_LTR_B,		msg_LTR_B_LEN,  msg_EdgesFalling,	NULL},
	{  10, 20, themeBlueWhite,	msg_LTR_C,		msg_LTR_C_LEN,  msg_EdgesBoth,	NULL},
	{  11, 20, themeBlueWhite,	msg_LTR_X,		msg_LTR_X_LEN,	msg_ExitMenu,			NULL},
};

static const DataEntry InterruptEdgeDataEntry[] PROGMEM = {
	{  8, 20, themeBlueWhite,	ZCD_INTMODE_RISING_gc, 	FIELDTYPE_UINT8,		NULL, NULL},
	{  9, 20, themeBlueWhite,	ZCD_INTMODE_FALLING_gc, FIELDTYPE_UINT8,		NULL, NULL},
	{  10, 20, themeBlueWhite,	ZCD_INTMODE_BOTH_gc, 	FIELDTYPE_UINT8,		NULL, NULL},
};


MenuSystem InterruptEdgeMainMenuSystem = {
	.menuHeader = InterruptEdgeHeader,
	.menuHeaderSize = sizeof(InterruptEdgeHeader) / sizeof(MenuHeader),
	.menuBody = InterruptEdgeBody,
	.menuBodySize = sizeof(InterruptEdgeBody) / sizeof(MenuBody),
	.dataEntry = InterruptEdgeDataEntry,
	.dataEntrySize = sizeof(InterruptEdgeDataEntry) / sizeof(DataEntry),
	.dataptr = NULL,
	.dataEntryMode = DATAENTRYMODE_DROPDOWN,
	.screenArea = {  3, 13, 20, 55, themeBlueWhite},
	.callingMenu = NULL,
	.saveConfig = {0, NULL}
};

void InterruptEdgeMainMenu(const char *msg)
{
	InterruptEdgeMainMenuSystem.dataptr = GetDataPtrForMenuItem(msg);
	ApplySaveConfigFromParentMenu(&InterruptEdgeMainMenuSystem);
	SetActiveMenu(&InterruptEdgeMainMenuSystem);
}


static const MenuHeader EventSystemModesHeader[] PROGMEM = {
	{  4, 20, themeRedWhite,	msg_EQUALLONG, msg_EQUAL_LEN},
	{  5, 22, themeBlueWhite,	msg_EventSystemModesMenu, msg_EventSystemModesMenu_LEN},
	{  6, 20, themeRedWhite,	msg_EQUALLONG, msg_EQUAL_LEN},
};

static const MenuBody EventSystemModesBody[] PROGMEM = {
	{  8, 20, themeBlueWhite,	msg_LTR_A,		msg_LTR_A_LEN,  msg_EventModeADC,	NULL},
	{  9, 20, themeBlueWhite,	msg_LTR_B,		msg_LTR_B_LEN,  msg_EventModeCCL,	NULL},
	{  10, 20, themeBlueWhite,	msg_LTR_C,		msg_LTR_C_LEN,  msg_EventModeRTC,	NULL},
	{  11, 20, themeBlueWhite,	msg_LTR_C,		msg_LTR_C_LEN,  msg_EventModeZCD,	NULL},
	{  12, 20, themeBlueWhite,	msg_LTR_X,		msg_LTR_X_LEN,	msg_ExitMenu,			NULL},
};

static const DataEntry EventSystemModesDataEntry[] PROGMEM = {
	{  8, 20, themeBlueWhite,	EVENTMODES_ADC, 	FIELDTYPE_UINT8,		NULL, NULL},
	{  9, 20, themeBlueWhite,	EVENTMODES_CCL,		FIELDTYPE_UINT8,		NULL, NULL},
	{  10, 20, themeBlueWhite,	EVENTMODES_RTC, 	FIELDTYPE_UINT8,		NULL, NULL},
	{  11, 20, themeBlueWhite,	EVENTMODES_ZCD, 	FIELDTYPE_UINT8,		NULL, NULL},
};


MenuSystem EventSystemModesMainMenuSystem = {
	.menuHeader = EventSystemModesHeader,
	.menuHeaderSize = sizeof(EventSystemModesHeader) / sizeof(MenuHeader),
	.menuBody = EventSystemModesBody,
	.menuBodySize = sizeof(EventSystemModesBody) / sizeof(MenuBody),
	.dataEntry = EventSystemModesDataEntry,
	.dataEntrySize = sizeof(EventSystemModesDataEntry) / sizeof(DataEntry),
	.dataptr = NULL,
	.dataEntryMode = DATAENTRYMODE_DROPDOWN,
	.screenArea = {  3, 13, 20, 55, themeBlueWhite},
	.callingMenu = NULL,
	.saveConfig = {0, NULL}
};

void EventSystemModesMainMenu(const char *msg)
{
	EventSystemModesMainMenuSystem.dataptr = GetDataPtrForMenuItem(msg);
	ApplySaveConfigFromParentMenu(&EventSystemModesMainMenuSystem);
	SetActiveMenu(&EventSystemModesMainMenuSystem);
}


// ============================================================
// DropDownId-only table-driven access
// ============================================================

static MenuSystem * const dropDownMenus[DROPDOWN_Count] =
{
	[DROPDOWN_EnableDisabled]	= &EnableDisabledMainMenuSystem,
	[DROPDOWN_PreScale]			= &ADCPreScaleMainMenu,
	[DROPDOWN_SampleDelay]		= &ADCSampleDelayMainMenu,
	[DROPDOWN_Bits]				= &ADCDataBitsMainMenu,
	[DROPDOWN_SampleNumber]		= &ADCSampleNumberMainMenu,
	[DROPDOWN_VRef]				= &VRefMainMenuSystem,
	[DROPDOWN_DACMode]			= &DACModeMainMenuSystem,
	[DROPDOWN_CCLInputSource]	= &CCLInputSourceMainMenuSystem,
	[DROPDOWN_ClkSelect]		= &ClkSelectMainMenuSystem,
	[DROPDOWN_Waveform]			= &WaveformMainMenuSystem,
	[DROPDOWN_DutyCycle]		= &DutyCycleMainMenuSystem,
	[DROPDOWN_BaudRate]			= &BaudRateMainMenuSystem,
	[DROPDOWN_Parity]			= &ParityMainMenuSystem,
	[DROPDOWN_StopBits]			= &StopBitsMainMenuSystem,
	[DROPDOWN_DataBits]			= &DataBitsMainMenuSystem,
	[DROPDOWN_InterruptEdges]	= &InterruptEdgeMainMenuSystem,
	[DROPDOWN_EventSystemModes] = &EventSystemModesMainMenuSystem,
	[DROPDOWN_PortDirection]	= &PortDirectionMainMenuSystem,
	[DROPDOWN_PortInverted]		= &PortInvertedMainMenuSystem,
	[DROPDOWN_PortResistor]		= &PortResistorsMainMenuSystem,
	[DROPDOWN_HighLowValue]		= &OutputHighLowMainMenuSystem,
	[DROPDOWN_PortInputLevel]	= &PortInputLevelMainMenuSystem,
};

static inline MenuSystem *DropDown_GetMenuSystem(DropDownId id)
{
	if ((uint8_t)id >= (uint8_t)DROPDOWN_Count)
	{
		return NULL;
	}
	return dropDownMenus[id];
}

char *LookupDropDownByValueId(DropDownId id, uint16_t value)
{
	MenuSystem *ms = DropDown_GetMenuSystem(id);
	if (ms == NULL)
	{
		return NULL;
	}
	return LookupDropDownByValue(ms, value);
}

uint16_t LookupDropDownStringId(DropDownId id, char *msg)
{
	MenuSystem *ms = DropDown_GetMenuSystem(id);
	if (ms == NULL)
	{
		return UINT16_MAX;
	}
	return LookupDropDownString(ms, msg);
}

// ============================================================
// Generic dropdown lookup (existing)
// ============================================================

char *LookupDropDownByValue(MenuSystem *MenuSystem, uint16_t value)
{
	const char *msg = NULL;
	int index = -1;
	DataEntry de;

	for (int i = 0; i < MenuSystem->dataEntrySize; i++)
	{
		MenuDataEntry_Read(&de, MenuSystem->dataEntry, i);
		if (de.offset == value)
		{
			index = de.row;
			break;
		}
	}

	if (index != -1)
	{
		for (int i = 0; i < MenuSystem->menuBodySize; i++)
		{
			MenuBody mb;
			MenuBody_Read(&mb, MenuSystem->menuBody, i);
			if (mb.row == index)
			{
				msg = mb.description_P;
				break;
			}
		}
	}
	return (char *)msg;
}

uint16_t LookupDropDownString(MenuSystem *MenuSystem1, char *msg)
{
	// Not implemented in your original file; preserve behavior,
	// but fix type/sentinel: return UINT16_MAX for "not found".
	(void)MenuSystem1;
	(void)msg;
	return UINT16_MAX;
}

// ============================================================
// Decode functions (updated to use DropDownId-based lookup)
// ============================================================

char *CCLInputSourceDecode(void *ptr)
{
	uint8_t value = *(uint8_t *)ptr;
	return LookupDropDownByValueId(DROPDOWN_CCLInputSource, (uint16_t)value);
}

char *ClkselModeDecode(void *ptr)
{
	uint8_t value = *(uint8_t *)ptr;
	return LookupDropDownByValueId(DROPDOWN_ClkSelect, (uint16_t)value);
}

char *WaveformModeDecode(void *ptr)
{
	uint8_t value = *(uint8_t *)ptr;
	return LookupDropDownByValueId(DROPDOWN_Waveform, (uint16_t)value);
}

char *DutyCycleModeDecode(void *ptr)
{
	uint8_t value = *(uint8_t *)ptr;
	return LookupDropDownByValueId(DROPDOWN_DutyCycle, (uint16_t)value);
}

char *InterruptEdgeDecode(void *ptr)
{
	uint8_t value = *(uint8_t *)ptr;
	return LookupDropDownByValueId(DROPDOWN_InterruptEdges, (uint16_t)value);
}

char *EventsSystemModeDecode(void *ptr)
{
	uint8_t value = *(uint8_t *)ptr;
	return LookupDropDownByValueId(DROPDOWN_EventSystemModes, (uint16_t)value);
}

// ============================================================
// Remaining utility code unchanged
// ============================================================

void ApplySaveConfigFromParentMenu(struct MenuSystem *editMenu)
{
	const struct MenuSystem *activeMenu = GetActiveMenu();
	editMenu->saveConfig.index = activeMenu->saveConfig.index;
	editMenu->saveConfig.Savefuncptr = activeMenu->saveConfig.Savefuncptr;
}

struct EnterValueByHand *GetEnterValuePtr()
{
	return &enterValue;
}

void EnterNumberValue(void)
{
	// Change Input Handler to Value
	SetCommandMode(COMMANDMODE_VALUE);

	// The Enter is handled by displaying a message the line
	// just above the command Prompt
	SetColor(themeYellowBlack->foreColor, themeYellowBlack->backGroundColor);
	ClearArea(TerminalValueLine, TerminalValueLine, 0, TerminalMaxCols);
	PrintAt(TerminalValueLine, 0);

	switch (enterValue.fieldtype)
	{
		case FIELDTYPE_BOOL:			// Currently not implemented
		case FIELDTYPE_UINT8:
		case FIELDTYPE_UINT16:
			printf_P(PSTR("%S Range %u-%u"), enterValue.msg, (uint16_t)enterValue.Min, (uint16_t)enterValue.Max);
			break;

		case FIELDTYPE_FLOAT:
			printf_P(PSTR("%S Range %3.3f-%3.3f"), enterValue.msg, enterValue.Min, enterValue.Max);
			break;
	}

	// Make sure the last command is cleared before displaying the prompt
	Flush_ReceiveBuffer();
	CommandPrompt();
}

char *DecodeFloat(void *ptr)
{
	float value = *(float*)ptr;
	printf("%3.3f", value);
	return NULL;
}

char *DecodeUint8_t(void *ptr)
{
	uint8_t value = *(uint8_t*)ptr;
	printf("%d (0x%02X)", value, value);
	return NULL;
}

char *DecodeUint16_t(void *ptr)
{
	uint16_t value = *(uint16_t*)ptr;
	printf("%d (0x%02X)", value, value);
	return NULL;
}
