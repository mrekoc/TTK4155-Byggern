#ifndef MENY_NY_33
#define MENY_NY_33

#include "joystick_driver.h"
#include <avr/io.h>
#include <stdio.h>

typedef void(*funksjonspeker)(void);

typedef struct node{
	struct node* next;
	struct node* prev;
	char* val;
	funksjonspeker selectedFunc;
	struct node* parent;
	struct node* child;
} node_t;

node_t* meny_insert(char[], funksjonspeker, node_t*);
void meny_print(node_t**);
void meny_print_oled(node_t*, int);
void meny_navigation(node_t**, int);
void meny_set_child(node_t*, node_t*);
void meny_set_order(node_t*, node_t*, node_t*);
void meny_fillScreen();
void meny_init(node_t**);
meny_clear_letter(int, int, int);



#endif