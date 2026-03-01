/**
 * @file    USART.h
 * @brief   Universal Synchronous/Asynchronous Receiver/Transmitter driver
 * @details Implements buffered serial communication on USART0 and USART1.
 *          USART1 is used for VT102 terminal interface (PC0/PC1), USART0
 *          provides secondary serial port (PA2/PA3).
 *
 * @author  Terry (ABS Data Systems)
 * @date    November 25, 2025
 * @version 1.0
 *
 * @par Pin Configuration
 *      USART1 (Primary - Terminal):
 *      - PC0: TxD (Transmit)
 *      - PC1: RxD (Receive)
 *      
 *      USART0 (Secondary):
 *      - PA2: TxD
 *      - PA3: RxD
 *
 * @par Features
 *      - Interrupt-driven TX/RX with circular buffers
 *      - Configurable baud rate (9600 to 115200 typical)
 *      - 8N1 format (8 data bits, no parity, 1 stop bit)
 *      - printf/scanf integration via USART1
 *      - Error detection (frame, parity, overrun)
 *
 * @note    USART1 is configured in atmel_start for terminal I/O
 * @note    Buffer sizes can be adjusted in usart_basic.h
 * @warning RX buffer overflow occurs if data not read promptly
 * @warning Incorrect baud rate causes garbled communication
 *
 * @see usart_basic.h for low-level USART operations
 * @see SerialComm_Receive.h for command processing
 */

#pragma once

#define USARTSystemSize 1
#include "Common/Common.h"

#define PA2_USART0_TX 0x04
#define PA3_USART0_RX 0x08
#define USART0_BAUD_RATE(BAUD_RATE) ((float)(24000000 * 64 / (16 * (float)BAUD_RATE)) + 0.5)
#define USART_0_BufferSize 100

enum BAUDRATES {
	BAUDRATES_300,
	BAUDRATES_600,
	BAUDRATES_1200,
	BAUDRATES_2400,
	BAUDRATES_4800,
	BAUDRATES_9600,
	BAUDRATES_14400,
	BAUDRATES_19200,
	BAUDRATES_28800,
	BAUDRATES_38400,
	BAUDRATES_57600,
	BAUDRATES_115200,
	BAUDRATES_230400,
	BAUDRATES_460800,
	BAUDRATES_921600,	
};

typedef struct BaudRateConversion
{
	enum BAUDRATES baudrate;
	uint32_t value;
}BaudRateConversion;

typedef struct USARTConfiguration
{
	enum SIGNATURE_BYTES signature;
	bool enabled;
	uint8_t baud_rate;
	uint8_t parity;      // 0=None, 1=Even, 2=Odd
	uint8_t stop_bits;   // 1 or 2
	uint8_t data_bits;   // 5, 6, 7, 8, or 9
} USARTConfiguration;

void USART_Display(void *ptrData, const char *msg);
void USART_ReInitialize(void);
void USART_Initialize(void);
void USART_Monitor(void);
void *GetPtrToUSARTData(void);
void USART_Save_Config(uint8_t channel);
void USART_Load_Config(void);

char *DecodeBaudRate(void *ptr);
char *DecodeParity(void *ptr);
char *DecodeStopbits(void *ptr);
char *DecodeDataBits(void *ptr);

void USART_0_enable(void);
void USART_0_enable_rx(void);
void USART_0_enable_tx(void);
void USART_0_disable(void);
uint8_t USART_0_get_data(void);
bool USART_0_is_tx_ready(void);
bool USART_0_is_rx_ready(void);
bool USART_0_is_tx_busy(void);
uint8_t USART_0_read(void);
void USART_0_write(const uint8_t data);

void USART_0_ClearBuffer(void);
bool USART_0_Print(char *msg);
// This forces the completions of the previous write so this 
// Message is alway accepted.
void USART_0_Print_Immediate(char *msg);

void USART_0_AppendCharToBuffer(char msg);