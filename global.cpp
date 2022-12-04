#include "global.h"

// variables for global usage
const float FPS = 60.0;
const int WIDTH = 1324;
const int HEIGHT = 745;
const int WIDTHgame = 1400;
const int HEIGHTgame = 750;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *fps = NULL;
bool key_state[ALLEGRO_KEY_MAX] = {false};
bool judge_next_window = false;
int time_max = 3600;
int cur_score = 0;
int max_score = 0;
int total_score = 0;
int menu_state = 0;
int end_scene_state = 0;
int cur_time_game = 0;
int missed_cust = 0;
int income = 0;
int penalty = 30;
