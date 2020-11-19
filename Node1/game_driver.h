/************************************************************************/
/* Game Driver                                                        */
/************************************************************************/

#ifndef GAME_33
#define GAME_33

typedef struct gameVar_t{
	uint8_t font_size;
	uint8_t K_i;
	uint8_t sliderFlag; //1 for slider as controller
	uint8_t life;
	uint8_t screenType;
	uint8_t goals;
	uint8_t game_over_flag;
	}gameVar_t;
	
void game_font_4();
void game_font_5();
void game_font_8();
void game_diff_easy();
void game_diff_medium();
void game_diff_hard();
void game_with_slider();
void game_with_joystick();
void game_init(node_t**);
void game_node_1();
void game_over_check();
void update_goals(node_t**);
void game_over(node_t **);
void game_screen_init(node_t **);
void update_game_screen(node_t**);
void game_over_print();

	
	
#endif
