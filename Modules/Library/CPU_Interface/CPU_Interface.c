/*
* Created: 9/30/2023 12:05:06 PM
* Author: TERRY SCHEVKER
*		   ABS Data Systems, Inc.
*		   terry@absdatasystems.com
*/

/**
* \file
* \brief CPU Communication Interface.
*
* All boards are considered Slave Device. (with the exception of the Board with the 6502 processor. This board is considered the CPU Board(Master))
*
* The CPU Interface is responsible to sending and receiving data between the CPU board.
*
* There 3 basic modes:
*	1. Reset
*  2. Read Mode, where the Master, CPU wants to read data from the slave device. This is usually prompted by the slave device lowering the CPU_nInt line.
*  3. Write Mode, where the Master, CPU wants to write date to the Slave device.
*
*  1. Reset = CPU_CS(low) and CPU_Reset(low). CPU_nInt will be used to acknowledge progress
*  2. Read Mode. Slave device will CPU_nInt(low).  CPU will then CPU_CS(low) and CPU_RnW(High). CPU_RnW(Low) will be used to acknowledge progress.
*  3. Write Mode. CPU will then CS(low) and CPU_RnW(low). CPU_nInt will be used to acknowledge progress.
*
*  Once the CPU_Monitor detects the mode, the appropriate monitor will now take focus, no matter what the states of CPU_CS and CPU-RnW are in.
*  With the exception that the Read and Write monitors must monitor the CPU_Reset(low) in case the two devices get out of sync.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CPU_Interface.h"
#include "../CommandList/CommandList.h"
#include "../../Library/Delay/Delay.h"
#include <avr/io.h>
#include <port.h>

static char ReportingBuffer[REPORTINGBUFFERSIZE];
static unsigned char Receive_Buffer[RECEIVE_BUFFER_SIZE + 1];
static unsigned char Send_Buffer[SEND_BUFFER_SIZE + 1];
static int Receive_Index;						// This points to the next recording location
static bool Cmd_Ready;							// This flag indicates that we have rec
static int Send_Index;
static int Xmit_Index;
static enum CPUDATADIRECTION CPU_DATADIRECTION_MODE;
static enum CPU_INTERFACE CPU_INTERFACE_STATEMACHINE;
static enum CPU_RESETSTATEMACHINE CPU_RESET_STATEMACHINE;
static enum CPU_READSTATEMACHINE CPU_READ_STATEMACHINE;
static enum CPU_WRITESTATEMACHINE CPU_WRITE_STATEMACHINE;

void CPU_Interface_Initialization()
{
	// Initialize Registers

	// By default the CPU Data bus will be in control of this lines.
	// By placing these lines as Inputs, this effectively places these lines
	// In high-Z;

	Set_CPUDataDirection(CPUDATADIRECTION_READ);

	// Set up the reset state
	// We are using the Reset line as a way of resetting the Communications state machine.
	PORTF.DIRCLR = CPU_Reset;

	// Set the CS as an Input with Falling Edge firing.
	PORTD.DIRCLR = CPU_CS;


	// Set up the interrupt pins needed by the IRQ
	// These are now pole driven because we are using these values as part of the protocol.
	//PORTF.PIN6CTRL = PORT_ISC_FALLING_gc;	// CPU_Reset
	//PORTD.PIN3CTRL = PORT_ISC_BOTHEDGES_gc;   // CPU_CS

	// Set the CPU RnW line as an input
	PORTD.DIRCLR = CPU_RnW;
	
	// Flush Buffers
	Flush_SendBuffer();
	Flush_ReceiveBuffer();

	// Set the int line high which indicates no
	PORTD.OUTSET = CPU_nInt;
	PORTD.DIRSET = CPU_nInt;

	CPU_INTERFACE_STATEMACHINE = CPU_INTERFACE_IDLE;
	CPU_RESET_STATEMACHINE = CPU_RESETSTATEMACHINE_START;
	CPU_READ_STATEMACHINE = CPU_READSTATEMACHINE_START;
	CPU_WRITE_STATEMACHINE = CPUWRITESTATEMACHINE_START;
}


enum CPUDATADIRECTION Get_CPUDataDirection()
{
	return CPU_DATADIRECTION_MODE;
}

void Set_CPUDataDirection(enum CPUDATADIRECTION mode)
{
	CPU_DATADIRECTION_MODE = mode;
	switch(mode)
	{
		case CPUDATADIRECTION_READ:
		PORTA.DIRCLR = CPU_D7;
		PORTA.DIRCLR = CPU_D6;
		PORTA.DIRCLR = CPU_D5;
		PORTA.DIRCLR = CPU_D4;
		PORTA.DIRCLR = CPU_D3;
		PORTA.DIRCLR = CPU_D2;
		PORTA.DIRCLR = CPU_D1;
		PORTA.DIRCLR = CPU_D0;
		break;
		default:
		PORTA.DIRSET = CPU_D7;
		PORTA.DIRSET = CPU_D6;
		PORTA.DIRSET = CPU_D5;
		PORTA.DIRSET = CPU_D4;
		PORTA.DIRSET = CPU_D3;
		PORTA.DIRSET = CPU_D2;
		PORTA.DIRSET = CPU_D1;
		PORTA.DIRSET = CPU_D0;
		break;
	}
}

void Flush_SendBuffer()
{
	Send_Index = 0;
	Xmit_Index = 0;
	memset((char *)Send_Buffer, 0, SEND_BUFFER_SIZE + 1);
}

void Flush_ReceiveBuffer()
{
	Receive_Index = 0;
	Cmd_Ready = false;
	memset((char *)Receive_Buffer, 0, RECEIVE_BUFFER_SIZE + 1);
}

void SendData(char *buf)
{
	int index = 0;
	while(buf[index] != 0)
	{
		Send_Buffer[index] = buf[index];		// Copy the data from the passed in buffer into the send buffer.
		if(index < SEND_BUFFER_SIZE)			// Increment the pointer only if there is room
		{
			index++;
		}
		else
		{
			// buffer is full. Stop filling, Mark and send wha we can.
			break;
		}
	}
	Send_Buffer[index] = EOL;

}

// TestForReset monitors the reset line and forces
bool CPU_RequestsAReset()
{
	bool results = false;
	if(((PORTD.IN & CPU_CS) == 0) && ((PORTF.IN & CPU_Reset) == 0))
	{
		Reset_CPU_Interface();
		results = true;
	}
	return results;
}

void Reset_CPU_Interface()
{
	CPU_INTERFACE_STATEMACHINE = CPU_INTERFACE_IDLE;
}

void CPU_Interface_Monitor()
{
	switch(CPU_INTERFACE_STATEMACHINE)
	{
		case CPU_INTERFACE_IDLE:
		/*
		* 1. Reset = CPU_CS(low) and CPU_Reset(low). CPU_nInt will be used to acknowledge progress
		* 2. Read Mode. Slave device will CPU_nInt(low).  CPU will then CPU_CS(low) and CPU_RnW(High). CPU_nInt will be used to acknowledge progress.
		* 3. Write Mode. CPU will then CS(low) and CPU_RnW(low). CPU_nInt will be used to acknowledge progress.
		*/
		if ((PORTD.IN & CPU_CS) == 0)
		{
			if((PORTF.IN & CPU_Reset) == 0)
			{
				CPU_INTERFACE_STATEMACHINE = CPU_INTERFACE_RESET_MONITOR;
			}
			else
			{
				if ((PORTD.IN & CPU_RnW) == 0)		// Write Line is low - Means CPU Wants to Write to this device.
				{
					CPU_INTERFACE_STATEMACHINE = CPU_INTERFACE_WRITE_MONITOR;
				}
				else
				{
					CPU_INTERFACE_STATEMACHINE = CPU_INTERFACE_READ_MONITOR;
				}
			}
		}
		else
		{
			SendDataToCPU_Monitor();
		}
		break;
		case CPU_INTERFACE_RESET_MONITOR:
		Reset_Monitor();
		break;
		case CPU_INTERFACE_READ_MONITOR:
		if(!CPU_RequestsAReset())
		{
			Read_Monitor();
		}
		break;
		case CPU_INTERFACE_WRITE_MONITOR:
		if(!CPU_RequestsAReset())
		{
			Write_Monitor();
		}
		break;
	}
}

// This function will report true if were are in the middle of processing commands.
bool Get_CPU_InterfaceStatusBusy()
{
	bool results = true;
	if(CPU_INTERFACE_STATEMACHINE == CPU_INTERFACE_IDLE)
	{
		results = false;
	}
	return results;
}
/*
* The Reset State Machine drops the CPU_nInt line to signal
* that it has completed its set of tasks.
*
* The next state waits for the CPU to acknowledge by raising the CPU_CS
*/

void Reset_Monitor()
{
	switch(CPU_RESET_STATEMACHINE)
	{
		case CPU_RESETSTATEMACHINE_START:
		Flush_SendBuffer();
		Flush_ReceiveBuffer();
		Set_CPUDataDirection(CPUDATADIRECTION_READ);
		PORTD.OUTCLR = CPU_nInt;
		CPU_RESET_STATEMACHINE = CPU_RESETSTATEMACHINE_CPU_WAIT;
		break;
		case CPU_RESETSTATEMACHINE_CPU_WAIT:
		if ((PORTD.IN & CPU_CS) == CPU_CS)
		{
			PORTD.OUTSET = CPU_nInt;
			CPU_RESET_STATEMACHINE = CPU_RESETSTATEMACHINE_START;
			Reset_CPU_Interface();
		}
		break;
	}
}

// This code monitors the Send buffer to see if there data to be sent
void SendDataToCPU_Monitor()
{
	if(Send_Index == Xmit_Index)
	{
		// Send index is used to track the difference between the value we are sending and the value we are about to send.
		// If the values are not the same, then we are in the middle of a request to send data to the CPU
		if(Send_Buffer[Xmit_Index] != 0)
		{
			Send_Index++;
			// Tell the CPU board we have an service request by lowering the int line
			PORTD.OUTCLR = CPU_nInt;
		}
	}
}

// Read is the CPU Reading data from the Device Module.
void Read_Monitor()
{
	switch(CPU_READ_STATEMACHINE)
	{
		case CPU_READSTATEMACHINE_START:
		Set_CPUDataDirection(CPUDATADIRECTION_WRITE);		// Change buffer to Write to the bus
		PORTA.OUT = Send_Buffer[Xmit_Index];				// Place the data on the bus
		CPU_READ_STATEMACHINE  = CPU_READSTATEMACHINE_CPU_ACK_AND_POST_DATA;
		PORTD.OUTSET = CPU_nInt;							// Tell the CPU that that data is read.
		break;
		case CPU_READSTATEMACHINE_CPU_ACK_AND_POST_DATA:
		// The CPU will drop the CPU_RnW (low)
		// Indicating that the CPU has release the Data bus and is waiting for data.
		if (((PORTD.IN & CPU_RnW) == 0) )
		{
			Set_CPUDataDirection(CPUDATADIRECTION_READ);
			if(Xmit_Index < SEND_BUFFER_SIZE)
			{
				// Point to the next value.
				Xmit_Index++;
				// Test to see if we are past the EOL character.
				// If so, Flush the send buffer and we are done.
				if(Send_Buffer[Xmit_Index] == 0)
				{
					Flush_SendBuffer();
				}
				Reset_CPU_Interface();
			}
			CPU_READ_STATEMACHINE  = CPU_READSTATEMACHINE_START;
		}
		break;
	}
}

// Write is the CPU Board writing to the device module
void Write_Monitor()
{
	switch(CPU_WRITE_STATEMACHINE)
	{
		case CPUWRITESTATEMACHINE_START:
		Receive_Buffer[Receive_Index] = PORTA.IN;		// Read the value from the bus
		CPU_WRITE_STATEMACHINE = CPUWRITESTATEMACHINE_CPU_WAIT_CS;
		PORTD.OUTCLR = CPU_nInt;						// Lower the Int line to indicate we have completed this step
		break;
		case CPUWRITESTATEMACHINE_CPU_WAIT_CS:
		if ((PORTD.IN & CPU_CS) == CPU_CS)				// The CPU Will Acknowledge our Int response by releasing the CS (high)
		{
			PORTD.OUTSET = CPU_nInt;					// We intern release the int line.
			CPU_WRITE_STATEMACHINE = CPUWRITESTATEMACHINE_CPU_WAIT_RW;
		}
		break;
		case CPUWRITESTATEMACHINE_CPU_WAIT_RW:
		if ((PORTD.IN & CPU_RnW) == CPU_RnW)				// The CPU Will Acknowledge our Int response by releasing the CS (high)
		{
			if(Receive_Buffer[Receive_Index] == EOL)	// Test to see of the last character received is the EOL (end of line or end of this section of data)
			{
				Receive_Buffer[Receive_Index] = 0;		// If, EOL, there is no need to store the value, only process it.
				ProcessCommand((char *)Receive_Buffer);	// Process the request.
				Flush_ReceiveBuffer();					// Clear the buffer and wait for the next set of data.
			}
			else if(Receive_Index  < RECEIVE_BUFFER_SIZE)	// Test to make sure we are not overflowing the buffer.
			{
				Receive_Index++;						// Point to the next memory location
			}
			CPU_WRITE_STATEMACHINE = CPUWRITESTATEMACHINE_START;	// Reset this start machine
			Reset_CPU_Interface();						// reset the CPU_Interface. If this is more to get, the CPU will restart the process. This single byte write is complete
		}
		break;

	}
}

void ClearReportingBuffer()
{
	memset((char *) ReportingBuffer, 0, REPORTINGBUFFERSIZE);
}

char *GetReportingBuffer()
{
	return &ReportingBuffer[0];
}

char *ParseValueFromCmdLine(char *Buffer, int *value)
{
	int data; 
	int count = sscanf(Buffer, "%d", &data);	
	if(count == 0)
	{
		*value = 0;
		return NULL;
	}

	*value = data;
	while(1)
	{
		if ((Buffer[count] == ' ') && (Buffer[count + 1] != 0x0))
		{
			return &Buffer[count+1];
		}
		if (Buffer[count] == 0x00)
		{
			return NULL;
		}
		count++;
	}
}