#ifndef __SSP0_H
#define __SSP0_H
#include "sys.h"

void SPIx_SetSpeed(u8 SpeedSet);
void ssp0_init (void);
u8 ssp0_send (u8 outb);

#endif


