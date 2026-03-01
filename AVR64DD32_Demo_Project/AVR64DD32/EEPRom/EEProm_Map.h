/**
 * @file    EEProm_Map.h
 * @brief   EEPROM memory map definitions for AVR64DD32 project
 * @details Defines address allocations for peripheral configuration storage
 *          in internal EEPROM. Organizes memory by module with defined
 *          boundaries to prevent overlap.
 *
 * @author  Terry (ABS Data Systems)
 * @date    November 25, 2025
 * @version 1.0
 *
 * @par Memory Organization
 *      Total: 512 bytes (0x0000 - 0x01FF)
 *      
 *      Address Range | Module          | Size
 *      --------------|-----------------|-------
 *      0x0000-0x001F | ADC Config      | 32 bytes
 *      0x0020-0x003F | DAC Config      | 32 bytes
 *      0x0040-0x005F | Timer Config    | 32 bytes
 *      0x0060-0x007F | USART Config    | 32 bytes
 *      0x0080-0x009F | SPI Config      | 32 bytes
 *      0x00A0-0x00BF | TWI Config      | 32 bytes
 *      0x00C0-0x00DF | Event Config    | 32 bytes
 *      0x00E0-0x00FF | CCL Config      | 32 bytes
 *      0x0100-0x011F | Port Config     | 32 bytes
 *      0x0120-0x013F | RTC Config      | 32 bytes
 *      0x0140-0x015F | ZCD Config      | 32 bytes
 *      0x0160-0x01FF | Reserved        | 160 bytes
 *
 * @note    Each module allocation includes signature byte for validation
 * @note    Reserved space available for future expansion
 * @warning Do not modify address definitions without updating all modules
 *
 * @see EEPRom.h for access functions
 */

#pragma once

#include "ADC.h"
#include "DAC.h"
#include "CCL.h"
#include "Events.h"
#include "Ports.h"
#include "RTC.h"
#include "Timers.h"
#include "USART.h"
#include "SPI.h"
#include "TWI_I2C.h"
#include "ZCD.h"
// ============================================================================
// EEPROM MEMORY MAP - Use your existing size macros!
// Format: EEPROM_ITEM(name, type, count)
// ============================================================================
#define EEPROM_MEMORY_MAP \
EEPROM_ITEM(adc_system, ADCConfiguration, ADCSystemSize) \
EEPROM_ITEM(dac_system, DACConfiguration, DACSystemSize ) \
EEPROM_ITEM(ccl_system, CCLConfiguration, CCLSystemSize ) \
EEPROM_ITEM(events_system, EventsConfiguration, EventsSystemSize ) \
EEPROM_ITEM(ports_system, PortsConfiguration, PortsSystemSize ) \
EEPROM_ITEM(rtc_system, RTCConfiguration, RTCSystemSize ) \
EEPROM_ITEM(timers_system, TimersConfiguration, TimersSystemSize ) \
EEPROM_ITEM(usart_system, USARTConfiguration, USARTSystemSize ) \
EEPROM_ITEM(spi_system, SPIConfiguration, SPISystemSize ) \
EEPROM_ITEM(twi_i2c_system, TWI_I2CConfiguration, TWI_I2CSystemSize ) \
EEPROM_ITEM(zcd_system, ZCDConfiguration, ZCDSystemSize )
