/*
 * usart.c
 *
 * Created: 28.08.2020 09:06:29
 * Author : Gruppe 33 Simen, Espen, Emre
 */ 

#define set_bit(reg, bit) (reg |= (1<< bit))
#define clear_bit(reg, bit) (reg &= ~(1<< bit))
#define test_bit(reg, bit) (reg & (1 << bit))
#define loop_until_bit_is_set(reg, bit) while( !test_bit(reg, bit)) 

#include <avr/io.h>
#include <stdio.h>
#include "usart.h"

void uart_init(unsigned int ubbr){
	/* set baud rate */
	UBRR0H = (unsigned char) (ubbr >> 8);
	UBRR0L = (unsigned char) ubbr;
	
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0) | (1 << TXEN0);
	/* set frame format: 8data, 2stop bit */
	UCSR0C = (1<<URSEL0) | (1<<USBS0) | (3<<UCSZ00);
}

void uart_trans(unsigned char data){
	while(! (UCSR0A & (1<<UDRE0)));  /* Wait for empty transmit buffer*/
	
	UDR0 = data;
}

unsigned char uart_rec(void){
	while(!(UCSR0A & (1<<RXEN0)));  /* Wait for data to be received*/
	
	return UDR0;
}

void init_printuart(unsigned int ubbr){
	uart_init(ubbr);
	fdevopen(uart_trans, uart_rec);
}