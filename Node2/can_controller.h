/*
 * can_controller.h
 *
 * Author: Gustav O. Often and Eivind H. J�lsgard
 *
 * For use in TTK4155 Embedded and Industrial Computer Systems Design
 * NTNU - Norwegian University of Science and Technology
 *
 */ 


#ifndef CAN_CONTROLLER_H_
#define CAN_CONTROLLER_H_

#include <stdint.h>

typedef struct can_message_t
{
	uint16_t id;
	char data_length;
	char data[8];
} CAN_MESSAGE;

uint8_t can_init_def_tx_rx_mb(uint32_t);
uint8_t can_init(uint32_t , uint8_t , uint8_t );

uint8_t can_send(CAN_MESSAGE* , uint8_t );
uint8_t can_receive(CAN_MESSAGE* , uint8_t );

#endif /* CAN_CONTROLLER_H_ */