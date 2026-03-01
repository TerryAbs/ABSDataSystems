/*
 * Ports.c
 *
 * Created: 1/2/2026 10:56:42 AM
 *  Author: terry
 */ 
#include <avr/io.h>
#include <stdio.h>
#include <stdbool.h>
#include "Ports/Ports.h"
#include "EEPRom/EEPRom.h"
#include "CommonStrings/CommonStrings.h"
#include "Display_Processor/Display_Processor.h"

static char msg_PortA_Output;
static char msg_PortA_Inverted;
static char msg_PortB_Inverted;

PortsConfiguration PortsSystem = {
	SIGNATURE_BYTES_PORTS,
	true,							//  enabled				Enabled/Disabled
	true,							// portAOutput			Port A is output
	true,							// portAOutputValue		if output, this is the default value.
	false,							// portAInputLevel		Port A Schmitt Trigger or TTL
	false,							// portAInverted		Port A invented
	false,							// portAPullUp			Port A No Pull Resistor (only available in input mode)
	false,							// portBInputLevel		Port B Schmitt Trigger or TTL
	false,							// portBInverted;		Port B invented
	false							// portBPullUp			Port B No Pull Resistor (only available in input mode)
};

// These pointers are not need for this system.
void Ports_Display(void *ptrData, const char *msg)
{
	char InputPort = 'L';
	char InputOutPutPort = 'L';

	if(PortsSystem.enabled)
	{
		SetColor(Color_White, Color_Black);
		// Input Port. Get Value
		if((PORTF.IN & PF6_PortInOnly) == PF6_PortInOnly)
		{
			InputPort = 'H';
		}
	
		// Output port can be in one of two states
		// Setup as a Output
		// Then we know the value
		if(PortsSystem.portAOutput)	
		{
			if((PortsSystem.portAOutputValue) && !PortsSystem.portAInverted)
			{
				InputOutPutPort = 'H';	
			}
		}
		else
		{
			// Getting here means that this is a input
			if((PORTC.IN & PC3_PortInOut) == PC3_PortInOut)
			{
				InputOutPutPort = 'H';
			}
		}
		printf("Ports: PC3 %c (%c-%c) PD7 %c (%c)", InputOutPutPort, msg_PortA_Output, msg_PortA_Inverted, InputPort,msg_PortB_Inverted);
	}
}	

void Ports_ReInitialize(void)
{
	Ports_Load_Config();

	// Port C	
	// reset port settings
	PORTC.PIN3CTRL = 0;

	msg_PortA_Output = 'O';
	msg_PortA_Inverted ='N';
	msg_PortB_Inverted = 'N';

	if(PortsSystem.portAOutput)
	{
		// Output
		PORTC.DIRSET = PC3_PortInOut;
		if(PortsSystem.portAOutputValue)
		{
			PORTC.OUTSET = PC3_PortInOut;
		}
		else
		{
			PORTC.OUTCLR = PC3_PortInOut;
		}
	}
	else
	{
		// Input
		msg_PortA_Output = 'I';

		PORTC.DIRCLR = PC3_PortInOut;
		if(PortsSystem.portAPullUp)
		{
			PORTC.PIN3CTRL |= PORT_PULLUPEN_bm;
		}
			// Levels
		if(PortsSystem.portAInputLevel)
		{
			PORTC.PIN3CTRL |= PORT_INLVL_bm;
		}
	}
	
	// Inverted
	if(PortsSystem.portAInverted)
	{
		msg_PortA_Inverted ='I';
		PORTC.PIN3CTRL |= PORT_INVEN_bm;
	}
	

	// Port F	
	// Reset settings
	PORTF.PIN6CTRL = 0;
	if(PortsSystem.portBInputLevel)
	{
		PORTF.PIN6CTRL |= PORT_INLVL_bm;
	}
	
	if(PortsSystem.portBInverted)
	{
		msg_PortB_Inverted = 'I';
		PORTF.PIN6CTRL |= PORT_INVEN_bm;
	}

	if(PortsSystem.portAPullUp)
	{
		PORTF.PIN6CTRL |= PORT_PULLUPEN_bm;
	}
}

void Ports_Initialize(void)
{
	Ports_ReInitialize();
	// Add item to display list.
	DisplayProcessorMap item;
	item.ptrData = &PortsSystem;
	item.funcptrPrint = Ports_Display;
	item.msg = NULL;
	item.enabled = PortsSystem.enabled;
	DisplayProcessor_Add(item);
}

void Ports_Monitor(void)
{
	if(PortsSystem.enabled)
	{
	}
}



void *GetPtrToPortsData(void)
{
	return &PortsSystem;
}

void Ports_Save_Config(uint8_t channel)
{
	// Rules for pull resistors on A
	// Port A No Pull Resistor (only available in input mode)
	if(PortsSystem.portAOutput)		
	{
		PortsSystem.portAPullUp = false;
	}

	// Save just one element
	// However, I'm maintaining the option of multiple channels. Like i did for ADC
	EEPROM_WRITE_ELEMENT(ports_system, 0, &PortsSystem);
	DisplayProcessor_SetEnabledFlag(&PortsSystem, PortsSystem.enabled);
	Ports_ReInitialize();
	
}

void Ports_Load_Config(void)
{
	// Load just one element
	EEPROM_VERIFY_SIGNATURE_AND_LOAD(ports_system, 0, &PortsSystem);
}
