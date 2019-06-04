/*
 ********************************************************************
 *                     STM32F4xx based on FreeRTOS
 ********************************************************************
 * FileName:    stm32f4xx_it.h
 * Description: Exported functions prototypes (ISR Handles)
 ********************************************************************
 * Dr.Santi Nuratch
 * Embedded Computing and Control Laboratory | INC@KMUTT
 * 03 June, 2019
 * ****************************************************************** 
 */

#ifndef __STM32F4xx_IT_H
#define __STM32F4xx_IT_H

#ifdef __cplusplus
 extern "C" {
#endif 

//!! Exported functions prototypes
void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void DebugMon_Handler(void);
void SysTick_Handler(void);
void EXTI0_IRQHandler(void);
void TIM2_IRQHandler(void);
void TIM4_IRQHandler(void);


#ifdef __cplusplus
}
#endif

#endif
