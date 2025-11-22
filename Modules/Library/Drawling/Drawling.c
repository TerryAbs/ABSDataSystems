/*
 * Drawling.c
 *
 * Created: 6/24/2024 3:02:18 PM
 *  Author: TERRY SCHEVKER
 */ 

 #include "Drawling.h"
#include "../Fonts/MS_Sans_Serif8.h"
#include "../Fonts/MS_Sans_Serif10.h"
#include "../Fonts/MS_Sans_Serif12.h"
#include "../Fonts/MS_Sans_Serif14.h"
#include "../Fonts/Arial10.h"
#include "../Fonts/Arial14.h"
#include "../Fonts/Arial18.h"
#include "../Fonts/Arial24.h"
#include <avr/pgmspace.h>

static struct structdrawSpecs drawSpecs;
const struct Font_Pattern * GetActiveFont(int Font_Size)
{
	const struct Font_Pattern *Active_Font = (struct Font_Pattern *) MS_Sans_Fonts_8;
	
	switch(Font_Size)
	{
		case FONT_NAMES_MS_SANS_SERIF8:
			Active_Font = (struct Font_Pattern *) MS_Sans_Fonts_8;
			break;
		case FONT_NAMES_MS_SANS_SERIF10:
			Active_Font = (struct Font_Pattern *) MS_Sans_Fonts_10;
			break;
		case FONT_NAMES_MS_SANS_SERIF12:
			Active_Font = (struct Font_Pattern *) MS_Sans_Fonts_12;
			break;
		case FONT_NAMES_MS_SANS_SERIF14:
			Active_Font = (struct Font_Pattern *) MS_Sans_Fonts_14;
			break;
		case FONT_NAMES_ARIAL10:
			Active_Font = (struct Font_Pattern *) Arial_Fonts_10;
			break;
		case FONT_NAMES_ARIAL14:
			Active_Font = (struct Font_Pattern *) Arial_Fonts_14;
			break;
		case FONT_NAMES_ARIAL18:
			Active_Font = (struct Font_Pattern *) Arial_Fonts_18;
			break;
		case FONT_NAMES_ARIAL24:
			Active_Font = (struct Font_Pattern *) Arial_Fonts_24;
			break;
	}
	return Active_Font;
}

void Drawling_Intialize(struct structdrawSpecs specs)
{
	drawSpecs.GetVideoData = specs.GetVideoData;
	drawSpecs.SetVideoData = specs.SetVideoData;
	drawSpecs.SetDirtyFlag = specs.SetDirtyFlag;
	drawSpecs.Cols = specs.Cols;
	drawSpecs.Rows = specs.Rows;
	drawSpecs.Columns = specs.Columns;
	drawSpecs.Lines = specs.Lines;
}

void Drawling_FillVideoBuffer(uint8_t value)
{
	for(int rows = 0; rows < drawSpecs.Rows; rows++)
	{
		for(int cols = 0; cols < drawSpecs.Columns; cols++)
		{
			SetVideoData(rows, cols, value);
		}
	}

	SetLCDDirtyFlag();			
}

/*
	This function Clears an area or region of the screen
*/

void Clear_Area(uchar S_Row, uchar S_Col, uchar E_Row, uchar E_Col)
{
	for(int i=S_Row; i<=E_Row; i++)
	{
		DrawHorzintalLine(i, S_Col, E_Col, 0);
	}
}

void Set_Pixel(int Row, int Col, char Pixel_State)
{
	uchar Data = 0;
	uchar tCol = 0;
	uchar bitPosition;
	uchar shift;

	// Make sure the value is within the Screen memory
	if((Row < drawSpecs.Rows) && (Col < drawSpecs.Cols))
	{
		tCol = Col / 8;
		shift = (Col % 8);
		bitPosition = 0x80 >> shift;

		// Need to convert Col Address in to Byte Location
		Data =  GetVideoData(Row, tCol);

		if(Pixel_State == 0x00)
		{
			Data &= ~bitPosition;
		}
		else
		{
			Data |= bitPosition;
		}
		
		SetVideoData(Row, tCol, Data);
	}
}

void DrawHorzintalLine(int Row, int S_Col, int E_Col, char PixelState)
{
	for(int i=S_Col; i<=E_Col; i++)
	{
		Set_Pixel(Row, i, PixelState);
	}

	SetLCDDirtyFlag();
}

void DrawVerticalLine(int sRow, int eRow, int Col, char PixelState)
{
	// Right Side Down
	for(int i=sRow; i<=eRow; i++)
	{
		Set_Pixel(i, Col, PixelState);
	}
	SetLCDDirtyFlag();
}

void Draw_Box(int S_Row, int S_Col, int E_Row, int E_Col, char PixelState)
{
	// Draw the Top
	DrawHorzintalLine(S_Row, S_Col, E_Col, PixelState);

	// Right Side Down
	DrawVerticalLine(S_Row, E_Row, E_Col, PixelState);
	
	// Bottom Line
	DrawHorzintalLine(E_Row, S_Col, E_Col, PixelState);

	// Left Side Down
	DrawVerticalLine(S_Row, E_Row, S_Col, PixelState);
}


uchar Put_Char(uchar  Row, uchar  Col, uchar Character, char ReverseVideo, int Font_Size, CallbackWithThreeArgs func)
{
	uchar Results = 0;
	uchar font_char;
	uchar font_mask;
	uchar Index;
	uchar p_Rows;
	uchar bit_col;
	uchar Byte_Count;
	uchar Bit_Count;
	uchar myIndex;
	unsigned char Rows_Per_Char;
	unsigned char Bytes_Per_Char;
	unsigned char Bits_Used;
	uchar *Char_Pattern;
	const struct Font_Pattern *Active_Font;

	Active_Font = GetActiveFont(Font_Size);

	// only allow printable characters to the Screen
	if(Character >= FIRST_FONT_CHAR)
	{
		Index = Character - FIRST_FONT_CHAR;
		Rows_Per_Char = pgm_read_byte(&Active_Font[Index].Rows_Per_Char);
		Bytes_Per_Char = pgm_read_byte(&Active_Font[Index].Bytes_Per_Char);
		Bits_Used =  pgm_read_byte(&Active_Font[Index].Bits_Used);
		Char_Pattern = pgm_read_ptr((unsigned char *) &Active_Font[Index].Char_Pattern);

		for(p_Rows=0; p_Rows< Rows_Per_Char; p_Rows++)
		{
			Bit_Count = 0;
			for(Byte_Count = 0; Byte_Count < Bytes_Per_Char; Byte_Count++)
			{
				myIndex = (p_Rows * Bytes_Per_Char) + Byte_Count;

				font_char = pgm_read_byte(&Char_Pattern[myIndex]);
				if(ReverseVideo == 1)
				{
					font_char = ~font_char;
				}
				font_mask = 0x80;

				for(bit_col=0; bit_col<8; bit_col++)
				{
					if(Bit_Count == Bits_Used)
					break;
					if(func != NULL)
					{
						// I changed this from call the SetPixel Function directly
						// I can could use the same code for multiple display Drivers. Graphics and Mini Graphics
						func((Row + p_Rows), (Col + Bit_Count), font_char & font_mask);
					}
					font_mask >>= 1;
					Bit_Count++;
				}
				if(Bit_Count == Bits_Used)
				break;
			}
		}

		Results = Bits_Used;
	}
	return(Results);
}

uint8_t Drawling_GetCols()
{
	return drawSpecs.Cols;
}

uint8_t Drawling_GetRows()
{
	return drawSpecs.Rows;
}

uint8_t Drawling_GetColumns()
{
	return drawSpecs.Columns;
}

uint8_t Drawling_GetLines()
{
	return drawSpecs.Lines;
}


uchar LCD_Print_String(uchar Row, uchar Col, char ReverseVideo, char Font_Size,  char *ptr_String, CallbackWithThreeArgs func)
{
	while(*ptr_String)
	{
		Col += Put_Char(Row, Col, *ptr_String++, ReverseVideo, Font_Size, func);
	}

	SetLCDDirtyFlag();
	return Col;
}

uint8_t GetVideoData(int Row, int Col)
{
	uint8_t results = 0;
	if(drawSpecs.GetVideoData != NULL)
	{
		return drawSpecs.GetVideoData(Row, Col);
	}
	return results;
}

void SetVideoData(int Row, int Col, uint8_t data)
{
	if(drawSpecs.SetVideoData != NULL)
	{
		drawSpecs.SetVideoData(Row, Col, data);
	}
}

void SetLCDDirtyFlag()
{
	if(drawSpecs.SetDirtyFlag != NULL)
	{
		drawSpecs.SetDirtyFlag();
	}
}

uchar GetFontWidthFromCharacter(uchar Font_Size, uchar Character)
{
	uchar Results = 0;
	uchar Index;
	const struct Font_Pattern *Active_Font;

	// only allow printable characters to the Screen
	if(Character >= FIRST_FONT_CHAR)
	{
		Active_Font = GetActiveFont(Font_Size);;
		Index = Character - FIRST_FONT_CHAR;
		Results = Active_Font[Index].Bits_Used;
	}
	return Results;
}

uchar GetFontCharacterWidth(char Font_Size, char *ptr_String)
{
	uchar LastCol = 0;
	while(*ptr_String)
	{
		LastCol += GetFontWidthFromCharacter(Font_Size, *ptr_String++);
	}
	return LastCol;
}