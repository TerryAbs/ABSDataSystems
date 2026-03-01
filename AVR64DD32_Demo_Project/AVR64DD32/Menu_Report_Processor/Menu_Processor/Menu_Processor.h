/**
 * @file    Menu_Processor.h
 * @brief   Menu processing engine for AVR64DD32 demonstration system
 * @details Implements table-driven menu system with state machine control.
 *          Handles menu rendering, user input, data entry, and navigation.
 *          Supports multiple menu types: lists, dropdowns, and data entry.
 *
 * @author  Terry (ABS Data Systems)
 * @date    November 25, 2025
 * @version 1.0
 *
 * @par Menu System Architecture
 *      1. Header rendering: Draws menu title and instructions
 *      2. Body rendering: Displays menu options with command shortcuts
 *      3. Data entry: Shows current configuration values
 *      4. Input processing: Handles user commands and navigation
 *
 * @par Menu Types
 *      - MENU: Command-driven list menu
 *      - DROPDOWN: Selection from predefined options
 *      - DATAENTRY: Direct value entry with validation
 *      - DISPLAYDATA: Read-only information display
 *
 * @par State Machine
 *      IDLE ? START ? HEADER ? BODY ? DATAENTRY ? DISPLAYPROMPT
 *
 * @note    Menus defined as const structures in PROGMEM
 * @note    Function pointers enable menu action callbacks
 * @warning Menu buffer size limits maximum menu complexity
 *
 * @see DataStructures.h for MenuSystem structure
 * @see Terminal_Commands.h for VT102 control
 */

#pragma once


#include "Menu_Report_Processor/Terminal_Commands/Terminal_Commands.h"
#include "Common/Common.h"
#include "Common/DataStructures.h"


static inline void MenuBody_Read(MenuBody *dst, const MenuBody *tableP, uint8_t idx)
{
	memcpy_P(dst, &tableP[idx], sizeof(*dst));
}

static inline void MenuHeader_Read(MenuHeader *dst, const MenuHeader *tableP, uint8_t idx)
{
	memcpy_P(dst, &tableP[idx], sizeof(*dst));
}

static inline void MenuDataEntry_Read(DataEntry*dst, const DataEntry *tableP, uint8_t idx)
{
	memcpy_P(dst, &tableP[idx], sizeof(*dst));
}


// Menu Processing Functions

void Menu_Processor_Initialize(void);
void MenuSystem_Processor_Monitor(void);
bool Menu_Processor_Status(void);
void Menu_DisplayMenu(void);
void SetActiveMenu(MenuSystem *menu);
const struct MenuSystem *GetActiveMenu(void);
void * GetDataPtrForMenuItem(const char *msg);