
/*
 * DAC.h
 *
 * Created: 3/16/2024 7:22:41 AM
 *  Author: TERRY SCHEVKER
 */ 

 /*
 * This module has 8 DAC output channels (12 bit) from 4 Dual channel DAC devices.
 * There are 2 dual channel ADC device that Monitor each of the 4 DAC power supplies.
 * This combination allows for accurate adjustment of the DAC outputs.
 * Connected to each DAC are power supply regulars to help with power supply stability and accuracy.
 * Reference voltage is regulated to be 4.096
 */

 
/*
	Notes: 
	1. Max Clock Frequency is 40 MHz
	2. SCLK start HI
	3. CS(SYNC) starts HI
	4. CS(SYNC) Drops Low to signal start of cycle
	5. DIn must be set up before Dropping SCLK
	6. 16 Bits in total are transfered. 4 control bits and 12 data bits.
	7. D15 = A1
	   D14 = A0
	   A1	A0
	   0	1		= DAC Channel A
	   1	0		= DAC Channel B

	   D13 = OP1
	   D12 = OP0
	   OP1	OP0
	   0	0	= Write to specified register but do not update outputs
	   0	1	= Write to specified register and update outputs
	   1	0	= Write to all registers and update outputs
	   1	1	= Power-down outputs.

	   Vout = 4.096 (ref Voltage) * (D / 4096) D = Decimal value of output
	   Example 3.75v is the desired results.
	   3750 is the D value.

	   D11-D0	data bits = 0xEA6

	   Power Down Modes (See page 15-16)
	   Not sure what to do with the load resistor options.
	   1. HI-Z
	   2. 2.5K
	   3. 100K

*/


#pragma once
#include <avr/io.h>
#include <port.h>
#include <stdbool.h>

#define PC3		0x08
#define PC2		0x04
#define PC1		0x02
#define PC0		0x01

#define PD5		0x20
#define PD4		0x10

#define HIGH	true
#define LOW		false

#define	DAC_CS3		PC3
#define	DAC_CS2		PC2
#define	DAC_CS1		PC1
#define	DAC_CS0		PC0

#define DAC_Pin_DataIn	PD5
#define DAC_Pin_SCLK	PD4

#define DAC_NoDACChannels		8
#define DAC_ClocksPerWriteCycle	16
#define DAC_CmdBits				4
#define DAC_DataBits			12

#define DAC_Start_Channel 1
#define DAC_End_Channel 8
#define DAC_TICKS	4.096

struct struct_DAC
{
	bool UpdateFlag;
	char CS;
	char Cmd;
	char ADC_Channel;
	double ADC_RefValue;
	int value;
};

// Initialize the Hardware
void DAC_Initialization();

// Initialize the different channel settings
void DAC_InitializeChannel(int channel, char CS, char cmd, char ADC_Channel);

// Performs the actual Write to the DAC Chip/Channel.
void DAC_Write_Monitor();

// Monitors when a channel needs to be updated.
void DAC_Monitor();

// These commands are used to drive the communications protocol
void DAC_Write(int channel);
void DAC_SCLK(bool state);
void DAC_DataIn(bool state);
void DAC_CS(char CS, bool state);

// User Programmed interface for setting a channel's data request
void DAC_WriteChannel(int channel, double value, double ADC_Value);
// Report back the current channels value, this is the recorded value at time of write.
double DAC_ReadChannel(int channel);

// This function converts a double in to a compensated value for setting the DAC
int ConvertChannelToInt(int channel, double value);

void DAC_WriteChannelByIndex_Cmd(char *cmd);
void DAC_ReadChannelByIndex_Cmd(char *cmd);
void GetDeviceType_Cmd(char *cmd);
void GetVersion_Cmd(char *cmd);

enum DAC_WRITESTATES
{
	DAC_WRITESTATES_IDLE,
	DAC_WRITESTATES_SETCS,
	DAC_WRITESTATES_CLOCKBITS,
	DAC_WRITESTATES_CLOCKBITS_SCLK,
	DAC_WRITESTATES_RESETTCS,
};

enum DAC_MONITORSTATES
{
	DAC_MONITORSTATES_STARTWRITING,
	DAC_MONITORSTATES_WAITFORWRITECOMPLETE,
	DAC_MONITORSTATES_PROCESSNEXTCHANNEL,
};

