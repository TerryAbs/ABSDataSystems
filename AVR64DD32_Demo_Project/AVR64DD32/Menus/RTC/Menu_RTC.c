/*
 * Menu_RTC.c
 *
 * Created: 11/28/2025 9:23:25 AM
 *  Author: terry
 */ 
#include <avr/pgmspace.h>
#include "RTC/RTC.h"
#include "Menu_RTC.h"
#include "Common/DataStructures.h"
#include "CommonStrings/CommonStrings.h"
#include "Menu_Report_Processor/Themes/Themes.h"

static const MenuHeader RTCHeader[] PROGMEM = {
	{  1, 0, themeWhiteBlack,	msg_EQUAL, msg_EQUAL_LEN},
	{  2, 0, themeYellowBlack,	msg_RTCMenu, msg_RTCMenu_LEN},
	{  3, 0, themeWhiteBlack,	msg_EQUAL, msg_EQUAL_LEN}
};


static const MenuBody RTCBody[] PROGMEM = {
	{  5, 2, themeWhiteBlack,	msg_LTR_A,		msg_LTR_A_LEN, msg_ENABLE,		EnableDisabledMainMenu},
	{  6, 2, themeWhiteBlack,	msg_LTR_X,		msg_LTR_X_LEN, msg_MainMenu,	DisplayMainMenu}
};


static const DataEntry RTCConfigDataEntry[] PROGMEM = {
	{  5, 32, themeCyanBlack,	offsetof(struct RTCConfiguration, enabled),		FIELDTYPE_BOOL,		EnableDisableDecode},
};


MenuSystem RTCMainMenu = {
	.menuHeader = RTCHeader,
	.menuHeaderSize = sizeof(RTCHeader) / sizeof(MenuHeader),
	.menuBody = RTCBody,
	.menuBodySize = sizeof(RTCBody) / sizeof(MenuBody),
	.dataEntry = RTCConfigDataEntry,
	.dataEntrySize = sizeof(RTCConfigDataEntry) / sizeof(DataEntry),
	.dataptr = NULL,
	.dataEntryMode = DATAENTRYMODE_DISPLAYDATA,
	.screenArea = {  0, 0, 0, 0, themeWhiteBlack},
	.callingMenu = NULL,
	.saveConfig = {0, NULL}
};


void RTC_Menu(const char *msg)
{
	RTCMainMenu.dataptr = GetPtrToRTCData();
	RTCMainMenu.saveConfig.Savefuncptr = RTC_Save_Config;
	RTCMainMenu.saveConfig.index = 0;	
	SetActiveMenu(&RTCMainMenu);
}
