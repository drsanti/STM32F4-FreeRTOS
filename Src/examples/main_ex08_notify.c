/*
 ********************************************************************
 *                     STM32F4xx based on FreeRTOS
 ********************************************************************
 * FileName:    main_ex08_notify.c
 * Description: Using xTaskNotify(), xTaskNotifyWait() and
 *              xTaskNotifyFromISR()
 ********************************************************************
 * Dr.Santi Nuratch
 * Embedded Computing and Control Laboratory | INC@KMUTT
 * 03 June, 2019
 * ****************************************************************** 
 */

#include "system_utils.h"

//!! Task Handles
TaskHandle_t TaskHandle_1, TaskHandle_2, TaskHandle_3;

//!! LEDs
uint16_t LEDs[] = { LED_RED, LED_GREEN, LED_ORANGE, LED_BLUE };

//!! Sends notify to Task2
static void Task1( void* pvParameters ) {
    int i = 0;
    for (;;) {
        xTaskNotify( TaskHandle_2, LEDs[(i++)%2], eSetBits );
        vTaskDelay( 1000/portTICK_PERIOD_MS );
	}
}

//!! Waits notify from Task1
static void Task2( void* pvParameters ) {
    uint32_t notificationValue = 0;
    for (;;) {
        xTaskNotifyWait(
            0x00,           //!! Don't clear any notification bits on entry.
            0xffffffffUL,   //!! Reset the notification value to 0 on exit
            &notificationValue, portMAX_DELAY );
        
        LED_Inv( notificationValue );
	}
}


//!! Waits notify from ISR
static void Task3( void* pvParameters ) {
    uint32_t notificationValue = 0;
    for (;;) {
        xTaskNotifyWait(
            0x00,           //!! Don't clear any notification bits on entry.
            0xffffffffUL,   //!! Reset the notification value to 0 on exit
            &notificationValue, portMAX_DELAY );
        
        LED_Inv( notificationValue );
	}
}

//!! Sends notify to Task3
void EXTI0_IRQHandler( void ) {
    static int i = 0;
    if( HAL_GPIO_ReadPin( GPIOA, GPIO_PIN_0 ) ) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xTaskNotifyFromISR( TaskHandle_2, LEDs[2+(i++)%2], eSetBits, &xHigherPriorityTaskWoken );
        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
    }
    HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_0 );
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
