#include "global.h"
#include <stdio.h>
#include <stdarg.h>
#include "uart.h"
#include "printf-stdarg.h"
#include "can_controller.h"
#include "sam.h"
#include "node2_bit_macros.h"
#include "can_interrupt.h"
#include "PWM_driver.h"
#include "ADC_driver.h"
#include "PI_controller_driver.h"
#include "delay.h"
#include "solenoid_driver.h"
#include "game_driver_n2.h"

void game_node_2(){
	/*---------init variables------------*/
	CAN_MESSAGE PCB_information;
	PCB_information.data[0] = 0x00;
	PCB_information.data[1] = 0x00;
	PCB_information.data[2] = 0x00;
	PCB_information.data[3] = 0x00;
	PCB_information.data[4] = 0x00;
	PCB_information.data[5] = 0x00;
	PCB_information.data_length = 6;
	PCB_information.id=2;
	
	CAN_MESSAGE game_information;
	game_information.data[0]=0x00;
	game_information.data_length = 1;
	game_information.id=5;
	
	
		
	//---------Initialize functions--------
	/* Initialize the SAM system */
	SystemInit();
	WDT->WDT_MR = WDT_MR_WDDIS;
	configure_uart();
	can_init_def_tx_rx_mb(CAN_BR_INIT_VALUE);
	
	//Other inti functions
	PWM_init();
	ADC_init();
	solenoid_init();
	motor_init();
	PI_timer_counter_init();
	
	int goals=-2; //Initializing goals, -2 because of interrupts during start up

	/**Loop**/
	while (1) {
		//---------Receiving node 2--------
			get_can_message(&PCB_information);     //Update the received_message values
			integral_gain=PCB_information.data[1]; //Get gain faktor, choose difficulty
			if(PCB_information.data[4]==1){ //Reset goals if new game
				goals=0;
			}
			
		//------printing for error detection---------	
/*  		printf("################################################\n\r");*/
//   		printf("MOTOR REFERENCE: %d\n\r", motor_CAN_to_pos(PCB_information.data[0]));
//  		printf("DIFFICULTY: %d\n\r", PCB_information.data[1]);
//   		printf("SERVO REFERENCE: %d\n\r", PCB_information.data[2]);
// 			printf("SOLENOID: %d\n\r", PCB_information.data[3]);
//  		printf("GAMEOVER FLAG: %d\n\r", PCB_information.data[4]);
// 			printf("SCORE: %d\n\r", goals);
// 			printf("MEASURE: %d\n\r", encoder_to_pos(motor_read_encoder()));
// 			printf("ERROR: %d\n\r",  motor_CAN_to_pos(PCB_information.data[0]) - encoder_to_pos(motor_read_encoder()));
/*  		printf("################################################\n\r");*/

	//-----update goals----
		ADC_update_goal(&goals);
		game_information.data[0]=goals;
		
	//------send goal information over CAN
		can_send(&game_information, 0);
		
	//------control the hardware
		PWM_set_DC(PWM_convert_from_can(PCB_information.data[2]));
		solenoid_drive(PCB_information.data[3]);
		
	}
}