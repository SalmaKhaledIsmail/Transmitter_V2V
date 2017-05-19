/*
 * IEEE802_11_Common_Variables.h
 *
 *  Created on: Feb 7, 2017
 *      Author: salma
 */

#ifndef IEEE802_11_COMMON_VARIABLES_H_
#define IEEE802_11_COMMON_VARIABLES_H_

typedef unsigned char uint8;
typedef signed char sint8;
typedef unsigned short uint16;
typedef signed short sint16;
typedef unsigned long uint32;
typedef signed long sint32;
typedef unsigned long longuint64;
typedef signed long longsint64;
typedef float float32;
typedef double float64;
typedef int* ptoi;
typedef int bool;
#define true              1
#define false             0
#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })
#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

#endif /* IEEE802_11_COMMON_VARIABLES_H_ */
