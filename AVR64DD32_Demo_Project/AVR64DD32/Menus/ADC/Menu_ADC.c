/*
 * Menu_ADC.c
 *
 * Created: 11/28/2025 9:23:25 AM
 *  Author: terry
 */ 
#include <avr/pgmspace.h>
#include "Menu_ADC.h"
#include "ADC/ADC.h"
#include "Common/DataStructures.h"
#include "CommonStrings/CommonStrings.h"
#include "Menu_Report_Processor/Themes/Themes.h"

static const MenuHeader ADCHeader[] PROGMEM = {
	{  1, 0, themeWhiteBlack,	msg_EQUAL, msg_EQUAL_LEN},
	{  2, 0, themeYellowBlack,	msg_ADCMenu, msg_ADCMenu_LEN},
	{  3, 0, themeWhiteBlack,	msg_EQUAL, msg_EQUAL_LEN}
};


static const MenuBody ADCBody[] PROGMEM = {
	{  4, 2, themeWhiteBlack,	msg_Num_1,		msg_Num_1_LEN, msg_INSTANT, ADC_ConfigureMenu},
	{  5, 2, themeWhiteBlack,	msg_Num_2,		msg_Num_2_LEN, msg_AVERAGE, ADC_ConfigureMenu},
	{  6, 2, themeWhiteBlack,	msg_Num_3,		msg_Num_3_LEN, msg_DIFFERENTIAL, ADC_ConfigureMenu},
	{  7, 2, themeWhiteBlack,	msg_LTR_X,		msg_LTR_X_LEN, msg_MainMenu, DisplayMainMenu},		
};

MenuSystem ADCMainMenu = {
	.menuHeader = ADCHeader,
	.menuHeaderSize = sizeof(ADCHeader) / sizeof(MenuHeader),
	.menuBody = ADCBody,
	.menuBodySize = sizeof(ADCBody) / sizeof(MenuBody),
	.dataEntry = NULL,
	.dataEntrySize = 0,
	.dataptr = NULL,
	.dataEntryMode = DATAENTRYMODE_MENU,
	.screenArea = {  0, 0, 0, 0, themeWhiteBlack},
	.callingMenu = NULL,
	.saveConfig = {0, NULL}
};

void ADC_Menu(const char *msg)
{
	SetActiveMenu(&ADCMainMenu);
}

static const MenuHeader ADCConfigHeader[] PROGMEM = {
	{  1, 0, themeWhiteBlack,	msg_EQUAL, msg_EQUAL_LEN},
	{  2, 0, themeYellowBlack,	msg_ADCConfigurationMenu, msg_ADCConfigurationMenu_LEN},
	{  3, 0, themeWhiteBlack,	msg_EQUAL, msg_EQUAL_LEN}
};

static const MenuBody ADCConfigBody[] PROGMEM = {
	{  5, 2, themeWhiteBlack,	msg_LTR_A,		msg_LTR_A_LEN, msg_ENABLE,	 EnableDisabledMainMenu},
	{  6, 2, themeWhiteBlack,	msg_LTR_B,		msg_LTR_B_LEN, msg_REFRANGE, VRefMainMenu},
	{  7, 2, themeWhiteBlack,	msg_LTR_C,		msg_LTR_C_LEN, msg_SAMPNUM,	 ADC_SampleNumberMainMenu},
	{  8, 2, themeWhiteBlack,	msg_LTR_D,		msg_LTR_D_LEN, msg_SAMPLEDLY,ADC_SampleDelayMainMenu},
	{  9, 2, themeWhiteBlack,	msg_LTR_E,		msg_LTR_E_LEN, msg_BITS,     ADC_DataBitsMainMenu},
	{  10, 2, themeWhiteBlack,	msg_LTR_F,		msg_LTR_F_LEN, msg_PRESCALE, ADC_PreScaleMainMenu},
	{  11, 2, themeWhiteBlack,	msg_LTR_X,		msg_LTR_X_LEN, msg_ADCMenu,  ADC_Menu},		
};

static const DataEntry ADCConfigDataEntry[] PROGMEM = {
	{  5, 32, themeCyanBlack,	offsetof(struct ADCConfiguration, enabled),		FIELDTYPE_BOOL,		EnableDisableDecode},
	{  6, 32, themeCyanBlack,	offsetof(struct ADCConfiguration, refRange),	FIELDTYPE_UINT8,	RefDecode},
	{  7, 32, themeCyanBlack,	offsetof(struct ADCConfiguration, sampNum),		FIELDTYPE_UINT8,	ADCSampleNumberDecode},
	{  8, 32, themeCyanBlack,	offsetof(struct ADCConfiguration, sampleDly),	FIELDTYPE_UINT8,	ADCSampleDelayDecode},
	{  9, 32, themeCyanBlack,	offsetof(struct ADCConfiguration, noDataBits),	FIELDTYPE_UINT8,	ADC_BitsDecode},
	{  10,32, themeCyanBlack,	offsetof(struct ADCConfiguration, prescale),	FIELDTYPE_UINT8,	ADCPresScaleDecode}
};


MenuSystem ADCConfigMainMenu = {
	.menuHeader = ADCConfigHeader,
	.menuHeaderSize = sizeof(ADCConfigHeader) / sizeof(MenuHeader),
	.menuBody = ADCConfigBody,
	.menuBodySize = sizeof(ADCConfigBody) / sizeof(MenuBody),
	.dataEntry = ADCConfigDataEntry,
	.dataEntrySize= sizeof(ADCConfigDataEntry) / sizeof(DataEntry),
	.dataptr = NULL,
	.dataEntryMode = DATAENTRYMODE_DISPLAYDATA,
	.screenArea = {  0, 0, 0, 0, themeWhiteBlack},
	.callingMenu = NULL,
	.saveConfig = {0, NULL}
};



// This determines the index used in the data for the Reporting and configuration menus.
// Since the started with 1.
// We need to convert this to an index.
void ADC_ConfigureMenu(const char *msg)
{
	uint8_t index = *msg - '1';			// "1" represents the first value in the menu
	ADCConfigMainMenu.dataptr = GetPtrToADCData(index);
	ADCConfigMainMenu.saveConfig.index = index;
	ADCConfigMainMenu.saveConfig.Savefuncptr = ADC_Save_Config;
	SetActiveMenu(&ADCConfigMainMenu);
}
