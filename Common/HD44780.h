#ifndef HD44780_H
#define HD44780_H

#include <stdint.h>

typedef enum
{
  LCD_NORMAL,
  LCD_OLED
}LCD_TypeDef;

typedef enum
{
  BUS_4b,
  BUS_8b
}HD44780_BusTypeDef;

typedef enum
{
  ENGLISH_JAPANESE_FONT,
  WESTERN_EUROPEAN_FONT,
  ENGLISH_RUSSIAN_FONT,
  WESTERN_EUROPEAN2_FONT,
}CharacterFontTableTypeDef;

typedef struct
{
  HD44780_BusTypeDef BUS;
  uint8_t data_shift;
  uint8_t RS_Pin;
  uint8_t E_Pin;
  uint8_t BackLightPin;
}HD44780_Connection_TypeDef;

typedef struct
{
  /* Functions */
  void (*WriteData)(uint8_t data);
  void (*Delay_Func)(uint16_t ms);
  
  /* Settings */
  uint8_t BackLightIsOn;
  LCD_TypeDef LCD_Type;
  CharacterFontTableTypeDef Font;
  
  /* Connection definition structure */
  HD44780_Connection_TypeDef * HD44780_Connection;
}HD44780_StructTypeDef;

typedef enum
{
  PCF8574_eBayPCB,
  mc74HC595,
}HD44780_KnowConnectionsTypeDef;

static const HD44780_Connection_TypeDef PCF8574_StdCon = {BUS_4b, 4, 1<<0, 1<<2, 1<<3};
static const HD44780_Connection_TypeDef mc74HC595_StdCon = {BUS_4b, 4, 1<<2, 1<<3, 3};

typedef enum
{
	HD44780_CMD_NOP					= 0x00,
	HD44780_CMD_Clear				= 0x01,
	HD44780_CMD_Home				= 0x02,
	HD44780_CMD_8b					= 0x03,
	HD44780_CMD_AutoIncrementOn		        = 0x06,
	HD44780_CMD_Blank				= 0x08,
	HD44780_CMD_CursorLeft			        = 0x10,
	HD44780_CMD_CursorRight			        = 0x14,
	HD44780_CMD_PowerON				= 0x17,
	HD44780_CMD_4b_1line			        = 0x20,
	HD44780_CMD_4b_2lines			        = 0x28,
	HD44780_CMD_8b_1line			        = 0x30,
	HD44780_CMD_8b_2lines			        = 0x38,
	HD44780_CMD_ScrollRight			        = 0x1E,
	HD44780_CMD_ScrollLeft			        = 0x18,
	HD44780_CMD_UnderlineCursorON	                = 0x0E,
	HD44780_CMD_BlockCursorON		        = 0x0F,
	HD44780_CMD_CursorOFF			        = 0x0C,
	HD44780_RAW_DATA				= 0xFF,
}HD44780_CMD_TypeDef;

void HD44780_Init (HD44780_StructTypeDef * HD44780_InitStruct);
void HD44780_StdConnectionInit (HD44780_Connection_TypeDef * HD44780_Connection,
                                HD44780_KnowConnectionsTypeDef Connection);
void HD44780_SetPos (uint8_t row, uint8_t col);
void HD44780_Print (char * string);
void HD44780_Clear (void);
void HD44780_PutChar (uint8_t data); //for connection with STDIO

static const struct
{
  void (* Init) (HD44780_StructTypeDef * HD44780_InitStruct);
  void (* StdConInit) (HD44780_Connection_TypeDef * HD44780_Connection,
                                HD44780_KnowConnectionsTypeDef Connection); 
  void (* SetPos) (uint8_t row, uint8_t col);
  void (* Print) (char * string);
  void (* Clear) (void);
}LCD_HD44780 = { HD44780_Init, HD44780_StdConnectionInit, 
            HD44780_SetPos, HD44780_Print, HD44780_Clear };

static const char HD44780_RusFont[] = {
  'A',  //'A'
  0xA0, //'�'
  'B',  //'B'
  0xA1, //'�'
  0xE0, //'�'
  'E',  //'�'
  0xA3, //'�'
  0xA4, //'�'
  0xA5, //'�'
  0xA6, //'�'
  'K',  //'�'
  0xA7, //'�'
  'M',  //'�'
  'H',  //'�'
  'O',  //'�'
  0xA8, //'�'
  'P',  //'�'
  'C',  //'�'
  'T',  //'�'
  0xA9, //'�'
  0xAA, //'�'
  'X',  //'�'
  0xE1, //'�'
  0xAB, //'�'
  0xAC, //'�'
  0xE2, //'�'
  0xAD, //'�'
  0xAE, //'�'
  'b',  //'�'
  0xA2, //'�'
  0xB0, //'�'
  0xB1, //'�'
  
  'a',  //'�'
  0xB2, //'�'
  0xB3, //'�'
  0xB4, //'�'
  0xE3, //'�'
  'e',  //'�'
  0xB6, //'�'
  0xB7, //'�'
  0xB8, //'�'
  0xB9, //'�'
  0xBA, //'�'
  0xBB, //'�'
  0xBC, //'�'
  0xBD, //'�'
  'o',  //'�'
  0xBE, //'�'
  'p',  //'�'
  'c',  //'�'
  0xBF, //'�'
  'y',  //'�'
  0xE4, //'�'
  'x',  //'�'
  0xE5, //'�'
  0xC0, //'�'
  0xC1, //'�'
  0xE6, //'�'
  0xC2, //'�'
  0xC3, //'�'
  0xC4, //'�'
  0xC5, //'�'
  0xC6, //'�'
  0xC7, //'�'
};
#endif
