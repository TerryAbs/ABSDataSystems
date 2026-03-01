/*
 * Menu_Timers.c
 *
 * Created: 11/28/2025 9:23:25 AM
 *  Author: terry
 */ 
#include <avr/pgmspace.h>
#include "Timers/Timers.h"
#include "SPI/SPI.h"
#include "Menu_Timers.h"
#include "Common/DataStructures.h"
#include "CommonStrings/CommonStrings.h"
#include "Menu_Report_Processor/Themes/Themes.h"

static const MenuHeader TimersHeader[] PROGMEM = {
	{  1, 0, themeWhiteBlack,	msg_EQUAL, msg_EQUAL_LEN},
	{  2, 0, themeYellowBlack,	msg_TimersMenu, msg_TimersMenu_LEN},
	{  3, 0, themeWhiteBlack,	msg_EQUAL, msg_EQUAL_LEN}
};


static const MenuBody TimersBody[] PROGMEM = {
	{  5, 2, themeWhiteBlack,	msg_LTR_A,		msg_LTR_A_LEN, msg_ENABLE,		EnableDisabledMainMenu},
	{  6, 2, themeWhiteBlack,	msg_LTR_B,		msg_LTR_B_LEN, msg_WaveformMode, WaveformMainMenu},
	{  7, 2, themeWhiteBlack,	msg_LTR_C,		msg_LTR_C_LEN, msg_ClkSelMode,	ClkSelectMainMenu},
	{  8, 2, themeWhiteBlack,	msg_LTR_D,		msg_LTR_D_LEN, msg_PeriodMode, GetTimersPeriod},
	{  9, 2, themeWhiteBlack,	msg_LTR_E,		msg_LTR_E_LEN, msg_CompareMode, GetTimersCompare},
	{  10, 2, themeWhiteBlack,	msg_LTR_F,		msg_LTR_F_LEN, msg_DutyCycleMode, DutyCycleMainMenu},
	{  11, 2, themeWhiteBlack,	msg_LTR_X,		msg_LTR_X_LEN, msg_MainMenu,	DisplayMainMenu}
};


static const DataEntry TimersConfigDataEntry[] PROGMEM = {
	{  5, 32, themeCyanBlack,	offsetof(struct TimersConfiguration, enabled),		FIELDTYPE_BOOL,		EnableDisableDecode},
	{  6, 32, themeCyanBlack,	offsetof(struct TimersConfiguration, waveform),		FIELDTYPE_UINT8,	WaveformModeDecode},
	{  7, 32, themeCyanBlack,	offsetof(struct TimersConfiguration, clksel),		FIELDTYPE_UINT8,	ClkselModeDecode},
	{  8, 32, themeCyanBlack,	offsetof(struct TimersConfiguration, period),		FIELDTYPE_UINT8,	DecodeUint16_t},
	{  9, 32, themeCyanBlack,	offsetof(struct TimersConfiguration, compare),		FIELDTYPE_UINT8,	DecodeUint16_t},
	{  10, 32, themeCyanBlack,	offsetof(struct TimersConfiguration, dutyCycle),	FIELDTYPE_UINT8,	DutyCycleModeDecode},
};

MenuSystem TimersMainMenu = {
	.menuHeader = TimersHeader,
	.menuHeaderSize = sizeof(TimersHeader) / sizeof(MenuHeader),
	.menuBody = TimersBody,
	.menuBodySize = sizeof(TimersBody) / sizeof(MenuBody),
	.dataEntry = TimersConfigDataEntry,
	.dataEntrySize = sizeof(TimersConfigDataEntry) / sizeof(DataEntry),
	.dataptr = NULL,
	.dataEntryMode = DATAENTRYMODE_DISPLAYDATA,
	.screenArea = {  0, 0, 0, 0, themeWhiteBlack},
	.callingMenu = NULL,
	.saveConfig = {0, NULL}
};

void Timers_Menu(const char *msg)
{
	TimersMainMenu.dataptr = GetPtrToTimersData();
	TimersMainMenu.saveConfig.Savefuncptr = Timers_Save_Config;
	TimersMainMenu.saveConfig.index = 0;
	SetActiveMenu(&TimersMainMenu);
}
