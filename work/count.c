#include "count.h"


/**
 * @brief Count pulse period 
 * Подсчет периода открытия форсунки 
 * 
 * @note сдвиг на значение k_d + 1, так как мы не учитывали что 
 * частота распредвала в 2 раза меньше
**/
float count_pulse(unsigned char kd, unsigned char kv, unsigned int hall, float fuel) {
    if ( hall == 0 ) {
        return  1.0;
    }
    return ( CONST * ( kd << ( kv + 1 ) ) * fuel / ( hall * DIV_CONST ) );
}