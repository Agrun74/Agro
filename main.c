#include <msp430f5529.h>

#include "config.h"
#include "hw_init.h"
#include "count.h"
#include "work.h"

unsigned char k_d = 0;                     // коэфициент подачи топлива

unsigned char k_v;                     // дозировка 10-20 процентов

unsigned int hall_freq = 0;                // частота датчика холла (это распредвал, т.е. для коленвала нужно умножить на 2)

float fuel_freq = 2;                       // счетчик топлива 

float period = 0.0;                        // длительность импульса (скорректированная)

unsigned int prev_hall_val = 0;
unsigned int prev_fuel_val = 0;

/*TODO:
1) ACLK <-- 32 kHz; SMCLK <-- 4 MHz
2) Timer A <-- ACLK
3) Timer B <-- SMCLK
*/


int main( void )
{
    // Stop watchdog timer to prevent time out reset
    WDTCTL = WDTPW + WDTHOLD;
    
    init_mcu();     // функция инициализации MSP430 с захардкоженными значениями
    
    /* В зависимости от того, определен ли макрос __DEBUG__, будет вызываться функция
       routine() определенная в файле work.c (ifdef) 
       Конфигурирование данного макроса осуществляется в файле include/config.h:15
    */
    routine();

    
  return 0;
}


/**
 * @brief Port2 interrupt vector routine
 * Обработчик нажатий кнопок с защитой от дребезга
 * 
**/
#pragma vector = PORT2_VECTOR
__interrupt void Port2_ISR() {
    // 
    delay_m(DELAY_PHYS);
    switch( P2IFG ) {
        case BIT0: { 
            timerb_stop();
            P6DIR = LED_RES;       // включаем светодиод сброса
            P7OUT &= ~BENZDIR;      // выключить бензонасос
            break; 
        }
        case BIT1: { 
            P6DIR = BIT1;
            P7OUT |= BENZDIR;
            timerb_start();
            k_d = 7;
            break; 
        }    // зажигаем/гасим светодиоды
        case BIT2: { 
            P6DIR = BIT2; 
            P7OUT |= BENZDIR;
            timerb_start();
            k_d = 8;
            break; 
        }
        case BIT3: { 
            P6DIR = BIT3;
            P7OUT |= BENZDIR;
            timerb_start();
            k_d = 9; 
            break; 
        }
        case BIT4: { 
            P6DIR = BIT4;
            P7OUT |= BENZDIR;
            timerb_start();
            k_d = 10; 
            break; 
        }
        case BIT5: { 
            P6DIR = BIT5;
            P7OUT |= BENZDIR;
            timerb_start();
            k_d = 11; 
            break; 
        }
        case BIT6: { 
            P6DIR = BIT6;
            P7OUT |= BENZDIR;
            timerb_start();
            k_d = 12; 
            break; 
        }
        case BIT7: { 
            P6DIR = BIT7; 
            P7OUT |= BENZDIR;
            timerb_start();
            k_d = 13;
            break; 
        }
    }
    P2IFG = 0;
    return;
}
                                                
/**
 * @brief Обработка событий: 
 * - датчик холла
 * - счетчик топлива
 * - перещелкнут тумблер 10/20
 **/
#pragma vector = PORT1_VECTOR
__interrupt void Port1_ISR() {

    if (P1IFG & HALL)  
    {   
        int tmp = TA0CCR1;
        
        if ( tmp == prev_hall_val ) { hall_freq = 1; }
        else 
        {
            hall_freq = TA0CCR0 / ( tmp > prev_hall_val ? \
                                  tmp - prev_hall_val : \
                                  prev_hall_val - tmp );
                                // сложная система определения частоты вращения
                                // распредвала
        }
        prev_hall_val = tmp;
        period = count_pulse( k_d, k_v, hall_freq, fuel_freq );
        // set_timerb();      // TODO: вычислить и передать аргументы в функцию
    } // end-handling HALL
    
    if ( P1IFG & FUEL ) 
    {   
        int tmp = TA0CCR1;

        if ( tmp == prev_fuel_val ) { fuel_freq = 1; }
        else 
        {
            fuel_freq = TA0CCR0 / ( float )( tmp > prev_fuel_val ? \
                                            tmp - prev_fuel_val : \
                                            prev_fuel_val - tmp ) ;    
                                    // сложная система определения времени, частоты 
                                    // (по времени, которое прошло с момента) последнего замера
                                    // 32768 / (TA0CCR1 - prev_ta0ccr1) - частота подачи 
        }
        prev_fuel_val = tmp; // todo: 
    }     // end handling FUEL                           
    
  /*  if ( P1IFG & SW10_20 ) {  
        delay_m(50);
        k_v ^= BIT0;               // change the state of k_v ( 0 or 1 )
        P1IES ^= SW10_20;           // set for another toggle take place
        P7DIR ^= LED10 + LED20;     // revert led states
    }     // end handling SWITCH
  */  
    
    P1IFG = 0;
}
                                          
// 1 second counted
#pragma vector = TIMER0_A1_VECTOR
__interrupt void TimerA_ISR()
{
    // period = count_pulse( k_d, k_v, hall_freq, fuel_freq );
    // hall_freq = 0;
    // todo: with period later - set it as a duty cycle
}