/*
 * Menu_USART.c
 *
 * Created: 11/28/2025 9:23:25 AM
 *  Author: terry
 */ 
#include <avr/pgmspace.h>
#include "USART/USART.h"
#include "Menu_USART.h"
#include "Common/DataStructures.h"
#include "CommonStrings/CommonStrings.h"
#include "Menu_Report_Processor/Themes/Themes.h"

static const MenuHeader USARTHeader[] PROGMEM = {
	{  1, 0, themeWhiteBlack,	msg_EQUAL, msg_EQUAL_LEN},
	{  2, 0, themeYellowBlack,	msg_USARTMenu, msg_USARTMenu_LEN},
	{  3, 0, themeWhiteBlack,	msg_EQUAL, msg_EQUAL_LEN}
};

static const MenuBody USARTBody[] PROGMEM = {
	{  5, 2, themeWhiteBlack,	msg_LTR_A,		msg_LTR_A_LEN,	msg_ENABLE,		EnableDisabledMainMenu},
	{  6, 2, themeWhiteBlack,	msg_LTR_B,		msg_LTR_B_LEN,	msg_BaudRate,	BaudRateMainMenu},
	{  7, 2, themeWhiteBlack,	msg_LTR_C,		msg_LTR_C_LEN,	msg_Parity,		ParityMainMenu},
	{  8, 2, themeWhiteBlack,	msg_LTR_D,		msg_LTR_D_LEN,	msg_StopBits,	StopBitsMainMenu},
	{  9, 2, themeWhiteBlack,	msg_LTR_E,		msg_LTR_E_LEN,	msg_DataBits,	DataBitsMainMenu},
	{  10, 2, themeWhiteBlack,	msg_LTR_X,		msg_LTR_X_LEN,	msg_MainMenu,	DisplayMainMenu}
};


static const DataEntry USARTConfigDataEntry[] PROGMEM = {
	{  5, 32, themeCyanBlack,	offsetof(struct USARTConfiguration, enabled),		FIELDTYPE_BOOL,		EnableDisableDecode},
	{  6, 32, themeCyanBlack,	offsetof(struct USARTConfiguration, baud_rate),		FIELDTYPE_UINT8,	DecodeBaudRate},		
	{  7, 32, themeCyanBlack,	offsetof(struct USARTConfiguration, parity),		FIELDTYPE_UINT8,	DecodeParity},
	{  8, 32, themeCyanBlack,	offsetof(struct USARTConfiguration, stop_bits),		FIELDTYPE_UINT8,	DecodeStopbits},
	{  9, 32, themeCyanBlack,	offsetof(struct USARTConfiguration, data_bits),		FIELDTYPE_UINT8,	DecodeDataBits},
};

MenuSystem USARTMainMenu = {
	.menuHeader = USARTHeader,
	.menuHeaderSize = sizeof(USARTHeader) / sizeof(MenuHeader),
	.menuBody = USARTBody,
	.menuBodySize = sizeof(USARTBody) / sizeof(MenuBody),
	.dataEntry = USARTConfigDataEntry,
	.dataEntrySize = sizeof(USARTConfigDataEntry) / sizeof(DataEntry),
	.dataptr = NULL,
	.dataEntryMode = DATAENTRYMODE_DISPLAYDATA,
	.screenArea = {  0, 0, 0, 0, themeWhiteBlack},
	.callingMenu = NULL,
	.saveConfig = {0, NULL}
};

void USART_Menu(const char *msg)
{
	USARTMainMenu.dataptr = GetPtrToUSARTData();
	USARTMainMenu.saveConfig.Savefuncptr = USART_Save_Config;
	USARTMainMenu.saveConfig.index = 0;
	SetActiveMenu(&USARTMainMenu);
}


static const MenuHeader BaudRateHeader[] PROGMEM = {
	{  4, 20, themeRedWhite,	msg_EQUALLONG, msg_EQUAL_LEN},
	{  5, 25, themeBlueWhite,	msg_BaudRateMenu, msg_BaudRateMenu_LEN},
	{  6, 20, themeRedWhite,	msg_EQUALLONG, msg_EQUAL_LEN}
};

static const MenuBody BaudRateBody[] PROGMEM = {
	{  8, 22, themeBlueWhite,	msg_LTR_A, 		msg_LTR_A_LEN,	msgBaudRate_300,	 NULL},
	{  9, 22, themeBlueWhite,	msg_LTR_B, 		msg_LTR_B_LEN,	msgBaudRate_600, 	 NULL},
	{ 10, 22, themeBlueWhite,	msg_LTR_C, 		msg_LTR_C_LEN,	msgBaudRate_1200, 	 NULL},
	{ 11, 22, themeBlueWhite,	msg_LTR_D, 		msg_LTR_D_LEN,	msgBaudRate_2400, 	 NULL},
	{ 12, 22, themeBlueWhite,	msg_LTR_E, 		msg_LTR_E_LEN,	msgBaudRate_4800, 	 NULL},
	{ 13, 22, themeBlueWhite,	msg_LTR_F, 		msg_LTR_F_LEN,	msgBaudRate_9600, 	 NULL},
	{ 14, 22, themeBlueWhite,	msg_LTR_G, 		msg_LTR_G_LEN,	msgBaudRate_14400, 	 NULL},
	{ 15, 22, themeBlueWhite,	msg_LTR_H, 		msg_LTR_H_LEN,	msgBaudRate_19200,	 NULL},
	{ 16, 22, themeBlueWhite,	msg_LTR_I, 		msg_LTR_I_LEN,	msgBaudRate_28800, 	 NULL},
	{ 17, 22, themeBlueWhite,	msg_LTR_J, 		msg_LTR_J_LEN,	msgBaudRate_38400, 	 NULL},
	{ 18, 22, themeBlueWhite,	msg_LTR_K, 		msg_LTR_K_LEN,	msgBaudRate_57600, 	 NULL},
	{ 19, 22, themeBlueWhite,	msg_LTR_L, 		msg_LTR_L_LEN,	msgBaudRate_115200,  NULL},
	{ 20, 22, themeBlueWhite,	msg_LTR_M, 		msg_LTR_M_LEN,	msgBaudRate_230400,  NULL},
	{ 21, 22, themeBlueWhite,	msg_LTR_N, 		msg_LTR_N_LEN,	msgBaudRate_460800,  NULL},
	{ 22, 22, themeBlueWhite,	msg_LTR_O, 		msg_LTR_O_LEN,	msgBaudRate_921600,	 NULL},
	{ 23, 22, themeBlueWhite,	msg_LTR_X,		msg_LTR_X_LEN,	msg_ExitMenu,  NULL},
};

static const DataEntry BaudRateDataEntry[] PROGMEM = {
	{  8, 22, themeBlueWhite,	BAUDRATES_300, 		FIELDTYPE_UINT8,		NULL, NULL},
	{  9, 22, themeBlueWhite,	BAUDRATES_600, 		FIELDTYPE_UINT8,		NULL, NULL},
	{ 10, 22, themeBlueWhite,	BAUDRATES_1200,		FIELDTYPE_UINT8,		NULL, NULL},
	{ 11, 22, themeBlueWhite,	BAUDRATES_2400,		FIELDTYPE_UINT8,		NULL, NULL},
	{ 12, 22, themeBlueWhite,	BAUDRATES_4800,		FIELDTYPE_UINT8,		NULL, NULL},
	{ 13, 22, themeBlueWhite,	BAUDRATES_9600,		FIELDTYPE_UINT8,		NULL, NULL},
	{ 14, 22, themeBlueWhite,	BAUDRATES_14400,	FIELDTYPE_UINT8,		NULL, NULL},
	{ 15, 22, themeBlueWhite,	BAUDRATES_19200,	FIELDTYPE_UINT8,		NULL, NULL},
	{ 16, 22, themeBlueWhite,	BAUDRATES_28800,	FIELDTYPE_UINT8,		NULL, NULL},
	{ 17, 22, themeBlueWhite,	BAUDRATES_38400,	FIELDTYPE_UINT8,		NULL, NULL},
	{ 18, 22, themeBlueWhite,	BAUDRATES_57600,	FIELDTYPE_UINT8,		NULL, NULL},
	{ 19, 22, themeBlueWhite,	BAUDRATES_115200,	FIELDTYPE_UINT8,		NULL, NULL},
	{ 20, 22, themeBlueWhite,	BAUDRATES_230400,	FIELDTYPE_UINT8,		NULL, NULL},
	{ 21, 22, themeBlueWhite,	BAUDRATES_460800,	FIELDTYPE_UINT8,		NULL, NULL},
	{ 22, 22, themeBlueWhite,	BAUDRATES_921600,	FIELDTYPE_UINT8,		NULL, NULL}

};

MenuSystem BaudRateMainMenuSystem = {
	.menuHeader = BaudRateHeader,
	.menuHeaderSize = sizeof(BaudRateHeader) / sizeof(MenuHeader),
	.menuBody = BaudRateBody,
	.menuBodySize = sizeof(BaudRateBody) / sizeof(MenuBody),
	.dataEntry = BaudRateDataEntry,
	.dataEntrySize = sizeof(BaudRateDataEntry) / sizeof(DataEntry),
	.dataptr = NULL,
	.dataEntryMode = DATAENTRYMODE_DROPDOWN,
	.screenArea = {  2, 17, 24, 50, themeBlueWhite},
	.callingMenu = NULL,
	.saveConfig = {0, NULL}
};

void BaudRateMainMenu(const char *msg)
{
	BaudRateMainMenuSystem.dataptr = GetDataPtrForMenuItem(msg);
	ApplySaveConfigFromParentMenu(&BaudRateMainMenuSystem);
	SetActiveMenu(&BaudRateMainMenuSystem);
}

static const MenuHeader ParityHeader[] PROGMEM = {
	{  4, 20, themeRedWhite,	msg_EQUALLONG, msg_EQUAL_LEN},
	{  5, 25, themeBlueWhite,	msg_ParityMenu, msg_ParityMenu_LEN},
	{  6, 20, themeRedWhite,	msg_EQUALLONG, msg_EQUAL_LEN}
};

static const MenuBody ParityBody[] PROGMEM = {
	{  8, 22, themeBlueWhite,	msg_LTR_A,		msg_LTR_A_LEN, msg_None,	 NULL},
	{  9, 22, themeBlueWhite,	msg_LTR_B,		msg_LTR_B_LEN, msg_Even, NULL},
	{  10, 22, themeBlueWhite,	msg_LTR_C,		msg_LTR_C_LEN, msg_Odd, NULL},
	{  11, 22, themeBlueWhite,	msg_LTR_X,		msg_LTR_X_LEN, msg_ExitMenu,  NULL},
};

static const DataEntry ParityDataEntry[] PROGMEM = {
	{  8, 22, themeBlueWhite,	USART_PMODE_DISABLED_gc,	FIELDTYPE_UINT8,		NULL, NULL},
	{  9, 22, themeBlueWhite,	USART_PMODE_EVEN_gc,		FIELDTYPE_UINT8,		NULL, NULL},
	{ 10, 22, themeBlueWhite,	USART_PMODE_ODD_gc,			FIELDTYPE_UINT8,		NULL, NULL},
};

MenuSystem ParityMainMenuSystem = {
	.menuHeader = ParityHeader,
	.menuHeaderSize = sizeof(ParityHeader) / sizeof(MenuHeader),
	.menuBody = ParityBody,
	.menuBodySize = sizeof(ParityBody) / sizeof(MenuBody),
	.dataEntry = ParityDataEntry,
	.dataEntrySize = sizeof(ParityDataEntry) / sizeof(DataEntry),
	.dataptr = NULL,
	.dataEntryMode = DATAENTRYMODE_DROPDOWN,
	.screenArea = {  3, 17, 13, 50, themeBlueWhite},
	.callingMenu = NULL,
	.saveConfig = {0, NULL}
};

void ParityMainMenu(const char *msg)
{
	ParityMainMenuSystem.dataptr = GetDataPtrForMenuItem(msg);
	ApplySaveConfigFromParentMenu(&ParityMainMenuSystem);
	SetActiveMenu(&ParityMainMenuSystem);
}
   
static const MenuHeader StopBitsHeader[] PROGMEM = {
	{  4, 20, themeRedWhite,	msg_EQUALLONG, msg_EQUAL_LEN},
	{  5, 25, themeBlueWhite,	msg_StopBitsMenu, msg_StopBitsMenu_LEN},
	{  6, 20, themeRedWhite,	msg_EQUALLONG, msg_EQUAL_LEN}
};

static const MenuBody StopBitsBody[] PROGMEM = {
	{  8, 22, themeBlueWhite,	msg_LTR_A,		msg_LTR_A_LEN, msg_Num_1,	 NULL},
	{  9, 22, themeBlueWhite,	msg_LTR_B,		msg_LTR_B_LEN, msg_Num_2, NULL},
	{  10, 22, themeBlueWhite,	msg_LTR_X,		msg_LTR_X_LEN, msg_ExitMenu,  NULL},
};

static const DataEntry StopBitsDataEntry[] PROGMEM = {
	{  8, 22, themeBlueWhite,	USART_SBMODE_1BIT_gc,	FIELDTYPE_BOOL,		NULL, NULL},
	{  9, 22, themeBlueWhite,	USART_SBMODE_2BIT_gc,	FIELDTYPE_BOOL,		NULL, NULL},
};

MenuSystem StopBitsMainMenuSystem = {
	.menuHeader = StopBitsHeader,
	.menuHeaderSize = sizeof(StopBitsHeader) / sizeof(MenuHeader),
	.menuBody = StopBitsBody,
	.menuBodySize = sizeof(StopBitsBody) / sizeof(MenuBody),
	.dataEntry = StopBitsDataEntry,
	.dataEntrySize = sizeof(StopBitsDataEntry) / sizeof(DataEntry),
	.dataptr = NULL,
	.dataEntryMode = DATAENTRYMODE_DROPDOWN,
	.screenArea = {  3, 17, 12, 50, themeBlueWhite},
	.callingMenu = NULL,
	.saveConfig = {0, NULL}
};

void StopBitsMainMenu(const char *msg)
{
	StopBitsMainMenuSystem.dataptr = GetDataPtrForMenuItem(msg);
	ApplySaveConfigFromParentMenu(&StopBitsMainMenuSystem);
	SetActiveMenu(&StopBitsMainMenuSystem);
}

static const MenuHeader DataBitsHeader[] PROGMEM = {
	{  4, 20, themeRedWhite,	msg_EQUALLONG, msg_EQUAL_LEN},
	{  5, 25, themeBlueWhite,	msg_DataBitsMenu, msg_DataBitsMenu_LEN},
	{  6, 20, themeRedWhite,	msg_EQUALLONG, msg_EQUAL_LEN}
};

static const MenuBody DataBitsBody[] PROGMEM = {
	{  8, 22, themeBlueWhite,	msg_LTR_A,		msg_LTR_A_LEN, msg_Num_5,	 NULL},
	{  9, 22, themeBlueWhite,	msg_LTR_B,		msg_LTR_B_LEN, msg_Num_6,	 NULL},
	{ 10, 22, themeBlueWhite,	msg_LTR_C,		msg_LTR_C_LEN, msg_Num_7,	 NULL},
	{ 11, 22, themeBlueWhite,	msg_LTR_D,		msg_LTR_D_LEN, msg_Num_8,	 NULL},
	{ 12, 22, themeBlueWhite,	msg_LTR_E,		msg_LTR_E_LEN, msg_Num_9,	 NULL},
	{ 13, 22, themeBlueWhite,	msg_LTR_X,		msg_LTR_X_LEN, msg_ExitMenu,  NULL},
};

static const DataEntry DataBitsDataEntry[] PROGMEM = {
	{  8, 22, themeBlueWhite,	USART_CHSIZE_5BIT_gc,	FIELDTYPE_UINT8,		NULL, NULL},
	{  9, 22, themeBlueWhite,	USART_CHSIZE_6BIT_gc,	FIELDTYPE_UINT8,		NULL, NULL},
	{ 10, 22, themeBlueWhite,	USART_CHSIZE_7BIT_gc,	FIELDTYPE_UINT8,		NULL, NULL},
	{ 11, 22, themeBlueWhite,	USART_CHSIZE_8BIT_gc,	FIELDTYPE_UINT8,		NULL, NULL},
	{ 12, 22, themeBlueWhite,	USART_CHSIZE_9BITL_gc,	FIELDTYPE_UINT8,		NULL, NULL},
};

MenuSystem DataBitsMainMenuSystem = {
	.menuHeader = DataBitsHeader,
	.menuHeaderSize = sizeof(DataBitsHeader) / sizeof(MenuHeader),
	.menuBody = DataBitsBody,
	.menuBodySize = sizeof(DataBitsBody) / sizeof(MenuBody),
	.dataEntry = DataBitsDataEntry,
	.dataEntrySize = sizeof(DataBitsDataEntry) / sizeof(DataEntry),
	.dataptr = NULL,
	.dataEntryMode = DATAENTRYMODE_DROPDOWN,
	.screenArea = {  3, 17, 15, 50, themeBlueWhite},
	.callingMenu = NULL,
	.saveConfig = {0, NULL}
};

void DataBitsMainMenu(const char *msg)
{
	DataBitsMainMenuSystem.dataptr = GetDataPtrForMenuItem(msg);
	ApplySaveConfigFromParentMenu(&DataBitsMainMenuSystem);
	SetActiveMenu(&DataBitsMainMenuSystem);
}
