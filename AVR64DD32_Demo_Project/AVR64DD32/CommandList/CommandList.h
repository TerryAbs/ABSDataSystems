/**
 * @file    CommandList.h
 * @brief   Command parser and dispatcher for AVR64DD32 system
 * @details Implements command table lookup and execution. Maps text commands
 *          to function callbacks. Supports command history and help system.
 *
 * @author  Terry (ABS Data Systems)
 * @date    November 25, 2025
 * @version 1.0
 *
 * @par Command Structure
 *      Each command contains:
 *      - Command string (e.g., "adc", "dac")
 *      - Description (help text)
 *      - Function pointer (callback on match)
 *
 * @par Command Processing
 *      1. Receive command string
 *      2. Search command table
 *      3. Execute callback if found
 *      4. Display error if not found
 *
 * @par Built-in Commands
 *      - help: Display command list
 *      - clear: Clear screen
 *      - version: Show firmware version
 *      - reset: Software reset
 *
 * @note    Commands stored in PROGMEM
 * @note    Case-insensitive matching
 * @see Menu_Processor.h for menu integration
 */

#pragma once

enum COMMANDMODE{
	COMMANDMODE_MENU,
	COMMANDMODE_VALUE
};

void ProcessCommandFromValueEntered(char *buf);
void SetCommandMode(enum COMMANDMODE mode);
void ProcessCommandFromMenus(char *buf);
void Command_Process_Monitor(void);
