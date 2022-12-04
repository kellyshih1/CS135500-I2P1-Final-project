#include "scene.h"

ALLEGRO_FONT *pmarker = NULL;
ALLEGRO_FONT *twriter = NULL;
ALLEGRO_FONT *font = NULL;
ALLEGRO_FONT *font1 = NULL;
ALLEGRO_FONT *font2 = NULL;

ALLEGRO_BITMAP *background = NULL;
ALLEGRO_BITMAP *end_background = NULL;
ALLEGRO_BITMAP *times_up = NULL;
ALLEGRO_BITMAP *BG[4];
ALLEGRO_BITMAP *badge[4];
char earned[50], highest[50];
int badge_x = 320, badge_y = 500;

// function of menu
void menu_init(){
    pmarker = al_load_ttf_font("./font/pm.ttf", 25, 0);
    twriter = al_load_ttf_font("./font/typewriter.ttf", 39, 0);
    font = al_load_ttf_font("./font/old.ttf",70,0);
    font1 = al_load_ttf_font("./font/title.ttf",180,0);
    //background = al_load_bitmap("./image17/backgroundNew.jpg");
    for (int i = 0; i < 4; i++) {
        char tmp[50];
        sprintf(tmp, "./image17/bg%d.png", i+1);
        BG[i] = al_load_bitmap(tmp);
    }
}
void menu_process(ALLEGRO_EVENT event){
    if( event.type == ALLEGRO_EVENT_KEY_UP )
        if( event.keyboard.keycode == ALLEGRO_KEY_ENTER )
            judge_next_window = true;

    if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
        int mouse_x = event.mouse.x, mouse_y = event.mouse.y;
        printf("%d %d\n", mouse_x, mouse_y);
        if (mouse_x >= 990 && mouse_x <= 1180 && mouse_y >= 330 && mouse_y <= 420) {
            judge_next_window = true;
            menu_state = 1;
        }
        else if (mouse_x >= 1040 && mouse_x <= 1220 && mouse_y >= 470 && mouse_y <= 570) {
            judge_next_window = true;
            menu_state = 2;
        }
        else if  (mouse_x >= 970 && mouse_x <= 1160 && mouse_y >= 600 && mouse_y <= 690) {
            judge_next_window = true;
            menu_state = 3;
        }
    }
}
void menu_draw(){
    //al_clear_to_color(al_map_rgb(100,100,100));
    al_draw_bitmap(BG[0], 0, 0, 0);
    //al_draw_bitmap(background, 0, 0, 0);
    /*al_draw_text(font1, al_map_rgb(148,20,20), 660, 90 , ALLEGRO_ALIGN_CENTRE, "Zombie Kitchen");
    al_draw_text(font, al_map_rgb(153,0,0), WIDTH/2, HEIGHT/2-70 , ALLEGRO_ALIGN_CENTRE, "Start");
    al_draw_text(font, al_map_rgb(153,0,0), WIDTH/2, HEIGHT/2+50 , ALLEGRO_ALIGN_CENTRE, "About");
    al_draw_text(font, al_map_rgb(153,0,0), WIDTH/2, HEIGHT/2+170 , ALLEGRO_ALIGN_CENTRE, "Exit");
    al_draw_rectangle(WIDTH/2-150, HEIGHT/2-65, WIDTH/2+150, HEIGHT/2+20, al_map_rgb(153,0,0), 3);
    al_draw_rectangle(WIDTH/2-150, HEIGHT/2+55, WIDTH/2+150, HEIGHT/2+140, al_map_rgb(153,0,0), 3);
    al_draw_rectangle(WIDTH/2-150, HEIGHT/2+175, WIDTH/2+150, HEIGHT/2+260, al_map_rgb(153,0,0), 3);
*/
}
void menu_destroy(){
    al_destroy_font(font);
    al_destroy_font(font1);
  //  al_destroy_bitmap(background);
}

// function of about

void about_init(){
    font = al_load_ttf_font("./font/old.ttf",32,0);
    //background = al_load_bitmap("./image17/wood.jpg");
}
void about_process(ALLEGRO_EVENT event){
    if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
        int mouse_x = event.mouse.x, mouse_y = event.mouse.y;
        printf("%d %d\n", mouse_x, mouse_y);
        if (mouse_x >= 880 && mouse_x <= 1150 && mouse_y >= 450 && mouse_y <= 520) {
            judge_next_window = true;
        }
    }
    if( event.type == ALLEGRO_EVENT_KEY_UP )
        if( event.keyboard.keycode == ALLEGRO_KEY_ENTER )
            judge_next_window = true;
}
void about_draw(){
    //al_draw_bitmap(background, 0, 0, 0);
    al_draw_bitmap(BG[1], 0, 0, 0);
   /* al_draw_text(font, al_map_rgb(153,0,0), 110, 140, ALLEGRO_ALIGN_LEFT, "Use the mouse to control the waiter to serve and collect money");
    al_draw_text(font, al_map_rgb(153,0,0), 110, 240, ALLEGRO_ALIGN_LEFT, "Remember, the customers will leave if they run out of patience");
    al_draw_text(font, al_map_rgb(153,0,0), 450, 330, ALLEGRO_ALIGN_LEFT, "Press ENTER to start");
    al_draw_text(font, al_map_rgb(153,0,0), 330, 650, ALLEGRO_ALIGN_LEFT, "Made by Hsuan, Kelly, Kostya, Sasha");
*/
}
void about_destroy(){
    al_destroy_font(font);
  //  al_destroy_bitmap(background);
}

// function of game_scene
void game_scene_init(){
    cur_time_game = 0;
    character_init();
    //background = al_load_bitmap("./image17/floorNew.jpg");
}
void game_scene_process() {
    cur_time_game++;
    if (cur_time_game > time_max) {
        judge_next_window = true;
    }
}
void game_scene_draw(){
    //al_draw_bitmap(background, 0, 0, 0);
   /*
    char revenue_str[50];
    sprintf(revenue_str, "%3d", REVENUE);
    */
    al_draw_bitmap(BG[2], 0, 0, 0);
    char tmp[10];
    sprintf(tmp, "%3d", cur_score);
    al_draw_text(pmarker, al_map_rgb(0, 0, 0), 220, 100, 0, tmp);

     char tmp2[10];
    sprintf(tmp2, "%3d", (time_max - cur_time_game) / 60) ;
    al_draw_text(pmarker, al_map_rgb(0, 0, 0), 210, 60, 0, tmp2);

    character_draw();
}
void game_scene_destroy(){
    //al_destroy_bitmap(background);
    character_destory();
}

void end_scene_init(){
    font1 = al_load_ttf_font("./font/old.ttf", 80,0);
    font2 = al_load_ttf_font("./font/old.ttf", 40,0);

    for(int i = 0; i < 4; i++) {
        char tmp[50];
        sprintf(tmp, "./image17/badge%d.png", i+1);
        badge[i] = al_load_bitmap(tmp);
    }
    income = cur_score - missed_cust * penalty;
    total_score += income;

    //end_background = al_load_bitmap("./image17/wood.jpg");
    //times_up = al_load_bitmap("./image17/timesup.png");

}
void end_scene_process(ALLEGRO_EVENT event) {
      if( event.type == ALLEGRO_EVENT_KEY_UP )
        if( event.keyboard.keycode == ALLEGRO_KEY_ENTER )
            judge_next_window = true;

       if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
        int mouse_x = event.mouse.x, mouse_y = event.mouse.y;
        printf("%d %d\n", mouse_x, mouse_y);
        if (mouse_x >= 900 && mouse_x <= 1190 && mouse_y >= 340 && mouse_y <= 410) {
            judge_next_window = true;
            end_scene_state = 1;
        }
        else if (mouse_x >= 960 && mouse_x <= 1120 && mouse_y >= 440 && mouse_y <= 510) {
            judge_next_window = true;
            end_scene_state = 2;
        }
        else if  (mouse_x >= 1010 && mouse_x <= 1170 && mouse_y >= 570 && mouse_y <= 630) {
            judge_next_window = true;
            end_scene_state = 3;
        }
    }


}
void end_scene_draw(){
   /* if (REVENUE > max_score) max_score = REVENUE;
    sprintf(earned, "You earned %3d$", REVENUE);
    sprintf(highest, "Highest score %3d", max_score);
    */
    //al_draw_bitmap(end_background, 0, 0, 0);

    al_draw_bitmap(BG[3], 0, 0, 0);

     char tmp[10];
    sprintf(tmp, "%5d", cur_score);
    al_draw_text(twriter, al_map_rgb(0, 0, 0), 420, 360, 0, tmp);

    char tmp1[10];
    sprintf(tmp1, "%5d", income);
    al_draw_text(twriter, al_map_rgb(0, 0, 0), 420, 520, 0, tmp1);


    char tmp2[10];
    sprintf(tmp2, "%5d", total_score);
    al_draw_text(twriter, al_map_rgb(0, 0, 0), 420, 620, 0, tmp2);

    char tmp3[50];
    sprintf(tmp3, "-%d * %d = -%d", missed_cust, penalty, missed_cust *penalty);
    al_draw_text(twriter, al_map_rgb(0, 0, 0), 260, 420, 0, tmp3);

    if(cur_score > 250) al_draw_bitmap(badge[0], badge_x, badge_y, 0);
    else if (cur_score > 210) al_draw_bitmap(badge[1], badge_x, badge_y,0);
    else if (cur_score > 180) al_draw_bitmap(badge[2], badge_x, badge_y, 0);
    else al_draw_bitmap(badge[3],badge_x, badge_y, 0);

    //al_draw_bitmap(times_up, 350, 200, 0);
    //al_draw_text(font1, al_map_rgb(153,0,0), 360, 380, 0, earned);
    //al_draw_text(font2, al_map_rgb(153,0,0), 50, 50, 0, "PLAY AGAIN");
    //al_draw_text(font2, al_map_rgb(153,0,0), 470, 550, 0, highest);

}
void end_scene_destroy(){
   // al_destroy_bitmap(end_background);
    //al_destroy_bitmap(times_up);
    al_destroy_font(font1);
    al_destroy_font(font2);
    for (int i = 0; i < 4; i++) al_destroy_bitmap(badge[i]);
}
