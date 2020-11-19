#include "meny_ny.h"
#include "joystick_driver.h"
#include "global.h"
#include <util/delay.h>

PAGE=0;

node_t* meny_insert(char value[], funksjonspeker selectedFunc, node_t* parent){
	node_t* myNode = (node_t*)malloc(sizeof(node_t));
	myNode->val=value;
	myNode->next=NULL;
	myNode->prev=NULL;
	myNode->parent = parent;
	myNode->child = NULL;
	myNode->selectedFunc=selectedFunc;
	return myNode;
}

void meny_set_child(node_t* node, node_t* child) {
	(node)->child = child;
}

void meny_set_order(node_t* node, node_t* prev, node_t* next) {
	node->next = next;
	node->prev = prev;
}


void meny_fillScreen(){
	printf("Hello world\n\r");
	oled_write_command(0b10100101);
}

void meny_print(node_t** head){
	printf("%s", (*head)->val);
	do{
		*head=(*head)->next;
		printf("%s", (*head)->val);
	}while((*head)->next != NULL);
	return 0;
}

void meny_print_oled(node_t* node, int font_size){
	while(node->prev != NULL){
		node=node->prev;
	}
	int page = 0;
	oled_pos(page, 8);
	while(node->next != NULL){
		oled_print_sec(node->val, font_size);
		node=node->next;
		page += 1;
		oled_pos(page, 8);
	}
	oled_print_sec(node->val, font_size);
}

void meny_navigation(node_t** head, int font_size){
	
	switch (get_direction()){
		case UP:
		_delay_ms(20); //Debounce joystick
		if(get_direction()!=UP){break;};
		printf("UP");
		if((*head)->prev != NULL) { //Check that marker is not at top of screen
			oled_clear_screen(); //clear screen
			PAGE = PAGE - 1; //decrease page number
			meny_print_oled(*head, font_size); //print meny
			oled_pos(PAGE, 0); //Set physical marker at the current page
			oled_print_letter('>', font_size); //print physical marker
			(*head)=(*head)->prev; //Set right node as head
		}
		printf("POS: %d-------CURRENT NODE: %s\n\r", PAGE, (*head)->val);
		_delay_ms(50); //Make sure a push of the joystick button is only counted once
		break;
		
		case DOWN: //Repeat as above
		_delay_ms(20); //Debounce joystick
		if(get_direction()!=DOWN){break;};
		printf("DOWN");
		if((*head)->next != NULL) {
			oled_clear_screen();
			PAGE = PAGE + 1;
			meny_print_oled(*head, font_size);
			oled_pos(PAGE, 0);
			oled_print_letter('>', font_size);
			(*head)=(*head)->next;
		}
		printf("POS: %d-------CURRENT NODE: %s\n\r", PAGE, (*head)->val);
		_delay_ms(50);
		break;
		
		case RIGHT: //Repeat as above
		_delay_ms(20);
		if(get_direction()!=RIGHT){break;};
		printf("RIGHT");
		if((*head)->child != NULL){ //We go into a submeny
			(*head)=(*head)->child;
			oled_clear_screen();
			PAGE=0;
			oled_pos(PAGE, 0);
			oled_print_letter('>', font_size);
			meny_print_oled(*head, font_size);
		}
		else if((*head)->selectedFunc != NULL) {//Call a functionpointer if there is one
			(*head)->selectedFunc();
		}
		printf("POS: %d-------CURRENT NODE: %s\n\r", PAGE, (*head)->val);
		_delay_ms(50);
		break;
		
		case LEFT: //Repeat as above
		_delay_ms(20);
		if(get_direction()!=LEFT){break;};
		printf("Left");
		if((*head)->parent != NULL){ //We go into a supermeny
			(*head)=(*head)->parent;
			while((*head)->prev != NULL){*head=(*head)->prev;}
			oled_clear_screen();
			PAGE=0;
			oled_pos(PAGE, 0);
			oled_print_letter('>', font_size);
			meny_print_oled(*head, font_size);
		}
		printf("POS: %d-------CURRENT NODE: %s\n\r", PAGE, (*head)->val);
		_delay_ms(50);
		break;
		
		default:// printf("error");
		break;
	}
}


void meny_init(node_t** head){
	int font_size = 5;

	node_t* meny_0 = meny_insert("MENU", NULL, NULL);
	
	node_t* new_game_1 = meny_insert("New Game", NULL, meny_0);
	node_t* high_scores_1 = meny_insert("High Scores", NULL, meny_0);
	node_t* reset_high_scores_1 = meny_insert("Reset High Scores", NULL, meny_0);
	node_t* settings_1 = meny_insert("Settings", NULL, meny_0);
	node_t* difficulty_1 = meny_insert("Difficulty", NULL, meny_0);
	
	node_t* fill_screen_2 = meny_insert("Fill screen", &meny_fillScreen, settings_1);
	node_t* volume_2 = meny_insert("Volume", NULL, settings_1);
	node_t* easy_2 = meny_insert("Easy", NULL, difficulty_1);
	node_t* medium_2 = meny_insert("Medium", NULL, difficulty_1);
	node_t* hard_2 = meny_insert("Hard", NULL, difficulty_1);
	
	meny_set_child(meny_0, new_game_1);
	meny_set_order(new_game_1, NULL, high_scores_1);
	meny_set_order(high_scores_1, new_game_1, reset_high_scores_1);
	meny_set_order(reset_high_scores_1, high_scores_1, settings_1);
	meny_set_order(settings_1, reset_high_scores_1, difficulty_1);
	meny_set_order(difficulty_1, settings_1, NULL);
	meny_set_child(settings_1, fill_screen_2);
	meny_set_child(difficulty_1,easy_2);
	meny_set_order(easy_2, NULL, medium_2);
	meny_set_order(medium_2, easy_2, hard_2);
	meny_set_order(hard_2, medium_2, NULL);
	meny_set_order(fill_screen_2, NULL, volume_2);
	meny_set_order(volume_2, fill_screen_2, NULL);
	
	*head= meny_0;
	oled_clear_screen();
	meny_print_oled(*head, font_size);
	oled_pos(0,0);
	oled_print_letter('>', font_size);
}

void meny_clear_letter(int page, int startcolumn, int font_size) {
	oled_pos(PAGE, 0); //Set position to start of current page
	for(int i=0;i < font_size;i++){ //clear current letter
		oled_write_data(0);
	}
}