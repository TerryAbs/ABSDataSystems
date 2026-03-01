/*
 * TWI_I2C.c
 *
 * Created: 1/1/2026 3:30:22 PM
 *  Author: terry
 */ 
#include <avr/io.h>
#include <stdio.h>
#include <stdbool.h>
#include "TWI_I2C.h"
#include "EEPRom/EEPRom.h"
#include "CommonStrings/CommonStrings.h"
#include "Display_Processor/Display_Processor.h"

static uint8_t PCF8591Data;
static uint8_t PCF8591Command;
static enum TWI_I2C_WRITESTATES TWI_I2C_WriteStateMachine;
static enum TWI_I2C_MONITORSTATES TWI_I2C_MonitorStateMachine;

TWI_I2CConfiguration TWI_I2CSystem = {
	SIGNATURE_BYTES_TWI,
	true,							// Enabled/Disabled
};

void TWI_I2C_Display(void *ptrData, const char *msg)
{
	if(TWI_I2CSystem.enabled)
	{
		SetColor(Color_White, Color_Black);
		float value = (float)PCF8591Data * PCF8591B_VoltPerTick;
		printf("PCF8591 reading %3.3f", value);
	}
}

void TWI_I2C_ReInitialize(void)
{
	TWI_I2C_MonitorStateMachine = TWI_I2C_MONITORSTATES_SETADDRESS;
	TWI_I2C_Load_Config();
}

void TWI_I2C_Initialize(void)
{
	
	PORTA.DIRCLR = PA0_TWI_I2C_SDA | PA1_TWI_I2C_SCL;  // Set as input initially
	PORTA.PIN0CTRL = PORT_PULLUPEN_bm; // Enable pull-up on SDA
	PORTA.PIN1CTRL = PORT_PULLUPEN_bm; // Enable pull-up on SCL

	// PORTMUX.TWIROUTEA = PORTMUX_TWI0_DEFAULT_gc;
	PORTMUX.TWIROUTEA = PORTMUX_TWI0_ALT3_gc;
	// Configure TWI0 in Host (Master) mode
	TWI0.MCTRLA = 0; // Disable TWI0 during configuration
		
	// Set baud rate (100kHz with 24MHz system clock)
	TWI0.MBAUD = TWI_BAUD_100K;
		
	// Enable TWI in master mode
	TWI0.MCTRLA = TWI_ENABLE_bm;
		
	// Force bus into IDLE state
	TWI0.MSTATUS = TWI_BUSSTATE_IDLE_gc;
	
	TWI_I2C_ReInitialize();

	// Add item to display list.
	DisplayProcessorMap item;
	item.ptrData = &TWI_I2CSystem;
	item.funcptrPrint = TWI_I2C_Display;
	item.msg = NULL;
	item.enabled = TWI_I2CSystem.enabled;
	DisplayProcessor_Add(item);
	
	// Since this is a demo, the code is 
	// going to be fixed on reading the first channel
	// and the first channel only.
	// The driver has need code just for that purpose.
	PCF8591Command = ADO_CHANNEL;
}

/**
 * @brief Send STOP condition
 */
void TWI_Stop(void) 
{
    TWI0.MCTRLB = TWI_MCMD_STOP_gc;
}

void TWI_I2C_WriteMonitor(void)
{
	switch(TWI_I2C_WriteStateMachine)
	{
		case TWI_I2C_WRITESTATES_WAITFORREADY:
			if((TWI0.MSTATUS & (TWI_WIF_bm | TWI_RIF_bm)))
			{
				TWI_I2C_WriteStateMachine = TWI_I2C_WRITESTATES_SENDDATA;
			}
			break;
		case TWI_I2C_WRITESTATES_SENDDATA:
		  TWI0.MDATA = PCF8591Command;
		  TWI_I2C_WriteStateMachine = TWI_I2C_WRITESTATES_WAITFORDATA;
		  break;
		case TWI_I2C_WRITESTATES_WAITFORDATA:
			if(TWI0.MSTATUS & TWI_WIF_bm)
			{
				if (TWI0.MSTATUS & TWI_RXACK_bm)
				{
					TWI_Stop();
					LogErrorToScreen(msg_TWII2C_WriteError);
					TWI_I2C_MonitorStateMachine = TWI_I2C_MONITORSTATES_IDLE;
				}
				else
				{
					TWI_I2C_MonitorStateMachine = TWI_I2C_MONITORSTATES_SENDREADBACKCOMMAND;
				}
			}
			break;
	}
}

void TWI_I2C_Monitor(void)
{
	if(TWI_I2CSystem.enabled)
	{
		switch(TWI_I2C_MonitorStateMachine)
		{
			case TWI_I2C_MONITORSTATES_IDLE:		// We only get here in the event of an error
				break;
			case TWI_I2C_MONITORSTATES_SETADDRESS:
		        // Send START + Device Address + Write bit (to set address pointer)
				TWI0.MADDR = (PCF8591B_ADDR << 1) | 0; // Write mode
				TWI_I2C_MonitorStateMachine = TWI_I2C_MONITORSTATES_WAITFORADDRESS;
				break;
		case TWI_I2C_MONITORSTATES_WAITFORADDRESS:
			if(TWI0.MSTATUS & TWI_WIF_bm)
			{
				if (TWI0.MSTATUS & TWI_RXACK_bm) 
				{
					TWI_Stop();
					LogErrorToScreen(msg_TWII2C_ACKError);
					TWI_I2C_MonitorStateMachine = TWI_I2C_MONITORSTATES_IDLE;
				}
				else
				{
					TWI_I2C_MonitorStateMachine = TWI_I2C_MONITORSTATES_SENDCOMMAND;
					// Setup Write state machine.
					TWI_I2C_WriteStateMachine = TWI_I2C_WRITESTATES_WAITFORREADY;
				}
			}
			break;
		case TWI_I2C_MONITORSTATES_SENDCOMMAND:
				// The Write state machine will auto increment the Monitor State machine
				TWI_I2C_WriteMonitor();
				break;
		case TWI_I2C_MONITORSTATES_SENDREADBACKCOMMAND:
		    // Send REPEATED START + Device Address + Read bit
		    TWI0.MADDR = (PCF8591B_ADDR << 1) | 1; // Read mode
			TWI_I2C_MonitorStateMachine = TWI_I2C_MONITORSTATES_WAITFORREADSTATUS;
			break;
		case TWI_I2C_MONITORSTATES_WAITFORREADSTATUS:
			if(TWI0.MSTATUS & TWI_RIF_bm)
			{
				// Read the data byte
				PCF8591Data = TWI0.MDATA;
				    
				// Send NACK and STOP (we're only reading one byte)
				TWI0.MCTRLB = TWI_ACKACT_bm | TWI_MCMD_STOP_gc;
				TWI_I2C_MonitorStateMachine = TWI_I2C_MONITORSTATES_SETADDRESS;
			}
			break;
		}
	}
}


void *GetPtrToTWI_I2CData(void)
{
	return &TWI_I2CSystem;
}

void TWI_I2C_Save_Config(uint8_t channel)
{
	// Save just one element
	// However, I'm maintaining the option of multiple channels. Like i did for ADC
	EEPROM_WRITE_ELEMENT(twi_i2c_system, 0, &TWI_I2CSystem);
	DisplayProcessor_SetEnabledFlag(&TWI_I2CSystem, TWI_I2CSystem.enabled);	
	TWI_I2C_ReInitialize();
}

void TWI_I2C_Load_Config(void)
{
	// Load just one element
	EEPROM_VERIFY_SIGNATURE_AND_LOAD(twi_i2c_system, 0, &TWI_I2CSystem);
}
