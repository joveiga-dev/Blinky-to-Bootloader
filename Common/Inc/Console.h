#ifndef USERIAL_H_
#define USERIAL_H_

#include <stdbool.h>
#include "Usart.h"


void Console_Init(void);
int Console_Printf( const char *format, ...);



#endif