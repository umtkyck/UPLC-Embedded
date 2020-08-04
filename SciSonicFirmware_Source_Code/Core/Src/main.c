/*
/  Filename: main.c		                                                    /
/  Feature:  					                                            /
/  Module:   	                                                            /
/  Author:	 Mesaflix						                                /
/                                                                           /
/                                                                           /
/  ******************** Ltd. owns the copyright in this                     /
/  and associated documents and all rights are reserved.                    /
/  This document and associated documents must not be                       /
/  used for any purpose other than that for which they                      /
/  are supplied and must not be copied in whole or in                       /
/  part, or disclosed to others without the prior                           /
/  written consent of Mesaflix Ltd.  Any copy                   			/
/  of this document made by any method must also contain a                  /
/  copy of this legend.                                                     /
/                                                                           /
*/
/* USER CODE BEGIN Header */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbd_cdc_if.h"
#include "Globals.h"
#include "Flasher.h"
#include "ProgParser.h"
#include "MyFlah.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */



/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

IWDG_HandleTypeDef hiwdg;

SPI_HandleTypeDef hspi2;

/* USER CODE BEGIN PV */

bool ADCReady;
uint32_t ADCValues[10];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_SPI2_Init(void);
static void MX_DMA_Init(void);
static void MX_IWDG_Init(void);
/* USER CODE BEGIN PFP */
void WDT();
void CDC_RX_IRQ_Handler(USBD_HandleTypeDef * usbfs);
bool Send_Data_To_CDC(uint8_t * Data, uint8_t Len);
void USB_RX_MainProcess();
void SysTickEvent();
void ADC_Interrupt_Handler();
void ProcessADC();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define convertToMili 1000

uint32_t BackTick;
uint32_t LedTick;
uint8_t Ticks;
uint8_t led_start_once;
int led_blink_val;
void LED_Service()
{
	if(!ProgrammingMode)
	 {
		 if(FlashIsProgrammed && ProgInited)
		 {
			 if((HAL_GetTick()-BackTick)>1000)
				 {
				 HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
				 BackTick=HAL_GetTick();
				 }
			 if(Ticks<2) Ticks++;
			 else CareOuts=true;
		 }
		 else
		 {
			 Ticks=0;
			 CareOuts=false;
			 if((HAL_GetTick()-BackTick)>300)
				 {
				 HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
				 BackTick=HAL_GetTick();
				 }
		 }
	 }
	 else if(ProgrammingMode)
	 {
		 Ticks=0;
		 CareOuts=false;
		 HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin, GPIO_PIN_RESET);
		 BackTick=HAL_GetTick();
	 }
}

void Led_Startup()
{
	Flash_ReadPage(pageData,1,2);
	uint8_t data[1];
	uint8_t *ledPtr=pageData;
	if(ledPtr[0] == 'L')
	{
		ledPtr = ledPtr + 1;
		memcpy(data,ledPtr,1);
		sscanf((char*)data,"%d",&led_blink_val);
	}
	else
	{
		/*Do Nothing. Error Stage*/
	}
	led_blink_val = led_blink_val * 2;
	while(led_blink_val>0)
	{
		if((HAL_GetTick()-BackTick)>300)
		{
			 HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
			 BackTick=HAL_GetTick();
			 led_blink_val--;
		}
		WDT();
	}
	HAL_Delay(1000);
}


/*
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc1)
{
	if(hadc1->Instance == ADC1)
	{
		ADCValues[0] = ADCValuesDma[0];
		ADCValues[1] = ADCValuesDma[1];
		ADCValues[2] = ADCValuesDma[2];
		ADCValues[3] = ADCValuesDma[3];
		ADCValues[4] = ADCValuesDma[4];
		ADCValues[5] = ADCValuesDma[5];
		ADCValues[6] = ADCValuesDma[6];
		ADCValues[7] = ADCValuesDma[7];
		ADCValues[8] = ADCValuesDma[8];
		ADCValues[9] = ADCValuesDma[9];
	}
}
*/
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	BackTick=0;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  //HAL_Delay(300);
  ADCReady=false;
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI2_Init();
  MX_USB_DEVICE_Init();
  MX_DMA_Init();
  MX_IWDG_Init();
  /* USER CODE BEGIN 2 */
//-----------------
  MX_ADC1_Init();
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&ADCValues,10);

//-----------------
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  InitGlobals();
  WDT();
  FlasherInit();
  WDT();
  Led_Startup();
  WDT();
  while (1)
  {
	 if(ADCReady) ProcessADC();
	 USB_RX_MainProcess();
	 WDT();
	 ProcessIO();
	 WDT();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	 LED_Service();
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC|RCC_PERIPHCLK_USB;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV4;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */
	  //HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&ADCValues,10);
  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 10;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_13CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = ADC_REGULAR_RANK_3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = ADC_REGULAR_RANK_4;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = ADC_REGULAR_RANK_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_5;
  sConfig.Rank = ADC_REGULAR_RANK_6;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_6;
  sConfig.Rank = ADC_REGULAR_RANK_7;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_7;
  sConfig.Rank = ADC_REGULAR_RANK_8;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_VREFINT;
  sConfig.Rank = ADC_REGULAR_RANK_9;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
  sConfig.Rank = ADC_REGULAR_RANK_10;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief IWDG Initialization Function
  * @param None
  * @retval None
  */
static void MX_IWDG_Init(void)
{

  /* USER CODE BEGIN IWDG_Init 0 */

  /* USER CODE END IWDG_Init 0 */

  /* USER CODE BEGIN IWDG_Init 1 */

  /* USER CODE END IWDG_Init 1 */
  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_128;
  hiwdg.Init.Reload = 2000;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN IWDG_Init 2 */

  /* USER CODE END IWDG_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, F_CS_Pin|NP5_Pin|NP4_Pin|NP3_Pin
                          |NP2_Pin|NP1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, EN2_Pin|EN1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_Pin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : F_CS_Pin NP5_Pin NP4_Pin NP3_Pin
                           NP2_Pin NP1_Pin */
  GPIO_InitStruct.Pin = F_CS_Pin|NP5_Pin|NP4_Pin|NP3_Pin
                          |NP2_Pin|NP1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : EN2_Pin EN1_Pin */
  GPIO_InitStruct.Pin = EN2_Pin|EN1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
//-----------------------------------------------------------------------------------
void WDT()
{
	HAL_IWDG_Refresh(&hiwdg);
}
//----------------------------------------------------------------------------------
int _write(int file, char *ptr, int len)
{
  /* Implement your write code here, this is used by puts and printf for example */
  int i=0;
  for(i=0 ; i<len ; i++)
    ITM_SendChar((*ptr++));
  return len;
}
//----------------------------------------------------------------------------------
bool Send_Data_To_CDC(uint8_t * Data, uint8_t Len)
{
	if(Len>MAX_BUF) return false;
	if(Data==NULL) return false;
	if(Data[0]==0) return false;
	memcpy(SendData.Data,Data,Len);
	if(Len>0) SendData.len=Len;
	else SendData.len=strlen((char*)Data);
	uint8_t result = CDC_Transmit_FS(SendData.Data, SendData.len);
	SendData.len=0;
	if(result==USBD_BUSY || result==USBD_FAIL) return false;
	return true;
}
//------------------------------------------------------------------------------
void CDC_RX_IRQ_Handler(USBD_HandleTypeDef * usbfs)
{
	if(usbfs->pClassData==NULL) return;
	USBD_CDC_HandleTypeDef *hcdc = (USBD_CDC_HandleTypeDef *)usbfs->pClassData;
	if(hcdc->RxLength==0) return;
	if(RecvData.len>0 || BusyFlag)
		{
		hcdc->RxLength=0;
		/*uint8_t answ[2];
		answ[0]=COMMAND_PREFIX;
		answ[1]=BUSY_ANSWER;
		Send_Data_To_CDC(answ, 2);*/
		return;				//е�?ли предыдущий пакет не обработан
		}
	if(hcdc->RxBuffer[0]==0)
	{
		RecvData.len=0;
		hcdc->RxLength=0;
		return;
	}
	else
	{
		memcpy(RecvData.Data,hcdc->RxBuffer,hcdc->RxLength);
		RecvData.len=hcdc->RxLength;
		hcdc->RxLength=0;
	}
}
//-----------------------------------------------------------------------------
uint8_t DataStage;
void USB_RX_MainProcess()
{
	  if(RecvData.len>0) //прин�?ты данные
	  {
		  BusyFlag=true;
		  //тут обработка
		  if(RecvData.Data[0]=='V' || RecvData.Data[0]=='v')
		  {
			  Send_Data_To_CDC((uint8_t*)FirmVer,strlen((char *)FirmVer));
		  }
		  else
		  {
			  PC_BuffProcessing(&RecvData);
		  }
		  //тут обработка
		  RecvData.len=0; //разрешаем прием �?леду�?щего пакета.
		  BusyFlag=false;
	  }
}
//------------------------------------------------------------------------------
void SysTickEvent()
{
	return;
}

//----------------------------------------------------------------------------------
void ADC_Interrupt_Handler()
{
	ADCReady=true;
}
//----------------------------------------------------------------------------------
void CalcProcents()
{
	WDT();
	if(Channels.Vin>0)
	{
	Channels.CH1p= (Channels.CH1/Channels.Vin)*100.0;
	Channels.CH2p= (Channels.CH2/Channels.Vin)*100.0;
	Channels.CH3p= (Channels.CH3/Channels.Vin)*100.0;
	Channels.CH4p= (Channels.CH4/Channels.Vin)*100.0;
	Channels.CH5p= (Channels.CH5/Channels.Vin)*100.0;
	}
	else
	{
	Channels.CH1p= 100;
	Channels.CH2p= 100;
	Channels.CH3p= 100;
	Channels.CH4p= 100;
	Channels.CH5p= 100;
    }
}
//----------------------------------------------------------------------------------
//#define USE_VREF;
//#define PLC_DEBUG
void ProcessADC()
{
		//Channels
	float Vref;
	float temp;
	#ifdef USE_VREF
	if(ADCValues[8]>0)
	Vref = 1.16*4095.0/(float)(ADCValues[8]);
	else Vref =3.3;
	#else
	Vref = 3.33;
	#endif
	#ifndef PLC_DEBUG
	temp = (float)ADCValues[9]*Vref/4095.0;
	Channels.Temp = ((1.43-temp)/4.3)+25.0;
	Channels.FB1p = (float)(ADCValues[0]/4095.0)*100.0;
	Channels.FB2p = (float)(ADCValues[1]/4095.0)*100.0;
	Channels.FB1 = (float)(ADCValues[0]/4095.0)*Vref*1000;
	Channels.FB2 = (float)(ADCValues[1]/4095.0)*Vref*1000;
	temp = (float)ADCValues[2]*Vref/4095.0;
	Channels.CH1 = ((24.49*temp)/2.49)*convertToMili;
	temp = (float)ADCValues[3]*Vref/4095.0;
	Channels.CH2 = ((24.49*temp)/2.49)*convertToMili;
	temp = (float)ADCValues[4]*Vref/4095.0;
	Channels.CH3 = ((24.49*temp)/2.49)*convertToMili;
	temp = (float)ADCValues[5]*Vref/4095.0;
	Channels.CH4 = ((24.49*temp)/2.49)*convertToMili;
	temp = (float)ADCValues[6]*Vref/4095.0;
	Channels.CH5 = ((24.49*temp)/2.49)*convertToMili;
	temp = (float)ADCValues[7]*Vref/4095.0;
	Channels.Vin = ((24.49*temp)/2.49)*convertToMili;//Diranc 10.7888k olarak alındı.
	#else
	temp = (float)ADCValues[9]*Vref/4095.0;
	Channels.Temp = ((1.43-temp)/4.3)+25.0;;
	Channels.FB1 = (float)(ADCValues[0]/4095.0)*100.0;
	Channels.FB2 = (float)(ADCValues[1]/4095.0)*100.0;
	Channels.Vin = Vref;
	Channels.CH1 = (float)ADCValues[2]*Vref/4095.0;
	Channels.CH2 = (float)ADCValues[3]*Vref/4095.0;
	Channels.CH3 = (float)ADCValues[4]*Vref/4095.0;
	Channels.CH4 = (float)ADCValues[5]*Vref/4095.0;
	Channels.CH5 = (float)ADCValues[6]*Vref/4095.0;
	#endif
	CalcProcents();
	ADCReady=false;
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
