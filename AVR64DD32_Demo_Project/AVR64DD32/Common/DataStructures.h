/**
 * @file    DataStructures.h
 * @brief   Core data structure definitions for AVR64DD32 project
 * @details Defines fundamental data structures used throughout the project
 *          including menu system types, display processor types, and common
 *          enumerations. Central location for type definitions.
 *
 * @author  Terry (ABS Data Systems)
 * @date    Created: 12/15/2025 3:59:05 PM
 * @version 1.0
 *
 * @par Key Structures
 *      - MenuSystem: Complete menu definition
 *      - MenuHeader/MenuBody: Menu component definitions
 *      - DataEntry: Data field configuration
 *      - DisplayProcessorMap: Display item registration
 *      - Theme: Color scheme definition
 *
 * @par Enumerations
 *      - SIGNATURE_BYTES: Module identification
 *      - FIELDTYPE: Data field types (bool, uint8, uint16, float)
 *      - DATAENTRYMODE: Menu interaction modes
 *      - SYSTEMMENU_PROCESSING_STATES: Menu state machine
 *
 * @note    All structures designed for PROGMEM storage
 * @note    Function pointers enable polymorphic behavior
 * @see Menu_Processor.h for menu engine
 * @see Display_Processor.h for display system
 */

#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <avr/pgmspace.h>

enum SIGNATURE_BYTES
{
	SIGNATURE_BYTES_ADC     = 0x10,
	SIGNATURE_BYTES_CCL     = 0x20,
	SIGNATURE_BYTES_DAC     = 0x30,
	SIGNATURE_BYTES_EVENTS  = 0x40,
	SIGNATURE_BYTES_PORTS   = 0x50,
	SIGNATURE_BYTES_RTC     = 0x60,
	SIGNATURE_BYTES_SPI     = 0x70,
	SIGNATURE_BYTES_TIMERS  = 0x80,
	SIGNATURE_BYTES_TWI     = 0x90,
	SIGNATURE_BYTES_USART   = 0xA0,
	SIGNATURE_BYTES_ZCD     = 0xB0
};

enum SYSTEMMENU_PROCESSING_STATES {
	SYSTEMMENU_PROCESSING_STATE_IDLE,
	SYSTEMMENU_PROCESSING_STATE_START,
	SYSTEMMENU_PROCESSING_STATE_HEADER,
	SYSTEMMENU_PROCESSING_STATE_HEADER_TEST,
	SYSTEMMENU_PROCESSING_STATE_BODY,
	SYSTEMMENU_PROCESSING_STATE_BODY_TEST,
	SYSTEMMENU_PROCESSING_STATE_DATAENTRY,
	SYSTEMMENU_PROCESSING_STATE_DATAENTRY_TEST,
	SYSTEMMENU_PROCESSING_STATE_DISPLAYPROMPT
};

enum FIELDTYPE {
//	FIELDTYPE_NONE,
	FIELDTYPE_BOOL,
	FIELDTYPE_UINT8,
	FIELDTYPE_UINT16,
	FIELDTYPE_FLOAT
//	FIELDTYPE_STRING,
};

enum DATAENTRYMODE {
	DATAENTRYMODE_MENU,
	DATAENTRYMODE_DISPLAYDATA,
	DATAENTRYMODE_DROPDOWN,
	DATAENTRYMODE_ENTERVALUE
};

// Theme is used to create a color scheme.
typedef struct Theme {
	char foreColor;
	char backGroundColor;
}Theme;

// Menu Header Description
typedef struct MenuHeader{
	char row;
	char col;
	const Theme *theme;
	PGM_P cmd_Msg_P;
	uint8_t cmd_MsgSize;
}MenuHeader;

// Menu Body Description
// This appears on the left side of the terminal
typedef struct MenuBody{
	char row;
	char col;
	const Theme *theme;
	PGM_P cmd_Msg_P;
	uint8_t cmdSize;
	PGM_P description_P;
	void (*funcptr)(const char *);      // Pointer to the function call that will be called when the user enters the CMD_Msg Text
}MenuBody;

// Data Entry Description
// This tell how to decode the data that appears on the right side of the terminal
typedef struct DataEntry {
	char row;
	char col;
	const Theme *theme;
	uint16_t offset;    
	enum FIELDTYPE fieldType;
	char *(*Decodefuncptr)(void *);	// Points to the decode function
	void (*Editfuncptr)(char *);	// Points to the Editor function
}DataEntry;

typedef struct DataEntryScreenArea {	char sRow;
	char sCol;
	char eRow;
	char eCol;
	const Theme *theme;
}DataEntryScreenArea;

typedef struct EnterValueByHand
{
	enum FIELDTYPE fieldtype;
	float Min;
	float Max;
	const char *msg;
	void (*ptrSaveFunction)(float value);
}EnterValueByHand;

typedef struct SaveConfig{
	uint8_t index;
	void (*Savefuncptr)(uint8_t index);	// Points to the Editor function
}SaveConfig;

// This describes the overall menu
typedef struct MenuSystem {
	const MenuHeader *menuHeader;
	uint8_t menuHeaderSize;
	const MenuBody *menuBody;
	uint8_t menuBodySize;
	const DataEntry *dataEntry;
	uint8_t dataEntrySize;
	void *dataptr;
	enum DATAENTRYMODE dataEntryMode;
	const DataEntryScreenArea screenArea; 
	struct MenuSystem *callingMenu;
	struct SaveConfig saveConfig;
} MenuSystem;

// This is used by the menu state machine
// to track progress.
typedef struct SystemMenuProcessor {
	enum SYSTEMMENU_PROCESSING_STATES StateMachine;
	uint8_t currentRow;
} SystemMenuProcessor;


// Display Processor

typedef struct DisplayProcessorMap  {
	void (*funcptrPrint)(void *ptrData, const char *msg);
	void *ptrData;
	const char *msg;
	bool enabled;
}DisplayProcessorMap;
	