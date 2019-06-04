/*
 ********************************************************************
 *                     STM32F4xx based on FreeRTOS
 ********************************************************************
 * FileName:    main_ex10_timer.c
 * Description: Using software timer
 ********************************************************************
 * Dr.Santi Nuratch
 * Embedded Computing and Control Laboratory | INC@KMUTT
 * 03 June, 2019
 * ****************************************************************** 
 */

#include "system_utils.h"

//!! Task Handle
TaskHandle_t TaskHandle_1;

//!! LEDs
uint16_t LEDs[] = { LED_BLUE, LED_GREEN, LED_ORANGE, LED_RED };

//!! Number of timers
#define NUM_TIMERS 4

//!! Array of timer handles
TimerHandle_t xTimers[ NUM_TIMERS ];

//!! Used to control start/stop of timers, see in Task1
static int32_t enable_timers[NUM_TIMERS];

//!! Timer callback function
void vTimerCallback( TimerHandle_t xTimer ) {

    //!! number of times this timer has expired is saved as the timer's ID
    uint32_t ulCount = ( uint32_t ) pvTimerGetTimerID( xTimer );

    if(1) {
        LED_Inv(LEDs[ulCount]);
    }
    else {
        ulCount++;
        vTimerSetTimerID( xTimer, ( void * ) ulCount );
    }
}

//!! Waits ISR
static void Task1( void* pvParameters ) {
    
    static int16_t itmer_index = 0;
    static int16_t bypass = 0;

    for (;;) {

        //!! Will be resumed by ISR
        vTaskSuspend( NULL );

        if( !bypass ) {
            enable_timers[itmer_index] ^= 0x1;      //!! Toggle
            if(enable_timers[itmer_index] & 0x1) {  //!! Check
                xTimerStart( xTimers[itmer_index],  portMAX_DELAY);   
            }
            else {
                xTimerStop( xTimers[itmer_index],  portMAX_DELAY);
                LED_Clr(LEDs[itmer_index]);
            }
            //!! Next timer
            itmer_index = (itmer_index+1)%NUM_TIMERS;

            //!! by pass flag
            bypass = 1;
        }
        //!! Simple debouncing
        vTaskDelay(200/portTICK_PERIOD_MS); 
        bypass = 0;
	}
}

//!! Resume Task1
void EXTI0_IRQHandler( void ) {
    if( HAL_GPIO_ReadPin( GPIOA, GPIO_PIN_0 ) ) {
        BaseType_t isYieldRequired = xTaskResumeFromISR( TaskHandle_1 );
        portYIELD_FROM_ISR( isYieldRequired );
    }
    HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_0 );
}

int main(void) {

    //!! Initialize
    System_Init();

    //!! Create timers
    for( int x = 0; x < NUM_TIMERS; x++ ) {
        xTimers[ x ] = xTimerCreate(
            "Timer",
            ( 50 * x ) + 100,           //!! timer period in ticks
            pdTRUE,                     //!! auto-reload
            (void *)x,//( void * ) 0,   //!! number of times the timer has expired
            vTimerCallback              //!! callback when it expires
        );

        if( xTimers[ x ] == NULL ) {
            //!! The timer was not created
        }
        else {
            if( xTimerStart( xTimers[ x ], 0 ) != pdPASS ) {
                //!! The timer could not be set into the Active state 
            }
        } 

        //!! Enable timer. This variable is used in Task1
        enable_timers[x] = 1; 
    }
    
    //!! Create task
    xTaskCreate( Task1, "Task_1", 128, NULL, tskIDLE_PRIORITY+1, &TaskHandle_1 );
    
    //!! Initial External Interupt (User button)
    ExInt_Init();

    //!! Start scheduler
    vTaskStartScheduler();

    //!!
    while(1);
}
