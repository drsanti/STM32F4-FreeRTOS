/*
 ********************************************************************
 *                     STM32F4xx based on FreeRTOS
 ********************************************************************
 * FileName:    main_ex02_tmrint.c
 * Description: Timer2 and Timer4 Interrupts
 ********************************************************************
 * Dr.Santi Nuratch
 * Embedded Computing and Control Laboratory | INC@KMUTT
 * 03 June, 2019
 * ****************************************************************** 
 */

#include "system_utils.h"

//!! Callbed by CPU
void TIM2_IRQHandler(void) {
    LED_Inv(LED_RED);
    HAL_TIM_IRQHandler(&htim2);
}

//!! Callbed by CPU
void TIM4_IRQHandler(void) {
    LED_Inv(LED_GREEN);
    HAL_TIM_IRQHandler(&htim4);
}


//!! Called by HAL_TIM_IRQHandler()
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if (htim->Instance == TIM2) {
    LED_Inv(LED_ORANGE);
  }
  else if (htim->Instance == TIM4) {
    LED_Inv(LED_BLUE);
  }
}

int main(void) {

    //!! Initialize
    System_Init();

    //!! Initial Timer2 and Timer4
    Timer2_Init();
    Timer4_Init();

    //!! Start scheduler
    vTaskStartScheduler();

    //!!
    while(1);
}

