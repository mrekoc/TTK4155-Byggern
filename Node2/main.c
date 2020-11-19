/*
 * gruppe33_node2__.c
 *
 * Created: 30.10.2020 10:58:50
 * Author : yunusek
 */ 

// #include <stdio.h>
// #include <stdarg.h>
// #include "uart.h"
// #include "printf-stdarg.h"
// #include "can_controller.h"
// #include "sam.h"
// #include "node2_bit_macros.h"
// #include "can_interrupt.h"
// #include "PWM_driver.h"
// #include "ADC_driver.h"
// #include "PI_controller_driver.h"
// #include "delay.h"
// #include "solenoid_driver.h"
#include "game_driver_n2.h"



int main(void)
{
	game_node_2();
}


// 
// 
// 
// void ex6(){
// 	/****Exercise 6 Recieves message from Node 1****/
// 	
// 	
// 	/*---------init variables------------*/
// 	CAN_MESSAGE received_message_n2;
// 	received_message_n2.data[0]	= 'i';
// 	received_message_n2.data[1]	= 'i';
// 	received_message_n2.data[2]	= 'i';
// 	received_message_n2.data[3]	= 'i';
// 	received_message_n2.data[4]	= 'i';
// 	received_message_n2.id = 7;
// 	received_message_n2.data_length = 5;
// 	
// 	CAN_MESSAGE sent_message_n2;
// 	sent_message_n2.id = 1;
// 	sent_message_n2.data[0]='w';
// 	sent_message_n2.data[1]='o';
// 	sent_message_n2.data[2]='r';
// 	sent_message_n2.data[3]='l';
// 	sent_message_n2.data[4]='d';
// 	sent_message_n2.data_length = 5;
// 	
// 	//---------Initialize functions--------
// 	/* Initialize the SAM system */
// 	SystemInit();
// 	WDT->WDT_MR = WDT_MR_WDDIS;
// 	configure_uart();
// 	can_init_def_tx_rx_mb(CAN_BR_INIT_VALUE);
// 	
// 	//-----init led blink-------
// 	set_bit(PMC, PMC_PCER0, 11);
// 	set_bit(PIOA, PIO_PER, 19);
// 	set_bit(PIOA, PIO_OER, 19);
// 	
// 	/**Loop**/
// 	while (1) {
// 		printf("---------New iteration---------\n\r");
// 		
// 		/**Leds blink**////
// 		set_bit(PIOA, PIO_SODR, 19);
// 		_delay_ms(100);
// 
//  		set_bit(PIOA, PIO_CODR, 19);
//  		_delay_ms(100);
// 		
// //---------Receiving node 2--------
// // 		get_can_message(&received_message_n2);     //Update the received_message values
// // 		printf("################################################\n\r");
// // 		printf("JOYSTICK DIRECTION: %d\n\r", received_message_n2.data[0]);
// // 		printf("JOYSTICK X POSITION: %d\n\r", received_message_n2.data[1]);
// // 		printf("JOYSTICK Y POSITION: %d\n\r", received_message_n2.data[2]);
// // 		printf("LEFT SLIDER POSITION: %d\n\r", received_message_n2.data[3]);
// // 		printf("RIGHT SLIDER POSITION: %d\n\r", received_message_n2.data[4]);
// // 		printf("RIGHT SLIDER BUTTON: %d\n\r", received_message_n2.data[5]);
// // 		printf("################################################\n\r");
// // 		
// // 		_delay_ms(1000);
// 
// //---------Sending node 2--------
// 		can_send(&sent_message_n2, 0);
// 		printf("SENT MESSAGE NODE 2 ID: %d \n\r", sent_message_n2.id);
//  		printf("SENT MESSAGE NODE 2 LENGTH: %d \n\r", sent_message_n2.data_length);
//   		for(int i=0;i<sent_message_n2.data_length;i++){
//   			printf("SENT MESSAGE node 2 DATA, data[%d]=%c \n\r",i,sent_message_n2.data[i]);
//    		}
// 		
// 	}
// 	
// }
// 
// void ex7(){
// 	/****Exercise 6 Recieves message from Node 1****/
// 	
// 	
// 	/*---------init variables------------*/
// 	CAN_MESSAGE PCB_information;
// 	PCB_information.data[0] = 0x0000;
// 	PCB_information.data[1] = 0x0000;
// 	PCB_information.data[2] = 0x0000;
// 	PCB_information.data[3] = 0x0000;
// 	PCB_information.data[4] = 0x0000;
// 	PCB_information.data[5] = 0x0000;
// 	PCB_information.data_length = 6;
// 	PCB_information.id=2;
// 	int goals=0;
// 	
// 	//---------Initialize functions--------
// 	/* Initialize the SAM system */
// 	SystemInit();
// 	WDT->WDT_MR = WDT_MR_WDDIS;
// 	configure_uart();
// 	can_init_def_tx_rx_mb(CAN_BR_INIT_VALUE);
// 	PWM_init();
// 	ADC_init();
// 
// 	
// 	/**Loop**/
// 	while (1) {
// 		printf("---------New iteration---------\n\r");
// 		
// 		
// 		//pi_activate_regulator();
// 		
// 		//---------Receiving node 2--------
// // 		get_can_message(&PCB_information);     //Update the received_message values
// // 		printf("################################################\n\r");
// // 		printf("JOYSTICK DIRECTION: %d\n\r", PCB_information.data[0]);
// // 		printf("JOYSTICK X POSITION: %d\n\r", PCB_information.data[1]);
// // 		printf("JOYSTICK Y POSITION: %d\n\r", PCB_information.data[2]);
// // 		printf("LEFT SLIDER POSITION: %d\n\r", PCB_information.data[3]);
// // 		printf("RIGHT SLIDER POSITION: %d\n\r", PCB_information.data[4]);
// // 		printf("RIGHT SLIDER BUTTON: %d\n\r", PCB_information.data[5]);
// // 		printf("################################################\n\r");
// 
// 		
// 		//--------Set PWM duty cycle-----
// 		//PWM_set_DC(1500);
// 		
// 		//--------Use joystick to control servo position----////
// 		//PWM_set_DC(PWM_convert_from_can(PCB_information.data[1]));
// 		
// 		//--------Use slider to control servo position----////
// 		//PWM_set_DC(PWM_convert_from_can(PCB_information.data[4]));		
// 		
// 		//--------Count goals----////
// 		ADC_update_goal(&goals);
// 		printf("SCORE: %d\n\r", goals);
// 		//printf("ADC value: %x\n\r", ADC->ADC_CDR[0]);
// 		
// 	}
// 		
// }
// 	
// 
// void ex8(){
// 	/*---------init variables------------*/
// 	
// 	CAN_MESSAGE PCB_information;
// 	PCB_information.data[0] = 0x0000;
// 	PCB_information.data[1] = 0x0000;
// 	PCB_information.data[2] = 0x0000;
// 	PCB_information.data[3] = 0x0000;
// 	PCB_information.data[4] = 0x0000;
// 	PCB_information.data[5] = 0x0000;
// 	PCB_information.data_length = 6;
// 	PCB_information.id=2;
// 	
// 
// 	//---------Initialize functions--------
// 	/* Initialize the SAM system */
// 	SystemInit();
// 	WDT->WDT_MR = WDT_MR_WDDIS;
// 	configure_uart();
// 	can_init_def_tx_rx_mb(CAN_BR_INIT_VALUE);
// 	
// 	//Init motor
// 	ADC_init();
// 	motor_init();
// 	
// 	/*init PWM*/
// 	PWM_init();
// 	
// 	//Init PI-regulator
// 	PI_timer_counter_init();
// 	
// 	//Init Solenoid driver
// 	solenoid_init();
// 	
// 	/*loop*/
// 	while (1) {
// 		printf("---------New iteration---------\n\r");
// 	//------Solenoid shot----
// 		get_can_message(&PCB_information);
// 		solenoid_drive(PCB_information.data[5]);
// 	}
// }