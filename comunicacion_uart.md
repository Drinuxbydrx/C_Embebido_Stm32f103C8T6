# Qué es UART?
UART (Universal Asynchronous Receiver-Transmitter) es un periférico de hardware que convierte datos en una secuencia serial de bits. Esto lo realiza mediante un pin como emisor y otro pin como receptor, estableciendo una comunicación bidireccional.

En esta comunicación bidireccional, lo primordial es la asincronía, ya que no existe una señal de reloj compartida (a diferencia de los protocolos SPI e I2C). Por ende, se debe establecer una velocidad a la cual se va a realizar tanto la emisión como la recepción; a esto se le llama baud rate (bits por segundo, típicamente 9600 o 115200).

### Conexión UART a STM32F103C8T6

TX : transmisión<br> RX : recepción.<br>Su conexión siempre es de forma cruzada:<br>

TX del STM32 → RX del adaptador USB-TTL<br>
RX del STM32 → TX del adaptador USB-TTL<br>
GND del STM32 → GND del adaptador <br>

<img src="Imagenes/img1.png">

### Configuración en STMCUBE MX

Damos clic en Connectivity, seleccionamos USART1 y en Mode cambiamos a Asynchronous. Se observa claramente que por defecto nos habilita PA9 y PA10, y en Baud rate dejamos por defecto la velocidad de 115200 Bits/s.

<img src="Imagenes/img2.png">

Y aquí damos clic en Generar código, lo que nos genera una estructura como esta:

```c

#include "main.h"

UART_HandleTypeDef huart1;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART1_UART_Init();

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


static void MX_USART1_UART_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }

}


static void MX_GPIO_Init(void)
{
  __HAL_RCC_GPIOA_CLK_ENABLE();
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
``` 
Para codificar nuestro primer **Hola mundo** vamos a configurar nuestra terminal con la misma velocidad de BAude rate que habias puesto en el proyecto **115200**

<img src="Imagenes/img3.png">

[Click para ver el codigo Hola Mundo](URL_de_GitHub)

y finalmente tenemos nuestro hola mundo corriendo en la placa stm32f103c8t6.
<img src="Imagenes/img4.png">
