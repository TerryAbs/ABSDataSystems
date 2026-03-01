/*
 * Menu_ZCD.c
 *
 * Created: 11/28/2025 9:23:25 AM
 *  Author: terry
 */ 
#include <avr/pgmspace.h>
#include "ZCD/ZCD.h"
#include "Menu_ZCD.h"
#include "Common/DataStructures.h"
#include "CommonStrings/CommonStrings.h"
#include "Menu_Report_Processor/Themes/Themes.h"

static const MenuHeader ZCDHeader[] PROGMEM = {
	{  1, 0, themeWhiteBlack,	msg_EQUAL, msg_EQUAL_LEN},
	{  2, 0, themeYellowBlack,	msg_ZCDMenu, msg_ZCDMenu_LEN},
	{  3, 0, themeWhiteBlack,	msg_EQUAL, msg_EQUAL_LEN}
};

static const MenuBody ZCDBody[] PROGMEM = {
	{  5, 2, themeWhiteBlack,	msg_LTR_A,		msg_LTR_A_LEN, msg_ENABLE,			EnableDisabledMainMenu},
	{  6, 2, themeWhiteBlack,	msg_LTR_B,		msg_LTR_B_LEN, msg_LoopDelay,		GetLoopDelay},
	{  7, 2, themeWhiteBlack,	msg_LTR_C,		msg_LTR_C_LEN, msg_InterruptEdge,	InterruptEdgeMainMenu},
	{  8, 2, themeWhiteBlack,	msg_LTR_X,		msg_LTR_X_LEN, msg_MainMenu,		DisplayMainMenu}
};


static const DataEntry ZCDConfigDataEntry[] PROGMEM = {
	{  5, 32, themeCyanBlack,	offsetof(struct ZCDConfiguration, enabled),		FIELDTYPE_BOOL,		EnableDisableDecode},
	{  6, 32, themeCyanBlack,	offsetof(struct ZCDConfiguration, loopDelay),	FIELDTYPE_UINT16,	DecodeUint16_t},
	{  7, 32, themeCyanBlack,	offsetof(struct ZCDConfiguration, edge),		FIELDTYPE_UINT8,	InterruptEdgeDecode},
	
};


MenuSystem ZCDMainMenu = {
	.menuHeader = ZCDHeader,
	.menuHeaderSize = sizeof(ZCDHeader) / sizeof(MenuHeader),
	.menuBody = ZCDBody,
	.menuBodySize = sizeof(ZCDBody) / sizeof(MenuBody),
	.dataEntry = ZCDConfigDataEntry,
	.dataEntrySize = sizeof(ZCDConfigDataEntry) / sizeof(DataEntry),
	.dataptr = NULL,
	.dataEntryMode = DATAENTRYMODE_DISPLAYDATA,
	.screenArea = {  0, 0, 0, 0, themeWhiteBlack},
	.callingMenu = NULL,
	.saveConfig = {0, NULL}
};


void ZCD_Menu(const char *msg)
{
	ZCDMainMenu.dataptr = GetPtrToZCDData();
	ZCDMainMenu.saveConfig.Savefuncptr = ZCD_Save_Config;
	ZCDMainMenu.saveConfig.index = 0;
	SetActiveMenu(&ZCDMainMenu);
}
