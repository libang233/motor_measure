#ifndef __PUBLIC_H_
#define __PUBLIC_H_

#include "stdlib.h"
#include "string.h"

/*------------------------------------------------------Def----------------------------------------------------*/

#ifndef u8 
typedef unsigned char u8;
#endif

#ifndef s8
typedef signed char s8;
#endif

#ifndef u16
typedef unsigned short u16;
#endif

#ifndef s16
typedef signed short s16;
#endif

#ifndef u32
typedef unsigned int u32;
#endif

#ifndef s32
typedef signed int s32;
#endif

#ifndef f32
typedef float f32;
#endif

#ifndef f64
typedef double f64;
#endif

#ifndef null
#define null 0
#endif

#ifndef bool
typedef enum
{
	false = 0,
	true = !false
}bool;
#endif


/*------------------------------------------------------Config-------------------------------------------------*/



/*-----------------------------------------------------BaudRate------------------------------------------------*/
#define BOUNDRATE 115200

#endif
