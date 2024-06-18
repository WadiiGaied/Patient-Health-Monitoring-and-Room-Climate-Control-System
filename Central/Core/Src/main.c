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
#include "ESPDataLogger.h"
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

UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
 uint8_t aRxData[6];
 uint16_t aRxDataT[5];
 uint8_t aRxDatac[5];
 uint8_t EtatFan = 0 ;
 uint8_t Flag = 0 ;
 uint8_t QtRxData[2];
 uint8_t aTxData[2];
 //////////paramétres CAN/////////
 uint32_t TxMailbox;
 CAN_RxHeaderTypeDef RxHeader;
 uint8_t FlagQtVentilateur = 0 ;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CAN1_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
static void CAN1_Filter(void);
static void CAN1_Tx(void);
static void send_data_uart(uint8_t *data, uint8_t size);
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
  MX_CAN1_Init();
  MX_USART3_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  CAN1_Filter();
 if (HAL_CAN_Start(&hcan1) !=HAL_OK)
  {
	  Error_Handler();
  }
 if (HAL_CAN_ActivateNotification(&hcan1,CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_TX_MAILBOX_EMPTY )!= HAL_OK)
  {
    Error_Handler();
  }

 HAL_UART_Receive_IT(&huart2, QtRxData, sizeof(QtRxData));
 ESP_Init("Redmi","09876543210X");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  if ( 1 == Flag )
	    {
		      HAL_GPIO_TogglePin(GREEN_GPIO_Port, GREEN_Pin);
              Flag = 0;

         }
	  else if ( 2 == Flag )
		  {
			  HAL_GPIO_TogglePin(RED_GPIO_Port, RED_Pin);
			  Flag = 0;
		  }
	  else if ( 3 == Flag )
		  {
			  HAL_GPIO_TogglePin(BLUE_GPIO_Port, BLUE_Pin);
			  send_data_uart(aRxDatac, sizeof(aRxDatac));
              ESP_Send_Multi("Q2II80D9IDVPRW74",5,aRxDataT);
		      HAL_Delay(1000);
              Flag = 0;
		  }
	 /* if ( 1 == FlagQtVentilateur )
	  {
		  HAL_UART_Receive_IT(&huart2, QtRxData, sizeof(QtRxData));
		  CAN1_Tx();
		  FlagQtVentilateur = 0 ;
	       if(QtRxData[0] == 1)
	    {
	      EtatFan = 1 ;
	      CAN1_Tx();
	      FlagQtVentilateur = 0 ;
	      HAL_GPIO_WritePin(ORANGE_GPIO_Port, ORANGE_Pin, GPIO_PIN_SET); // Turn on LED
        }
	    else if(QtRxData[0] == 0)
	    {
	      EtatFan = 0 ;
	      HAL_GPIO_WritePin(ORANGE_GPIO_Port, ORANGE_Pin, GPIO_PIN_RESET); // Turn off LED
	      CAN1_Tx();
          FlagQtVentilateur = 0 ;
	    }
	    else
	    {
	    	FlagQtVentilateur = 0 ;
	    }

	  }*/


    //  HAL_Delay(50);
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
  hcan1.Init.TimeSeg1 = CAN_BS1_8TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_1TQ;
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
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

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
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GREEN_Pin|ORANGE_Pin|RED_Pin|BLUE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : GREEN_Pin ORANGE_Pin RED_Pin BLUE_Pin */
  GPIO_InitStruct.Pin = GREEN_Pin|ORANGE_Pin|RED_Pin|BLUE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
static void CAN1_Filter(void)
 {
 	CAN_FilterTypeDef can1_filter_init;
 	can1_filter_init.FilterActivation = ENABLE;
 	can1_filter_init.FilterBank = 0;
 	can1_filter_init.FilterFIFOAssignment = CAN_RX_FIFO0;
 	can1_filter_init.FilterIdHigh = 0x4FF<<5;
 	can1_filter_init.FilterIdLow = 0x0000;
 	can1_filter_init.FilterMaskIdHigh = 0x8FF<<5;
 	can1_filter_init.FilterMaskIdLow = 0x0000;
 	can1_filter_init.FilterMode = CAN_FILTERMODE_IDMASK;
 	can1_filter_init.FilterScale = CAN_FILTERSCALE_32BIT;
 	if(HAL_CAN_ConfigFilter(&hcan1, &can1_filter_init) != HAL_OK)
 	{
 		Error_Handler();
 	}
 }
 static void CAN1_Rx(void)
 {

 	if(HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHeader, aRxData) != HAL_OK)
 	{
 		Error_Handler();
 	}
 	if (RxHeader.RTR == DOMINANT  )
 	{

 	/*if( RxHeader.StdId == 0x2FF ||  RxHeader.StdId  == 0x3FF)
 	{
		HAL_GPIO_TogglePin(ORANGE_GPIO_Port,ORANGE_Pin);
		HAL_GPIO_WritePin(BLUE_GPIO_Port,BLUE_Pin,RESET);
		HAL_GPIO_WritePin(RED_GPIO_Port,RED_Pin,RESET);

 	}
 	else if( RxHeader.StdId  == 0x1FF )
 	{
 		 HAL_GPIO_TogglePin(RED_GPIO_Port,RED_Pin);
 		 HAL_GPIO_WritePin(BLUE_GPIO_Port,BLUE_Pin,RESET);
 		 HAL_GPIO_WritePin(ORANGE_GPIO_Port,ORANGE_Pin,RESET);
 	}
 	else if( RxHeader.StdId == 0x0FF )
 	{
 		 HAL_GPIO_TogglePin(BLUE_GPIO_Port,BLUE_Pin);
 		 HAL_GPIO_WritePin(ORANGE_GPIO_Port,ORANGE_Pin,RESET);
 		 HAL_GPIO_WritePin(RED_GPIO_Port,RED_Pin,RESET);
 	}
 	else if( RxHeader.StdId == 0x4FF )
 	{
 		HAL_GPIO_WritePin(RED_GPIO_Port,BLUE_Pin,RESET);
 		HAL_GPIO_WritePin(ORANGE_GPIO_Port,ORANGE_Pin,RESET);
 		HAL_GPIO_WritePin(RED_GPIO_Port,RED_Pin,RESET);

 	}
 	*/
 	if (RxHeader.StdId == 0x0FF || RxHeader.StdId == 0x1FF || RxHeader.StdId == 0x3FF ){
 		for(int i = 0; i < 3; i++) {
 			aRxDatac[i] = aRxData[i];
 			aRxDataT[i] = aRxData[i];
 		}
 	}
 	else if (RxHeader.StdId == 0x2FF || RxHeader.StdId == 0x4FF ){
 		for(int i = 0; i < 2; i++) {
 			aRxDatac[i+3] = aRxData[i+3];
 			aRxDataT[i+3] = aRxData[i+3];
 		}
 	}
 }
 }
 void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
  {
 		if (hcan->Instance== CAN1)
 		{
 			Flag=1;
 	    }


  }

  void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan)
  {
 		if (hcan->Instance== CAN1)
 		{
 			Flag=1;
 	    }

  }

 void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan)
  {
 		if (hcan->Instance== CAN1)
 		{
 			Flag=1;
 	    }

  }

 void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
 {
	 if (hcan->Instance== CAN1)
	{
		 CAN1_Rx();
	     Flag=3;
	}

 }
void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
  if (hcan->Instance== CAN1)
	{
	 	 Flag=2;
	}

 }
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART2)
	  {

		  HAL_UART_Receive_IT(&huart2, QtRxData, sizeof(QtRxData));
		  CAN1_Tx();

	  }

}

 static void send_data_uart(uint8_t *data, uint8_t size) {
	   HAL_UART_Transmit(&huart2,  data, size, HAL_MAX_DELAY);
	}
 void convert_uint8_to_uint16(uint8_t *input, uint16_t *output, size_t length) {
     for (size_t i = 0; i < length; i++) {
         output[i] = (uint16_t)input[i];
     }
 }
 static void CAN1_Tx(void)
 {
 	CAN_TxHeaderTypeDef TxHeader;
 	aTxData[0] = QtRxData[0];
 	aTxData[1] = QtRxData[1];
 	if (0 == aTxData[0])
 	{
 		TxHeader.StdId= 0x0EF;
 	}
 	else if (1 == aTxData[0])
 	{
 		TxHeader.StdId= 0x1EF;
 	}

 	TxHeader.RTR= CAN_RTR_DATA;
 	TxHeader.IDE= CAN_ID_STD;
 	TxHeader.DLC= 2;
 	TxHeader.TransmitGlobalTime = DISABLE;
 	if(HAL_CAN_AddTxMessage(&hcan1, &TxHeader,aTxData , &TxMailbox) != HAL_OK)
 	{
 		Error_Handler();
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
