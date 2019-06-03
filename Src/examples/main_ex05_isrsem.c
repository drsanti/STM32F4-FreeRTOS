#include "system_utils.h"

SemaphoreHandle_t xBinSemaphore;

static void Task1(void* pvParameters) {
    for (;;) {
        LED_Inv(LED_BLUE);
        vTaskDelay(100/portTICK_PERIOD_MS);
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

void EXTI0_IRQHandler(void) {
    if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)) {
        LED_Set(LED_RED);
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xSemaphoreGiveFromISR( xBinSemaphore, &xHigherPriorityTaskWoken );
        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
        //portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
        LED_Clr(LED_RED);
    }
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

int main(void) {

    //!! Initialize
    System_Init();

    //!! Initial External Interupt (User button)
    ExInt_Init();

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
