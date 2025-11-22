/*
 * CPU_Interface.h
 *
 * Created: 9/30/2023 12:04:55 PM
 * Author: TERRY SCHEVKER
 *		   ABS Data Systems, Inc.
 *		   terry@absdatasystems.com
 */ 


/**
*	\file
*	\brief CPU Communications Interface.
*
* Communications between the CPU and the modules is controlled by this module.
* 
* Here's how the interface works:
* 
* The CPU Board is the Master device while the Module(s) are all Slave devices.
* For the CPU board to communicate with a Slave Module, the CPU will assert(drive low) the CS line.
*
* Here's what the interface looks like and what each of the controls lines are defined to do.
* 1. CPU_D0-CPU_D7 are the data lines.
* 2. CPU_CS, is by default high and goes low when the selected Module is to be controlled.
*	 The CPU board has 12 individual CS lines, one for each Module connected. 
*    This allows the CPU to talk to single device at a time. 
*	
*	 There is a potential command sequence where the CPU could reset all the communication state machines at the same time.
*	 See the CPU Commands Reset State machine command for more details.
*
* 3. CPU_RnW
*  	 When this line is high, the CPU is expecting to Read data from the Module. (CPU is placed in Input mode and the module takes over the data bus)
*    When this line is low, the CPU is expecting to Write date to the Module.
*
* 4. CPU_CS
*	 By default this line is high, the Module is considered in a high-z. (Data lines are inputs)
*	 The CPU_CS line will be uses as a clock line to walk the Module state machine through the different steps needed to transfer data.
*
* 5. CPU_Reset
*	 This line will be used to reset the State Machine. 
*    The way this will work as follows:
*	 1. The CPU will set CPU_RnW Low
*	 2. The CPU_CS will transition low.
*	 3. The CPU_Reset (by default this line will be high) transition H->L and then L->H (Clocked) to tell the Module to reset its state machine.
*
* 6. CPU_Int
*	 This line is from the Module to the CPU board to allow the Module to inform the CPU that the module has data ready for the CPU's consumption.
*	 The CPU board has 12 independent lines which allows the CPU board to set and control IQR priority even though the Interrupts are numbered.
*    
*
*	Master Module:
*	Both the master and slave devices will be locked stepped using state machines to confirm communication process.
*	This will ensure that there is no mis-communication. The master has the ability to reset the slave communications with a reset command sequence.
*   (in the event at the ack was not received by the master)
* 	The master drivers, in the Ack common code will have a timeout. This is used in the event that the slave devices do not respond in a timely basis.
*
* Slave Module:
* 
* 1. On power up, the Slave Module sets all the shared CPU_D0-CPU_D7 lines as inputs. This prevents CPU Bus conflicts.
		(Direction as inputs in the default state of processor. Therefore, there should be no power on issues while the CPU as being initialized.)
* 2. The state machine is then set up in a mode where CS line is monitored via an interrupt process.
*  
* The basic concept is that the master issue a CS low, starting the cycle. The slave will ack by lowing the Int line as an ack. This will cause both the
* master and slave device to move to the next states.
*
* The master will then raise CS and the slave device will respond with Raising the Int line)
*
* This completes the transaction request.
*
* All commands are initiate by the master. The command is sent to the slave and once the slave has received a complete commands the slave will respond with
* some sort of response. 
*
* This response will be command specific.
* Example: 
*	1. Relay on command will response with OK or FAILED.
*	2. Relay Status request will response with a status response message.
* 
* See the individual slave devices and commands for the expected response.
*
*/


 #pragma once
 #include <avr/io.h>
 #include <port.h>
 /** \cond */

#define PA7			0x80
#define PA6			0x40
#define PA5			0x20
#define PA4			0x10
#define PA3			0x08
#define PA2			0x04
#define PA1			0x02
#define PA0			0x01

#define PF6			0x40
#define PD3			0x08
#define PD2			0x04
#define PD1			0x02


#define EOL			0x0d

/** \endcond */

#define CPU_D7		PA7
#define CPU_D6		PA6
#define CPU_D5		PA5
#define CPU_D4		PA4
#define CPU_D3		PA3
#define CPU_D2		PA2
#define CPU_D1		PA1
#define CPU_D0		PA0

#define CPU_Reset	PF6
#define CPU_CS		PD3
#define CPU_RnW		PD2
#define CPU_nInt	PD1

#define REPORTINGBUFFERSIZE 255
#define RECEIVE_BUFFER_SIZE 255
#define SEND_BUFFER_SIZE 255

enum CPUDATADIRECTION
{
	CPUDATADIRECTION_READ,
	CPUDATADIRECTION_WRITE
};

enum CPU_INTERFACE
{
	CPU_INTERFACE_IDLE,
	CPU_INTERFACE_RESET_MONITOR,
	CPU_INTERFACE_READ_MONITOR,
	CPU_INTERFACE_WRITE_MONITOR
};

enum CPU_RESETSTATEMACHINE {
	CPU_RESETSTATEMACHINE_START,
	CPU_RESETSTATEMACHINE_CPU_WAIT,
};

enum CPU_READSTATEMACHINE {
	CPU_READSTATEMACHINE_START,
	CPU_READSTATEMACHINE_CPU_ACK_AND_POST_DATA,
};

enum CPU_WRITESTATEMACHINE {
	CPUWRITESTATEMACHINE_START,
	CPUWRITESTATEMACHINE_CPU_WAIT_CS,
	CPUWRITESTATEMACHINE_CPU_WAIT_RW
};

void CPU_Interface_Initialization();
void CPU_Interface_Monitor();
void Reset_CPU_Interface();
bool CPU_RequestsAReset();

void Read_Monitor();
void Write_Monitor();
void Reset_Monitor();

void Flush_SendBuffer();
void Flush_ReceiveBuffer();
void SendData(char *buf);
void SendDataToCPU_Monitor();
void Set_CPUDataDirection(enum CPUDATADIRECTION mode);
enum CPUDATADIRECTION Get_CPUDataDirection();

bool Get_CPU_InterfaceStatusBusy();
void ClearReportingBuffer();
char *GetReportingBuffer();
char *ParseValueFromCmdLine(char *Buffer, int *value);
