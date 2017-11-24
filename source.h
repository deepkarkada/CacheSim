/*
 * source.h
 *
 *  Created on: Nov 18, 2017
 *      Author: deep
 */

#ifndef SOURCE_H_
#define SOURCE_H_

// Library initializations
#pragma once

//#include <string.h>
//#include <stdlib.h>
#include <cstdlib>
//#include <stdint.h>
#include <cstdio>
#include <math.h>
//#include <stddef.h>
//#include <errno.h>

#pragma warning (disable : 4996)

#define LOG2(x) (((int) log((double)(x)) / (int)log(2) ))

/*************************************************/
/*Variable intitializations*/
/*************************************************/
static FILE *fp;

/************************************************/
/***Function initializations ********************/
/*************************************************/

int* address_calc(int, int, int);

/*************************************************/

#endif /* SOURCE_H_ */
