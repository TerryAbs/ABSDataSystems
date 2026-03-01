/*
 * Menu_PORTS.c
 *
 * Created: 11/28/2025 9:23:25 AM
 *  Author: terry
 */ 
#include <avr/pgmspace.h>
#include "Ports/Ports.h"
#include "Menu_PORTS.h"
#include "Common/DataStructures.h"
#include "CommonStrings/CommonStrings.h"
#include "Menu_Report_Processor/Themes/Themes.h"

static const MenuHeader PortsHeader[] PROGMEM = {
	{  1, 0, themeWhiteBlack,	msg_EQUAL, msg_EQUAL_LEN},
	{  2, 0, themeYellowBlack,	msg_PortsMenu, msg_PortsMenu_LEN},
	{  3, 0, themeWhiteBlack,	msg_EQUAL, msg_EQUAL_LEN}
};


static const MenuBody PortsBody[] PROGMEM = {
	{  5, 2, themeWhiteBlack,	msg_LTR_A,		msg_LTR_A_LEN, msg_ENABLE,					EnableDisabledMainMenu},
	{  6, 2, themeWhiteBlack,	msg_LTR_B,		msg_LTR_B_LEN, msg_PortDirectionPC3,		PortDirectionMainMenu},
	{  7, 2, themeWhiteBlack,	msg_LTR_C,		msg_LTR_C_LEN, msg_OutputDefault,			OutputHighLowMainMenu},
	{  8, 2, themeWhiteBlack,	msg_LTR_D,		msg_LTR_D_LEN, msg_PortInputLevelPC3,		PortInputLevelMainMenu},
	{  9, 2, themeWhiteBlack,	msg_LTR_E,		msg_LTR_E_LEN, msg_PortInvertedPC3,			PortInvertedMainMenu},
	{  10, 2, themeWhiteBlack,	msg_LTR_F,		msg_LTR_F_LEN, msg_PortPullUpResistoPC3,	PortResistorsMainMenu},
	{  11, 2, themeWhiteBlack,	msg_LTR_G,		msg_LTR_G_LEN, msg_PortInputLevelPF6,		PortInputLevelMainMenu},
	{  12, 2, themeWhiteBlack,	msg_LTR_H,		msg_LTR_H_LEN, msg_PortInvertedPF6,			PortInvertedMainMenu},		
	{  13, 2, themeWhiteBlack,	msg_LTR_I,		msg_LTR_I_LEN, msg_PortPullUpResistoPF6,	PortResistorsMainMenu},		
	{  14, 2, themeWhiteBlack,	msg_LTR_X,		msg_LTR_X_LEN, msg_MainMenu,				DisplayMainMenu}
};

static const DataEntry PortsConfigDataEntry[] PROGMEM = {
	{  5, 32, themeCyanBlack,	offsetof(struct PortsConfiguration, enabled),			FIELDTYPE_BOOL,		EnableDisableDecode},
	{  6, 32, themeCyanBlack,	offsetof(struct PortsConfiguration, portAOutput),		FIELDTYPE_BOOL,		PortDirectionDecode},
	{  7, 32, themeCyanBlack,	offsetof(struct PortsConfiguration, portAOutputValue),	FIELDTYPE_BOOL,		HighLowDecode},
	{  8, 32, themeCyanBlack,	offsetof(struct PortsConfiguration, portAInputLevel),	FIELDTYPE_BOOL,		PortInputLevelDecode},
	{  9, 32, themeCyanBlack,	offsetof(struct PortsConfiguration, portAInverted),		FIELDTYPE_BOOL,		PortInvertedDecode},
	{  10, 32, themeCyanBlack,	offsetof(struct PortsConfiguration, portAPullUp),		FIELDTYPE_BOOL,		PortResistorDecode},
	{  11, 32, themeCyanBlack,	offsetof(struct PortsConfiguration, portBInputLevel),	FIELDTYPE_BOOL,		PortInputLevelDecode},
	{  12, 32, themeCyanBlack,	offsetof(struct PortsConfiguration, portBInverted),		FIELDTYPE_BOOL,		PortInvertedDecode},
	{  13, 32, themeCyanBlack,	offsetof(struct PortsConfiguration, portBPullUp),		FIELDTYPE_BOOL,		PortResistorDecode},						
};

MenuSystem PortsMainMenu = {
	.menuHeader = PortsHeader,
	.menuHeaderSize = sizeof(PortsHeader) / sizeof(MenuHeader),
	.menuBody = PortsBody,
	.menuBodySize = sizeof(PortsBody) / sizeof(MenuBody),
	.dataEntry = PortsConfigDataEntry,
	.dataEntrySize = sizeof(PortsConfigDataEntry) / sizeof(DataEntry),
	.dataptr = NULL,
	.dataEntryMode = DATAENTRYMODE_DISPLAYDATA,
	.screenArea = {  0, 0, 0, 0, themeWhiteBlack},
	.callingMenu = NULL,
	.saveConfig = {0, NULL}
};

void Ports_Menu(const char *msg)
{
	PortsMainMenu.dataptr = GetPtrToPortsData();
	PortsMainMenu.saveConfig.Savefuncptr = Ports_Save_Config;
	PortsMainMenu.saveConfig.index = 0;	
	SetActiveMenu(&PortsMainMenu);
}
