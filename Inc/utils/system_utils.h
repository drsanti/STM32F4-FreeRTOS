/*
 ********************************************************************
 *                     STM32F4xx based on FreeRTOS
 ********************************************************************
 * FileName:    system_utils.h
 * Description: Utillity functions, declaration and etc.
 ********************************************************************
 * Dr.Santi Nuratch
 * Embedded Computing and Control Laboratory | INC@KMUTT
 * 03 June, 2019
 * ****************************************************************** 
 */

#ifndef __SYSTEM_UTILS_H__
#define __SYSTEM_UTILS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include "cmsis_os.h"

#define LED_GREEN    ((uint16_t)0x1000 << 0) //GPIO_Pin_12 // Green LED connects to PD12
#define LED_ORANGE   ((uint16_t)0x1000 << 1) //GPIO_Pin_13 // Orange LED connects to PD13
#define LED_RED      ((uint16_t)0x1000 << 2) //GPIO_Pin_14 // Red LED connects to PD14
#define LED_BLUE     ((uint16_t)0x1000 << 3) //GPIO_Pin_15 // Blue LED connects to PD15

void System_Init(void);
void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_TIM2_Init(void);
void MX_TIM4_Init(void);
void Error_Handler(void);
void ExInt_Init(void);


#define Timer2_Init() MX_TIM2_Init()
#define Timer4_Init() MX_TIM4_Init()


void LED_Set(uint16_t led_id);
void LED_Clr(uint16_t led_id);
void LED_Inv(uint16_t led_id);

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim4;

#ifdef __cplusplus
}
#endif

#endif 



