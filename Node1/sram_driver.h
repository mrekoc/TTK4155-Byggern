#include <avr/io.h>
#include <stdio.h>

#ifndef SRAM_33
#define SRAM_33

void sram_init(void);
void sram_write(int, int);

#endif