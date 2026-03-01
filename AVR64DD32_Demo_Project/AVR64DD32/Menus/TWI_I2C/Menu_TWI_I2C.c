/*
 * Menu_TWI_I2C.c
 *
 * Created: 11/28/2025 9:23:25 AM
 *  Author: terry
 */ 
#include <avr/pgmspace.h>
#include "TWI_I2C/TWI_I2C.h"
#include "Menu_TWI_I2C.h"
#include "Common/DataStructures.h"
#include "CommonStrings/CommonStrings.h"
#include "Menu_Report_Processor/Themes/Themes.h"

static const MenuHeader TWIHeader[] PROGMEM = {
	{  1, 0, themeWhiteBlack,	msg_EQUAL, msg_EQUAL_LEN},
	{  2, 0, themeYellowBlack,	msg_TWIMenu, msg_TWIMenu_LEN},
	{  3, 0, themeWhiteBlack,	msg_EQUAL, msg_EQUAL_LEN}
};

static const MenuBody TWIBody[] PROGMEM = {
	{  5, 2, themeWhiteBlack,	msg_LTR_A,		msg_LTR_A_LEN, msg_ENABLE,		EnableDisabledMainMenu},
	{  6, 2, themeWhiteBlack,	msg_LTR_X,		msg_LTR_X_LEN, msg_MainMenu,	DisplayMainMenu}
};


static const DataEntry TWIConfigDataEntry[] PROGMEM = {
	{  5, 32, themeCyanBlack,	offsetof(struct TWI_I2CConfiguration, enabled),		FIELDTYPE_BOOL,		EnableDisableDecode},
};

MenuSystem TWIMainMenu = {
	.menuHeader = TWIHeader,
	.menuHeaderSize = sizeof(TWIHeader) / sizeof(MenuHeader),
	.menuBody = TWIBody,
	.menuBodySize = sizeof(TWIBody) / sizeof(MenuBody),
    .dataEntry = TWIConfigDataEntry,
    .dataEntrySize = sizeof(TWIConfigDataEntry) / sizeof(DataEntry),
	.dataptr = NULL,
	.dataEntryMode = DATAENTRYMODE_DISPLAYDATA,
	.screenArea = {  0, 0, 0, 0, themeWhiteBlack},
	.callingMenu = NULL,
	.saveConfig = {0, NULL}	
};

void TWI_Menu(const char *msg)
{
	TWIMainMenu.dataptr = GetPtrToTWI_I2CData();
	TWIMainMenu.saveConfig.Savefuncptr = TWI_I2C_Save_Config;
	TWIMainMenu.saveConfig.index = 0;
	SetActiveMenu(&TWIMainMenu);
}
