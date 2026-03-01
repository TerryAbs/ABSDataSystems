/*
 * USART.c
 *
 * Created: 1/2/2026 10:59:45 AM
 *  Author: terry
 */ 
#include <avr/io.h>
#include <stdio.h>
#include <stdbool.h>
#include "USART/USART.h"
#include "EEPRom/EEPRom.h"
#include "CommonStrings/CommonStrings.h"
#include "Display_Processor/Display_Processor.h"

static char USART_0_Buffer[USART_0_BufferSize + 1];
static uint8_t USART_0_Index = 0;
static uint8_t USART_0_CurrentIndex = 0;

BaudRateConversion BaudRateTable[] = {
	{BAUDRATES_300,		300},
	{BAUDRATES_600,		600},
	{BAUDRATES_1200,	1200},
	{BAUDRATES_2400,	2400},
	{BAUDRATES_4800,	4800},
	{BAUDRATES_9600,	9600},
	{BAUDRATES_14400,	14400},
	{BAUDRATES_19200,	19200},
	{BAUDRATES_28800,	28800},
	{BAUDRATES_38400,	38400},
	{BAUDRATES_57600,	57600},
	{BAUDRATES_115200,	115200},
	{BAUDRATES_230400,	230400},
	{BAUDRATES_460800,	460800},
	{BAUDRATES_921600,	921600}
};
#define BaudRateTableSize  sizeof(BaudRateTable) / sizeof(BaudRateConversion)

static const char* msg_Config_BaudRate;
static const char* msg_Config_Parity;
static const char* msg_Config_StopBits;
static const char* msg_Config_DataBits;

USARTConfiguration USARTSystem = {
	SIGNATURE_BYTES_USART,
	true,							
	BAUDRATES_115200,
	USART_PMODE_DISABLED_gc,
	USART_SBMODE_1BIT_gc,
	USART_CHSIZE_8BIT_gc
};

// These pointers are not need for this system.
void USART_Display(void *ptrData, const char *msg)
{
	if(USARTSystem.enabled)
	{
		SetColor(Color_White, Color_Black);
		printf_P(PSTR("USART: %S %S %S %S"), msg_Config_BaudRate, msg_Config_Parity, msg_Config_StopBits, msg_Config_DataBits);
	}
}

void USART_0_ClearBuffer(void)
{
	memset(USART_0_Buffer, 0, USART_0_BufferSize + 1);
	USART_0_Index = 0;
	USART_0_CurrentIndex = 0;
}

void USART_ReInitialize(void)
{
	float DefaultBaudRate = 19200.0;
	USART_Load_Config();
	PORTMUX.USARTROUTEA = 0x0;
	PORTA.DIRCLR = PA3_USART0_RX;
	PORTA_set_port_level(PA3_USART0_RX, false);
	// Set pin direction to output

	PORTA_set_port_level(PA2_USART0_TX, false);
	PORTA.DIRSET = PA2_USART0_TX;
	 
	if(USARTSystem.baud_rate <= BaudRateTableSize)
	{
		DefaultBaudRate = (float)BaudRateTable[USARTSystem.baud_rate].value;
	}
	USART0.BAUD = (uint16_t)USART0_BAUD_RATE(DefaultBaudRate); /* set baud rate register */

	// USART0.CTRLA = 0 << USART_ABEIE_bp /* Auto-baud Error Interrupt Enable: disabled */
	//		 | 0 << USART_DREIE_bp /* Data Register Empty Interrupt Enable: disabled */
	//		 | 0 << USART_LBME_bp /* Loop-back Mode Enable: disabled */
	//		 | USART_RS485_DISABLE_gc /* RS485 Mode disabled */
	//		 | 0 << USART_RXCIE_bp /* Receive Complete Interrupt Enable: disabled */
	//		 | 0 << USART_RXSIE_bp /* Receiver Start Frame Interrupt Enable: disabled */
	//		 | 0 << USART_TXCIE_bp; /* Transmit Complete Interrupt Enable: disabled */

	USART0.CTRLB = 0 << USART_MPCM_bp       /* Multi-processor Communication Mode: disabled */
	| 0 << USART_ODME_bp     /* Open Drain Mode Enable: disabled */
	| 1 << USART_RXEN_bp     /* Receiver Enable: enabled */
	| USART_RXMODE_NORMAL_gc /* Normal mode */
	| 0 << USART_SFDEN_bp    /* Start Frame Detection Enable: disabled */
	| 1 << USART_TXEN_bp;    /* Transmitter Enable: enabled */

	USART0.CTRLC = USART_CMODE_ASYNCHRONOUS_gc /* Asynchronous Mode */
	 	| USARTSystem.data_bits /* Character size: 8 bit */
	 	| USARTSystem.parity /* No Parity */
 		| USARTSystem.stop_bits; /* 1 stop bit */
			  
	// USART0.DBGCTRL = 0 << USART_DBGRUN_bp; /* Debug Run: disabled */

	// USART0.EVCTRL = 0 << USART_IREI_bp; /* IrDA Event Input Enable: disabled */

	// USART0.RXPLCTRL = 0x0 << USART_RXPL_gp; /* Receiver Pulse Length: 0x0 */

	// USART0.TXPLCTRL = 0x0 << USART_TXPL_gp; /* Transmit pulse length: 0x0 */


	PORTMUX.USARTROUTEA = 0x02;	    
	msg_Config_BaudRate = DecodeBaudRate(&USARTSystem.baud_rate);
	msg_Config_Parity = DecodeParity(&USARTSystem.parity);
	msg_Config_StopBits = DecodeStopbits(&USARTSystem.stop_bits);
	msg_Config_DataBits = DecodeDataBits(&USARTSystem.data_bits);
	
	USART_0_ClearBuffer();
}

void USART_Initialize(void)
{
	USART_ReInitialize();
		
	// Add item to display list.
	DisplayProcessorMap item;
	item.ptrData = &USARTSystem;
	item.funcptrPrint = USART_Display;
	item.msg = NULL;
	item.enabled = USARTSystem.enabled;
	DisplayProcessor_Add(item);
}

void USART_Monitor(void)
{
	if(USARTSystem.enabled)
	{
		if(USART_0_Index != 0)
		{
			if(((uint8_t)USART_0_Buffer[USART_0_CurrentIndex] == 0) || (USART_0_CurrentIndex == USART_0_BufferSize))
			{
				USART_0_ClearBuffer();			
			}
			else
			{
				USART_0_write((uint8_t)USART_0_Buffer[USART_0_CurrentIndex]);	
				USART_0_CurrentIndex++;
			}		
		}
	}
}

// using this function means that you really need to get this message out.
// so the function force the existing message out and then accepts this message
// prior to its return.

void USART_0_Print_Immediate(char *msg)
{
	if(USARTSystem.enabled)
	{	
		while(!USART_0_Print(msg))
		{
			USART_Monitor();
		}
	}
}

void USART_0_AppendCharToBuffer(char msg)
{
	if(USART_0_Index < USART_0_BufferSize)
	{
		USART_0_Buffer[USART_0_Index] = msg;
		USART_0_Index++;	
	}
}

// if the system is not enabled, then it return's accepted 
bool USART_0_Print(char *msg)
{
	if(USARTSystem.enabled)
	{			
		// Only accept a message if the buffer is empty
		if(USART_0_Index != 0)
		{
			return false;
		}
	
		// By have the relocate to buffer maxed at buffer size, we can never overflow the buffer.
		// Buffer is set to USART_0_BufferSize  + 1
		for(int i = 0; i < USART_0_BufferSize; i++)
		{
			if(*msg == 0)
			{
				break;
			}
			USART_0_AppendCharToBuffer(*msg++);
		}
		// Since not of our string include CR/LF
		// if there is room append
		USART_0_AppendCharToBuffer('\r');
		USART_0_AppendCharToBuffer('\n');
	}
	return true;
}

void *GetPtrToUSARTData(void)
{
	return &USARTSystem;
}

void USART_Save_Config(uint8_t channel)
{
	// Save just one element
	// However, I'm maintaining the option of multiple channels. Like i did for ADC
	EEPROM_WRITE_ELEMENT(usart_system, 0, &USARTSystem);
	DisplayProcessor_SetEnabledFlag(&USARTSystem, USARTSystem.enabled);
	USART_ReInitialize();
}

void USART_Load_Config(void)
{
	// Load just one element
	EEPROM_VERIFY_SIGNATURE_AND_LOAD(usart_system, 0, &USARTSystem);
}


char *DecodeBaudRate(void *ptr)
{
	uint8_t value = *(uint8_t *)ptr;
	return LookupBaudRateByValue(value);
}

char *DecodeParity(void *ptr)
{
	uint8_t value = *(uint8_t *)ptr;
	return LookupParityByValue(value);
}

char *DecodeStopbits(void *ptr)
{
	uint8_t value = *(uint8_t *)ptr;
	return LookupStopBitsByValue(value);
}

char *DecodeDataBits(void *ptr)
{
	uint8_t value = *(uint8_t *)ptr;
	return LookupDataBitsByValue(value);
}


/**
 * \brief Enable RX and TX in USART_0
 * 1. If supported by the clock system, enables the clock to the USART
 * 2. Enables the USART module by setting the RX and TX enable-bits in the USART control register
 *
 * \return Nothing
 */
void USART_0_enable(void)
{
	USART0.CTRLB |= USART_RXEN_bm | USART_TXEN_bm;
}

/**
 * \brief Enable RX in USART_0
 * 1. If supported by the clock system, enables the clock to the USART
 * 2. Enables the USART module by setting the RX enable-bit in the USART control register
 *
 * \return Nothing
 */
void USART_0_enable_rx(void)
{
	USART0.CTRLB |= USART_RXEN_bm;
}

/**
 * \brief Enable TX in USART_0
 * 1. If supported by the clock system, enables the clock to the USART
 * 2. Enables the USART module by setting the TX enable-bit in the USART control register
 *
 * \return Nothing
 */
void USART_0_enable_tx(void)
{
	USART0.CTRLB |= USART_TXEN_bm;
}

/**
 * \brief Disable USART_0
 * 1. Disables the USART module by clearing the enable-bit(s) in the USART control register
 * 2. If supported by the clock system, disables the clock to the USART
 *
 * \return Nothing
 */
void USART_0_disable(void)
{
	USART0.CTRLB &= ~(USART_RXEN_bm | USART_TXEN_bm);
}

/**
 * \brief Get recieved data from USART_0
 *
 * \return Data register from USART_0 module
 */
uint8_t USART_0_get_data(void)
{
	return USART0.RXDATAL;
}

/**
 * \brief Check if the usart can accept data to be transmitted
 *
 * \return The status of USART TX data ready check
 * \retval false The USART can not receive data to be transmitted
 * \retval true The USART can receive data to be transmitted
 */
bool USART_0_is_tx_ready(void)
{
	return (USART0.STATUS & USART_DREIF_bm);
}

/**
 * \brief Check if the USART has received data
 *
 * \return The status of USART RX data ready check
 * \retval true The USART has received data
 * \retval false The USART has not received data
 */
bool USART_0_is_rx_ready(void)
{
	return (USART0.STATUS & USART_RXCIF_bm);
}

/**
 * \brief Check if USART_0 data is transmitted
 *
 * \return Receiver ready status
 * \retval true  Data is not completely shifted out of the shift register
 * \retval false Data completely shifted out if the USART shift register
 */
bool USART_0_is_tx_busy(void)
{
	return (!(USART0.STATUS & USART_TXCIF_bm));
}

/**
 * \brief Read one character from USART_0
 *
 * Function will block if a character is not available.
 *
 * \return Data read from the USART_0 module
 */
uint8_t USART_0_read(void)
{
	while (!(USART0.STATUS & USART_RXCIF_bm))
		;
	return USART0.RXDATAL;
}

/**
 * \brief Write one character to USART_0
 *
 * Function will block until a character can be accepted.
 *
 * \param[in] data The character to write to the USART
 *
 * \return Nothing
 */

void USART_0_write(const uint8_t data)
{
	while (!(USART0.STATUS & USART_DREIF_bm))
	;
	USART0.TXDATAL = data;
	
	// Optional: wait for this character to actually transmit
	while (!(USART0.STATUS & USART_TXCIF_bm))
	;
	USART0.STATUS = USART_TXCIF_bm;  // Clear flag by writing 1
}