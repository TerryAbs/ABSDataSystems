
#include "Common/Common.h"

#include "LED_Bar/LED_Bar.h"
#include "Display_Processor/Display_Processor.h"
#include "ADC/ADC.h"
#include "DAC/DAC.h"
#include "Events/Events.h"
#include "Ports/Ports.h"
#include "Timers/Timers.h"

#include "SPI/SPI.h"
#include "TWI_I2C/TWI_I2C.h"
#include "USART/usart_basic.h"
#include "SerialComm/SerialComm_Receive.h"
#include "Menus/Main_Menu/Main_Menu.h"
#include "Menu_Report_Processor/Menu_Processor/Menu_Processor.h"

#include "EEPRom/EEPRom.h"

// Design notes and thoughts
// The original design goal was to create a sample project
// that outlined the different sub systems.
// As typical, i started thinking about way I would use this demo code
// and shifted gears to make it more like a meter or piece of test equipment.
// So this current iteration looks like the following:
// 
// I'm using a VT102 terminal emulator software called Minicom
// The terminal software is able to handle window sizing so I'm
// taking advantage of that and I decided to create the following user interface
//
// My resolution is 120 columns by 40 lines. I know its a weird layout.
//
// I broke the screen into sections
// 80x40 starting at 1,0 is for menus.
// 85-120 is used for data.
// Row0, 40 is where the command prompt lives.
//
// All the menus are driven by tables
// 
// There are current 3 types of menus
// Menus: where a list is displayed
// Menus: where there is a drop down.
// Menus: where you .... TDB


/*
ATMEL-ICE
Pin	2					GND
Pin 4					+5
Pin 3					Pin 27 AVR64DD32
*/

/**
 * @file    main.c
 * @brief   Main entry point for AVR64DD32 educational demonstration project
 * @details Comprehensive demonstration of AVR64DD32 micro controller peripheral
 *          features including ADC, DAC, USART, SPI, I2C, timers, events, and more.
 *          Implements a VT102 terminal-based menu system for interactive hardware
 *          exploration and configuration.
 *
 * @author  Terry (ABS Data Systems)
 * @date    Created: 11/25/2025
 * @version 1.0
 *
 * @par Hardware Configuration
 *      Terminal: VT102 emulator (Minicom) @ 120x40 resolution
 *      Screen Layout:
 *      - Columns 1-80: Menu display area
 *      - Columns 85-120: Data display area
 *      - Row 40: Command prompt
 *
 * @par Pin Configuration (FTDI Demo Board)
 * @verbatim
 *   AVR64DD32    FTDI Board   Usage
 *   ---------    ----------   -----
 *   PA0          H2-2         TWI/I2C (SDA)
 *   PA1          H2-4         TWI/I2C (SCL), EVOUT_A
 *   PA2          H2-6         UART0_TX
 *   PA3          H2-8         UART0_RX
 *   PA4          H2-10        SPI_MOSI
 *   PA5          H2-12        SPI_MISO
 *   PA6          H2-14        SPI_SCK
 *   PA7          H2-16        SPI_SS, ZC_Out
 *   
 *   PC0          TxD          UART
 *   PC1          RxD          UART
 *   PC2          TP14         ZCD (ZC_In)
 *   PC3          TP16         Ports_InOut
 *   
 *   PD1          H2-5         CCL Test 0, PORT_DEMO_0
 *   PD2          H2-7         CCL Test 1, PORT_DEMO_1
 *   PD3          H2-9         CCL Test 2, PORT_DEMO_2
 *   PD4          TP15         ADC Average
 *   PD5          TP13         ADC Instant
 *   PD6          TP12         DAC Output
 *   PD7          TP11         EVOUTD
 *   
 *   PF0          TP10         CCL Input 0
 *   PF1          TP9          CCL Input 1
 *   PF2          TP8          CCL Input 2
 *   PF3          TP7          CCL Output
 *   PF4          TP6          Timer
 *   PF5          TP5          ZCD_Simulator
 *   PF6          H2-11        Ports_In (input only)
 * @endverbatim
 *
 * @note    Global interrupts (sei) must be enabled for proper SPI operation
 * @warning Ensure all peripherals are initialized before entering main loop
 *
 * @see atmel_start_init()
 */
int main() 
{	
	atmel_start_init();
	USART_1_Initialize();
	EEPROM_Initialize();
	DisplayProcessor_Initialize();
	//LED_Bar_Initialize();  // This function is no longer viable as we have used these pins for alternate functions
	ADC_Initialize();
	CCL_Initialize();
	DAC_Initialize();
	Ports_Initialize();
	RTC_Initialize();
	SPI_Initialize();
	Timers_Initialize();
	TWI_I2C_Initialize();
	USART_Initialize();
	ZCD_Initialize();
    // The events system using other sub systems and therefore needs to be load
    // last.
    Events_Initialize();
	SerialComm_Receive_Initialize();
	Menu_Processor_Initialize();
	MainMenu_Initialize();

    // CRITICAL: Enable global interrupts (SPI driver uses interrupts!)
    sei();

	while (1) {
		MenuSystem_Processor_Monitor();
		DisplayProcessor_Monitor();
		SerialComm_Receive_Monitor();
		Command_Process_Monitor();
		ADC_Monitor();
		DAC_Monitor();
		CCL_Monitor();
		Events_Monitor();
		Ports_Monitor();
		RTC_Monitor();
		Timers_Monitor();
		USART_Monitor();
		SPI_Monitor();
		TWI_I2C_Monitor();
		ZCD_Monitor();
	}
}
