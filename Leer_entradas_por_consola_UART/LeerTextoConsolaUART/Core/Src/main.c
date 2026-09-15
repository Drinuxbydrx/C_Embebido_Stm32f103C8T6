
#include "main.h"
#include <string.h>
#include<stdio.h>
UART_HandleTypeDef huart1;
HAL_StatusTypeDef detecciontexto;
#define UART_BUFFER_SIZE 64

char buffer[UART_BUFFER_SIZE];
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
void UART_Leertexto(char *buffer,uint16_t max_longitud);

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  uint8_t prompt[]="Escribe un texto:";
  while (1)
  {
	  uint8_t resp[100];
	  HAL_UART_Transmit(&huart1, prompt, sizeof(prompt) - 1, 100);
	  UART_Leertexto(buffer, UART_BUFFER_SIZE);

	  int longitud = sprintf((char*)resp,"\r\nTexto recibido:[%s]\r\n",buffer);
	  HAL_UART_Transmit(&huart1,resp,longitud,100);


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
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK                       |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
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
void UART_Leertexto(char *buffer,uint16_t max_longitud){

	uint16_t id = 0;
	uint8_t caracter = 0;

	memset(buffer,0,max_longitud);

	while(id<max_longitud -1){
		detecciontexto=HAL_UART_Receive(&huart1,&caracter,1,10000);

		if(detecciontexto == HAL_OK){
			HAL_UART_Transmit(&huart1,&caracter,1,10);
			if(caracter=='\r'||caracter=='\n'){
				break;
			}
			buffer[id++]=(char) caracter;
		}
	}
            buffer[id]='\0';
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
