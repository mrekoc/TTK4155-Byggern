#include "CAN_driver.h"
#include "mcp2515_driver.h"
#include "bit_macros.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "mcp2515_constants.h"
#include "global.h"


int MCP_t_buffer_full=0;

void CAN_interrupt_init(){
	/**Initializes the CAN interrupts**/
	
	DDRD &= ~(1 << PD2);				//Make PD2 input pin
    cli();								//Deactivate all the interrupts									
    GICR |= (1 << INT0);				//General Interrupt Control Register, Activate INT0 													
    MCUCR |= ( 1 << ISC01);				//MCU Control Register, Activates ISC01 (Interrupt Sense Control)												
    sei();								//Activate all interrupts again
	MCP2515_write(MCP_CANINTE, 0x03);   //MCP CAN Interrupt Enable, Receive interrupt activated for both of the buffers																		
}


void CAN_init(){
	/**CAN driver initialize function**/
	MCP2515_init();
	CAN_interrupt_init();
	printf("MODE: %d\n\r",MCP2515_read(0xE) >> 5);   // Print the current MCP controller mode
}

int CAN_interrupt_handler(can_message* receiveMessage){
	MCP2515_bit_modify(MCP_CANINTF, 0b00000001, 0);//interrupt handled, delete interrupt flag on MCP
	if(MCP_t_buffer_full){ //Receive message if buffer is full (gets an interrupt)
		CAN_receive(receiveMessage);
		MCP_t_buffer_full=0;
		sleep_enable();
		return 1; //Return 1 if buffer is handled
	}
	return 0;
}



void CAN_send(can_message* sendMessage) {
	uint8_t SIDL_val = (sendMessage->ID << 5);  //Calculate upper part of ID
	SIDL_val &= ~(1<<3);
	uint8_t SIDH_val = (sendMessage->ID >> 3); //Calculate lower part of ID
	MCP2515_write(MCP_TXB0SIDL, SIDL_val); //Set ID
	MCP2515_write(MCP_TXB0SIDH, SIDH_val);
	MCP2515_write(MCP_TXB0DLC, sendMessage->length & 0x0F);//Set length
	for(int i=0;i<(sendMessage->length);i++){
		MCP2515_write(TXB0Dm + i,(sendMessage->data)[i]); //Write data to buffer 0
	}
	MCP2515_bit_modify(MCP_CANINTF, 0b00000100, 0); //transmit buffer 0 empty interrupt flag bit, throws interrupt that transmission is done
	MCP2515_request_to_send(0); //Asks to send message
}

void CAN_receive(can_message *receiveMessage){
	can_message tempReceiveMessage;
	uint8_t SIDH_val, SIDL_val, length;
	SIDH_val=MCP2515_read(MCP_RXB0SIDH); //Reads ID
	SIDL_val=MCP2515_read(MCP_RXB0SIDL);
	tempReceiveMessage.ID=SIDL_val>>5 | SIDH_val<<3; //Calculate ID
	tempReceiveMessage.length = (MCP2515_read(MCP_RXB0DLC)) & 0b00001111; //Read length
 	for(int i=0;i<tempReceiveMessage.length;i++){ //Read data
		tempReceiveMessage.data[i]=MCP2515_read(RXB0DM + i); 
 	}
	*receiveMessage=tempReceiveMessage; //Write data to receiveMessage
	MCP2515_bit_modify(MCP_CANINTF, 0b00000001, 0);//interrupt handled, delete interrupt flag on MCP
}


ISR(INT0_vect){
	MCP_t_buffer_full=1;
	}

