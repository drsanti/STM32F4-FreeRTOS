/*
 ********************************************************************
 *                     STM32F4xx based on FreeRTOS
 ********************************************************************
 * FileName:    main_ex11_countingsem.c
 * Description: Using Counting Semaphore
 *              "#define configUSE_COUNTING_SEMAPHORES 1" is required
 ********************************************************************
 * Asst.Prof.Dr.Santi Nuratch
 * Embedded Computing and Control Laboratory | INC@KMUTT
 * Initial: 03 June 2019
 * Update:  23 Sebtember 2020
 * ******************************************************************
 */

#include "system_utils.h"

//!! Task Handle
TaskHandle_t TaskHandle_1, TaskHandle_2;

//!! Semaphore Handle
SemaphoreHandle_t xSemaphore;
#define SEM_COUNT 10

//!! LEDs
// static uint16_t LEDs[] = { LED_BLUE, LED_GREEN, LED_ORANGE, LED_RED };
// static uint16_t LedIdx = 0;
// #define NEXT_LED() ((LedIdx+1)%4)


//!! Task1 will be resumed by ISR
static void Task1( void* pvParameters ) {
    static int32_t k = 0;
    for (;;) {

        //!! Waiting for ISR

        xSemaphoreTake( xSemaphore, portMAX_DELAY  );

        //!! Flash the RED-LED
        LED_Set( LED_RED );
        k=0; while(k++ < 50000);
        LED_Clr( LED_RED );

        //!! Simple debouncing
        vTaskDelay(200/portTICK_PERIOD_MS);
	}
}

static void Task2( void* pvParameters ) {
    UBaseType_t semCount;
    for(;;) {
        semCount = uxSemaphoreGetCount( xSemaphore );
        if( semCount > 2 ) {
            LED_Set(LED_BLUE);
        }
        else {
            LED_Clr(LED_BLUE);
        }
        taskYIELD();
    }
}

//!! Resume Task1
void EXTI0_IRQHandler( void ) {
    if( HAL_GPIO_ReadPin( GPIOA, GPIO_PIN_0 ) ) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xSemaphoreGiveFromISR( xSemaphore, &xHigherPriorityTaskWoken );

        //!! If no bouncing signal, give it more by uncomment/add the line below
        xSemaphoreGiveFromISR( xSemaphore, &xHigherPriorityTaskWoken );

        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
    }
    HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_0 );
}

int main(void) {

    //!! Initialize
    System_Init();

    //!! Create a counting semaphore that has a maximum count of 10 and an initial count of 0
    xSemaphore = xSemaphoreCreateCounting( SEM_COUNT, 0 ); //!! 0 means no semaphore is given(available)

    if( xSemaphore == NULL ) {
        //!! Failed to create the semaphore
    }

    //!! Create task
    xTaskCreate( Task1, "Task_1", 128, NULL, tskIDLE_PRIORITY+1, &TaskHandle_1 );
    xTaskCreate( Task2, "Task_2", 128, NULL, tskIDLE_PRIORITY+1, &TaskHandle_2 );

    //!! Initial External Interupt (User button)
    ExInt_Init();

    //!! Start scheduler
    vTaskStartScheduler();

    //!!
    while(1);
}
