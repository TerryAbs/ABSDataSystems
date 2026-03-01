/*
 * SPI.c
 *
 * Created: 1/1/2026 3:30:06 PM
 *  Author: terry
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <stdbool.h>
#include "SPI/SPI.h"
#include "EEPRom/EEPRom.h"
#include "CommonStrings/CommonStrings.h"
#include "Display_Processor/Display_Processor.h"
#include "SPI_Basic.h"  // MicroStudio SPI driver

static uint8_t spi_buffer[SPIbuffersize];
static uint8_t 	manufacturer;
static uint8_t 	device_id1;
static enum SPI_STATE_MACHINE spiStateMachine;
static uint8_t SPI_TimedOut;

/*
 * AT45DB041D Manufacturer and Device ID Reader
 * MCU: AVR64DD32 @ 24MHz
 * Interface: SPI using spi_basic.c driver from MicroStudio
 */


SPIConfiguration SPISystem = {
	SIGNATURE_BYTES_SPI,
	true,							// Enabled/Disabled
};

// These pointers are not need for this system.
void SPI_Display(void *ptrData, const char *msg)
{
	if(SPISystem.enabled)
	{
		const char *msgStatus = msg_Passed;
		SetColor(Color_White, Color_Black);
		if((manufacturer != AT45DB_ManufacturerID) |(manufacturer != AT45DB_ManufacturerID))
		{
			msgStatus = msg_Failed;
		}
		printf_P(PSTR("SPI Man ID (0x%02X) Dev ID (0x%02X) - %S"), manufacturer, device_id1, msgStatus);
	}
}

void SPI_Initialize(void)
{
	    // Set MOSI and SCK as outputs
	PORTA.DIRSET = PA4_SPI_MOSI_PIN | PA6_SPI_SCK_PIN | PA7_SPI_CS_PIN;
	    
	// Set MISO as input (should be default, but explicit is good)
	PORTA.DIRCLR = PA5_SPI_MISO_PIN;
	    
	PORTA.OUTSET = PA7_SPI_CS_PIN;
		
	// Initialize SPI
	SPI_0_init();

	spiStateMachine = SPI_STATE_MACHINE_SEND_PACKET;
	SPI_Load_Config();
		
	// Add item to display list.
	DisplayProcessorMap item;
	item.ptrData = &SPISystem;
	item.funcptrPrint = SPI_Display;
	item.msg = NULL;
	item.enabled = SPISystem.enabled;
	DisplayProcessor_Add(item);
}

void SPI_Monitor(void)
{
	if(SPISystem.enabled)
	{
		switch(spiStateMachine)
		{
			case SPI_STATE_MACHINE_SEND_PACKET:
				// State 1
				PORTA.OUTCLR = PA7_SPI_CS_PIN;
				// Prepare buffer: command followed by dummy bytes
				spi_buffer[0] = AT45DB_CMD_READ_ID;
				spi_buffer[1] = 0xFF;
				spi_buffer[2] = 0xFF;
				spi_buffer[3] = 0xFF;
			
				SPI_TimedOut = 0;
				// Exchange 4 bytes
				SPI_0_exchange_block((void*)spi_buffer, SPIbuffersize);
				spiStateMachine = SPI_STATE_MACHINE_WAIT_FOR_COMPLETE;
				break;
			case SPI_STATE_MACHINE_WAIT_FOR_COMPLETE:
				SPI_TimedOut++;
				if(SPI_0_status_done() || (SPI_TimedOut == 10))
				{
					if(SPI_TimedOut == 10)
					{
						LogErrorToScreen(msgSPI_TimedOut);
					}
					spiStateMachine = SPI_STATE_MACHINE_SEND_PACKET_SAVE;
				}
				break;
			case SPI_STATE_MACHINE_SEND_PACKET_SAVE:
				PORTA.OUTSET = PA7_SPI_CS_PIN;
				// Extract results (command byte is overwritten with first response)
				// spi_buffer[0]; contains no data. The data is 1 read behind
				manufacturer = spi_buffer[1];
				device_id1 = spi_buffer[2];
				spiStateMachine = SPI_STATE_MACHINE_SEND_PACKET;
				break;
		}
	}
}



void *GetPtrToSPIData(void)
{
	return &SPISystem;
}

void SPI_Save_Config(uint8_t channel)
{
	// Save just one element
	// However, I'm maintaining the option of multiple channels. Like i did for ADC
	EEPROM_WRITE_ELEMENT(spi_system, 0, &SPISystem);
	DisplayProcessor_SetEnabledFlag(&SPISystem, SPISystem.enabled);
	spiStateMachine = SPI_STATE_MACHINE_SEND_PACKET;
}

void SPI_Load_Config(void)
{
	// Load just one element
	EEPROM_VERIFY_SIGNATURE_AND_LOAD(spi_system, 0, &SPISystem);
}
