/*
 ********************************************************************
 *                     STM32F4xx based on FreeRTOS
 ********************************************************************
 * FileName:    main_ex11_countingsem.c
 * Description: Using Counting Semaphore
 *              "#define configUSE_COUNTING_SEMAPHORES 1" is required
 ********************************************************************
 * Dr.Santi Nuratch
 * Embedded Computing and Control Laboratory | INC@KMUTT
 * 11 June, 2019
 * ****************************************************************** 
 */

#include "system_utils.h"
#include <math.h>

//!! Task Handle
TaskHandle_t TaskHandle_1, TaskHandle_2, TaskHandle_3;

//!! Used foe debugging
TaskStatus_t xTaskDetails;

//!! Semaphore Handle
SemaphoreHandle_t xSemaphore;

//!! Shared resource
#define BUFFER_LENGTH 256
float Buffer[BUFFER_LENGTH];

//!! Task1 will be resumed by ISR
static void Task1( void* pvParameters ) {
    static int32_t k = 0;
    for (;;) {

        //!! Waiting for ISR
        xSemaphoreTake( xSemaphore, portMAX_DELAY  );

        //!! Will be resumed by Task2
        vTaskSuspend( NULL );

        //!! Access the shared resource
        LED_Set( LED_RED ); //!! Press the Blue button when the RED-Led on
        for(int i=0; i<BUFFER_LENGTH; i++) {

            Buffer[i] = sin(2.0f*(22.0f/7.0f)*(float)i/(float)BUFFER_LENGTH);

            //!!
            //!! If the higher proprity task (Task3) taks the semaphore here, 
            //!! the priority of this task will be increased to the priority of higher proprity, the Task3
            //!!

            vTaskGetInfo( TaskHandle_1, &xTaskDetails, pdTRUE, eInvalid  );
            if( xTaskDetails.uxCurrentPriority > xTaskDetails.uxBasePriority ) {
                LED_Inv(LED_ORANGE);    //!! Break pont here
            }
            k=0; while(k++ < 50000);
            
        }
        
        LED_Clr( LED_RED );
        xSemaphoreGive( xSemaphore );

        //!! Simple debouncing
        vTaskDelay(200/portTICK_PERIOD_MS); 
	}
}

static void Task2( void* pvParameters ) {
    static int32_t k = 0;
    for(;;) {
        LED_Inv(LED_GREEN);
        k=0; while(k++ < 5000000);
        //!! Resume the Task1
        vTaskResume( TaskHandle_1 );
        vTaskDelay(100/portTICK_PERIOD_MS); 
    }
}


//!! Task3 waits for ISR
static void Task3( void* pvParameters ) {
    static int32_t k = 0;
    for(;;) {

        //!! Waits ISR
        vTaskSuspend( NULL );

        //!! Wait Mutex from Task1
        LED_Set(LED_BLUE);
        xSemaphoreTake( xSemaphore, portMAX_DELAY  );

        for(int i=0; i<BUFFER_LENGTH; i++) {
            Buffer[i] = sin(2.0f*(22.0f/7.0f)*(float)i/(float)BUFFER_LENGTH);
            k=0; while(k++ < 25000);
        }
        LED_Clr(LED_BLUE);
        xSemaphoreGive( xSemaphore );
    }
}

//!! Resume Task1
void EXTI0_IRQHandler( void ) {
    if( HAL_GPIO_ReadPin( GPIOA, GPIO_PIN_0 ) ) {
        //!! Resume the Task2
        BaseType_t xHigherPriorityTaskWoken = xTaskResumeFromISR( TaskHandle_3 );
        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
    }
    HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_0 );
}

int main(void) {

    //!! Initialize
    System_Init();

    //!! Create a mutex type semaphore
    xSemaphore = xSemaphoreCreateMutex();

    if( xSemaphore == NULL ) {
        //!! Failed to create the mutex semaphore
    }

    xSemaphoreGive( xSemaphore );

    //!! Create task
    xTaskCreate( Task1, "Task_1", 128, NULL, tskIDLE_PRIORITY+1, &TaskHandle_1 );
    xTaskCreate( Task2, "Task_2", 128, NULL, tskIDLE_PRIORITY+2, &TaskHandle_2 );
    xTaskCreate( Task3, "Task_3", 128, NULL, tskIDLE_PRIORITY+3, &TaskHandle_3 );
    
    //!! Initial External Interupt (User button)
    ExInt_Init();

    //!! Start scheduler
    vTaskStartScheduler();

    //!!
    while(1);
}
