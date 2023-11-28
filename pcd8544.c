#include "pcd8544.h"

#define RST             (GPIO_PIN_2)
#define CE              (GPIO_PIN_3)
#define DC              (GPIO_PIN_4)
#define CLK             (GPIO_PIN_5)
#define DO              (GPIO_PIN_7)
#define WIDTH_SIZE      (6)
#define CLEAR_SCREEN    (504)
#define CURSOR_X_POS    (0x80U)
#define CURSOR_Y_POS    (0x40U)


static size_t SPI_Init(void);
static void lcd_io_init(void);
static void lcd_set_command(void);
static void lcd_send_data(uint8_t data);
static void lcd_send_command(uint8_t data);

static SPI_HandleTypeDef hspi1;


void lcd_putch(char character)
{ 
  for(int i = 0; i < WIDTH_SIZE; i++)
  lcd_send_data(ASCII[character - 0x20][i]);

}


void lcd_print(const char *str,size_t X_Pos, size_t Y_Pos)
{
  lcd_set_cursor(X_Pos,Y_Pos);
  while(*str != '\0'){
    lcd_putch(*str);
    str++;
  }
}


void print_github_image(void)
{
  lcd_set_cursor(0,0);
  const size_t SIZE_BIT = (sizeof(Github)/sizeof(char));
  for(size_t i = 0; i< SIZE_BIT; i++)
  {
    lcd_send_data(Github[i]);
  }
}


void lcd_inverse_colour(void)
{
   lcd_send_command(0x0Du);
}


void lcd_non_inverse_colour(void)
{
   lcd_send_command(0x0CU);
}


/** @brief sendData sends data to LCD 
  * @param Data is parameter to be send to LCD
  * @return void
**/
static void lcd_send_data(uint8_t data)
{
  HAL_GPIO_WritePin(GPIOA,CE,GPIO_PIN_RESET);  // CE is LOW to sending DATA
  HAL_GPIO_WritePin(GPIOA,DC,GPIO_PIN_SET);// DC = 1 Data MODE
  HAL_SPI_Transmit(&hspi1,&data, 1, 100);// 5 Byte will be send
  HAL_GPIO_WritePin(GPIOA,CE,GPIO_PIN_SET); 
}


/** @brief sendCommand sends command to LCD 
  * @param Data is parameter to be send to LCD
  * @return void
**/
static void lcd_send_command(uint8_t data)
{
  HAL_GPIO_WritePin(GPIOA,CE,GPIO_PIN_RESET);  // CE is LOW to sending COMMAND
  HAL_GPIO_WritePin(GPIOA,DC,GPIO_PIN_RESET);// DC = 0 Command MODE
  HAL_SPI_Transmit(&hspi1,&data, 1, 100);// 1 Byte will be send
  HAL_GPIO_WritePin(GPIOA,DC,GPIO_PIN_SET); 
  HAL_GPIO_WritePin(GPIOA,CE,GPIO_PIN_SET); 
  HAL_GPIO_WritePin(GPIOA,RST,GPIO_PIN_SET); 
}


void lcd_clear()
{
  for(int i = 0; i < CLEAR_SCREEN; i++)
  {
    lcd_send_data(0x00);
  }
}




void lcd_set_cursor(int positionX, int positionY)
{
  lcd_send_command(CURSOR_X_POS | positionX);
  lcd_send_command(CURSOR_Y_POS | positionY);
}



/** @brief displayInit initialize display
 *  @return void
**/
void lcd_init(void)
{
  lcd_io_init();
  SPI_Init();
  HAL_GPIO_WritePin(GPIOA,RST,GPIO_PIN_SET); // Reset is HIGH for data flow
  lcd_set_command();  
}



/** @brief LCDInit, initialize  LCD
  * @return void
**/
static void lcd_set_command(void)
{
    lcd_send_command(0x21);
    lcd_send_command(0xBB);
    lcd_send_command(0x04);
    lcd_send_command(0x14);
    lcd_send_command(0x20);
    lcd_send_command(0x0C);
    lcd_clear();
}

/** @brief DisplayIO, initialize  display port
  * @return void
**/
static void lcd_io_init(void)
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
static size_t SPI_Init(void)
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
    return HAL_ERROR;
  }
  return HAL_OK;
}