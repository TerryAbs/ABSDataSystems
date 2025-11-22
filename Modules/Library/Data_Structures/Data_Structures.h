/**
 *	Data_Structures
 *
 * This file contains all common data the structures used
 * throughout all the applications.
 *
*/
#pragma once

typedef void (*Callback) (void);
typedef void (*CallbackWithBufferPtr) (char *value);
typedef void (*CallbackWithInt) (int value);
typedef void (*CallbackWithThreeArgs) (int a, int b, char c);

struct structCommandList
{
	char *cmd;
	char cmdSize;
	CallbackWithBufferPtr func;
};