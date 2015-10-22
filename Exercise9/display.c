
#include <stdio.h>
#include <stdint.h>

#include "display.h"
#include "orchestrator.h"
#include "bsp_glcd.h"

#define STRING_LEN 12
static char p_displayLine[STRING_LEN];

//#define BRAND_NAME "DIVE BARR"

#define R0W_FOR_BRAND_NAME      1
#define R0W_FOR_DEPTH           3
#define R0W_FOR_RATE            4
#define R0W_FOR_AIR_LEVEL       5
#define R0W_FOR_DIVE_TIME       6
#define R0W_FOR_ALARM           8
#define LCD_ROW_IDX(x) (x-1)

char const brandName[] =  "DIVE BARR";

void updateDisplay(char * depth, char * rate, uint16_t air,  char * edt, char * alarm){
  
  //Write company name
  BSP_GraphLCD_String(LCD_ROW_IDX(R0W_FOR_BRAND_NAME), brandName);
  
  //Write current depth
  BSP_GraphLCD_String(LCD_ROW_IDX(R0W_FOR_DEPTH), depth );

  //Write current rate
  BSP_GraphLCD_String(LCD_ROW_IDX(R0W_FOR_RATE), rate );
  
  //Write current air level
  sprintf(p_displayLine, "AIR: %4u", air );
  BSP_GraphLCD_String(LCD_ROW_IDX(R0W_FOR_AIR_LEVEL), p_displayLine );
  
  //Write current rate
  BSP_GraphLCD_String(LCD_ROW_IDX(R0W_FOR_DIVE_TIME), edt );
  
  //Write current rate
  BSP_GraphLCD_String(LCD_ROW_IDX(R0W_FOR_ALARM), alarm );
  
 }