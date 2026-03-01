/*
 * Menu_EVENTS.c
 *
 * Created: 11/28/2025 9:23:25 AM
 *  Author: terry
 */ 
#include <avr/pgmspace.h>
#include "Events/Events.h"
#include "Menu_EVENTS.h"
#include "Common/DataStructures.h"
#include "CommonStrings/CommonStrings.h"
#include "Menu_Report_Processor/Themes/Themes.h"

static const MenuHeader EventsHeader[] PROGMEM = {
	{  1, 0, themeWhiteBlack,	msg_EQUAL, msg_EQUAL_LEN},
	{  2, 0, themeYellowBlack,	msg_EventsMenu, msg_EventsMenu_LEN},
	{  3, 0, themeWhiteBlack,	msg_EQUAL, msg_EQUAL_LEN}
};

static const MenuBody EventsBody[] PROGMEM = {
	{  5, 2, themeWhiteBlack,	msg_LTR_A,		msg_LTR_A_LEN, msg_ENABLE,				EnableDisabledMainMenu},
	{  6, 2, themeWhiteBlack,	msg_LTR_B,		msg_LTR_B_LEN, msg_EventSystemModes,	EventSystemModesMainMenu},
	{  7, 2, themeWhiteBlack,	msg_LTR_X,		msg_LTR_X_LEN, msg_MainMenu,			DisplayMainMenu}
};


static const DataEntry EventsConfigDataEntry[] PROGMEM = {
	{  5, 32, themeCyanBlack,	offsetof(struct EventsConfiguration, enabled),	FIELDTYPE_BOOL,		EnableDisableDecode},
	{  6, 32, themeCyanBlack,	offsetof(struct EventsConfiguration, mode),		FIELDTYPE_UINT8,	EventsSystemModeDecode},
};

MenuSystem EventsMainMenu = {
	.menuHeader = EventsHeader,
	.menuHeaderSize = sizeof(EventsHeader) / sizeof(MenuHeader),
	.menuBody = EventsBody,
	.menuBodySize = sizeof(EventsBody) / sizeof(MenuBody),
	.dataEntry = EventsConfigDataEntry,
	.dataEntrySize = sizeof(EventsConfigDataEntry) / sizeof(DataEntry),
	.dataptr = NULL,
	.dataEntryMode = DATAENTRYMODE_DISPLAYDATA,
	.screenArea = {  0, 0, 0, 0, themeWhiteBlack},
	.callingMenu = NULL,
	.saveConfig = {0, NULL}
};

void Events_Menu(const char *msg)
{
	EventsMainMenu.dataptr = GetPtrToEventsData();
	EventsMainMenu.saveConfig.Savefuncptr = Events_Save_Config;
	EventsMainMenu.saveConfig.index = 0;
	SetActiveMenu(&EventsMainMenu);
}
