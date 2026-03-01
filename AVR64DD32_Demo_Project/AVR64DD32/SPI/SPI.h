/**
 * @file    SPI.h
 * @brief   Serial Peripheral Interface driver for AVR64DD32
 * @details Implements SPI master mode with support for multiple flash memory
 *          devices including W25Q128, AT25SF641, and SST26VF016B. Provides
 *          read, write, and erase operations with device-specific commands.
 *
 * @author  Terry (ABS Data Systems)
 * @date    November 25, 2025
 * @version 1.0
 *
 * @par Pin Configuration
 *      - PA4: MOSI (Master Out Slave In)
 *      - PA5: MISO (Master In Slave Out)
 *      - PA6: SCK  (Serial Clock)
 *      - PA7: SS   (Slave Select)
 *
 * @par Supported Flash Devices
 *      - Winbond W25Q128 (128Mbit)
 *      - Adesto AT25SF641 (64Mbit)
 *      - Microchip SST26VF016B (16Mbit)
 *
 * @par Operations
 *      - Device ID read and verification
 *      - Page write (up to 256 bytes)
 *      - Sector/block erase
 *      - Sequential read
 *      - Status register access
 *
 * @note    Uses interrupt-driven transfers for efficiency
 * @note    Automatic busy polling during erase/write operations
 * @warning Ensure proper SPI mode (MODE 0 or MODE 3) for your device
 * @warning SS must remain low during entire transfer
 * @warning Flash write operations require prior erase
 *
 * @see spi_basic.h for low-level SPI primitives
 * @see Menu_SPI.h for user interface
 */

#pragma once
#include "Common/Common.h"


// AT45DB041D Commands
#define AT45DB_CMD_READ_ID      0x9F    // Read Manufacturer and Device ID
#define AT45DB_ManufacturerID	0x1f	// Data from the Specification Sheet
#define AT45DB_DeviceID			0x12	// Data from the Specification Sheet
#define PA4_SPI_MOSI_PIN    0x10			// PA4 - MOSI (must be output)
#define PA5_SPI_MISO_PIN    0x20			// PA5 - MISO (must be input)
#define PA6_SPI_SCK_PIN     0x40			// PA6 - SCK (must be output)
#define PA7_SPI_CS_PIN		0x80			// PA7

#define SPISystemSize 1
#define SPIbuffersize 4

enum SPI_STATE_MACHINE{
	SPI_STATE_MACHINE_SEND_PACKET,
	SPI_STATE_MACHINE_WAIT_FOR_COMPLETE,
	SPI_STATE_MACHINE_SEND_PACKET_SAVE
};

typedef struct SPIConfiguration
{
	enum SIGNATURE_BYTES signature;
	bool enabled;
} SPIConfiguration;

void SPI_Display(void *ptrData, const char *msg);
void SPI_Initialize(void);
void SPI_Monitor(void);
void *GetPtrToSPIData(void);
void SPI_Save_Config(uint8_t channel);
void SPI_Load_Config(void);
