/*
 ********************************************************************
 *                     STM32F4xx based on FreeRTOS
 ********************************************************************
 * FileName:    main_ex00_init.c
 * Description: Getting Started
 ********************************************************************
 * Asst.Prof.Dr.Santi Nuratch
 * Embedded Computing and Control Laboratory | INC@KMUTT
 * Initial: 03 June 2019
 * Update:  23 Sebtember 2020
 * ******************************************************************
 */

#include "system_utils.h"

//!! Blue LED Blinking
static void Task1(void* pvParameters) {
	for (;;) {
		vTaskDelay(200/portTICK_PERIOD_MS);
        LED_Inv(LED_BLUE);
	}
}

int main(void) {

    //!! Initialize
    System_Init();

    //!! Create task
    xTaskCreate(Task1, "Task_1", 128, NULL, tskIDLE_PRIORITY+1, NULL);

    //!! Start scheduler
    vTaskStartScheduler();

    //!!
    while(1);
}
