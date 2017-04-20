#include <msp430f5529.h>
#include "work.h"

#include "config.h"
#include "hw_init.h"

// пока просто заглушка, потом может быть что-то будет тут
void routine() {
    #ifdef __DEBUG__

        while(1) 
        {     
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
        }
    
    #else
    
        while(1)
        {
            _NOP(); _NOP();
        }
    
    #endif
        
}