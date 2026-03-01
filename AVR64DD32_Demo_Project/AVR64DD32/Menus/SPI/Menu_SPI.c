/*
 * Menu_SPI.c
 *
 * Created: 11/28/2025 9:23:25 AM
 *  Author: terry
 */ 
#include <avr/pgmspace.h>
#include "SPI/SPI.h"
#include "Menu_SPI.h"
#include "Common/DataStructures.h"
#include "CommonStrings/CommonStrings.h"
#include "Menu_Report_Processor/Themes/Themes.h"

static const MenuHeader SPIHeader[] PROGMEM = {
	{  1, 0, themeWhiteBlack,	msg_EQUAL, msg_EQUAL_LEN},
	{  2, 0, themeYellowBlack,	msg_SPIMenu, msg_SPIMenu_LEN},
	{  3, 0, themeWhiteBlack,	msg_EQUAL, msg_EQUAL_LEN}
};

static const MenuBody SPIBody[] PROGMEM = {
	{  5, 2, themeWhiteBlack,	msg_LTR_A,		msg_LTR_A_LEN, msg_ENABLE,		EnableDisabledMainMenu},
	{  6, 2, themeWhiteBlack,	msg_LTR_X,		msg_LTR_X_LEN, msg_MainMenu,	DisplayMainMenu}
};


static const DataEntry SPIConfigDataEntry[] PROGMEM = {
	{  5, 32, themeCyanBlack,	offsetof(struct SPIConfiguration, enabled),		FIELDTYPE_BOOL,		EnableDisableDecode},
};


MenuSystem SPIMainMenu = {
	.menuHeader = SPIHeader,
	.menuHeaderSize = sizeof(SPIHeader) / sizeof(MenuHeader),
	.menuBody = SPIBody,
	.menuBodySize = sizeof(SPIBody) / sizeof(MenuBody),
	.dataEntry = SPIConfigDataEntry,
	.dataEntrySize = sizeof(SPIConfigDataEntry) / sizeof(DataEntry),
	.dataptr = NULL,
	.dataEntryMode = DATAENTRYMODE_DISPLAYDATA,
	.screenArea = {  0, 0, 0, 0, themeWhiteBlack},
	.callingMenu = NULL,
	.saveConfig = {0, NULL}			
};

void SPI_Menu(const char *msg)
{
	SPIMainMenu.dataptr = GetPtrToSPIData();
	SPIMainMenu.saveConfig.Savefuncptr = SPI_Save_Config;
	SPIMainMenu.saveConfig.index = 0;
	SetActiveMenu(&SPIMainMenu);
}
