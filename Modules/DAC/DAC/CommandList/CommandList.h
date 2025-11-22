
/*
* CommandList.h
*
* Created: 11/21/2023 3:53:26 PM
*  Author: TERRY SCHEVKER
*/
#pragma once
#include "../../../Library/Data_Structures/Data_Structures.h"

/**< DAC Board commands */
#define CMD_GETDEVICETYPE			"GETDEVICETYPE"		/**< Returns the name off Module*/
#define CMD_GETVERSION				"GETVERSION"		/**< Returns the Version number of the source code*/
#define CMD_SETCALVALUE				"SETCALVALUE"		/**< Sets the calibration value used during the ADC Conversion process.*/
#define CMD_ADC_READ_CHANNEL		"ADCReadChannel"	/**< Read ADC Value of Channel*/
#define CMD_DAC_READ_CHANNEL		"DACReadChannel"	/**< Read ADC Value of Channel*/
#define CMD_DAC_WRITE_CHANNEL		"WriteChannel"		/**< Write DAC Value of Channel*/
#define CMD_SETTIME					"SETTIME"			/**< Set RTC CLock Time*/

void CommandList_Initialize();
void ProcessCommand(char *buf);
