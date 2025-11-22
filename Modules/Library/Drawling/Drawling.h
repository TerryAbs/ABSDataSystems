/*
 * Drawling.h
 *
 * Created: 6/24/2024 3:02:31 PM
 *  Author: TERRY SCHEVKER

	This module contains the 
 */ 


#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../Library/Delay/Delay.h"
#include <avr/io.h>
#include <port.h>
#include "../../../Library/CommonInclude/DataTypes.h"
#include "../../../Library/Data_Structures/Data_Structures.h"

void Drawling_Intialize(struct structdrawSpecs specs);
uint8_t GetVideoData(int Row, int Col);
void SetVideoData(int Row, int Col, uint8_t data);

void Set_Pixel(int Row, int Col, char Pixel_State);
void DrawHorzintalLine(int Row, int S_Col, int Col, char Pixel_State);
void DrawVerticalLine(int Row, int S_Col, int Col, char Pixel_State);

void Draw_Box(int S_Row, int S_Col, int E_Row, int E_Col, char Pixel_State);
uchar Put_Char(uchar Row, uchar Col, uchar Character, char ReverseVideo, int Font_Size, CallbackWithThreeArgs func);
uchar LCD_Print_String(uchar Row, uchar Col, char ReverseVideo, char Font_Size,  char  *ptr_String, CallbackWithThreeArgs func);

void SetLCDDirtyFlag();
uint8_t Drawling_GetCols();
uint8_t Drawling_GetRows();
uint8_t Drawling_GetColumns();
uint8_t Drawling_GetLines();
void Drawling_FillVideoBuffer(uint8_t value);
void Clear_Area(uchar S_Row, uchar S_Col, uchar E_Row, uchar E_Col);
uchar GetFontWidthFromCharacter(uchar Font_Size, uchar Character);
uchar GetFontCharacterWidth(char Font_Size, char *ptr_String);

const struct Font_Pattern * GetActiveFont(int Font_Size);

enum FONT_NAMES{
	FONT_NAMES_MS_SANS_SERIF8 = 0,
	FONT_NAMES_MS_SANS_SERIF10 = 1,
	FONT_NAMES_MS_SANS_SERIF12 = 2,
	FONT_NAMES_MS_SANS_SERIF14 = 3,
	FONT_NAMES_ARIAL10 = 4,
	FONT_NAMES_ARIAL14 = 5,
	FONT_NAMES_ARIAL18 = 6,
	FONT_NAMES_ARIAL24 = 7
};