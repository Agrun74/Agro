// @file: counts.h

#ifndef __COUNT_H__
#define __COUNT_H__

// тут будут инклуды

#define CONST           4612,5  // константа по формуле
#define DIV_CONST       100     // для представления Kв Kд как целые числа

float count_pulse(unsigned char kd, unsigned char kv, unsigned int hall, float fuel);

#endif