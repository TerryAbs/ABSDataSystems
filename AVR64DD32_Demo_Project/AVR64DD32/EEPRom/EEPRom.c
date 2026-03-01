/*
 * EEPRom.c
 *
 * Created: 12/25/2025 9:38:12 AM
 *  Author: terry
 */ 
#include "EEPRom/EEPRom.h"
#include "Menu_Report_Processor/Terminal_Commands/Terminal_Commands.h"
#include "CommonStrings/CommonStrings.h"


// There is a section of Flash memory that is
// allocated for application Data called AppData
// it's 256 bytes in size.

// I plan on using this area to store our configuration data 
// for this application

// The way this code if going to work
// 1. There is a EEProm map file which controls the memory layout.
// 2. The first byte of each element will be unique to that configuration.
// 3. On the read request of the Element or section of configuration information.
//    the read function will evaluate the signature byte. If they match,
//    the assumption is that the data stored was previously written.
//    If the signature byte does not match, them the data provided, which are the default values
//    will be saved.

// 4. When saving to EEPROM, which I'll assume is often, the
//   code will compare for changes first, prior to saving the data.
//   This will help to reduce the number of erases and writes, which will preserve the life span of this part.
//   

// Other notes:

// While this hardware supports 1/2/4/8/16/32 blocks.
// I intend to keep this simple and only work with a single byte at a time.
// That way I can perform a read prior to write

// This is not state machine driven by design.
// I know this goes against my design constructs.
// This is because of the critical nature of timing within this code.

// Since this is not a production product, I have not added CRC support, which I would normally have added.
// Signature values cannot be FF as that is the default value from a erased EEPROM

// This code can have errors because of a host of potential issues. Like write failure.
// All error are sent to a common area are are printed on the screen. Just above the prompt.

// _PROTECTED_WRITE_SPM is a macro which controls the proper way to set NVMCTRL.CTRLA register.

// ============================================================================
// EEPROM STORAGE LAYOUT DEFINITION
// ============================================================================
// Define all your structures here in one place
// The system will automatically calculate addresses and check size at compile time
// See and modify EEProm_Map.h file


 // ============================================================================
 // Size and count tables
 // ============================================================================
 const uint16_t eeprom_sizes[] = {
	 #define EEPROM_ITEM(name, type, count) (sizeof(type) * (count)),
	 EEPROM_MEMORY_MAP
	 #undef EEPROM_ITEM
 };

 const uint16_t eeprom_counts[] = {
	 #define EEPROM_ITEM(name, type, count) (count),
	 EEPROM_MEMORY_MAP
	 #undef EEPROM_ITEM
 };

 const uint16_t eeprom_element_sizes[] = {
	 #define EEPROM_ITEM(name, type, count) sizeof(type),
	 EEPROM_MEMORY_MAP
	 #undef EEPROM_ITEM
 };

 // ============================================================================
 // Compile-time size check
 // ============================================================================
 #define EEPROM_CALC_SIZE(name, type, count) + (sizeof(type) * (count))
 #define EEPROM_ITEM(name, type, count) EEPROM_CALC_SIZE(name, type, count)
 #define EEPROM_TOTAL_SIZE (0 EEPROM_MEMORY_MAP)

 _Static_assert(EEPROM_TOTAL_SIZE <= EEPROM_SIZE,
 "EEPROM storage exceeds 256 bytes!");

 // ============================================================================
 // Higher-level array/element functions
 // ============================================================================

 uint16_t EEPROM_Get_Address(uint8_t item_index)
 {
	 uint16_t addr = 0;
	 for (uint8_t i = 0; i < item_index; i++)
	 {
		 addr += eeprom_sizes[i];
	 }
	 return addr;
 }

 void EEPROM_Write_Array(uint8_t item_index, const void *data, uint16_t size)
 {
	 uint16_t base_addr = EEPROM_Get_Address(item_index);
	 const uint8_t *byte_ptr = (const uint8_t *)data;
	 
	 if (size != eeprom_sizes[item_index]) 
	 {
		LogErrorToScreen(msg_EEPROM_INVALIDSIZE);
		return;
	 }
	 
	 for (uint16_t i = 0; i < size; i++)
	 {
		 if (!EEPROM_Write_Byte(base_addr + i, byte_ptr[i]))
		 {
			 // Failed. EEPROM_Write_Byte reported the error.
			 break;
		 }
	 }
 }

// This function will make the decision whether to 
// use the Default data or to load the stored data.
void EEPROM_Verify_Signature_And_Load(uint8_t item_index, uint16_t element_index, void *data)
{
	if (element_index >= eeprom_counts[item_index])
	{
		LogErrorToScreen(msg_EEPROM_NO_DATA);
	}
	else
	{	
		uint16_t base_addr = EEPROM_Get_Address(item_index);
		uint16_t element_size = eeprom_element_sizes[item_index];
		uint16_t element_addr = base_addr + (element_index * element_size);
	
		uint8_t *byte_ptr = (uint8_t *)data;
		// If the first byte, which is the signature byte matches
		// Then continue to perform the read operation.
		// else
		// save an return results;
		if(byte_ptr[0] == EEPROM_Read_Byte(element_addr))
		{
			EEPROM_Read_Element(item_index, element_index, data);
		}
		else
		{
			EEPROM_Write_Element(item_index, element_index, data);
		}
	}
}

 void EEPROM_Write_Element(uint8_t item_index, uint16_t element_index, const void *data)
 {
	if (element_index >= eeprom_counts[item_index])
	{
		LogErrorToScreen(msg_EEPROM_NO_DATA);
	}
	else
	{
		uint16_t base_addr = EEPROM_Get_Address(item_index);
		uint16_t element_size = eeprom_element_sizes[item_index];
		uint16_t element_addr = base_addr + (element_index * element_size);
	 
		const uint8_t *byte_ptr = (const uint8_t *)data;
	 
		for (uint16_t i = 0; i < element_size; i++)
		{
			if (!EEPROM_Write_Byte(element_addr + i, byte_ptr[i]))
			{
				// EEPROM_Write_Byte reported the error
				break;
			}
		}
	}
 }


 void EEPROM_Read_Element(uint8_t item_index, uint16_t element_index, void *data)
 {
	if (element_index >= eeprom_counts[item_index])
	{
		LogErrorToScreen(msg_EEPROM_NO_DATA);
	}
	else
	{	 
		 uint16_t base_addr = EEPROM_Get_Address(item_index);
		 uint16_t element_size = eeprom_element_sizes[item_index];
		 uint16_t element_addr = base_addr + (element_index * element_size);
	 
		 uint8_t *byte_ptr = (uint8_t *)data;
	 
		 for (uint16_t i = 0; i < element_size; i++)
		 {
			 byte_ptr[i] = EEPROM_Read_Byte(element_addr + i);
		 }
	}
 }

 uint16_t EEPROM_Get_Total_Used(void)
 {
	 uint16_t total = 0;
	 for (uint8_t i = 0; i < EEPROM_NUM_ITEMS; i++)
	 {
		 total += eeprom_sizes[i];
	 }
	 return total;
 }

 uint16_t EEPROM_Get_Free_Space(void)
 {
	 return EEPROM_SIZE - EEPROM_Get_Total_Used();
 }
 

// Initialize the NVM Controller
void EEPROM_Initialize(void)
{
    // Wait for any ongoing operations
    while (NVMCTRL.STATUS & (NVMCTRL_FBUSY_bm | NVMCTRL_EEBUSY_bm));
    
    // Clear any error flags
    NVMCTRL.STATUS = NVMCTRL_ERROR0_bm;
    
    // Set command to NONE
    _PROTECTED_WRITE_SPM(NVMCTRL.CTRLA, NVMCTRL_CMD_NONE_gc);
}

// Wait for NVM to be ready
bool EEProm_Wait_For_Ready(void)
{
    uint32_t timeout = 100000;
    while ((NVMCTRL.STATUS & NVMCTRL_EEBUSY_bm) && timeout--)
    {
        // Wait for EEPROM operation to complete
    }
	if(timeout == 0)
	{
		LogErrorToScreen(msg_EEPROM_TIMEDOUT);
		return false;
	}
    return true;
}

// Erase a single EEPROM byte
bool EEPROM_Erase_Byte(uint16_t address)
{
    uint8_t *eeprom_ptr = (uint8_t *)(EEPROM_START + address);
    
    // Validate address
    if (address >= EEPROM_SIZE)
	{
		LogErrorToScreen(msg_EEPROM_INVALIDSIZE);
		return false;
	}
	
    // Wait for ready
    if (!EEProm_Wait_For_Ready())
	{
		return false;
	}
    
    // Issue byte erase command
    _PROTECTED_WRITE_SPM(NVMCTRL.CTRLA, NVMCTRL_CMD_EEBER_gc);
    
    // Write to address to trigger erase
    *eeprom_ptr = 0xFF;
    
    // Wait for completion
    if (!EEProm_Wait_For_Ready())
	{
		return false;
	}
    // Clear command
    _PROTECTED_WRITE_SPM(NVMCTRL.CTRLA, NVMCTRL_CMD_NONE_gc);
	
    return true;	
}

// Write a single byte to EEPROM (erases first)
bool EEPROM_Write_Byte(uint16_t address, uint8_t data)
{
    uint8_t *eeprom_ptr = (uint8_t *)(EEPROM_START + address);
	
    // We want to read the current value and compare it
	// against the value we are going to write
	// and if its the same.	Were done.
	// This sames wear and tear on the EEPROM.
	if(EEPROM_Read_Byte(address) == data)
	{
		return true;
	}
	
    // Validate address
    if (address >= EEPROM_SIZE)
    {
	    LogErrorToScreen(msg_EEPROM_INVALIDSIZE);
	    return false;
    }
    
    // Erase the byte first
    if (!EEPROM_Erase_Byte(address))
    {
	    return false;
    }
    
    if (!EEProm_Wait_For_Ready())
    {
	    return false;
    }
    
    // Issue write command
    _PROTECTED_WRITE_SPM(NVMCTRL.CTRLA, NVMCTRL_CMD_EEWR_gc);
    
    // Write data
    *eeprom_ptr = data;
    
    // Wait for completion
    if (!EEProm_Wait_For_Ready())
	{
		 return false;
	}
    
    // Check for write error
    if (NVMCTRL.STATUS & NVMCTRL_ERROR0_bm)
    {
        NVMCTRL.STATUS = NVMCTRL_ERROR0_bm;
        _PROTECTED_WRITE_SPM(NVMCTRL.CTRLA, NVMCTRL_CMD_NONE_gc);
		 LogErrorToScreen(msg_EEPROM_FAILED_STATUS);
        return false;
    }
    
    // Clear command
    _PROTECTED_WRITE_SPM(NVMCTRL.CTRLA, NVMCTRL_CMD_NONE_gc);
	
    return true;
}

// Read a single byte from EEPROM
uint8_t EEPROM_Read_Byte(uint16_t address)
{
    uint8_t *eeprom_ptr = (uint8_t *)(EEPROM_START + address);
    
    // Validate address
	if (address >= EEPROM_SIZE)
	{
		LogErrorToScreen(msg_EEPROM_INVALIDSIZE);
		return 0xFF;
	}
    
    // Wait for any operations to complete
    EEProm_Wait_For_Ready();
    
    // Read directly from EEPROM mapped memory
    return *eeprom_ptr;
}
