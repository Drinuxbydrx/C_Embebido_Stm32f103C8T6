
#include "main.h"
// Definicion de pines
#define RS_GPIO_Port GPIOA
#define RS_Pin       GPIO_PIN_0

#define EN_GPIO_Port GPIOA
#define EN_Pin       GPIO_PIN_1

#define D4_GPIO_Port GPIOA
#define D4_Pin       GPIO_PIN_2

#define D5_GPIO_Port GPIOA
#define D5_Pin       GPIO_PIN_3

#define D6_GPIO_Port GPIOA
#define D6_Pin       GPIO_PIN_4

#define D7_GPIO_Port GPIOA
#define D7_Pin       GPIO_PIN_5

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void LCD_EnablePulse(void);
void LCD_SendNibble(uint8_t nibble);
void LCD_SendByte(uint8_t data, uint8_t isDataMode);
void LCD_Init(void);
void LCD_SendString(char *str);
int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  LCD_Init();
  // Posicionar cursor e imprimir mensaje
      LCD_SendByte(0x80, 0); // Fila 0, Columna 0
      LCD_SendString("Hola Mundo!");

      LCD_SendByte(0xC0, 0); // Fila 1, Columna 0
      LCD_SendString("STM32F103C8T6");
  while (1)
  {

  }
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3
                           PA4 PA5 PA6 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

// Genera un pulso en el pin Enable para registrar el dato
void LCD_EnablePulse(void) {
    HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, GPIO_PIN_SET);
    HAL_Delay(1); // 1 ms es más que suficiente para el enganche
    HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, GPIO_PIN_RESET);
    HAL_Delay(1);
}

// Envía un nibble (4 bits) al bus de datos del LCD
void LCD_SendNibble(uint8_t nibble) {
    HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, (nibble & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(D5_GPIO_Port, D5_Pin, (nibble & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(D6_GPIO_Port, D6_Pin, (nibble & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(D7_GPIO_Port, D7_Pin, (nibble & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    LCD_EnablePulse();
}

// Envía un byte dividiéndolo en MSB (4 bits altos) y LSB (4 bits bajos)
void LCD_SendByte(uint8_t data, uint8_t isDataMode) {
    // Definir si es Comando (RS=0) o Dato/Carácter (RS=1)
    HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin, isDataMode ? GPIO_PIN_SET : GPIO_PIN_RESET);

    // Enviar MSB primero, luego LSB
    LCD_SendNibble(data >> 4);
    LCD_SendNibble(data & 0x0F);
}

void LCD_Init(void) {
    HAL_Delay(50); // Tiempo de estabilización de voltaje post-power-on

    // Secuencia de inicialización para modo 4-bits según datasheet HD44780
    LCD_SendNibble(0x03);
    HAL_Delay(5);
    LCD_SendNibble(0x03);
    HAL_Delay(1);
    LCD_SendNibble(0x03);
    LCD_SendNibble(0x02); // Cambia oficialmente a modo 4-bits

    // Configuración de pantalla
    LCD_SendByte(0x28, 0); // 4-bits, 2 líneas, fuente 5x8
    LCD_SendByte(0x0C, 0); // Display ON, Cursor OFF, Blinking OFF
    LCD_SendByte(0x01, 0); // Limpiar pantalla
    HAL_Delay(2);
    LCD_SendByte(0x06, 0); // Incremento de cursor a la derecha
}

void LCD_SendString(char *str) {
    while (*str) {
        LCD_SendByte((uint8_t)(*str++), 1);
    }
}

void Error_Handler(void)
{

  __disable_irq();
  while (1)
  {
  }

}
#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t *file, uint32_t line)
{

}
#endif
