/**
 * @file    Ports.h
 * @brief   GPIO Port driver for AVR64DD32
 * @details Implements GPIO port configuration and control. Supports digital
 *          input/output, pull-up resistors, interrupt-on-change, and pin
 *          inversion. Provides high-level interface for port manipulation.
 *
 * @author  Terry (ABS Data Systems)
 * @date    November 25, 2025
 * @version 1.0
 *
 * @par Available Ports
 *      - PORT A (PA0-PA7): I2C, SPI, USART
 *      - PORT C (PC0-PC3): USART, ZCD
 *      - PORT D (PD0-PD7): ADC, DAC, CCL outputs, Event outputs
 *      - PORT F (PF0-PF6): CCL inputs, Timer output, ZCD simulator
 *
 * @par Pin Features
 *      - Direction control (input/output)
 *      - Pull-up resistor enable
 *      - Input level configuration
 *      - Pin inversion
 *      - Interrupt sense control
 *
 * @par Demo Pins
 *      - PC3 (TP16): General purpose I/O demo
 *      - PD1-PD3: PORT/CCL test outputs
 *      - PF6 (H2-11): Input-only test pin
 *
 * @note    Some pins have input-only capability (PF6 on 32-pin package)
 * @note    Pull-ups typically 20-50k? (varies with supply voltage)
 * @warning Avoid conflicting peripheral and GPIO usage on same pin
 * @warning Check datasheet for maximum current source/sink per pin
 *
 * @see port.h for low-level port macros
 * @see Menu_PORTS.h for configuration interface
 */

#pragma once

#include "Common/Common.h"

#define PortsSystemSize 1

#define PC3_PortInOut 0x08
#define PF6_PortInOnly 0x40

enum PORTRESISTORS{
	PORTRESISTORS_NONE,
	PORTRESISTORS_PULLUP
};

typedef struct PortsConfiguration
{
	enum SIGNATURE_BYTES signature;
	bool enabled;
	bool portAOutput;
	bool portAOutputValue;
	bool portAInputLevel;
	bool portAInverted;
	bool portAPullUp;
	bool portBInputLevel;
	bool portBInverted;
	bool portBPullUp;
} PortsConfiguration;

void Ports_Display(void *ptrData, const char *msg);
void Ports_Initialize(void);
void Ports_ReInitialize(void);
void Ports_Monitor(void);
void *GetPtrToPortsData(void);
void Ports_Save_Config(uint8_t channel);
void Ports_Load_Config(void);
