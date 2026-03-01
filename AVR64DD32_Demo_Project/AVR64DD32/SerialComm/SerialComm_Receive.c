/*
 * Serial_Comm_Receive.c
 *
 * Created: 12/01/2025 2:31:30 PM
 *  Author: terry
 */ 

/*
	This file contains all code for Receiving data from the USART
*/


#include "SerialComm/SerialComm_Receive.h"


static unsigned char Receive_Buffer[RECEIVE_BUFFER_SIZE + 1];
static int Receive_Index;	// This points to the next recording location
static bool Cmd_Ready;		// This flag indicates that we have rec
static enum SERIALCOMM_RECEIVESTATES SERIALCOMM_ReceiveStateMachine; 

void SerialComm_Receive_Initialize(void)
{
	Flush_ReceiveBuffer();
	SERIALCOMM_ReceiveStateMachine = SERIALCOMM_RECEIVESTATES_WAITFORDATA;
}

// Reset the Receive Buffer
void Flush_ReceiveBuffer(void)
{
	Receive_Index = 0;
	Cmd_Ready = false;
	memset((char *)Receive_Buffer, 0, RECEIVE_BUFFER_SIZE + 1);	
}

bool GetCmdReady_Status(void)
{
	return Cmd_Ready;
}

char *GetReceiveBuffer(void)
{
	return (char *)Receive_Buffer;
}

bool Store_ReceviedData(uint8_t data)
{
	bool results = false;
	
	// This function returns a true only if the EOL is collect.
	// EOL is used to determine that we have a value command type string.
	
	// Otherwise it will return a false.
	
	// Since some terminal program will return a CR and LF
	// We don't want to collect either of these values.
	if(data == EOL)
	{
		Cmd_Ready = true;
		return true;
	}

	if(data != FALSE_EOL)
	{
		// Store the keyboard value
		Receive_Buffer[Receive_Index] = data;
		Receive_Index++;

		// Local echo back to the terminal
		// the character we are using		
		// The Updating to commandPrompt
		// This way our screen should not be affected by the DisplayProcessor
		CommandPrompt();
		
		
		// Test to see if the Receive_Index exceeds out storage area.
		// If so, flush the buffer as no command is this large.
		// Someone is trying to overflow the system.
		if(Receive_Index >= RECEIVE_BUFFER_SIZE)
		{
			Flush_ReceiveBuffer();
		}
	}
	return results;
}

void SerialComm_Receive_Monitor(void)
{
	switch(SERIALCOMM_ReceiveStateMachine)
	{
		case SERIALCOMM_RECEIVESTATES_WAITFORDATA:
			// Test to see if there is data waiting.
			if(USART_1_is_rx_ready())
			{
				// A true response means we have a valid command to send to the
				// Command processor.
				if(Store_ReceviedData(USART_1_read()))
				{
					SERIALCOMM_ReceiveStateMachine = SERIALCOMM_RECEIVESTATES_PROCESSDATA;
				}
			}		
			break;
		case SERIALCOMM_RECEIVESTATES_PROCESSDATA:
			if(!Cmd_Ready)
			{
				SERIALCOMM_ReceiveStateMachine = SERIALCOMM_RECEIVESTATES_WAITFORDATA;
			}
			break;
	}
}
