/*
 * gruppe33_ex1.c
 *
 * Created: 28.08.2020 09:06:29
 * Author : Gruppe 33 Simen, Espen, Emre
 */ 

#include "global.h" //Global variables
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "usart.h"
#include "sram_test.h"
#include "sram_driver.h"
#include "adc_driver.h"
#include "joystick_driver.h"
#include "slider_driver.h"
#include "OLED_driver.h"
#include "meny.h"
#include "MCP2515_driver.h"
#include "CAN_driver.h"
#include "game_driver.h"


int main(void){
	game_node_1();
}



 void ex1(){
 /*--------------Initializing variables---------------*/
 	DDRA = 0b01;  // PORTA bit 1 skal være output
 	char data='i'; //Initializing data to be received over the UART line
 	
 /*--------------Initializing functions---------------*/
 	init_printuart(MYUBBR);
 	
 	while(1){
 	//-----------Test program to toggle the pins-----------------------
 		PORTA |= (1<< 0 ); //set PA0 high
 		_delay_ms(500);
 		PORTA &= ~(1<<0); // set PA0 low
 		_delay_ms(500);
 	//----------Test program that sends data from atmega to pc---------
 		printf("Hello world");
 	//---------Test program that sends data from pc to atmega-----------
 		uart_trans('H');
 		_delay_ms(1);
 		uart_rec();
 	}
 }
 
 void ex2(){
 /*--------------Initializing variables---------------*/
 	volatile char *ext_ram = (char *) 0x00; // Start address for the SRAM
 
 /*--------------Initializing functions---------------*/
 	init_printuart(MYUBBR);
 	sram_init();
 	while (1)
 	{
 		SRAM_test();
 	}
 }
 
 void ex3(){
 /*--------------Initializing variables---------------*/
 	can_message PCB_information;
 	PCB_information.data[0] = 0x0000;
 	PCB_information.data[1] = 0x0000;
 	PCB_information.data[2] = 0x0000;
 	PCB_information.data[3]= 0x0000;
 	PCB_information.data[4]= 0x0000;
 	PCB_information.data[5]= 0x0000;
 	PCB_information.length=6;
 	PCB_information.ID=2;
 
 /*--------------Initializing functions---------------*/
 	init_printuart(MYUBBR);
 	sram_init();
 	adc_init();
 
 	while (1)
 	{
 		PCB_information.data[0] = get_direction();
 		PCB_information.data[1] = get_x_pos_joy();
 		PCB_information.data[2] = get_y_pos_joy();
 		PCB_information.data[3]= slider_get_left_slider();
 		PCB_information.data[4]= slider_get_right_slider();
 		PCB_information.data[5] = slider_get_right_button();
 		printf("################################################\n\r");
 		printf("JOYSTICK DIRECTION: %d\n\r", PCB_information.data[0]);
 		printf("JOYSTICK X POSITION: %d\n\r", PCB_information.data[1]);
  		printf("JOYSTICK Y POSITION: %d\n\r", PCB_information.data[2]);
  		printf("LEFT SLIDER POSITION: %d\n\r", PCB_information.data[3]);
  		printf("RIGHT SLIDER POSITION: %d\n\r", PCB_information.data[4]);
 		 printf("RIGHT SLIDER BUTTON: %d\n\r", PCB_information.data[5]);
 		printf("################################################\n\r");
 		_delay_ms(500);
 	}
 }
 
 void ex4() {
 /*--------------Initializing variables---------------*/
   	node_t* head = NULL;
   	node_t* currentNode = NULL;
   	int font_size=5;
 
 /*--------------Initializing functions---------------*/
 	init_printuart(MYUBBR);
 	sram_init();
 	adc_init();
 	oled_init();
 	meny_init(&head);
 	
 	while (1)
 	{
 		meny_navigation(&head, font_size);
 	}
 }
 
 void ex5() {
 	/****Working with MODE_LOOPBACK in the MCP2515 Driver ****/
 	/*--------------Initializing variables---------------*/
 	can_message sentMessage;
 	sentMessage.data[0]='h';
 	sentMessage.data[1]='e';
 	sentMessage.data[2]='l';
 	sentMessage.data[3]='l';
 	sentMessage.data[4]='o';
 	sentMessage.data[5]='o';
 	sentMessage.ID=6;
 	sentMessage.length=6;
 	
 	////
 	
 	can_message receivedMessage;
 	receivedMessage.ID=7;
 	receivedMessage.data[0]='i';
 	receivedMessage.data[1]='i';
 	receivedMessage.length=2;
 	
 	/*--------------Initializing functions---------------*/
 	init_printuart(MYUBBR);
 	sram_init();
 	adc_init();
 	CAN_init();
 	
 	while (1)
 	{
 		
 		printf("///////////////New Iteration/////////////////// \n\r");
 		
 		//------sending messages over CAN---------------//
 		CAN_send(&sentMessage);
 		printf("Sent message NODE1 length:%d\n\r",sentMessage.length);
 		printf("Sent message NODE1 ID:%d\n\r",sentMessage.ID);
 		for(int i=0;i<sentMessage.length;i++){
 		printf("Sent message NODE1:, data[%d]=%d\n\r",i,sentMessage.data[i]);}
 		printf("//////////////////////////////////////////////////////////////// \n\r");
 		
 		_delay_ms(10);
 		
 		//------receiving messages over CAN---------------//
 		//CAN_receive(&receivedMessage);
 		CAN_interrupt_handler(&receivedMessage);	
 		printf("Received message NODE1 length:%d\n\r",receivedMessage.length);
 		printf("Received message NODE1 ID:%d\n\r",receivedMessage.ID);
 		for(int i=0;i<receivedMessage.length;i++){
 			printf("Received message NODE1:, data[%d]=%d\n\r",i,receivedMessage.data[i]);}
 		printf("//////////////////////////////////////////////////////////////// \n\r");
 	}
 }
 
 void ex6() {
 	/****Working with MODE_NORMAL in the MCP2515 Driver ****/
 	/*--------------Initializing variables---------------*/
 	can_message PCB_information;
 	PCB_information.data[0] = 0x0000;
 	PCB_information.data[1] = 0x0000;
 	PCB_information.data[2] = 0x0000;
 	PCB_information.data[3] = 0x0000;
 	PCB_information.data[4] = 0x0000;
 	PCB_information.data[5] = 0x0000;
 	PCB_information.length = 6;
 	PCB_information.ID=2;
 	
 	////
 	
 	can_message receivedMessage;
 	receivedMessage.ID=7;
 	receivedMessage.data[0]='i';
 	receivedMessage.data[1]='i';
 	receivedMessage.length=2;
 	
 	/*--------------Initializing functions---------------*/
 	init_printuart(MYUBBR);
 	sram_init();
 	adc_init();
 	CAN_init();
 	
 	while (1)
 	{
 		
 		printf("///////////////New Iteration/////////////////// \n\r");
 		
 		
 		
 		//------sending information from PCB to Node 2 over CAN---------------//
 		PCB_information.data[0] = get_direction();
 		PCB_information.data[1] = get_x_pos_joy();
 		PCB_information.data[2] = get_y_pos_joy();
 		PCB_information.data[3]= slider_get_left_slider();
 		PCB_information.data[4]= slider_get_right_slider();
 		PCB_information.data[5] = slider_get_right_button();
 		printf("################################################\n\r");
 		printf("JOYSTICK DIRECTION: %d\n\r", PCB_information.data[0]);
 		printf("JOYSTICK X POSITION: %d\n\r", PCB_information.data[1]);
 		printf("JOYSTICK Y POSITION: %d\n\r", PCB_information.data[2]);
 		printf("LEFT SLIDER POSITION: %d\n\r", PCB_information.data[3]);
 		printf("RIGHT SLIDER POSITION: %d\n\r", PCB_information.data[4]);
 		printf("RIGHT SLIDER BUTTON: %d\n\r", PCB_information.data[5]);
 		printf("################################################\n\r");
 		
 		CAN_send(&PCB_information);
 
 		//------receiving messages over CAN---------------//
 // 		CAN_interrupt_handler(&receivedMessage);
 // 		printf("Received message NODE1 length:%d\n\r",receivedMessage.length);
 // 		printf("Received message NODE1 ID:%d\n\r",receivedMessage.ID);
 // 		for(int i=0;i<receivedMessage.length;i++){
 // 		printf("Received message NODE1:, data[%d]=%c\n\r",i,receivedMessage.data[i]);}
 // 		printf("//////////////////////////////////////////////////////////////// \n\r");
 	}
 }
