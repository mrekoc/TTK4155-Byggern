#include "meny_ny.h"
#include "joystick_driver.h"
#include "global.h"
#include <util/delay.h>


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



// void meny_child_insert(struct node* parent, char value[]){
// 	if(parent.child == NULL){
// 		struct node* myNode = (struct node*)malloc(sizeof(struct node));
// 		myNode->val=value;
// 		parent->child=*myNode;
// 		myNode->parent=*parent;
// 	}
// // 	else{
// // 		meny_insert(parent->child, value);
// // 	}
// }

void fillScreen(){
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

void oled_menu_navigation(node_t** head, int font_size){
	meny_print_oled(*head, font_size);
	
  	switch (get_direction()){
  		case UP:
 			_delay_ms(20);
 			if(get_direction()!=UP){break;};
			printf("Up");
 			oled_pos(PAGE, 0);
  			for(int i=0;i < font_size;i++){
 				oled_write_data(0);
 			}
 			if (PAGE <= 0){
 				PAGE=7;
 			}
 			else {
 				PAGE = PAGE - 1;
 			}
 			oled_pos(PAGE, 0);
 			oled_print_letter('>', font_size);
			if((*head)->prev != NULL) {(*head)=(*head)->prev;}
 			_delay_ms(1000);
 		break;
		 
  		case DOWN:
 			_delay_ms(20);
 			if(get_direction()!=DOWN){break;};
			printf("Down");
 			oled_pos(PAGE, 0);
  			for(int i=0;i < font_size;i++){
 				oled_write_data(0);
 			}
 			if (PAGE >= 7){
 				PAGE=0;
 			}
 			else {
 				PAGE = PAGE + 1;
 			}
 			oled_pos(PAGE, 0);
 			oled_print_letter('>', font_size);
			if((*head)->next != NULL) {(*head)=(*head)->next;}
 			_delay_ms(1000);
 		break;
		
		case RIGHT:
			_delay_ms(20);
 			if(get_direction()!=RIGHT){break;};
			printf("Right");
			if((*head)->child != NULL){
				(*head)=(*head)->child;
				oled_clear_screen();
				PAGE=0;
				oled_pos(PAGE, 0);
 				oled_print_letter('>', font_size);
			}
			else if((*head)->selectedFunc != NULL) {
				(*head)->selectedFunc();
			}
			_delay_ms(1000);
		break;
		
   		case LEFT:
		   	_delay_ms(20);
 			if(get_direction()!=LEFT){break;};
			printf("Left");
			if((*head)->parent != NULL){
  				(*head)=(*head)->parent;
				oled_clear_screen();
				PAGE=0;
				oled_pos(PAGE, 0);
 				oled_print_letter('>', font_size);
			}
			_delay_ms(1000);
  		break;
		  
 		default:
 			printf("error");
 		break;
 	}
}

	
void meny_init1(node_t** head){
	int font_size = 5;
	
	//node_t* temp_head = NULL;
	node_t* meny_0 = meny_insert("MENU", NULL, NULL);	
	node_t* new_game_1 = meny_insert("New Game", NULL, meny_0);
	node_t* high_scores_1 = meny_insert("High Scores", NULL, meny_0);
	node_t* reset_high_scores_1 = meny_insert("Reset High Scores", NULL, meny_0);
	node_t* settings_1 = meny_insert("Settings", NULL, meny_0);
	node_t* difficulty_1 = meny_insert("Difficulty", NULL, meny_0);
	node_t* fill_screen_2 = meny_insert("Fill screen", &fillScreen, settings_1);
	
	
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
}
