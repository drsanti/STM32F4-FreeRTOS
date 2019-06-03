/*
 ********************************************************************
 *                     STM32F4xx based on FreeRTOS
 ********************************************************************
 * FileName:    main_ex01_exint.c
 * Description: External Interrupt 
 ********************************************************************
 * Dr.Santi Nuratch
 * Embedded Computing and Control Laboratory | INC@KMUTT
 * 03 June, 2019
 * ****************************************************************** 
 */

#include "system_utils.h"

//!! Blinks Blue-LED
static void Task1(void* pvParameters) {
	for (;;) {
		vTaskDelay(200/portTICK_PERIOD_MS);
        LED_Inv(LED_BLUE);
	}
}

//!! EXTI0 Toggles Red-LED
void EXTI0_IRQHandler(void) {
    if( HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0 )) {
        LED_Inv(LED_RED);
    }
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

//!! Called by the HAL_GPIO_EXTI_IRQHandler()
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    LED_Inv(LED_GREEN);
}


int main(void) {

    //!! Initialize
    System_Init();

    //!! Initial External Interupt (User button)
    ExInt_Init();

    //!! Create task
    xTaskCreate(Task1, "Task_1", 128, NULL, tskIDLE_PRIORITY+1, NULL);

    //!! Start scheduler
    vTaskStartScheduler();

    //!!
    while(1);
}

