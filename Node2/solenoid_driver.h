#ifndef SOLENOID_DRIVER_H
#define SOLENOID_DRIVER_H

#include "sam.h"

void solenoid_init(void);
void solenoid_drive(uint8_t);

#endif