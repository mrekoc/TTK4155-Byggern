#ifndef OLED_333
#define OLED_333

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/pgmspace.h>


void oled_write_command(int);
void oled_write_data(int);
void oled_init(void);
void oled_goto_column(int);
void oled_fill_page(int);
void oled_pos(int, int);
void oled_print_letter(char, int);
void oled_clear_screen(void);
void oled_print_sec(char*, int);


#endif 
