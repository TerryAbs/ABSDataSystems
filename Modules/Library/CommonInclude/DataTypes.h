/*
 * DataTypes.h
 *
 * Created: 6/3/2024 2:45:31 PM
 *  Author: TERRY SCHEVKER
 */ 


#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define uchar unsigned char
#define HIGH	true
#define LOW		false

struct structdrawSpecs
{
	int Rows;
	int Cols;
	int Columns;
	int Lines;
	uint8_t (*GetVideoData)(uint8_t row, uint8_t col);
	void (*SetVideoData)(uint8_t row, uint8_t col, uint8_t data);
	void (*SetDirtyFlag)(void);
};

struct Font_Pattern
{
	unsigned char Bits_Used;
	unsigned char Bytes_Per_Char;
	unsigned char Rows_Per_Char;
	const unsigned char *Char_Pattern;
};

#define FIRST_FONT_CHAR		0x20	// Space character

struct sFont_List
{
	const struct Font_Pattern *Font;
};
