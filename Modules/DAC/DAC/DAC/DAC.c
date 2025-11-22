
/*
 * DAC.c
 *
 * Created: 3/16/2024 11:07:46 AM
 *  Author: TERRY SCHEVKER
 */ 

 

#include "DAC.h"
#include "../ADC/ADC.h"
#include "../../Library/Delay/Delay.h"
#include "../../../Library/CPU_Interface/CPU_Interface.h"
#include <string.h>
#include <stdio.h>

static int DAC_Cmdmask = 0x08;
static int DAC_Cmd = 0;
static int DAC_DataMask = 0x800;
static int DAC_Channel = 0;
static int DAC_Active_I = 0;
static int DAC_Active_Index = 0;
static enum DAC_WRITESTATES DAC_WriteStateMachine;
static enum DAC_MONITORSTATES DAC_MonitorStateMachine;
static struct struct_DAC DAC_Data[DAC_NoDACChannels];

void DAC_Initialization()
{
	// Setup Communications bus to DAC
	PORTD.DIRSET = DAC_Pin_DataIn;
	PORTD.DIRSET = DAC_Pin_SCLK;

	// Set Clock High
	DAC_SCLK(HIGH);

	PORTC.DIRSET = DAC_CS3;
	PORTC.DIRSET = DAC_CS2;
	PORTC.DIRSET = DAC_CS1;
	PORTC.DIRSET = DAC_CS0;

	// Set all the Chip Selects to high
	DAC_CS(DAC_CS3, HIGH);
	DAC_CS(DAC_CS2, HIGH);
	DAC_CS(DAC_CS1, HIGH);
	DAC_CS(DAC_CS0, HIGH);

	// Set all the default values to 0
	// and Power down all DAC Channels
	// Setup the method on how to talk to the hardware.
	// The DAC cmd includes all 4 bits
	DAC_InitializeChannel(0,DAC_CS0, 0x01, 0);
	DAC_InitializeChannel(1,DAC_CS0, 0x05, 0);
	DAC_InitializeChannel(2,DAC_CS1, 0x01, 1);
	DAC_InitializeChannel(3,DAC_CS1, 0x05, 1);
	DAC_InitializeChannel(4,DAC_CS2, 0x01, 2);
	DAC_InitializeChannel(5,DAC_CS2, 0x05, 2);
	DAC_InitializeChannel(6,DAC_CS3, 0x01, 3);
	DAC_InitializeChannel(7,DAC_CS3, 0x05, 3);

	DAC_WriteStateMachine = DAC_WRITESTATES_IDLE;
	DAC_MonitorStateMachine  = DAC_MONITORSTATES_STARTWRITING;

}

void DAC_InitializeChannel(int channel, char CS, char cmd, char ADC_Channel)
{
	DAC_Data[channel].UpdateFlag = true;
	DAC_Data[channel].value = 0;
	DAC_Data[channel].CS = CS;
	DAC_Data[channel].Cmd = cmd;
	DAC_Data[channel].ADC_Channel = ADC_Channel;
}

void DAC_CS(char CS, bool state)
{
	switch(CS)
	{
		case DAC_CS3:
			if(state)
			{
				PORTC.OUTSET = DAC_CS3;
			}
			else
			{
				PORTC.OUTCLR = DAC_CS3;
			}
			break;
		case DAC_CS2:
			if(state)
			{
				PORTC.OUTSET = DAC_CS2;
			}
			else
			{
				PORTC.OUTCLR = DAC_CS2;
			}
			break;
		case DAC_CS1:
			if(state)
			{
				PORTC.OUTSET = DAC_CS1;
			}
			else
			{
				PORTC.OUTCLR = DAC_CS1;
			}
			break;
		case DAC_CS0:
			if(state)
			{
				PORTC.OUTSET = DAC_CS0;
			}
			else
			{
				PORTC.OUTCLR = DAC_CS0;
			}
			break;
	}
}

void DAC_SCLK(bool state)
{
	if(state == HIGH)
	{
		PORTD.OUTSET = DAC_Pin_SCLK;
	}
	else
	{
		PORTD.OUTCLR = DAC_Pin_SCLK;
	}
}

void DAC_DataIn(bool state)
{
	if(state == HIGH)
	{
		PORTD.OUTSET = DAC_Pin_DataIn;
	}
	else
	{
		PORTD.OUTCLR = DAC_Pin_DataIn;
	}
}

void DAC_Write(int channel)
{
	DAC_Cmdmask = 0x08;
	DAC_Cmd = DAC_Data[channel].Cmd;
	DAC_DataMask = 0x800;
	DAC_Channel = channel;
	DAC_Active_I = 0;
	DAC_WriteStateMachine = DAC_WRITESTATES_SETCS;
}

void DAC_Write_Monitor()
{
	switch(DAC_WriteStateMachine)
	{
		case DAC_WRITESTATES_IDLE:
			break;
		case DAC_WRITESTATES_SETCS:
			DAC_CS(DAC_Data[DAC_Channel].CS, LOW);
			DAC_WriteStateMachine = DAC_WRITESTATES_CLOCKBITS;
			break;
		case DAC_WRITESTATES_CLOCKBITS:				
			// only translate the Cmd if we are in the first 8 bits
			if(DAC_Active_I < DAC_CmdBits)
			{
				// Setup data command
				DAC_DataIn(DAC_Cmd & DAC_Cmdmask);
				// Setup for next value
				DAC_Cmdmask >>= 1;
			}
			else if(DAC_Active_I >= DAC_CmdBits)
			{
				// Test value if not 0 the
				DAC_DataIn(DAC_Data[DAC_Channel].value & DAC_DataMask);
				// move to the next bit mask
				DAC_DataMask >>= 1;
			}
			// Clock it in.
			DAC_SCLK(LOW);
			DAC_WriteStateMachine = DAC_WRITESTATES_CLOCKBITS_SCLK;
			break;
		case DAC_WRITESTATES_CLOCKBITS_SCLK:
			DAC_SCLK(HIGH);
			DAC_Active_I++;
			if(DAC_Active_I > (DAC_ClocksPerWriteCycle -1))
			{
				DAC_WriteStateMachine = DAC_WRITESTATES_RESETTCS;
			}
			else
			{
				DAC_WriteStateMachine = DAC_WRITESTATES_CLOCKBITS;
			}
			break;
		case DAC_WRITESTATES_RESETTCS:
			DAC_CS(DAC_Data[DAC_Channel].CS, HIGH);
			DAC_Data[DAC_Channel].UpdateFlag = false;
			DAC_WriteStateMachine = DAC_WRITESTATES_IDLE;
			break;
	}
}

void DAC_Monitor()
{
	switch(DAC_MonitorStateMachine)
	{
		case DAC_MONITORSTATES_STARTWRITING:
			if(DAC_Data[DAC_Active_Index].UpdateFlag == true)
			{
				DAC_Write(DAC_Active_Index);
				DAC_MonitorStateMachine = DAC_MONITORSTATES_WAITFORWRITECOMPLETE;
			}
			else
			{
				DAC_MonitorStateMachine = DAC_MONITORSTATES_PROCESSNEXTCHANNEL;
			}
			break;
		case DAC_MONITORSTATES_WAITFORWRITECOMPLETE:
			DAC_Write_Monitor();
			if(DAC_WriteStateMachine == DAC_WRITESTATES_IDLE)
			{
				DAC_MonitorStateMachine = DAC_MONITORSTATES_PROCESSNEXTCHANNEL;
			}
			break;
		case DAC_MONITORSTATES_PROCESSNEXTCHANNEL:
			DAC_Active_Index++;
			if(DAC_Active_Index == DAC_NoDACChannels)
			{
				DAC_Active_Index = 0;
			}
			DAC_MonitorStateMachine = DAC_MONITORSTATES_STARTWRITING;
			break;
	}
}

void DAC_WriteChannel(int channel, double value, double ADC_Value)
{
	DAC_Data[channel].ADC_RefValue = ADC_Value;
	DAC_Data[channel].value = ConvertChannelToInt(channel, value);
	DAC_Data[channel].UpdateFlag = true;
}

double DAC_ReadChannel(int channel)
{
	return DAC_Data[channel].value;
}

int ConvertChannelToInt(int channel, double d)
{
	double value = 0.0;
	// use actual reference value as reported by the ADC Value reading.
	// to determine per Tick value/
	double perTick = DAC_Data[channel].ADC_RefValue  * d;
	value = perTick / DAC_TICKS;
	value *= 1000.0;
	int ticks = (int) value;
	if(ticks > 4095)
	{
		ticks = 4095;
	}

	return ticks;
}

void DAC_ReadChannelByIndex_Cmd(char *cmd)
{
	char buffer[100];
	memset((char *) buffer,0, 100);

	int channel;
	int count = sscanf(cmd, "%d", &channel);
	
	if(count != 1)
	{
		SendData("Bad Parameter");
		return;
	}
	// Channel numbers are in decimal and values stored in base 0
	if((channel < DAC_Start_Channel ) || (channel > DAC_End_Channel))
	{
		SendData("Bad Index");
		return;
	}
	double value = DAC_ReadChannel(channel - 1);
	sprintf((char *) buffer,"Channel(%d) Reading(%1.3f)",  channel, value);
	SendData(buffer);
}

void DAC_WriteChannelByIndex_Cmd(char *cmd)
{
	double value = 0.0; 
	int exponent = 0;
	int fraction = 0;
	int index = 0;
	int channel = 0;

	// I was unable to find the compile settings that would make the 
	// sscanf work with Doubles. Therefore I treat everything as integers
	// and convert it into a double.
	int count = sscanf(cmd, "%d %d.%d", &channel, &exponent, &fraction);
		
	if(count != 3)
	{
		SendData("Bad Parameter");
		return;
	}

	value = (double)exponent + ((double)fraction / 1000.0);
	// Channel numbers are in decimal and values stored in base 0
	if((channel < DAC_Start_Channel ) || (channel > DAC_End_Channel))
	{
		SendData("Bad Channel");
		return;
	}

	index = channel - 1;		// Adjust for base o
	DAC_WriteChannel(index, value, ADC_ReadChannel(DAC_Data[index].ADC_Channel));
	SendData("OK");
}


void GetDeviceType_Cmd(char *cmd)
{
	SendData("DAC");
}


void GetVersion_Cmd(char *cmd)
{
	SendData("v1.0");
}