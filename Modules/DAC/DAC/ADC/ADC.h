
/*
 * ADC.h
 *
 * Created: 3/16/2024 7:22:41 AM
 *  Author: TERRY SCHEVKER
 */ 

 /*
 * This module has 8 ADC Input channels and 1 ADC to monitor the power supply.
 * The ADC that will be used to monitor the power supply is monitoring two power supplies through a voltage divider.
 * The Two Power Supplies are the 5 and 6 volt power supplies
 * The voltage divider are two 220 Ohm resistor networks.
 * Intent of the voltage divider is to monitor the power suppliers are 50% of their range. Since one of the power supplies
 * Supplies power to the ADC, we would not be able to monitor the full power of the power supply
 * The 6 volt power supply is used to power the Unity gain isolation buffers on the board.
 * There is a set of jumpers to allow the user to control the path of the ADC input, either director to the ADC or through the Unity Gain Buffer.
 * An External 6 volt Power supply is required and it is connect via the Programming port Pin 2
 */

 
/*
	Notes: (See pages 5, 6, 7)
	1. Max Clock Frequency is 3.2 MHz
	2. SCLK start HI
	3. CS starts HI
	4. CS Drops Low to signal start of cycle
	5. Data In and Out at the same time.
	6. DIn must be set up before Dropping SCLK
	6. DI7, DI6, DI5, DI4 are sent before DO is ready.
	7. DO starts with MSB (5th Clock Cycle)
	8. Read DO on Rising Edge
	9. There are a total of 16 clocks. in a cycle.
	10. Data In Commands:
		a. Bit 7, 6, 2, 1, 0 are don't care bits
		b. To Read Input Channel 1 5 = x, 4 = 0, 3 = 0
		c. To Read Input Channel 2 5 = x, 4 = 0, 3 = 1
	11. When setting the read channel the data is not available for this channel until the next read.
		So you are writing the channel for the next read cycle.
	12. See Page 17 for rules on CS and SCLK States
	13. There are 8192 bits. vcc (va)/8192 is the value reported per tick

	Page 17 just above table 2 is a note that states that after a power up the default read in In1

	Write Command In2 reading In1
	Write Command In1 reading In2
*/

#pragma once
#include <avr/io.h>
#include <port.h>
#include <stdbool.h>


#define PF4			0x10
#define PF3			0x08
#define PF2			0x04
#define PF1			0x02
#define PF0			0x01

#define	ADC_CS1		PF4
#define	ADC_CS0		PF3

#define ADC_Pin_DataIn	PF2
#define ADC_Pin_DataOut	PF1
#define ADC_Pin_SCLK	PF0

#define ADC_IN1		0x00
#define ADC_IN2		0x08

#define HIGH	true
#define LOW		false

#define ADC_IN1		0x00
#define ADC_IN2		0x08

#define ADC_ClocksPerReadCycle	16
#define ADC_CmdBits				8
#define ADC_DataOutClock		4

#define ADC_TICK_COUNTS 4096.0
#define DEFAULT_ADC_LSB 5.0/ADC_TICK_COUNTS	// This design uses a precision regulator for the Reference/va to the ADC

#define ADC_Start_Channel 1
#define ADC_End_Channel 4

void ADC_Initialization();
void ADC_Monitor();
double ADC_ReadChannel(int channel);
void ADC_SCLK(bool state);
void ADC_DataIn(bool state);
void ADC_CS(char CS, bool state);

void ADC_Read(char CS, char Cmd, int index);
void ADC_Read_Monitor();

void ADC_ReadChannelByIndex_Cmd(char *cmd);
void SetCalibration_Value_Cmd(char *cmd);

struct struct_ADC{
	char CS;
	char Cmd;
	int data;
	double value;
};

enum ADC_READSTATES
{
	ADC_READSTATES_IDLE,
	ADC_READSTATES_SETCS,
	ADC_READSTATES_CLOCKBITS,
	ADC_READSTATES_RESETTCS,
};

enum ADC_MONITORSTATES
{
	ADC_MONITORSTATES_STARTREADING,
	ADC_MONITORSTATES_WAITFORREADCOMPLETE,
	ADC_MONITORSTATES_PROCESSCHANNEL,
	ADC_MONITORSTATES_PROCESSNEXTCHANNEL,
};


