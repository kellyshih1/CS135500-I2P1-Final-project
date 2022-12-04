#include "global.h"
#include "charater.h"

void menu_init();
void menu_process(ALLEGRO_EVENT event);
void menu_draw();
void menu_destroy();

void about_init();
void about_process(ALLEGRO_EVENT event);
void about_draw();
void about_destroy();

void game_scene_init();
void game_scene_process();
void game_scene_draw();
void game_scene_destroy();

void end_scene_init();
void end_scene_draw();
void end_scene_process(ALLEGRO_EVENT event);
void end_scene_destroy();
