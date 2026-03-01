/**
 * @file    EEPRom.h
 * @brief   Internal EEPROM driver for AVR64DD32 configuration storage
 * @details Provides read/write access to internal EEPROM for persistent
 *          storage of peripheral configurations. Supports byte, word, and
 *          block operations with wear-leveling considerations.
 *
 * @author  Terry (ABS Data Systems)
 * @date    November 25, 2025
 * @version 1.0
 *
 * @par EEPROM Specifications
 *      - Size: 512 bytes (0x0000 to 0x01FF)
 *      - Endurance: 100,000 write cycles typical
 *      - Data retention: 20 years @ 55°C
 *      - Write time: ~3.3ms typical
 *
 * @par Memory Map
 *      See EEProm_Map.h for address allocations:
 *      - 0x0000-0x001F: ADC configurations
 *      - 0x0020-0x003F: DAC configurations
 *      - 0x0040-0x005F: Timer configurations
 *      - ... (see EEProm_Map.h for complete map)
 *
 * @par Features
 *      - Byte, word, and block read/write
 *      - Busy-wait during write operations
 *      - Data validation via signature bytes
 *      - Default value initialization
 *
 * @note    EEPROM writes are blocking operations (~3.3ms each)
 * @note    Signature bytes detect uninitialized/corrupted data
 * @warning Limit write frequency to preserve EEPROM lifespan
 * @warning Ensure power stability during write operations
 *
 * @see EEProm_Map.h for address definitions
 */

#pragma once

// Include headers for all structures you want to store
#include "Common/Common.h"
#include "EEPRom/EEProm_Map.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

// ============================================================================
// Generate enum for indices
// ============================================================================
typedef enum {
	#define EEPROM_ITEM(name, type, count) EEPROM_ADDR_##name,
	EEPROM_MEMORY_MAP
	#undef EEPROM_ITEM
	EEPROM_NUM_ITEMS
} eeprom_item_t;

// Array/element access functions
void EEPROM_Write_Array(uint8_t item_index, const void *data, uint16_t size);
uint8_t EEPROM_Read_Array(uint8_t item_index, void *data, uint16_t size);
void EEPROM_Write_Element(uint8_t item_index, uint16_t element_index, const void *data);
void EEPROM_Read_Element(uint8_t item_index, uint16_t element_index, void *data);
void EEPROM_Verify_Signature_And_Load(uint8_t item_index, uint16_t element_index, void *data);

// Size tables (defined in eeprom.c)
extern const uint16_t eeprom_sizes[];
extern const uint16_t eeprom_counts[];
extern const uint16_t eeprom_element_sizes[];

// Helper functions
uint16_t EEPROM_GET_TOTAL_USED(void);
uint16_t EEPROM_Get_Free_Space(void);

// ============================================================================
// Easy-to-use macros
// ============================================================================

// Write entire array
#define EEPROM_WRITE_ARRAY(name, data_ptr) \
EEPROM_Write_Array(EEPROM_ADDR_##name, data_ptr, sizeof(*data_ptr) * eeprom_counts[EEPROM_ADDR_##name])

// Read entire array
#define EEPROM_READ_ARRAY(name, data_ptr) \
EEPROM_Read_Array(EEPROM_ADDR_##name, data_ptr, sizeof(*data_ptr) * eeprom_counts[EEPROM_ADDR_##name])

// Write single element
#define EEPROM_WRITE_ELEMENT(name, index, data_ptr) \
EEPROM_Write_Element(EEPROM_ADDR_##name, index, data_ptr)

// Read single element
#define EEPROM_VERIFY_SIGNATURE_AND_LOAD(name, index, data_ptr) \
EEPROM_Verify_Signature_And_Load(EEPROM_ADDR_##name, index, data_ptr)


uint16_t EEPROM_Get_Address(uint8_t item_index);


 // Initialize the NVM Controller

void EEPROM_Initialize(void);
bool EEProm_Wait_For_Ready(void);
bool EEPROM_Erase_Byte(uint16_t address);
bool EEPROM_Write_Byte(uint16_t address, uint8_t data);
uint8_t EEPROM_Read_Byte(uint16_t address);
