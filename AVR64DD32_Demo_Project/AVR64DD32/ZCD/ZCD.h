/**
 * @file    ZCD.h
 * @brief   Zero-Cross Detector driver for AVR64DD32
 * @details Implements ZCD (Zero-Cross Detector) for detecting AC zero-crossing
 *          points. Useful for AC phase control, dimmer circuits, and AC
 *          frequency measurement. Can generate interrupts on positive or
 *          negative zero crossings.
 *
 * @author  Terry (ABS Data Systems)
 * @date    November 25, 2025
 * @version 1.0
 *
 * @par Pin Configuration
 *      - PC2 (TP14): ZCD Input (AC signal)
 *      - PA7: ZCD Output (optional)
 *      - PF5 (TP5): ZCD Simulator (test signal generator)
 *
 * @par Features
 *      - Detects positive and negative zero crossings
 *      - Interrupt generation on crossing events
 *      - Output signal on zero crossing
 *      - Software-enabled/disabled
 *
 * @par Typical Applications
 *      - AC dimmer control (TRIAC triggering)
 *      - AC frequency measurement
 *      - Power line synchronization
 *      - AC phase control
 *
 * @par Test Circuit
 *      Use PF5 to generate test waveform for ZCD input on PC2.
 *      Resistor divider may be needed to scale AC voltage to safe levels.
 *
 * @note    Input voltage must be within safe operating range (see datasheet)
 * @warning Do NOT connect mains AC directly - use isolation transformer
 * @warning Ensure proper voltage scaling and isolation
 * @warning ZCD has specific input voltage requirements (0-3.3V range)
 *
 * @see AVR64DD32 datasheet section 32 (ZCD)
 * @see Menu_ZCD.h for configuration interface
 */

#pragma once
#include "Common/Common.h"

#define ZCDSystemSize 1
#define PC2_ZCD_PortPin 0x04
#define PF5_SimulationPin	0x20

typedef struct ZCDConfiguration
{
	enum SIGNATURE_BYTES signature;
	bool enabled;
	uint8_t edge;
	uint16_t loopDelay;
} ZCDConfiguration;

void ZCD_Display(void *ptrData, const char *msg);
void ZCD_ReInitialize(void);
void ZCD_Initialize(void);
void ZCD_Monitor(void);
void *GetPtrToZCDData(void);
void ZCD_Save_Config(uint8_t channel);
void ZCD_Load_Config(void);
void GetLoopDelay(const char *msg);
void SetLoopDelay(float value);
char *DecodeInterruptEdge(void *ptr);
bool GetZCDEnabledState(void);