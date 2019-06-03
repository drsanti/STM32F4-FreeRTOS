#include "system_utils.h"

SemaphoreHandle_t xMutexSem;
char sharedBuffer[32];

static void Task1(void* pvParameters) {
    static int tskCnt = 0;
    for (;;) {

        LED_Set(LED_RED);
        if(xSemaphoreTake(xMutexSem, 100)) {
            LED_Clr(LED_RED);
            sprintf(sharedBuffer, "Task1: %d\r\n", tskCnt++);
            xSemaphoreGive( xMutexSem );
        }
        else {
            //!! could not access the shared buffer    
        }

        LED_Inv(LED_BLUE);
        vTaskDelay(200/portTICK_PERIOD_MS);
	}
}

static void Task2(void* pvParameters) {
    static int tskCnt = 0;
    for (;;) {
        LED_Set(LED_GREEN);
        if(xSemaphoreTake(xMutexSem, 100)) {
            LED_Clr(LED_GREEN);

            sprintf(sharedBuffer, "Task2: %d\r\n", tskCnt++);
            xSemaphoreGive( xMutexSem );
        }
        else {
            //!! could not access the shared buffer  
        }

        LED_Inv(LED_ORANGE);
        vTaskDelay(500/portTICK_PERIOD_MS);
	}
}

int main(void) {

    //!! Initialize
    System_Init();

    //!! Create a mutex type semaphore.
    xMutexSem = xSemaphoreCreateMutex();

    if( xMutexSem != NULL ) {

    }
    //!! Create tasks
    xTaskCreate(Task1, "Task_1", 128, NULL, tskIDLE_PRIORITY+1, NULL);
    xTaskCreate(Task2, "Task_2", 128, NULL, tskIDLE_PRIORITY+1, NULL);

    //!! Start scheduler
    vTaskStartScheduler();

    //!!
    while(1);
}
