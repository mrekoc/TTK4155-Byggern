#include "OLED_driver.h"
#include "font4.h"
#include "font5.h"
#include "font8.h"

volatile char *oled_command_addr = (char *) 0x1000; // Start address for the OLED
volatile char *oled_data_addr = (char *) 0x1200; //Adress for the data

void oled_write_command(int command){
	oled_command_addr[0] = command;  // write OLEd command
}

void oled_write_data(int data){
	oled_data_addr[0] = data;  //write OELD data
}

//--------Direct copy from data sheet--------------
void oled_init(){

	oled_write_command(0xae);	// display off
	oled_write_command(0xa1);	// segment remap
	oled_write_command(0xda);	// common pads hardware: alternative
	oled_write_command(0x12);
	oled_write_command(0xc8);	// common output scan direction :com63~com0
	oled_write_command(0xa8);	// multiplex ration mode : 63
	oled_write_command(0x3f);	
	oled_write_command(0xd5);	// display divide ratio/osc. freq. mode
	oled_write_command(0x80);
	oled_write_command(0x81);	// contrast control
	oled_write_command(0x50);
	oled_write_command(0xd9);	// set pre-charge period
	oled_write_command(0x21);
	oled_write_command(0x20);	// Set Memory Addressing Mode
	oled_write_command(0x02);
	oled_write_command(0xdb);	// VCOM deselect level mode
	oled_write_command(0x30);
	oled_write_command(0xad);	// master configuration
	oled_write_command(0x00);
	oled_write_command(0xa4);	// out follows RAM content
	oled_write_command(0xa6);	// set normal display
	oled_write_command(0xaf);	// display on
	
	oled_write_command(0b01000000); // not to have offset on columns 
}
//-----------Direct copy end--------------


void oled_goto_page(int page){ //Access different pages
	switch (page)
	{
	case 0: 
		oled_write_command(0xb0);
		break;
	case 1: 
		oled_write_command(0xb1);
		break;
	case 2: 
		oled_write_command(0xb2);
		break;
	case 3: 
		oled_write_command(0xb3);
		break;
	case 4: 
		oled_write_command(0xb4);
		break;
	case 5: 
		oled_write_command(0xb5);
		break;
	case 6: 
		oled_write_command(0xb6);
		break;
	case 7: 
		oled_write_command(0xb7);
		break;
}
}

void oled_goto_column(int column){ //Access different columns
	int low = column % 16;
	int high = column/ 16;
	
	oled_write_command(low);
	oled_write_command(high + 16);
}


void oled_clear_screen(){//clear the entire screen
	oled_pos(0,0);
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 128; j++){
			oled_pos(i, j);
			oled_write_data(0b00000000);
		}
	}
}

void oled_pos(int page, int column){ //Move marker to a specified position
	oled_goto_page(page);
	oled_goto_column(column);
}

void oled_fill_page(int page){ //Fill the entire page
	oled_goto_page(page);
	oled_goto_column(50);
	for (int i = 0; i <50; i++){
		oled_write_data(~0b00000000);
		_delay_ms(1);
	}
}


void oled_print_letter(char letter, int font_size){//Print one letter to the OLED screen
 	int ascii_offset = 32;
 	int char_value = letter - ascii_offset; //Adjust for offset
 	char char_out;
	switch (font_size)
	{
	case 4:
		for (int i = 0; i < 4; i++){
			oled_write_data((char) pgm_read_byte(&myfont4[char_value][i])); //write the data stored in PRGM to the OLED screen
		}
		break;
	case 5:
		for (int i = 0; i < 5; i++){
			oled_write_data((char) pgm_read_byte(&myfont5[char_value][i]));
		}
		break;
	case 8:
		for (int i = 0; i < 8; i++){
			oled_write_data((char) pgm_read_byte(&myfont8[char_value][i]));
		}
		break;
	}
	oled_write_data(0b00000000); //Space before next character
}

void oled_print_sec(char sec[], int font_size){ //Print multiple letters to the OLED screen
	for (int i = 0;i<strlen(sec);i++){
		oled_print_letter(sec[i], font_size);
	}
	return 0;
}


