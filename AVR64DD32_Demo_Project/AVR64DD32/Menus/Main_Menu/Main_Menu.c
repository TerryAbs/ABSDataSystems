/*
 * Main_Menu.c
 *
 * Created: 11/28/2025 9:23:25 AM
 *  Author: terry
 */ 
#include <avr/pgmspace.h>
#include "Main_Menu/Main_Menu.h"
#include "Menu_Report_Processor/Menu_Processor/Menu_Processor.h"
#include "Common/DataStructures.h"
#include "CommonStrings/CommonStrings.h"
#include "Menu_Report_Processor/Themes/Themes.h"

static const MenuHeader MainHeader[] PROGMEM = {
	{  1, 0, themeWhiteBlack,	msg_EQUAL, msg_EQUAL_LEN},
	{  2, 0, themeYellowBlack,	msg_MainMenu, msg_MainMenu_LEN},
	{  3, 0, themeWhiteBlack,	msg_EQUAL, msg_EQUAL_LEN}
};

static const MenuBody MainBody[] PROGMEM = {
	{  4, 2, themeWhiteBlack,	msg_ADC,		msg_ADC_LEN,	msg_ADC_Description,	ADC_Menu },
	{  5, 2, themeWhiteBlack,	msg_CCL,		msg_CCL_LEN,	msg_CCL_Description,	CCL_Menu },
	{  6, 2, themeWhiteBlack,	msg_DAC,		msg_DAC_LEN,	msg_DAC_Description,	DAC_Menu  },
	{  7, 2, themeWhiteBlack,	msg_Events,		msg_Events_LEN,	msg_Events_Description,	Events_Menu},
	{  8, 2, themeWhiteBlack,	msg_Ports,		msg_Ports_LEN,	msg_Ports_Description,	Ports_Menu },
	{  9, 2, themeWhiteBlack,	msg_RTC,		msg_RTC_LEN,	msg_RTC_Description,	RTC_Menu },
	{ 10, 2, themeWhiteBlack,	msg_SPI,		msg_SPI_LEN,	msg_SPI_Description,	SPI_Menu },
	{ 11, 2, themeWhiteBlack,	msg_Timers,		msg_Timers_LEN,	msg_Timers_Description,	Timers_Menu },
	{ 12, 2, themeWhiteBlack,	msg_TWI,		msg_TWI_LEN,	msg_TWI_Description,	TWI_Menu },
	{ 13, 2, themeWhiteBlack,	msg_USART,		msg_USART_LEN,	msg_USART_Description,	USART_Menu },
	{ 14, 2, themeWhiteBlack,	msg_ZCD,		msg_ZCD_LEN,	msg_ZCD_Description,	ZCD_Menu }
};


MenuSystem MainMenu = {
	.menuHeader = MainHeader,
	.menuHeaderSize = sizeof(MainHeader) / sizeof(MenuHeader),
	.menuBody = MainBody,
	.menuBodySize = sizeof(MainBody) / sizeof(MenuBody),
    .dataEntry = NULL,
    .dataEntrySize = 0,
	.dataptr = NULL,
	.dataEntryMode = DATAENTRYMODE_MENU,
	.screenArea = {  0, 0, 0, 0, themeWhiteBlack},
	.callingMenu = NULL,
	.saveConfig = {0, NULL}	
};


void MainMenu_Initialize(void)
{
	// Tell the Menu Processor to Process the Main enum
	DisplayMainMenu(NULL);
}

void DisplayMainMenu(const char *msg)
{
	// Tell the Menu Processor to Process the Main enum
	SetActiveMenu(&MainMenu);
}