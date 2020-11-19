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

//Defining variable that holds game information
gameVar_t game;

void game_node_1(){
	/*--------------Initializing variables---------------*/
	can_message PCB_information; //data from node 1
	PCB_information.data[0] = 0x0000;
	PCB_information.data[1] = 0x0000;
	PCB_information.data[2] = 0x0000;
	PCB_information.data[3] = 0x0000;
	PCB_information.data[4] = 0x0000;
	PCB_information.data[5] = 0x0000;
	PCB_information.length = 6;
	PCB_information.ID=2;
	
	////
	node_t* meny_head = NULL; //initializing the heads of the node systems
	node_t* game_screen_head = NULL;

	/*--------------Initializing functions---------------*/
	game_init(&meny_head);
	game_screen_init(&game_screen_head);
	
	while (1)
	{
		switch(game.screenType){//Depending on where in the game progress we are, shift between different screens
			case 0: meny_navigation(&meny_head, game.font_size); break; //menu mode
			case 1: update_game_screen(&game_screen_head); break; //game mode
			case 2: game_over(&meny_head);break; //Game over mode
			default: game.screenType=0;
		}
		
		//------sending information from PCB to Node 2 over CAN---------------//
		if(game.sliderFlag){PCB_information.data[0] = slider_get_left_slider();}
		else {PCB_information.data[0] = get_x_pos_joy();}
		PCB_information.data[1]=game.K_i;
		PCB_information.data[2]=slider_get_right_slider();
		PCB_information.data[3]=slider_get_right_button();
		PCB_information.data[4]=game.game_over_flag;
		CAN_send(&PCB_information);
		
		//Printing to putty for error detection
//  		printf("################################################\n\r");
//  		printf("MOTOR REFERENCE: %d\n\r", PCB_information.data[0]);
// 			printf("DIFFICULTY: %d\n\r", PCB_information.data[1]);
// 			printf("SERVO REFERENCE: %d\n\r", PCB_information.data[2]);
// 			printf("SOLENOID: %d\n\r", PCB_information.data[3]);
// 			printf("GAME OVER FLAG: %d\n\r", PCB_information.data[4]);
//  		printf("################################################\n\r");
		 
		update_goals(&game_screen_head); //Check for goals
		game_over_check(&meny_head); //Check if player is out of lives
	}
}

void game_over_check() {
	if(game.life==0){ //game over
		game.screenType=2;
		game.game_over_flag=1;
	}
};

void update_goals(node_t** game_screen_head){
	//------receiving messages over CAN---------------//
	can_message gameInformation; //data to node 1
	CAN_interrupt_handler(&gameInformation);
	if(game.goals != gameInformation.data[0]){//Check if we have a new goal
		game.goals = gameInformation.data[0]; //Update right amout of goals
		if (game.life > 0) {game.life = game.life-1;} //Check to make sure we dont overflow (uint variable)
		char lifestr[8];
		sprintf(lifestr, "Life: %d", game.life);
		(*game_screen_head)->child->val=lifestr;//Update life
	}
}

void game_over(node_t **meny_head) {
	game_over_print();
	if(game.goals==0 && get_direction()==3){ //resetting game information after ack from node 2 and moving of joystick
	//Resetting menu node
		node_t* workingNode; //Resetting menu head
		workingNode=*meny_head;
		do{
		workingNode=workingNode->parent;
		}while(workingNode->parent != NULL);
		(*meny_head)=workingNode;
	
	//Printing menu
		oled_clear_screen();
		meny_print_oled(*meny_head, game.font_size);
		PAGE=0;
		oled_pos(PAGE, 0);
		oled_print_letter('>', game.font_size);
	
	//Update game information
		game.screenType=0;
		game.game_over_flag = 0;
		game.life=3;
	}
}

void game_init(node_t **head){
	//Initializing functions
	init_printuart(MYUBBR);
	sram_init();
	adc_init();
	oled_init();
	CAN_init();
	
	//Initializing game variables
	game.font_size=5;
	game.sliderFlag=1;
	game.screenType=0;
	game.life=3;
	game.goals=0;
	game.game_over_flag=0;
	game.K_i=1;

	//Initializing meny node structure
	//Making nodes
	node_t* meny_0 = meny_insert("MENU", NULL, NULL);
	node_t* new_game_1 = meny_insert("New Game", NULL, meny_0);
	node_t* difficulty_1 = meny_insert("Difficulty", NULL, meny_0);
	node_t* settings_1 = meny_insert("Settings", NULL, meny_0);
	
	node_t* new_game_slider_2 = meny_insert("Slider", &game_with_slider, new_game_1);
	node_t* new_game_joystick_2 = meny_insert("Joystick", &game_with_joystick, new_game_1);
	
	node_t* easy_2 = meny_insert("Hard", &game_diff_easy, difficulty_1);
	node_t* medium_2 = meny_insert("Impossible", &game_diff_medium, difficulty_1);
	node_t* hard_2 = meny_insert("Byggern", &game_diff_hard, difficulty_1);
	
	node_t* change_fonts_2 = meny_insert("Change fonts", NULL, settings_1);
	node_t* fill_screen_2 = meny_insert("Fill screen", &meny_fillScreen, settings_1);
	
	node_t* font_4_3 = meny_insert("Font size 4", &game_font_4, change_fonts_2);
	node_t* font_5_3 = meny_insert("Font size 5", &game_font_5, change_fonts_2);
	node_t* font_8_3 = meny_insert("Font size 8", &game_font_8, change_fonts_2);
	
	//Setting first child on each level
	meny_set_child(meny_0, new_game_1);
	meny_set_child(new_game_1, new_game_slider_2);
	meny_set_child(difficulty_1, easy_2);
	meny_set_child(settings_1, change_fonts_2);
	meny_set_child(change_fonts_2, font_4_3);
	
	//Setting order of nodes on each level
	meny_set_order(new_game_1, NULL, difficulty_1);
	meny_set_order(difficulty_1, new_game_1, settings_1);
	meny_set_order(settings_1, difficulty_1, NULL);
	
	meny_set_order(new_game_slider_2, NULL, new_game_joystick_2);
	meny_set_order(new_game_joystick_2, new_game_slider_2, NULL);
	
	meny_set_order(easy_2, NULL, medium_2);
	meny_set_order(medium_2, easy_2, hard_2);
	meny_set_order(hard_2, medium_2, NULL);
	
	meny_set_order(change_fonts_2, NULL, fill_screen_2);
	meny_set_order(fill_screen_2, change_fonts_2, NULL);
	
	meny_set_order(font_4_3, NULL, font_5_3);
	meny_set_order(font_5_3, font_4_3, font_8_3);
	meny_set_order(font_8_3, font_5_3, NULL);
	
	*head= meny_0;
	
	//Printing menu
	oled_clear_screen();
	meny_print_oled(*head, game.font_size);
	oled_pos(0,0);
	oled_print_letter('>', game.font_size);
}

void game_screen_init(node_t **game_screen_head){
	//Making node structure for game screen
	node_t* game_screen0 = meny_insert("MENU", NULL, NULL);
	node_t* life_1 = meny_insert("Life: 3", NULL, game_screen0);
	node_t* grade_1 = meny_insert("Grade: A", NULL, game_screen0);
	node_t* quality_1 = meny_insert("Quality: Epic", NULL, game_screen0);
	
	meny_set_child(game_screen0, life_1);
	meny_set_order(life_1, NULL, grade_1);
	meny_set_order(grade_1, life_1, quality_1);
	meny_set_order(quality_1, grade_1, NULL);
	*game_screen_head= game_screen0;
}


void update_game_screen(node_t** game_screen_head){
	//Update the screen that are shown during a game. Main task, update value of game.life
	node_t* currNode = (*game_screen_head)->child; 
	char lifestr[8];
	sprintf(lifestr, "Life: %d", game.life);
	currNode->val=lifestr;
	oled_clear_screen();
	meny_print_oled((*game_screen_head)->child, game.font_size);
	oled_pos(0,0);
	_delay_ms(30);
}

game_over_print(){
	oled_clear_screen();
	oled_pos(0,0);
	oled_print_sec("\t\tGAME OVER\t\t", 8);
	oled_pos(2,0);
	oled_print_sec("You did terrible ", 5);
	oled_pos(3,0);
	oled_print_sec("Better luck next time", 4);
	oled_pos(5,0);
	oled_print_sec("Move joystick to retry", 4);
	oled_pos(0,0);
}

void game_font_4(){game.font_size = 4;}
	
void game_font_5(){game.font_size = 5;}
	
void game_font_8(){game.font_size = 8;}
	
void game_diff_easy(){ //Set difficulty through changing regulator parameters
	game.K_i=0;
}
	
void game_diff_medium(){
		game.K_i=1;
}
	
void game_diff_hard(){
		game.K_i=5;
}
	
void game_with_slider(){ //Chose slider to play the game
	game.sliderFlag=1;
	game.screenType=1;
	}
	
void game_with_joystick(){//Choose joystick to play the game
	game.sliderFlag=0;
	game.screenType=1;
	}
	