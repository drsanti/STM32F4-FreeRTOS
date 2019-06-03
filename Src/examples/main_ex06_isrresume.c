#include "system_utils.h"

TaskHandle_t TaskHandle_1, TaskHandle_2;

static void Task1(void* pvParameters) {
    for (;;) {
        LED_Inv(LED_GREEN);
        vTaskDelay(100/portTICK_PERIOD_MS);
	}
}

static void Task2(void* pvParameters) {
    for (;;) {
        vTaskSuspend(NULL);
        LED_Inv(LED_RED);
	}
}


void EXTI0_IRQHandler(void) {
    if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)) {
        BaseType_t isYieldRequired = xTaskResumeFromISR(TaskHandle_2);
        portYIELD_FROM_ISR( isYieldRequired );
    }
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
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
