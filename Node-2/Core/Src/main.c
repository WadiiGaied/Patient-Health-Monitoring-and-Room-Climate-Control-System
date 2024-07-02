/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define   DOMINANT    0
#define   RECESSIF    1

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CAN_HandleTypeDef hcan1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim6;
DMA_HandleTypeDef hdma_tim1_ch1;

/* USER CODE BEGIN PV */
//////////////DHT11/////////////
uint8_t TempByte1,TempByte2,HumByte1,HumByte2;
uint16_t Sum;
uint8_t Temperature = 0;
uint8_t Humidity = 0;
uint8_t Presence = 0 ;
////////////////fan////////////////////////
int PwmData = 0 ;
///////////////CAN/////////////////////////
uint32_t TxMailbox;
CAN_TxHeaderTypeDef TxHeader;
uint8_t aTxData[6] ;
uint8_t aRxData[2] ;
uint8_t CanFlag = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_CAN1_Init(void);
static void MX_TIM6_Init(void);
static void MX_TIM1_Init(void);
/* USER CODE BEGIN PFP */
////////////DHT11/////////////////
 static void Micro_Delay(uint16_t time);
 static void Set_Pin_Output(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin);
 static void Set_Pin_Input(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin);
 static void Dht11_Start (void);
 static uint8_t Check_Response (void);
 static uint8_t Dht11_Read (void);
 static void Dht11_Fct(void);
 ///////////////FAN/////////////////////
 static void Fan_Fct(void);
 ////////////////CAN////////////
 static void CAN1_Filter(void);
 static void CAN1_Tx(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_CAN1_Init();
  MX_TIM6_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  CAN1_Filter();
  if(HAL_TIM_Base_Start(&htim6)!=HAL_OK)
  {
   	Error_Handler();
  }

  HAL_GPIO_WritePin(FAN_S1_GPIO_Port, FAN_S1_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(FAN_S1_GPIO_Port, FAN_S2_Pin , GPIO_PIN_RESET);

  if(HAL_TIM_PWM_Start_DMA(&htim1,TIM_CHANNEL_1,(uint32_t *) &PwmData,1)!= HAL_OK)
  {
	  Error_Handler();
  }
  if (HAL_CAN_Start(&hcan1) !=HAL_OK)
  {
	  Error_Handler();
  }
  if ( HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_TX_MAILBOX_EMPTY )!= HAL_OK)
   {
     Error_Handler();
   }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	   Dht11_Fct();
	   Fan_Fct();
	   CAN1_Tx();

		  if( 1 == CanFlag )
		  {
			  HAL_GPIO_WritePin(GREEN_GPIO_Port, GREEN_Pin , GPIO_PIN_SET);
			  HAL_Delay(1000);
			  HAL_GPIO_WritePin(GREEN_GPIO_Port, GREEN_Pin , GPIO_PIN_RESET);
			  CanFlag = 0;
		  }
		  else if ( 2 == CanFlag )
		  {
			  HAL_GPIO_WritePin(RED_GPIO_Port, RED_Pin , GPIO_PIN_SET);
			  HAL_Delay(1000);
			  HAL_GPIO_WritePin(RED_GPIO_Port, RED_Pin   , GPIO_PIN_RESET);
			  CanFlag = 0;
		  }
		  else if ( 3 == CanFlag )
		  {
			  HAL_GPIO_WritePin(BLUE_GPIO_Port, BLUE_Pin , GPIO_PIN_SET);
			  HAL_Delay(1000);
			  HAL_GPIO_WritePin(BLUE_GPIO_Port, BLUE_Pin   , GPIO_PIN_RESET);
			  CanFlag = 0;
		  }
      HAL_Delay(50);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 50;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CAN1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */

  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 5;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_1TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_8TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */

  /* USER CODE END CAN1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 50-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 99;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 50-1;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 0xfff-1;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GREEN_Pin|ORANGE_Pin|RED_Pin|BLUE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, FAN_S2_Pin|FAN_S1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : DHT11_Pin */
  GPIO_InitStruct.Pin = DHT11_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : GREEN_Pin ORANGE_Pin RED_Pin BLUE_Pin */
  GPIO_InitStruct.Pin = GREEN_Pin|ORANGE_Pin|RED_Pin|BLUE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : FAN_S2_Pin FAN_S1_Pin */
  GPIO_InitStruct.Pin = FAN_S2_Pin|FAN_S1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
static void Micro_Delay(uint16_t time){
	  __HAL_TIM_SET_COUNTER(&htim6, 0);
	  while (__HAL_TIM_GET_COUNTER(&htim6) < time);
}
static void Set_Pin_Output(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin)
{
GPIO_InitTypeDef GPIO_InitStruct = {0};
GPIO_InitStruct.Pin = GPIO_Pin;
GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
GPIO_InitStruct.Pull = GPIO_PULLUP;
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

static void Set_Pin_Input(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin)
{
GPIO_InitTypeDef GPIO_InitStruct = {0};
GPIO_InitStruct.Pin = GPIO_Pin;
GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
GPIO_InitStruct.Pull = GPIO_NOPULL;
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}
static void Dht11_Start (void)
{
	Set_Pin_Output(  DHT11_GPIO_Port, DHT11_Pin );
	HAL_GPIO_WritePin( DHT11_GPIO_Port, DHT11_Pin, 1 );   // pull the pin high
	HAL_Delay(1000);
	HAL_GPIO_WritePin( DHT11_GPIO_Port, DHT11_Pin, 0 );   // pull the pin low
	Micro_Delay(18000);
	HAL_GPIO_WritePin( DHT11_GPIO_Port, DHT11_Pin, 1 );   // pull the pin high
	Micro_Delay(20);
	Set_Pin_Input( DHT11_GPIO_Port, DHT11_Pin );
}


static uint8_t Check_Response (void)
{
	uint8_t Response = 0;
	Micro_Delay(40);
	if (!(HAL_GPIO_ReadPin (DHT11_GPIO_Port, DHT11_Pin)))
	{
	 Micro_Delay(80);
		if ((HAL_GPIO_ReadPin (DHT11_GPIO_Port, DHT11_Pin)))
		{
			Response = 1;
		}
		else
	    {
			Response = -1;
	    }
	}
	while ((HAL_GPIO_ReadPin (DHT11_GPIO_Port, DHT11_Pin)));   // wait for the pin to go low

	return Response;
}



static uint8_t Dht11_Read (void)
{
	uint8_t i,j;
	for (j=0;j<8;j++)
	{
		while (!(HAL_GPIO_ReadPin (DHT11_GPIO_Port, DHT11_Pin)));   // wait for the pin to go high
		Micro_Delay(40);   // wait for 40 us
		if (!(HAL_GPIO_ReadPin (DHT11_GPIO_Port, DHT11_Pin)))   // if the pin is low
		{
			i&= ~(1<<(7-j));   // write 0
		}
		else i|= (1<<(7-j));  // if the pin is high, write 1
		while ((HAL_GPIO_ReadPin (DHT11_GPIO_Port, DHT11_Pin)));  // wait for the pin to go low
	}
	return i;
}
static void Dht11_Fct(void)
 {
	  Dht11_Start();
	  Presence   = Check_Response();
	  HumByte1  =  Dht11_Read();
	  HumByte2  =  Dht11_Read();
	  TempByte1 =  Dht11_Read();
	  TempByte2 =  Dht11_Read();
	  Sum        =  Dht11_Read();
	  Temperature = TempByte1;
	  Humidity    =  HumByte1 ;
}
static void Fan_Fct(void)
 {

	   if (30 < Temperature && 1 == aRxData[0])
	  {
	    HAL_GPIO_WritePin(ORANGE_GPIO_Port, ORANGE_Pin , GPIO_PIN_SET);
	    PwmData = 60;
	  }
	  else if (30 >= Temperature && 1 == aRxData[0] )
	  {
	    HAL_GPIO_WritePin(ORANGE_GPIO_Port, ORANGE_Pin , GPIO_PIN_SET);
	    PwmData = 30;
	  }
	  else if (0 == aRxData[0])
	  {
	   HAL_GPIO_WritePin(ORANGE_GPIO_Port, ORANGE_Pin , GPIO_PIN_RESET);
	   PwmData = aRxData[1];
	  }

}

static void CAN1_Tx(void)
{

	aTxData[3]=Temperature;
	aTxData[4]=Humidity;
	aTxData[5]=aRxData[0];

    if (30 < aTxData[3])
    {
    	 TxHeader.StdId= 0x2FF;
    }
    else
    {
    	 TxHeader.StdId= 0x4FF;
    }
	TxHeader.RTR= CAN_RTR_DATA;
	TxHeader.IDE= CAN_ID_STD;
	TxHeader.DLC= 6;
	TxHeader.TransmitGlobalTime = DISABLE;
	if(HAL_CAN_AddTxMessage(&hcan1, &TxHeader,aTxData , &TxMailbox) != HAL_OK)
	{
		Error_Handler();
	}
}
static void CAN1_Filter(void)
 {
 	CAN_FilterTypeDef can1_filter_init;
 	can1_filter_init.FilterActivation = ENABLE;
 	can1_filter_init.FilterBank = 0;
 	can1_filter_init.FilterFIFOAssignment = CAN_RX_FIFO0;
 	can1_filter_init.FilterIdHigh = 0x0EF<<5;
 	can1_filter_init.FilterIdLow = 0x0000;
 	can1_filter_init.FilterMaskIdHigh = 0x1EF<<5;
 	can1_filter_init.FilterMaskIdLow = 0x0000;
 	can1_filter_init.FilterMode = CAN_FILTERMODE_IDLIST;
 	can1_filter_init.FilterScale = CAN_FILTERSCALE_32BIT;
 	if(HAL_CAN_ConfigFilter(&hcan1, &can1_filter_init) != HAL_OK)
 	{
 		Error_Handler();
 	}
 }
 static void CAN1_Rx(void)
 {
 	 CAN_RxHeaderTypeDef RxHeader;
 	if(HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHeader, aRxData) != HAL_OK)
 	{
 		Error_Handler();
 	}

 }
void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
 {
		if (hcan->Instance== CAN1)
		{
			CanFlag = 1;
	    }


 }

 void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan)
 {
		if (hcan->Instance== CAN1)
		{
			CanFlag = 1;
	    }

 }

void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan)
 {
		if (hcan->Instance== CAN1)
		{
			CanFlag = 1;
	    }

 }


 void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
 {
        if (hcan->Instance== CAN1)
	      {
	          CAN1_Rx();
	          CanFlag = 3;
	      }
 }

 void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
 {
		if (hcan->Instance== CAN1)
		{
			CanFlag = 2;
	    }


 }
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
