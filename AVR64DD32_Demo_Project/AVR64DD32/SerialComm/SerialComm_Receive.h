/**
 * @file    SerialComm_Receive.h
 * @brief   Serial command reception and parsing for AVR64DD32 system
 * @details Implements buffered reception of serial commands from USART1.
 *          Handles line editing (backspace), echo, and command termination
 *          (Enter key). Integrates with menu processor for command execution.
 *
 * @author  Terry (ABS Data Systems)
 * @date    November 25, 2025
 * @version 1.0
 *
 * @par Command Processing
 *      1. Buffer received characters
 *      2. Echo characters to terminal
 *      3. Handle backspace/delete
 *      4. Detect Enter (newline)
 *      5. Pass command to Menu Processor
 *      6. Clear buffer for next command
 *
 * @par Features
 *      - Line editing with backspace support
 *      - Command echo to terminal
 *      - Case-insensitive command matching
 *      - Command prompt display
 *      - Buffer overflow protection
 *
 * @par Buffer Management
 *      - Circular buffer for received characters
 *      - Automatic overflow handling
 *      - Configurable buffer size
 *
 * @note    Integrates with USART interrupt handler
 * @note    Commands are whitespace-terminated
 * @warning Buffer size limits maximum command length
 *
 * @see Menu_Processor.h for command execution
 * @see USART.h for serial communication
 */

#pragma once
#include "Common/Common.h"


#define EOL			0x0D
#define FALSE_EOL	0x0A

#define RECEIVE_BUFFER_SIZE 255

// ProtoTypes for Receive
void SerialComm_Receive_Initialize(void);
void Flush_ReceiveBuffer(void);
bool GetCmdReady_Status(void);
char *GetReceiveBuffer(void);
bool Store_ReceviedData(uint8_t data);
void SerialComm_Receive_Monitor(void);

enum SERIALCOMM_RECEIVESTATES{
	SERIALCOMM_RECEIVESTATES_WAITFORDATA,
	SERIALCOMM_RECEIVESTATES_PROCESSDATA
};