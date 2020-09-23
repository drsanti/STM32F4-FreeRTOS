/*
 ********************************************************************
 *                     STM32F4xx based on FreeRTOS
 ********************************************************************
 * FileName:    main_ex04_binsem.c
 * Description: Binary Semaphore
 ********************************************************************
 * Asst.Prof.Dr.Santi Nuratch
 * Embedded Computing and Control Laboratory | INC@KMUTT
 * Initial: 03 June 2019
 * Update:  23 Sebtember 2020
 * ******************************************************************
 */

#include "system_utils.h"

//!! Semaphore
SemaphoreHandle_t xBinSemaphore;

//!! Gives semaphore to Task2
static void Task1(void* pvParameters) {
    for (;;) {
        LED_Inv( LED_BLUE );
        xSemaphoreGive( xBinSemaphore );
        vTaskDelay( 1000/portTICK_PERIOD_MS );
	}
}

//!! Waits semaphore from Task1
static void Task2(void* pvParameters) {
    for (;;) {

        if( xSemaphoreTake( xBinSemaphore, portMAX_DELAY ) ) {
            LED_Set( LED_GREEN );
            vTaskDelay( 100/portTICK_PERIOD_MS );
            LED_Clr( LED_GREEN );
        }
	}
}

int main(void) {

    //!! Initialize
    System_Init();

    //!! Create a mutex type semaphore.
    xBinSemaphore = xSemaphoreCreateBinary();

    if( xBinSemaphore == NULL ) {
        //!! insufficient heap available for the semaphore
    }
    //!! Create tasks
    xTaskCreate(Task1, "Task_1", 128, NULL, tskIDLE_PRIORITY+1, NULL);
    xTaskCreate(Task2, "Task_2", 128, NULL, tskIDLE_PRIORITY+1, NULL);

    //!! Start scheduler
    vTaskStartScheduler();

    //!!
    while(1);
}
