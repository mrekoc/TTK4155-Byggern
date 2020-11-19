#include <avr/io.h>
#include <stdio.h>

#ifndef UART_33
#define UART_33



void uart_init(unsigned int ubbr);
void uart_trans(unsigned char data);
unsigned char uart_rec(void);
void init_printuart(unsigned int ubbr);

#endif