#include "system_utils.h"


int _tmr2_isr_flag = 0;


// 
void TIM2_IRQHandler(void) {
    _tmr2_isr_flag = 1;
    HAL_TIM_IRQHandler(&htim2);
}

typedef void (*tmrcallback)(unsigned short);


#define TIMER_CNT 5

unsigned short tmr_cnt[TIMER_CNT];  // Counter
unsigned short tmr_cmp[TIMER_CNT];  // Compare ()
unsigned char  tmr_rdy[TIMER_CNT];  // Ready

unsigned char  tmr_enb[TIMER_CNT];  // Enable

tmrcallback    tmr_cbk[TIMER_CNT];  // Callback functions


void SoftwareTimer( void ) {
    unsigned int i;
    for( i=0; i<TIMER_CNT; i++ ) {

        if( !tmr_enb[i] ) continue;
        tmr_cnt[i]++;
        if( tmr_cnt[i] >= tmr_cmp[i] ) {
            tmr_cnt[i] = 0;         // Reset counter
            tmr_rdy[i] = 1;         // Ready  
        }    
    }   
}

void SoftwareTimerInit(void) {
    for( int i=0; i<TIMER_CNT; i++ ) {
        tmr_cnt[i] = 0;  
        tmr_cmp[i] = (i+1)*10;  // Test
        tmr_rdy[i] = 0;   
        tmr_enb[i] = 1; 
    }
}

void SoftwareTimerCreate(int id, unsigned short period, tmrcallback cbk) {
    tmr_cnt[id] = 0;  
    tmr_cmp[id] = period;
    tmr_rdy[id] = 0;   
    tmr_enb[id] = 1; 
    tmr_cbk[id] = cbk;
}

int LEDs[] = { LED_RED, LED_GREEN, LED_BLUE, LED_ORANGE };
void TimerCallback(unsigned short tmr_id) {

    if(tmr_id > 3) {
        return;
    }
    
    LED_Inv(LEDs[tmr_id]);
}


int main(void) {
    System_Init();
    Timer2_Init();

    SoftwareTimerCreate( 0, 10, TimerCallback );
    SoftwareTimerCreate( 1, 16, TimerCallback );
    SoftwareTimerCreate( 2, 33, TimerCallback );
    SoftwareTimerCreate( 3, 45, TimerCallback );

    while(1) {
        if( _tmr2_isr_flag ) {
            _tmr2_isr_flag = 0; // reset
            SoftwareTimer();
        }

        for(int i=0; i<TIMER_CNT; i++) {
            if( tmr_rdy [i] ) {
                tmr_rdy[i] = 0;
                tmr_cbk[i](i);
            }
        }
    }
    return 1;
}