// @file: hw_init.h

#ifndef __HW_INIT__
#define __HW_INIT__

// Port1
#define HALL    BIT0    // p1.1
#define FUEL    BIT1    // p1.0
#define SW10_20 BIT7    // p1.7

// Port2
#define BUT_RES BIT0    // p2.0

// Port6
#define LED_RES BIT0    // p6.0 

// Port7 
#define LED10   BIT0    // p7.0
#define LED20   BIT1    // p7.1
#define FAR34   BIT5    // p7.5
#define FAR12   BIT6    // p7.6
#define BENZDIR BIT7    // p7.7

// Functions 
// документация к функциям приведена в файле hw_init.c

extern unsigned char k_v;

void button_init();

void cntl_init();

void led_init();

void timerb_init();

void timera_init();

void timerb_stop();
void timerb_start();
void set_timer_b(unsigned int rate, 
                    unsigned int far34, 
                    unsigned int far12);

void init_mcu();

# endif