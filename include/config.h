#ifndef __CONFIG_H__
#define __CONFIG_H__

#define XT2_FREQ                4000000
#define CPU_TICKS_IN_MSEC       (XT2_FREQ / 1000) 
#define CPU_TICKS_IN_USEC       (XT2_FREQ / 1000000) 

#define DELAY_HALL      100     // 100 microsec for hall sensor
#define DELAY_FUEL      500     // 500 microsec for fuel sensor
#define DELAY_PHYS      300     // 100 millisec for buttons and switchers

// uncomment in case of DEBUGGING/TESTING
#define __DEBUG__

// macros for delays

#define delay_m(msec)       (( __delay_cycles ( ( long long ) \
                                                  msec * CPU_TICKS_IN_MSEC ) ))
#define delay_u(usec)       (( __delay_cycles ( ( long long ) \
                                                  usec * CPU_TICKS_IN_USEC ) ))

#endif