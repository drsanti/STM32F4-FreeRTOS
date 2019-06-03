#include "system_utils.h"

SemaphoreHandle_t xBinSemaphore;

static void Task1(void* pvParameters) {
    for (;;) {
        LED_Inv(LED_BLUE);
        xSemaphoreGive(xBinSemaphore);
        vTaskDelay(1000/portTICK_PERIOD_MS);
	}
}

static void Task2(void* pvParameters) {
    for (;;) {
        
        if(xSemaphoreTake(xBinSemaphore, portMAX_DELAY )) {
            LED_Set(LED_GREEN);    
            vTaskDelay(100/portTICK_PERIOD_MS);
            LED_Clr(LED_GREEN);    
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
