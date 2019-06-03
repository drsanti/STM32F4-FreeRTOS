#include "system_utils.h"

TaskHandle_t TaskHandle_1, TaskHandle_2, TaskHandle_3;

//!! Sends notification to Task2
static void Task1(void* pvParameters) {
    for (;;) {
        LED_Inv(LED_GREEN);
        xTaskNotifyGive(TaskHandle_2);
        vTaskDelay(100/portTICK_PERIOD_MS);
	}
}

//!! Waits notification from Task1
static void Task2(void* pvParameters) {
    uint32_t notificationCount = 0;
    for (;;) {
        notificationCount = ulTaskNotifyTake(pdFALSE, portMAX_DELAY);
        //!! pdFALSE means the ulTaskNotifyTake not resets the notification counter

        if(notificationCount > 0) {
            LED_Inv(LED_RED);
        }

        //!! Add longer delay to allow the notification counter more than 1
        vTaskDelay(2000/portTICK_PERIOD_MS);
        if( notificationCount > 1 ) {
            LED_Inv(LED_BLUE);    //!! Break on this line and check the  notificationCount
        }
	}
}


//!! Waits notification from ISR
static void Task3(void* pvParameters) {
    uint32_t notificationCount = 0;
    for (;;) {
        notificationCount = ulTaskNotifyTake(pdFALSE, portMAX_DELAY);
        //!! pdFALSE means the ulTaskNotifyTake not resets the notification counter

        if(notificationCount > 0) {
            LED_Inv(LED_ORANGE);
        }
	}
}

//!! Sends notification to Task3
void EXTI0_IRQHandler(void) {
    if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)) {
        
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        vTaskNotifyGiveFromISR( TaskHandle_3, &xHigherPriorityTaskWoken );
        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
    }
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

int main(void) {

    //!! Initialize
    System_Init();

    //!! Create tasks
    xTaskCreate(Task1, "Task_1", 128, NULL, tskIDLE_PRIORITY+1, &TaskHandle_1);
    xTaskCreate(Task2, "Task_2", 128, NULL, tskIDLE_PRIORITY+1, &TaskHandle_2);
    xTaskCreate(Task3, "Task_3", 128, NULL, tskIDLE_PRIORITY+1, &TaskHandle_3);

    //!! Initial External Interupt (User button)
    ExInt_Init();

    //!! Start scheduler
    vTaskStartScheduler();

    //!!
    while(1);
}
