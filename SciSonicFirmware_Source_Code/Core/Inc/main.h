/* USER CODE BEGIN Header */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
extern bool Send_Data_To_CDC(uint8_t * Data, uint8_t Len);
/* USER CODE END EFP */
int val2;
/* Private defines -----------------------------------------------------------*/
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define FB1_Pin GPIO_PIN_0
#define FB1_GPIO_Port GPIOA
#define FB2_Pin GPIO_PIN_1
#define FB2_GPIO_Port GPIOA
#define CH1_Pin GPIO_PIN_2
#define CH1_GPIO_Port GPIOA
#define CH2_Pin GPIO_PIN_3
#define CH2_GPIO_Port GPIOA
#define CH3_Pin GPIO_PIN_4
#define CH3_GPIO_Port GPIOA
#define CH4_Pin GPIO_PIN_5
#define CH4_GPIO_Port GPIOA
#define CH5_Pin GPIO_PIN_6
#define CH5_GPIO_Port GPIOA
#define Vin_MEAS_Pin GPIO_PIN_7
#define Vin_MEAS_GPIO_Port GPIOA
#define F_CS_Pin GPIO_PIN_12
#define F_CS_GPIO_Port GPIOB
#define EN2_Pin GPIO_PIN_9
#define EN2_GPIO_Port GPIOA
#define EN1_Pin GPIO_PIN_10
#define EN1_GPIO_Port GPIOA
#define NP5_Pin GPIO_PIN_3
#define NP5_GPIO_Port GPIOB
#define NP4_Pin GPIO_PIN_4
#define NP4_GPIO_Port GPIOB
#define NP3_Pin GPIO_PIN_5
#define NP3_GPIO_Port GPIOB
#define NP2_Pin GPIO_PIN_6
#define NP2_GPIO_Port GPIOB
#define NP1_Pin GPIO_PIN_7
#define NP1_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
