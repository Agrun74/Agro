#include <msp430f5529.h>
#include "hw_init.h"


/**
 * @brief Init buttons
 * Инициализация клавиш управления подачей топлива (сброс/0.7--1.3)
**/
void button_init() {
    // 1 -> 0 P2.0
    P2DIR = 0x00;       
    P2IE = 0xFF;        
    P2IES = 0xFF;       // все клавиши по заднему фронту
    P2IFG = 0;
}

/**
 * @brief Init hall and fuel sensor
 * Инициализация обработки датчика Холла, счетчика топлива, 
 * тумблера 10-20 процентной подачи 
 * 
**/
void cntl_init() {
    P1OUT = 0;
    P1DIR = 0;
    P1IE = HALL + FUEL ;//+ SW10_20;
    //P1IE = SW10_20;
    P1IES = HALL + FUEL;                    // falling edge from sensors
    
    led_init();
                                          // after leds initialized we can 
                                          // turn on the right one
    if (P1IN & BIT7) 
    {    
        P1IES &= ~SW10_20;
        P7DIR = (P7DIR & ~(LED10 + LED20)) + LED10;    // switch on led20 of level is zero
        k_v = 0;
    } else 
    {   
        P1IES |= SW10_20;
        P7DIR = (P7DIR & ~(LED10 + LED20)) + LED20;
        k_v = 1;
    }
  
    P1IFG = 0;

    P7OUT &= ~( FAR34 + FAR12 + BENZDIR );    // P7OUT = BIT5 + BIT6 + BIT7; // ???
    P7DIR &= ~( FAR34 + FAR12 );
}

/**
 * @brief Init leds
 * Инициализация светодиодов для индикации состояния устройства: 
 * светодиод ПРОГРЕВ, светодиоды 10/20, светодиоды 0.7--1.3
**/
void led_init() {
    P6OUT = 0;
    P7OUT &= ~( LED10 + LED20 + BENZDIR );
    P6DIR = LED_RES;
    P7DIR = BENZDIR;
    P7DIR &= ~( LED10 + LED20 );  
}

/**
 * @brief Init timer B for fuel control
 * Инициализация таймера B для управления подачей топлива (управления форсунками)
 * Тактирование модуля таймера: SMCLK (4 МГц) с делителем 4 --> 1 МГц
**/
void timerb_init() {
    TB0CCTL3 = OUTMOD_2;                    // check it later (toggle/reset)
    TB0CCTL4 = OUTMOD_6;                    // check it later (toggle/set)
    set_timer_b(1250, 1150, 100);    // default values: 1000000/1250 = 800 об/мин
    TB0CTL = TBSSEL__SMCLK + ID__4 + MC__UPDOWN;
}

/**
 * @brief Set timer B timer counter values
 * Установка значений таймера для управления открытия форсунками
 * 
**/
void set_timer_b(unsigned int rate, unsigned int far34, unsigned int far12) {
    TB0R = rate;
    TB0CCR0 = rate;
    TB0CCR3 = far34;
    TB0CCR4 = far12;
}

/**
 * @brief Stop timer B count operations
 * Остановка таймера В ( из-за режима ПРОГРЕВ ) 
 * Константы не изменяются при этом 
**/
void timerb_stop() {
    TB0CTL &= ~MC__UPDOWN;        // mc_stop state set
}

void timerb_start() {
    TB0CTL |= MC__UPDOWN;
}

/**
 * @brief Init timer A for hall sensor detection
 * Инициализация таймера А для определения:
 * количества оборотов распредвала; счетчика поданного топлива
**/
void timera_init() {
    //
    TA0CTL = TASSEL__ACLK + MC__UP; // + TAIE;  // enable interrupt from timer

    TA0CCR0 = 32768;    // must be 1 second
}


/**
 * @brief Init MCU clock system, peipherials
 * Инициализация микроконтроллера:
 * - тактирование: MCLK = 4 MHz (XT2), SMCLK = 4 MHz (XT2), ACLK = 32 kHz (XT1)
 * - периферия: см. функции timera_init()   timerb_init()   cntl_init()     button_init()
**/
void init_mcu() {
  
    P5SEL |= 0x3C;                        // enable XT1 and XT2 pins for F5529
    //for F5529
    // use REFO for FLL and ACLK
    UCSCTL3 = ( UCSCTL3 & ~( SELREF_7 )) | (SELREF__REFOCLK );      // ����: 
    UCSCTL4 = ( UCSCTL4 & ~( SELA_7 + SELM_7 + SELS_7 )) \
      | (SELA__REFOCLK + SELM__XT2CLK + SELS__XT2CLK );
    
    do
    {
        UCSCTL7 &= ~( XT2OFFG + XT1LFOFFG + DCOFFG );
                                            // Clear XT2,XT1,DCO fault flags
        SFRIFG1 &= ~OFIFG;                      // Clear fault flags
    }   
    while ( SFRIFG1 & OFIFG );                   // Test oscillator fault flag
    
    button_init();
    cntl_init();
    timera_init();
    timerb_init();
    timerb_stop();
    
    __bis_SR_register(GIE);
}
