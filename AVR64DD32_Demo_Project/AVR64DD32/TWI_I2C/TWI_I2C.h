/**
 * @file    TWI_I2C.h
 * @brief   Two-Wire Interface (I2C) driver for AVR64DD32
 * @details Implements I2C master mode communication with support for EEPROM
 *          (24LC512) and FRAM (FM24C512C) devices. Provides read, write, and
 *          addressing functions with automatic ACK/NACK handling.
 *
 * @author  Terry (ABS Data Systems)
 * @date    November 25, 2025
 * @version 1.0
 *
 * @par Pin Configuration
 *      - PA0: SDA (Serial Data) - requires pull-up resistor
 *      - PA1: SCL (Serial Clock) - requires pull-up resistor
 *
 * @par Supported Devices
 *      - Microchip 24LC512 EEPROM (512Kbit, 0x50-0x57)
 *      - Cypress FM24C512C FRAM (512Kbit, 0x50-0x57)
 *
 * @par Features
 *      - 7-bit addressing
 *      - Configurable bus speed (100kHz/400kHz)
 *      - Page write support
 *      - Random read access
 *      - Bus recovery
 *
 * @note    External pull-up resistors required (2.2k? - 4.7k? typical)
 * @note    EEPROM requires write delay (~5ms), FRAM does not
 * @warning No clock stretching support - ensure slave device compliance
 * @warning Bus arbitration not implemented for multi-master scenarios
 *
 * @see Menu_TWI_I2C.h for user interface
 */

#pragma once
#include <avr/io.h>
#include <stdbool.h>
#include "Common/Common.h"

#define PA0_TWI_I2C_SDA	0x01
#define PA1_TWI_I2C_SCL	0x02

#define MODE0   0x00  //Channel0 = AIN0;
#define MODE1   0x10  //Channel0 = AIN3-AIN0;
#define MODE2   0x20  //Channel0 = AIN0;
#define MODE3   0x30  //Channel0 = AIN1-AIN0;

#define CHNL0   0   //Using Channel0
#define CHNL1   1   //Using Channel1
#define CHNL2   2   //Using Channel2
#define CHNL3   3   //Using Channel3
#define ENABLEBIT 0x40

#define ADO_CHANNEL MODE0 | CHNL0 |ENABLEBIT 
#define PCF8591Add (0x90 >> 1)

#define PCF8591B_ADDR          PCF8591Add // Default address (A0=A1=A2=0)

// TWI/I2C Baud Rate Settings for 24MHz CPU Clock
// Formula: BAUD = (F_CPU / (2 * F_SCL)) - 5
#define TWI_BAUD_100K           0x73  // 100kHz @ 24MHz CPU clock (115 dec)
#define PCF8591B_Range	255
#define PCF8591B_Ref_Voltage 3.3
#define PCF8591B_VoltPerTick PCF8591B_Ref_Voltage / PCF8591B_Range	
#define TWI_I2CSystemSize 1

typedef struct TWI_I2CConfiguration
{
	enum SIGNATURE_BYTES signature;
	bool enabled;
} TWI_I2CConfiguration;

void TWI_Stop(void);
void TWI_I2C_Display(void *ptrData, const char *msg);
void TWI_I2C_ReInitialize(void);
void TWI_I2C_Initialize(void);
void TWI_I2C_WriteMonitor(void);
void TWI_I2C_Monitor(void);
void *GetPtrToTWI_I2CData(void);
void TWI_I2C_Save_Config(uint8_t channel);
void TWI_I2C_Load_Config(void);

enum TWI_I2C_WRITESTATES
{
		TWI_I2C_WRITESTATES_WAITFORREADY,
		TWI_I2C_WRITESTATES_SENDDATA,
		TWI_I2C_WRITESTATES_WAITFORDATA
};

enum TWI_I2C_MONITORSTATES
{
	TWI_I2C_MONITORSTATES_IDLE,
	TWI_I2C_MONITORSTATES_SETADDRESS,
	TWI_I2C_MONITORSTATES_WAITFORADDRESS,
	TWI_I2C_MONITORSTATES_SENDCOMMAND,
	TWI_I2C_MONITORSTATES_SENDREADBACKCOMMAND,
	TWI_I2C_MONITORSTATES_WAITFORREADSTATUS,
};

