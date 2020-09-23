/*
 ********************************************************************
 *                     STM32F4xx based on FreeRTOS
 ********************************************************************
 * FileName:    main_ex06_isrresume.c
 * Description: Using portYIELD_FROM_ISR
 ********************************************************************
 * Asst.Prof.Dr.Santi Nuratch
 * Embedded Computing and Control Laboratory | INC@KMUTT
 * Initial: 03 June 2019
 * Update:  23 Sebtember 2020
 * ******************************************************************
 */

#include "system_utils.h"

//!! Task handles
TaskHandle_t TaskHandle_1, TaskHandle_2;

//!! Blinks Green-LED
static void Task1( void* pvParameters ) {
    for (;;) {
        LED_Inv( LED_GREEN );
        vTaskDelay( 100/portTICK_PERIOD_MS );
	}
}

//!! Will be resumed by ISR
static void Task2( void* pvParameters ) {
    for (;;) {
        vTaskSuspend( NULL );
        LED_Inv( LED_RED );
	}
}

//!! Resumes the Task2
void EXTI0_IRQHandler( void ) {
    if( HAL_GPIO_ReadPin( GPIOA, GPIO_PIN_0 ) ) {
        BaseType_t isYieldRequired = xTaskResumeFromISR( TaskHandle_2 );
        portYIELD_FROM_ISR( isYieldRequired );
    }
    HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_0 );
}


int main(void) {

    //!! Initialize
    System_Init();

    //!! Initial External Interupt (User button)
    ExInt_Init();

    //!! Create tasks
    xTaskCreate(Task1, "Task_1", 128, NULL, tskIDLE_PRIORITY+1, &TaskHandle_1);
    xTaskCreate(Task2, "Task_2", 128, NULL, tskIDLE_PRIORITY+1, &TaskHandle_2);

    //!! Start scheduler
    vTaskStartScheduler();

    //!!
    while(1);
}
