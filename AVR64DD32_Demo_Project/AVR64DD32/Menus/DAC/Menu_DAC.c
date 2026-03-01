/*
 * Menu_DAC.c
 *
 * Created: 11/28/2025 9:23:25 AM
 *  Author: terry
 */ 
#include <avr/pgmspace.h>
#include "Menu_DAC.h"
#include "DAC/DAC.h"
#include "Common/DataStructures.h"
#include "CommonStrings/CommonStrings.h"
#include "Menu_Report_Processor/Themes/Themes.h"

static const MenuHeader DACHeader[] PROGMEM = {
	{  1, 0, themeWhiteBlack,	msg_EQUAL, msg_EQUAL_LEN},
	{  2, 0, themeYellowBlack,	msg_DACMenu, msg_DACMenu_LEN},
	{  3, 0, themeWhiteBlack,	msg_EQUAL, msg_EQUAL_LEN}
};

static const MenuBody DACBody[] PROGMEM = {
	{  5, 2, themeWhiteBlack,	msg_LTR_A,		msg_LTR_A_LEN, msg_ENABLE,	 EnableDisabledMainMenu},
	{  6, 2, themeWhiteBlack,	msg_LTR_B,		msg_LTR_B_LEN, msg_REFRANGE, VRefMainMenu},
	{  7, 2, themeWhiteBlack,	msg_LTR_C,		msg_LTR_C_LEN, msg_DACMODE,	 DACModeMainMenu},
	{  8, 2, themeWhiteBlack,	msg_LTR_D,		msg_LTR_D_LEN, msg_PEAK,	 GetDACPeakValue},
	{  9, 2, themeWhiteBlack,	msg_LTR_E,		msg_LTR_E_LEN, msg_OFFSET,   GetDACOffsetValue},
	{  10, 2, themeWhiteBlack,	msg_LTR_F,		msg_LTR_F_LEN, msg_STEPS,	 GetDACStepValue},
	{  11, 2, themeWhiteBlack,	msg_LTR_X,		msg_LTR_X_LEN, msg_MainMenu, DisplayMainMenu},
};


static const DataEntry DACConfigDataEntry[] PROGMEM = {
	{  5, 32, themeCyanBlack,	offsetof(struct DACConfiguration, enabled),		FIELDTYPE_BOOL,		EnableDisableDecode},
	{  6, 32, themeCyanBlack,	offsetof(struct DACConfiguration, refRange),	FIELDTYPE_UINT8,	RefDecode},
	{  7, 32, themeCyanBlack,	offsetof(struct DACConfiguration, mode),		FIELDTYPE_UINT8,	DACModeDecode},
	{  8, 32, themeCyanBlack,	offsetof(struct DACConfiguration, peak),	FIELDTYPE_FLOAT,		DecodeFloat},
	{  9, 32, themeCyanBlack,	offsetof(struct DACConfiguration, Offset),	FIELDTYPE_FLOAT,		DecodeFloat},
	{  10,32, themeCyanBlack,	offsetof(struct DACConfiguration, steps),	FIELDTYPE_UINT16,		DecodeUint16_t}
};


MenuSystem DACMainMenu = {
	.menuHeader = DACHeader,
	.menuHeaderSize = sizeof(DACHeader) / sizeof(MenuHeader),
	.menuBody = DACBody,
	.menuBodySize = sizeof(DACBody) / sizeof(MenuBody),
    .dataEntry = DACConfigDataEntry,
    .dataEntrySize = sizeof(DACConfigDataEntry) / sizeof(DataEntry),
	.dataptr = NULL,
	.dataEntryMode = DATAENTRYMODE_DISPLAYDATA,
	.screenArea = {  0, 0, 0, 0, themeWhiteBlack},
	.callingMenu = NULL,
	.saveConfig = {0, NULL}	
};

void DAC_Menu(const char *msg)
{
	DACMainMenu.dataptr = GetPtrToDACData();
	DACMainMenu.saveConfig.Savefuncptr = DAC_Save_Config;
	DACMainMenu.saveConfig.index = 0;
	SetActiveMenu(&DACMainMenu);
}
