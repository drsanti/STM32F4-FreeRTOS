/*
 ********************************************************************
 *                     STM32F4xx based on FreeRTOS
 ********************************************************************
 * FileName:    main_ex03_mutex.c
 * Description: Mutex Semaphore
 ********************************************************************
 * Dr.Santi Nuratch
 * Embedded Computing and Control Laboratory | INC@KMUTT
 * 03 June, 2019
 * ****************************************************************** 
 */

#include "system_utils.h"


//!! Mutex
SemaphoreHandle_t xMutexSem;

//!! Shared variable
char sharedBuffer[32];

//!! Takes and gives mutex
static void Task1( void* pvParameters ) {
    static int tskCnt = 0;
    for (;;) {

        LED_Set( LED_RED );

        if( xSemaphoreTake(xMutexSem, 100) ) {
            LED_Clr( LED_RED );
            sprintf( sharedBuffer, "Task1: %d\r\n", tskCnt++ );
            xSemaphoreGive( xMutexSem );
        }
        else {
            //!! could not access the shared buffer    
        }

        LED_Inv( LED_BLUE );
        vTaskDelay( 200/portTICK_PERIOD_MS );
	}
}

//!! Takes and gives mutex
static void Task2( void* pvParameters ) {
    static int tskCnt = 0;
    for (;;) {

        LED_Set( LED_GREEN );

        if( xSemaphoreTake( xMutexSem, 100 ) ) {
            LED_Clr( LED_GREEN );

            sprintf( sharedBuffer, "Task2: %d\r\n", tskCnt++ );
            xSemaphoreGive( xMutexSem );  //!! Comment this line, and check what happen?
        }
        else {
            //!! could not access the shared buffer  
        }

        LED_Inv( LED_ORANGE );
        vTaskDelay( 500/portTICK_PERIOD_MS );
	}
}

int main(void) {

    //!! Initialize
    System_Init();

    //!! Create a mutex type semaphore.
    xMutexSem = xSemaphoreCreateMutex();

    if( xMutexSem == NULL ) {
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
