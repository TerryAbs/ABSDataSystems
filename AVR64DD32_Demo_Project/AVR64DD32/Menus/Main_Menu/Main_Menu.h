/**
 * @file    Main_Menu.h
 * @brief   Main menu interface for AVR64DD32 demonstration system
 * @details Implements top-level menu navigation for accessing all peripheral
 *          configuration and demonstration menus. Uses table-driven menu
 *          system with VT102 terminal control.
 *
 * @author  Terry (ABS Data Systems)
 * @date    November 25, 2025
 * @version 1.0
 *
 * @par Menu Structure
 *      The main menu provides access to:
 *      - ADC Configuration and Monitoring
 *      - DAC Waveform Generation
 *      - SPI Flash Memory Operations
 *      - I2C/TWI Device Communication
 *      - Timer/PWM Configuration
 *      - Event System Routing
 *      - CCL Logic Configuration
 *      - Port I/O Configuration
 *      - USART Communication Settings
 *      - RTC Configuration
 *      - ZCD Configuration
 *
 * @par Terminal Layout
 *      - Columns 1-80: Menu area
 *      - Columns 85-120: Real-time data display
 *      - Row 40: Command prompt
 *
 * @note    Menu system uses PROGMEM for string storage
 * @note    VT102 escape codes for cursor positioning and colors
 * @see Menu_Processor.h for menu engine
 * @see DataStructures.h for menu system types
 */

#pragma once

#include "Menu_Report_Processor//Menu_Processor/Menu_Processor.h"
#include "Menus/ADC/Menu_ADC.h"
#include "Menus/CCL_LUT/Menu_CCL_LUT.h"
#include "Menus/DAC/Menu_DAC.h"
#include "Menus/Events/Menu_Events.h"
#include "Menus/Ports/Menu_Ports.h"
#include "Menus/RTC/Menu_RTC.h"
#include "Menus/SPI/Menu_SPI.h"
#include "Menus/Timers/Menu_Timers.h"
#include "Menus/TWI_I2C/Menu_TWI_I2C.h"
#include "Menus/USART/Menu_USART.h"
#include "Menus/ZCD/Menu_ZCD.h"
#include "Menus/CommonMenus/CommonMenus.h"

void MainMenu_Initialize(void);
void DisplayMainMenu(const char *msg);