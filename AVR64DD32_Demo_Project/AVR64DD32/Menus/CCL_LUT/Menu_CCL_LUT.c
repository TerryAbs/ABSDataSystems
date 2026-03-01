/*
 * Menu_CCL_LUT.c
 *
 * Created: 11/28/2025 9:23:25 AM
 *  Author: terry
 */ 
#include <avr/pgmspace.h>
#include "CCL/CCL.h"
#include "Menu_CCL_LUT.h"
#include "Common/DataStructures.h"
#include "CommonStrings/CommonStrings.h"
#include "Menu_Report_Processor/Themes/Themes.h"

static const MenuHeader CCLHeader[] PROGMEM = {
	{  1, 0, themeWhiteBlack,	msg_EQUAL, msg_EQUAL_LEN},
	{  2, 0, themeYellowBlack,	msg_CCLMenu, msg_CCLMenu_LEN},
	{  3, 0, themeWhiteBlack,	msg_EQUAL, msg_EQUAL_LEN}
};

static const MenuBody CCLBody[] PROGMEM = {
	{  5, 2, themeWhiteBlack,	msg_LTR_A,		msg_LTR_A_LEN, msg_ENABLE,		EnableDisabledMainMenu},
	{  6, 2, themeWhiteBlack,	msg_LTR_B,		msg_LTR_B_LEN, msg_STEPS,		GetCCLSteps},
	{  7, 2, themeWhiteBlack,	msg_LTR_C,		msg_LTR_C_LEN, msg_INPUTSOURCE,	CCLInputSourceMainMenu},
	{  8, 2, themeWhiteBlack,	msg_LTR_D,		msg_LTR_D_LEN, msg_PATTERN,		GetCCLPattern},
	{  11, 2, themeWhiteBlack,	msg_LTR_X,		msg_LTR_X_LEN, msg_MainMenu,	DisplayMainMenu}
};


static const DataEntry CCLConfigDataEntry[] PROGMEM = {
	{  5, 32, themeCyanBlack,	offsetof(struct CCLConfiguration, enabled),		FIELDTYPE_BOOL,		EnableDisableDecode},
	{  6, 32, themeCyanBlack,	offsetof(struct CCLConfiguration, steps),		FIELDTYPE_UINT8,	DecodeUint8_t},
	{  7, 32, themeCyanBlack,	offsetof(struct CCLConfiguration, inputSource),	FIELDTYPE_UINT8,	CCLInputSourceDecode},
	{  8, 32, themeCyanBlack,	offsetof(struct CCLConfiguration, pattern),		FIELDTYPE_UINT8,	DecodeUint8_t}
};

MenuSystem CCLMainMenu = {
	.menuHeader = CCLHeader,
	.menuHeaderSize = sizeof(CCLHeader) / sizeof(MenuHeader),
	.menuBody = CCLBody,
	.menuBodySize = sizeof(CCLBody) / sizeof(MenuBody),
	.dataEntry = CCLConfigDataEntry,
	.dataEntrySize = sizeof(CCLConfigDataEntry) / sizeof(DataEntry),
	.dataptr = NULL,
	.dataEntryMode = DATAENTRYMODE_DISPLAYDATA,
	.screenArea = {  0, 0, 0, 0, themeWhiteBlack},
	.callingMenu = NULL,
	.saveConfig = {0, NULL}	
};


void CCL_Menu(const char *msg)
{
	CCLMainMenu.dataptr = GetPtrToCCLData();
	CCLMainMenu.saveConfig.Savefuncptr = CCL_Save_Config;
	CCLMainMenu.saveConfig.index = 0;
	SetActiveMenu(&CCLMainMenu);
}

