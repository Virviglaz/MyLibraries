#include "HD44780.h"
#include <string.h>

#define INIT_DELAY              10

/* Internal pointer to current driver */
HD44780_StructTypeDef * HD44780;
static void (* HD44780_Write) (uint8_t data, uint8_t cmd);

/* Internal functions protorypes */
static void HD44780_WriteToBus (uint8_t dat);
static void HD44780_Write_4bit (uint8_t data, uint8_t cmd);
static void HD44780_Write_8bit (uint8_t data, uint8_t cmd);
static uint8_t Translate (uint8_t dat);

/* Public functions */

HD44780_StructTypeDef * HD44780_Driver (HD44780_StructTypeDef * HD44780_InitStruct)
{
  if (HD44780_InitStruct) //assign driver if defined
    HD44780 = HD44780_InitStruct;
  return HD44780;
}

void HD44780_Init (HD44780_StructTypeDef * HD44780_InitStruct)
{
  if (HD44780_InitStruct == 0) return;
  if (HD44780_InitStruct->HD44780_Connection == 0) return;
  
  /* Assign the driver */
  HD44780 = HD44780_InitStruct;
  
  /* Assign interface function */
  HD44780_Write = HD44780->HD44780_Connection->BUS ? 
            HD44780_Write_8bit : HD44780_Write_4bit;

	
  if (HD44780->HD44780_Connection->BUS == BUS_4b)
  {
		/* 4 bit wide bus init */
    HD44780_WriteToBus(0x03 << HD44780->HD44780_Connection->data_shift);
    HD44780_WriteToBus(0x03 << HD44780->HD44780_Connection->data_shift);
    HD44780_WriteToBus(0x03 << HD44780->HD44780_Connection->data_shift);
    HD44780_WriteToBus(0x02 << HD44780->HD44780_Connection->data_shift);  
  }
	else

  HD44780->Delay_Func(INIT_DELAY);	
  HD44780_Write(HD44780_CMD_4b_2lines | HD44780->Font, 1);
                                                                                                                                                                                                                                                            					
  if (HD44780->LCD_Type == LCD_NORMAL)
  {
    // HD44780
    HD44780_Cmd(HD44780_CMD_Home);
    HD44780_Cmd(HD44780_CMD_Clear);
    HD44780->Delay_Func(INIT_DELAY);	
    HD44780_Cmd(HD44780_CMD_CursorOFF);
  }
  else
  {
    // OLED LCD TYPE		
    HD44780_Cmd(HD44780_CMD_Blank);
    HD44780_Cmd(HD44780_CMD_AutoIncrementOn);
    HD44780_Cmd(HD44780_CMD_PowerON);
    HD44780_Cmd(HD44780_CMD_Clear);
    HD44780->Delay_Func(INIT_DELAY);	
    HD44780_Cmd(HD44780_CMD_Home);	
    HD44780_Cmd(HD44780_CMD_CursorOFF);	
  }
}

void HD44780_StdConnectionInit (HD44780_Connection_TypeDef * HD44780_Connection,
                                HD44780_KnowConnectionsTypeDef Connection)
{
  if (HD44780_Connection == 0) return;
  switch (Connection)
  {
    case PCF8574_eBayPCB: 
      memcpy(HD44780_Connection, &PCF8574_StdCon, sizeof(*HD44780_Connection));
      break;
    case mc74HC595:
      memcpy(HD44780_Connection, &mc74HC595_StdCon, sizeof(*HD44780_Connection));
      break;
  }
}

void HD44780_SetPos (uint8_t row, uint8_t col)
{
	uint8_t position = 0x80;
	if (row) position |= 0x40;
	HD44780_Cmd ((HD44780_CMD_TypeDef)(position | col));
}

void HD44780_Print (char * string)
{
  while (* string)
    HD44780_PutChar(* string++);
}

void HD44780_Clear (void)
{
  HD44780_Cmd(HD44780_CMD_Clear);
  HD44780_Cmd(HD44780_CMD_Home);
}

void HD44780_PutChar (uint8_t data)
{
  if (data == '\n')
    HD44780_Cmd((HD44780_CMD_TypeDef)(0x80 | 0x40)); 
  else 
  {
    data = Translate(data);
    HD44780_Write(data, 0);
  }
}

void HD44780_Update (void)
{
  HD44780_Cmd(HD44780_NOP_DATA);
}

void HD44780_Cmd (HD44780_CMD_TypeDef CMD)
{
  HD44780_Write(CMD, 1);
}

void HD44780_CustomChar (uint8_t num, uint8_t * data)
{
	uint8_t cnt;
	HD44780_Write (HD44780_CMD_SetCGRAM + num * 8, 1);
	for (cnt = 0; cnt != 8; cnt++)
		HD44780_Write(data[cnt], 0);

	HD44780_Cmd (HD44780_CMD_Home);	
}

/* Internal functions */
static uint8_t Translate (uint8_t dat)
{
  if (HD44780->Font != ENGLISH_RUSSIAN_FONT) return dat; //support only russian
  if (dat < 0xC0) return dat;
  
  return HD44780_RusFont[dat - 0xC0];
}

static void HD44780_Write_4bit (uint8_t data, uint8_t cmd)
{ 
  /* Prepare SPI data */
  uint8_t LatchData = 0;
	
  if (!cmd) LatchData = HD44780->HD44780_Connection->RS_Pin;

  /* Set pin states */
  if (HD44780->BackLightIsOn)
    LatchData |= HD44780->HD44780_Connection->BackLightPin;

  /* Add external connection if used */
  if (HD44780->HD44780_ExtConnection)
    LatchData |= HD44780->HD44780_ExtConnection->EXT1_Pin
      | HD44780->HD44780_ExtConnection->EXT2_Pin;  

	if (cmd == HD44780_NOP_DATA) 
	{
		HD44780->WriteData(LatchData);
		return;
	}	
	
	/* Send high nible */
	HD44780_WriteToBus(LatchData | ((data & 0xF0) >> 4) << HD44780->HD44780_Connection->data_shift);
	
	/* Send low nible */
	HD44780_WriteToBus(LatchData | ((data & 0x0F) >> 0) << HD44780->HD44780_Connection->data_shift);
}

static void HD44780_WriteToBus (uint8_t dat)
{
	/* Send positive strobe */
	dat |= HD44780->HD44780_Connection->E_Pin;
	HD44780->WriteData(dat);

	/* Send negative strobe */
	dat &= ~HD44780->HD44780_Connection->E_Pin;
	HD44780->WriteData(dat);
}

// data format MSB->DDDDDDDD->CCCCCCCC->LSB, C-ctrl, D-data pins
static void HD44780_Write_8bit (uint8_t data, uint8_t cmd)
{	
  /* Backlight enable/disable and data prepare*/
  uint16_t LatchData = (data << 8) | HD44780->BackLightIsOn; 

  if (!cmd) LatchData |= HD44780->HD44780_Connection->RS_Pin;
 
  /* Add external connection if used */
  if (HD44780->HD44780_ExtConnection)
    LatchData |= HD44780->HD44780_ExtConnection->EXT1_Pin
      | HD44780->HD44780_ExtConnection->EXT2_Pin;

	if (cmd == HD44780_NOP_DATA) 
	{
		HD44780->WriteData(LatchData);
		return;
	}	
  
  /* Send data */	
	HD44780_WriteToBus(LatchData);
}
