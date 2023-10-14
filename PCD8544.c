#include "pcd8544.h"
#include "main.h"
#include "stdint.h"

#define RST GPIO_PIN_2
#define CE  GPIO_PIN_3
#define DC  GPIO_PIN_4
#define CLK GPIO_PIN_5
#define DO  GPIO_PIN_7
#define Width_Size 6



static void SPI1_Init(void);
static void DisplayIO(void);
static void LCDCursor(int positionX, int positionY);
static void LCDInit(void);


SPI_HandleTypeDef hspi1;


/** @brief displayInit initialize display
 *  @return void
**/
void displayInit(void)
{
  DisplayIO();
  SPI1_Init();
  HAL_GPIO_WritePin(GPIOA,RST,GPIO_PIN_SET); // Reset is HIGH for data flow
  LCDInit();  
}



/** @brief Print character 
 *  @param character is char to be printed
 *   @return character
**/
int Putch(char character)
{
 
  for(int i = 0; i < Width_Size; i++)
  sendData(ASCII[character - 0x20][i]);

return character;  
}



/** @brief Print string using X,Y position
 *  @param str is string to be printed
 *  @param X_Pos is x position ( 0 to 83 )
 *  @param Y_Pos is y position ( 0 to 5 )
    @return *str
**/
int lcdPrint(char *str,int X_Pos, int Y_Pos)
{
  LCDCursor(X_Pos,Y_Pos);
  while(*str != '\0'){
    Putch(*str);
    str++;
  }
return *str;
}



/** @brief printImage prints array named github in font.h
 *  @return void
**/
void printImage(void)
{
  LCDCursor(0,0);
  const int SIZE_BIT = (sizeof(Github)/sizeof(char));
  for(int i = 0; i< SIZE_BIT; i++)
  {
    sendData(Github[i]);
  }
}



/** @brief lcdInverse inverse colors
  * @return void
**/
void lcdInverse(void)
{
   sendCommand(0x0D);
}


/** @brief lcdNonInverse inverts the color of the screen
  * @return void
**/
void lcdNonInverse(void)
{
   sendCommand(0x0C);
}



/** @brief sendData sends data to LCD 
  * @param Data is parameter to be send to LCD
  * @return void
**/
void sendData(uint8_t Data)
{
  HAL_GPIO_WritePin(GPIOA,CE,GPIO_PIN_RESET);  // CE is LOW to sending DATA
  HAL_GPIO_WritePin(GPIOA,DC,GPIO_PIN_SET);// DC = 1 Data MODE
  HAL_SPI_Transmit(&hspi1,&Data, 1, 100);// 5 Byte will be send
  HAL_GPIO_WritePin(GPIOA,CE,GPIO_PIN_SET); 
}




/** @brief sendCommand sends command to LCD 
  * @param Data is parameter to be send to LCD
  * @return void
**/
int sendCommand(uint8_t Data)
{
  HAL_GPIO_WritePin(GPIOA,CE,GPIO_PIN_RESET);  // CE is LOW to sending COMMAND
  HAL_GPIO_WritePin(GPIOA,DC,GPIO_PIN_RESET);// DC = 0 Command MODE
  HAL_SPI_Transmit(&hspi1,&Data, 1, 100);// 1 Byte will be send

  HAL_GPIO_WritePin(GPIOA,DC,GPIO_PIN_SET); 
  HAL_GPIO_WritePin(GPIOA,CE,GPIO_PIN_SET); 
  HAL_GPIO_WritePin(GPIOA,RST,GPIO_PIN_SET); 

  return Data;
}



/** @brief LCDClear clear the scren
 *  @return void
**/
void LCDClear(){
  for(int i = 0; i < 504; i++){
    sendData(0x00);
  }
}



/** @brief LCDCursor adjust cursor
  * @param positionX, set X poisiton
  * @param positionY, set Y position
  * @return void
**/
static void LCDCursor(int positionX, int positionY)
{
  sendCommand(0x80 | positionX);
  sendCommand(0x40 | positionY);
}


/** @brief LCDInit, initialize  LCD
  * @return void
**/
static void LCDInit(void)
{
    sendCommand(0x21);
    sendCommand(0xBB);
    sendCommand(0x04);
    sendCommand(0x14);
    sendCommand(0x20);
    sendCommand(0x0C);
    LCDClear();
}


/** @brief DisplayIO, initialize display pins 
  * @return void
**/
static void DisplayIO(void)
{
  HAL_GPIO_WritePin(GPIOA,RST,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA,CE,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA,DC,GPIO_PIN_RESET);
  
  GPIO_InitTypeDef IOinit;
  
  IOinit.Pin = RST|CE|DC;
  IOinit.Mode = GPIO_MODE_OUTPUT_PP;
  IOinit.Pull = GPIO_NOPULL;
  IOinit.Speed = GPIO_SPEED_FREQ_HIGH;
    
  HAL_GPIO_Init(GPIOA,&IOinit);
}


/** @brief SPI1_Init, initialize spi1
  * @return void
**/
static void SPI1_Init(void)
{

   
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
}