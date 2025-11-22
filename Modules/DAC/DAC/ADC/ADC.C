
/*
 * ADC.C
 *
 * Created: 3/16/2024 9:29:07 AM
 *  Author: TERRY SCHEVKER
 */ 

 #include "ADC.h"
 #include "../../../Library/CPU_Interface/CPU_Interface.h"
 #include <string.h>
 #include <stdio.h>
 
static char active_CS;
static char active_Cmd;
static int  active_Index;
static int  active_I;
static int  active_Results;
static int  active_Cmdmask = 0x80;
static int  active_DataMask = 0x800;
static int  ADC_Channel = 0;
static enum ADC_READSTATES ADC_ReadStateMachine;
static enum ADC_MONITORSTATES ADC_MonitorStateMachine;
static double ADC_LSB = 0.0;

 struct struct_ADC ADC_Reading[] =
 {
	{ ADC_CS0, ADC_IN2, 0, 0.0},
	{ ADC_CS0, ADC_IN1, 0, 0.0},
	{ ADC_CS1, ADC_IN2, 0, 0.0},
	{ ADC_CS1, ADC_IN1, 0, 0.0},
 };

 #define NoElements sizeof(ADC_Reading)/sizeof(struct struct_ADC)

 void ADC_Initialization()
 {
	// Setup Communications bus to ADC
	PORTF.DIRSET = ADC_Pin_DataIn;
	PORTF.DIRCLR = ADC_Pin_DataOut;
	PORTF.DIRSET = ADC_Pin_SCLK;

	// Set Clock High
	ADC_SCLK(HIGH);

	PORTF.DIRSET = ADC_CS1;
	PORTF.DIRSET = ADC_CS0;

	// Set all the Chip Selects to high
	ADC_CS(ADC_CS1, HIGH);
	ADC_CS(ADC_CS0, HIGH);

	ADC_ReadStateMachine = ADC_READSTATES_IDLE;
	ADC_MonitorStateMachine = ADC_MONITORSTATES_STARTREADING;
	ADC_Channel = 0;
	ADC_LSB = DEFAULT_ADC_LSB;
 }

 void ADC_CS(char CS, bool state)
 {
	switch(CS)
	{
		case ADC_CS1:
			if(state)
			{
				PORTF.OUTSET = ADC_CS1;
			}
			else
			{
				PORTF.OUTCLR = ADC_CS1;
			}
			break;
		case ADC_CS0:
			if(state)
			{
				PORTF.OUTSET = ADC_CS0;
			}
			else
			{
				PORTF.OUTCLR = ADC_CS0;
			}
			break;
	}
 }

 void ADC_SCLK(bool state)
 {
	if(state == HIGH)
	{
		PORTF.OUTSET = ADC_Pin_SCLK;
	}
	else
	{
		PORTF.OUTCLR = ADC_Pin_SCLK;
	}
 }

 void ADC_DataIn(bool state)
  {
	  if(state == HIGH)
	  {
		  PORTF.OUTSET = ADC_Pin_DataIn;
	  }
	  else
	  {
		  PORTF.OUTCLR = ADC_Pin_DataIn;
	  }
  }

void ADC_Read(char CS, char Cmd, int index)
{
	active_CS = CS;
	active_Cmd = Cmd;
	active_Index = index;
	active_Cmdmask = 0x80;
	active_DataMask = 0x800;
	active_I = 0;
	active_Results = 0;
	ADC_ReadStateMachine = ADC_READSTATES_SETCS;
}

void ADC_Read_Monitor()
{
	switch(ADC_ReadStateMachine)
	{
		case ADC_READSTATES_IDLE:
			break;
		case ADC_READSTATES_SETCS: 
			ADC_CS(active_CS, LOW);
			ADC_ReadStateMachine = ADC_READSTATES_CLOCKBITS;
			break;
		case ADC_READSTATES_CLOCKBITS:
			// only translate the Cmd if we are in the first 8 bits
			if(active_I < ADC_CmdBits)
			{
				// Setup data command
				ADC_DataIn(active_Cmd & active_Cmdmask);
				// Setup for next value
				active_Cmdmask >>= 1;
			}
			else
			{
				ADC_DataIn(LOW);
			}

			// Clock it in.
			ADC_SCLK(LOW);

			// See if we have hit the read data out clock yet
			if(active_I >= ADC_DataOutClock)
			{
				// Test value if not 0 then store the bit
				if((PORTF.IN & ADC_Pin_DataOut) != 0)
				{
					active_Results += active_DataMask;
				}
				// move to the next bit mask
				active_DataMask >>= 1;
			}

			// Clock it in.
			ADC_SCLK(HIGH);

			active_I++;
			if(active_I >= ADC_ClocksPerReadCycle)
			{
				ADC_ReadStateMachine = ADC_READSTATES_RESETTCS;
			}
			break;
		case ADC_READSTATES_RESETTCS:
			// DeSelect part
			ADC_CS(active_CS, HIGH);
			ADC_Reading[active_Index].data = active_Results;
			ADC_ReadStateMachine = ADC_READSTATES_IDLE;
			break;
	}
}
void ADC_Monitor()
{
	switch(ADC_MonitorStateMachine)
	{
		case ADC_MONITORSTATES_STARTREADING:
			ADC_Read(ADC_Reading[ADC_Channel].CS, ADC_Reading[ADC_Channel].Cmd, ADC_Channel);
			ADC_MonitorStateMachine = ADC_MONITORSTATES_WAITFORREADCOMPLETE;
			break;
		case ADC_MONITORSTATES_WAITFORREADCOMPLETE:
			ADC_Read_Monitor();
			if(ADC_ReadStateMachine == ADC_READSTATES_IDLE)
			{
				ADC_MonitorStateMachine = ADC_MONITORSTATES_PROCESSCHANNEL;
			}
			break;
		case ADC_MONITORSTATES_PROCESSCHANNEL:
			ADC_Reading[ADC_Channel].value = ADC_LSB * (double)(ADC_Reading[ADC_Channel].data);
			ADC_MonitorStateMachine = ADC_MONITORSTATES_PROCESSNEXTCHANNEL;
			break;
		case ADC_MONITORSTATES_PROCESSNEXTCHANNEL:
			ADC_Channel++;
			if(ADC_Channel == NoElements)
			{
				ADC_Channel = 0;
			}
			ADC_MonitorStateMachine = ADC_MONITORSTATES_STARTREADING;
			break;
	}
}

 double ADC_ReadChannel(int channel)
 {
	// The returned results is adjusted by the value we collect from reading the power supply
	return ADC_Reading[channel].value;
 }

 void ADC_ReadChannelByIndex_Cmd(char *cmd)
 {
	 char buffer[100];
	 memset((char *) buffer,0, 100);

	 int index = 0;
	 int channel;
	 int count = sscanf(cmd, "%d", &channel);
	 
	 if(count != 1)
	 {
		 SendData("Bad Parameter");
		 return;
	 }
	 // Channel numbers are in decimal and values stored in base 0
	 if((channel < ADC_Start_Channel ) || (channel > ADC_End_Channel ))
	 {
		 SendData("Bad Index");
		 return;
	 }
	 index = channel - 1;		// Adjust for base o
	 double value = ADC_ReadChannel(index);
	 sprintf((char *) buffer,"Channel(%d) Reading(%1.3f)", (int) channel, value);
	 SendData(buffer);
 }

 
void SetCalibration_Value_Cmd(char *cmd)
{
	int exponent = 0;
	int fraction = 0;
	double value = 0.0;
	// I was unable to find the compile settings that would make the
	// sscanf work with Doubles. Therefore I treat everything as integers
	// and convert it into a double.
	int count = sscanf(cmd, "%d.%d", &exponent, &fraction);
	
	if(count != 2)
	{
		SendData("Bad Parameter");
		return;
	}


	// The value entered in the value read by a meter at test point 10
	// The value for the fraction must be three digits in size
	value = (double)exponent + ((double)fraction / 1000.0);
	ADC_LSB = value / ADC_TICK_COUNTS ;	// Now convert to the tick value.
	SendData("OK");
}