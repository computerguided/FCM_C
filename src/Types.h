// -------------------------------------------------------------------------------------------------
//Types.h
// -------------------------------------------------------------------------------------------------

#ifndef TYPES_H
#define TYPES_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef char bool;

#define true	1
#define false	0

#ifndef NULL
#define NULL 0
#endif

//typedef unsigned long int uint32_t;

/*
 * The following type designates an unsigned integer type with the property that any valid pointer
 * to void can be converted to this type, then converted back to a pointer to void,
 * and the result will compare equal to the original pointer: uintptr_t
*/
typedef uintptr_t address_t;


#endif
