/**
\file
\brief DAC Module Commands List

These are the commands that are recognized by the DAC Module.
*/

#include "CommandList.h"
#include "../ADC/ADC.h"
#include "../DAC/DAC.h"
#include "../../../Library/RTC/rtc.h"
#include "../../../Library/CPU_Interface/CPU_Interface.h"
#include <string.h>

struct structCommandList CommandList[] = {
	{(char *) CMD_GETDEVICETYPE, 0, GetDeviceType_Cmd},
	{(char *) CMD_GETVERSION, 0, GetVersion_Cmd},
	{(char *) CMD_SETCALVALUE, 0, SetCalibration_Value_Cmd},
	{(char *) CMD_DAC_READ_CHANNEL, 0, DAC_ReadChannelByIndex_Cmd},
	{(char *) CMD_DAC_WRITE_CHANNEL, 0, DAC_WriteChannelByIndex_Cmd},
	{(char *) CMD_ADC_READ_CHANNEL, 0, ADC_ReadChannelByIndex_Cmd},
	{(char *) CMD_SETTIME, 0, SetTime_Cmd} 	
};

#define CmdListSize sizeof(CommandList)/sizeof(struct structCommandList)

void CommandList_Initialize()
{
	// Read through the list of commands and calculate the size of each command and store
	// the size in the table.

	for(int i = 0; i < CmdListSize; i++)
	{
		CommandList[i].cmdSize = strlen(CommandList[i].cmd);
	}
}

void ProcessCommand(char *buf)
{
	int index;

	for(int i = 0; i < CmdListSize; i++)
	{
		index = strncasecmp(CommandList[i].cmd, buf, CommandList[i].cmdSize);
		// Test to make sure at least the first x match.
		if(index == 0 )
		{
			if(CommandList[i].func != NULL)
			{
				// Skip over the command place space
				CommandList[i].func(&buf[CommandList[i].cmdSize + 1]);
			}
			break;
		}
	}
}