/**
 * @file    CCL.h
 * @brief   Configurable Custom Logic driver for AVR64DD32
 * @details Implements CCL (Configurable Custom Logic) for hardware-based
 *          boolean logic operations without CPU intervention. Four independent
 *          Look-Up Tables (LUTs) can implement any 3-input logic function.
 *
 * @author  Terry (ABS Data Systems)
 * @date    November 25, 2025
 * @version 1.0
 *
 * @par LUT Configuration
 *      - LUT0-LUT3: Four independent 3-input truth tables
 *      - Input sources: GPIO pins, peripherals, or other LUTs
 *      - Output: GPIO pin or internal event
 *
 * @par Pin Configuration (Demo Board)
 *      Inputs:
 *      - PF0 (TP10): LUT Input 0
 *      - PF1 (TP9):  LUT Input 1
 *      - PF2 (TP8):  LUT Input 2
 *      
 *      Outputs:
 *      - PF3 (TP7):  LUT Output
 *      - PD1 (H2-5): CCL Test 0
 *      - PD2 (H2-7): CCL Test 1
 *      - PD3 (H2-9): CCL Test 2
 *
 * @par Example Logic Functions
 *      - AND, OR, XOR, NAND, NOR gates
 *      - Multiplexers, decoders
 *      - Edge detectors
 *      - D flip-flops, RS latches
 *      - Frequency dividers
 *
 * @par Features
 *      - 8-bit truth table for any 3-input logic function
 *      - Synchronous or asynchronous operation
 *      - Sequential logic via feedback
 *      - Filter for input debouncing
 *      - Edge detector
 *
 * @note    Truth table format: bit 0 = 000 inputs, bit 7 = 111 inputs
 * @note    LUTs can be cascaded for complex functions
 * @warning Sequential mode requires careful timing analysis
 * @warning Filter adds propagation delay - verify timing
 *
 * @see AVR64DD32 datasheet section 14 (CCL)
 * @see Menu_CCL_LUT.h for configuration interface
 */
#pragma once
#include "Common/Common.h"

#define PF0		0x01
#define PF1		0x02
#define PF2		0x04
#define PF3		0x08

#define PD1		0x02
#define PD2		0x04
#define PD3		0x08

// Pin definitions
#define PF0_INPUT_A_PIN     PF0
#define PF1_INPUT_B_PIN     PF1
#define PF2_INPUT_C_PIN     PF2
#define PF3_LUT3_OUT_PIN    PF3

#define PD1_OUTPUT_A_PIN     PD1
#define PD2_OUTPUT_B_PIN     PD2
#define PD3_OUTPUT_C_PIN     PD3



#define CCLSystemSize 1

enum CCL_INPUTSOURCE {
	CCL_INPUTS_A,
	CCL_INPUTS_A_B,
	CCL_INPUTS_A_B_C,
};
typedef struct CCLConfiguration
{
	enum SIGNATURE_BYTES signature;
	bool enabled;
	uint8_t steps;
	enum CCL_INPUTSOURCE inputSource;
	uint8_t pattern;
} CCLConfiguration;



void CCL_Display(void *ptrData, const char *msg);
void CCL_DriveTestData(int value);
void CCL_ReInitialize(void);
void CCL_Initialize(void);
void CCL_Monitor(void);
void *GetPtrToCCLData(void);
void CCL_Save_Config(uint8_t channel);
void CCL_Load_Config(void);
void GetCCLSteps(const char *msg);
void SetCCLSteps(float value);
void GetCCLPattern(const char *msg);
void SetCCLPattern(float value);
bool GetCCLEnabledState(void);