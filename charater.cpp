#include "charater.h"


ALLEGRO_BITMAP *cust_imgs[8][2];
ALLEGRO_BITMAP *chara_img[3];
ALLEGRO_BITMAP *cust_msg[8];
ALLEGRO_BITMAP *cust_heart[3];
ALLEGRO_BITMAP *chara_food[6];
ALLEGRO_BITMAP *potion[2];
ALLEGRO_BITMAP *plate[1];

ALLEGRO_SAMPLE_INSTANCE *burp_sound;
ALLEGRO_SAMPLE_INSTANCE *eat_sound;
ALLEGRO_SAMPLE_INSTANCE *zombie_groan;
ALLEGRO_SAMPLE_INSTANCE *drink_sound;
ALLEGRO_SAMPLE_INSTANCE *cash_sound;
ALLEGRO_SAMPLE_INSTANCE *ding_sound;


ALLEGRO_SAMPLE *sample = NULL;
ALLEGRO_SAMPLE *sample1 = NULL;
ALLEGRO_SAMPLE *sample2 = NULL;
ALLEGRO_SAMPLE *sample3 = NULL;
ALLEGRO_SAMPLE *sample4 = NULL;
ALLEGRO_SAMPLE *sample5 = NULL;
ALLEGRO_SAMPLE *sample6 = NULL;


ALLEGRO_FONT * fontDisplay = NULL;

int mouse_x, mouse_y; // mouse position
int cust_walk = 15;
int cust_speed = 5;
int chara_speed;
int chara_default_speed= 2;
int price_heart[3] = {50, 30, 5};
int cust_gen_speed = 150;
int patience = 900;
int heart_x[4] = {20, 130, 1150, 1050};
int heart_y[4] = {200, 450, 240, 450};
int ani_time = 0;
int ani = 50;
int potion_ct;
int sound_ct = 0;
// the state of character

enum cust_state {STANDBY, WALK, WAIT};
enum reaction {DISH, MONEY, LEAVE, LEAVE2, NONE, POTION};
enum food_state{order, F1, F2, F3, F4, F5, F6, pay, gift}; // food state: 0 - no food or order, 1 - 5 carry food, 6 - money, 7 - potion, 8 - food
enum path_type{MV0, MV1, MV2, MV3, MV4, MV5, MV6, MV7, MV8, MV9, MV10}; //path type
enum {STOP = 0, MOVE, ATK, L, R};
enum customer_path{L0, L1, L2, L3, L4}; // L0: stop
enum heart{FULL, HALF, EMPTY};


typedef struct {
    int x1, y1, x2, y2;
} button;

button tables[10] = {
{200, 260, 360, 380}, {170, 440, 330, 540}, {950, 250, 1110, 350}, {980, 420, 1170, 540},
// table1                 2                   3                       4
{330, 100, 510, 270}, {540, 90, 720, 270}, {770, 90, 960, 260}, { 420, 480, 560, 620}, {600, 510, 740, 610}, {780, 510, 930, 610}
}; // 5                  6                           7                   8                   9                   10

int clickButton (button b, int x, int y) {
    if (x >= b.x1 && x <= b.x2 && y >= b.y1 && y <= b.y2) return 1;
    else return 0;
}

int genFood()  {
    int r = rand()%6;
    return r + 1;
}

int reaction( int chara, int cust) {
    if (cust == order) return DISH;
    else if (cust == pay) {
        return LEAVE;
    }
    else if (cust == gift) {
        chara_speed *= 2;
        return LEAVE2;
    }
    else if (chara == cust) {
      if (cur_time_game > time_max *.6 && potion_ct < 1) {
            potion_ct++;
            return POTION;
      }
      else return MONEY;

    }
    else if (chara != cust) return NONE;

}

typedef struct {
    int facing_dir;
    int cust_path;
    int food_state;
    int state;
    int path;
    int wait_time;
    bool path_dir; // true - in, false - out
    int heart;
    int id;
    int skin_id;
    int x, y;
    int msg_x, msg_y;
    int wait_x, wait_y;
    int walk_time;

} Customer;

Customer customer1;
Customer customer2;
Customer customer3;
Customer customer4;
typedef struct
{
    int chara_move; // path type (map to enum move type)
    bool move_dir; // true = moving towards object; false = away from
    int food_state; // carry food state

    int init_x, init_y;
    int x, y; // the position of image
    int width, height; // the width and height of image
    bool dir; // left: false, right: true
    int state; // the state of character
    /*
    ALLEGRO_BITMAP *img_move[4];
    ALLEGRO_BITMAP *img_atk[2];
    ALLEGRO_SAMPLE_INSTANCE *atk_Sound;
    */
    int anime; // counting the time of animation
    int anime_time; // indicate how long the animation
}Character;


Character chara;
void load_image() {
    for (int i = 0; i < 3; i++) {
        char tmp[50];
        sprintf(tmp, "./image17/chara%d.png", i+1);
        chara_img[i] = al_load_bitmap(tmp);
    }

    for (int i = 0; i < 8; i++) {
        char tmp[50];
        sprintf(tmp, "./image17/msg%d.png", i );
        cust_msg[i] = al_load_bitmap(tmp);
    }
    for (int i = 0; i < 6; i++ ) {
        char tmp[50];
        sprintf(tmp, "./image17/food%d.png", i + 1);
        chara_food[i] = al_load_bitmap(tmp);
    }
     for (int i = 0; i < 3; i++ ) {
        char tmp[50];
        sprintf(tmp, "./image17/heart%d.png", i + 1);
        cust_heart[i] = al_load_bitmap(tmp);
    }

     // load character images
   /* for(int i = 1 ; i <= 4 ; i++){
        char temp[50];
        sprintf( temp, "./image17/Me%d.png", i );
        chara.img_move[i-1] = al_load_bitmap(temp);
    }
    for(int i = 1 ; i <= 2 ; i++){
        char temp[50];
        sprintf( temp, "./image17/Me%d.png", i );
        chara.img_atk[i-1] = al_load_bitmap(temp);
    }
    */

    for (int i = 0; i < 2; i++) {
        char tmp[50];
        sprintf(tmp, "./image17/potion1-%d.png", i+1);
        potion[i] = al_load_bitmap(tmp);
    }

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 2; j++ ) {
            char tmp[50];
            sprintf(tmp, "./image17/cust%d-%d.png", i+1, j+1);
            cust_imgs[i][j] = al_load_bitmap(tmp);
        }
    }

    for (int i = 0; i < 1; i++) {
        char tmp[50];
        sprintf(tmp, "./image17/plate%d.png", i+1);
        plate[i] = al_load_bitmap(tmp);
    }



}
void load_coor() {
    chara.init_x =620;
    chara.init_y = 310;
    chara.x = chara.init_x;
    chara.y = chara.init_y;

    customer1.wait_x = 80;
    customer1.wait_y = 250;
    customer1.msg_x = tables[0].x1;
    customer1.msg_y = tables[0].y1 - 100;

    customer2.wait_x = 110;
    customer2.wait_y = 400;
    customer2.msg_x = tables[1].x1 + 30;
    customer2.msg_y = tables[1].y1 - 100;

    customer3.wait_x = 1150;
    customer3.wait_y = 200;
    customer3.msg_x = tables[2].x1 + 90;
    customer3.msg_y = tables[2].y1 - 100;

    customer4.wait_x = 1130;
    customer4.wait_y = 400;
    customer4.msg_x = 1030;
    customer4.msg_y = 350;

    customer1.x = -300;
    customer1.y = customer1.wait_y;


    customer2.x = -400;
    customer2.y = customer2.wait_y;

    customer3.x = 1400;
    customer3.y = customer3.wait_y;


   customer4.x = 1400;
    customer4.y = customer4.wait_y;

    /*
    for (int i = 0; i < 4; i++) {
        custs[i].x = init_x[i];
        custs[i].y = wait_y[i];
    }
    */
}
void load_sound() {
  // load effective sound
/*    sample = al_load_sample("./sound/atk_sound.wav");
    chara.atk_Sound  = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(chara.atk_Sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(chara.atk_Sound, al_get_default_mixer());
    */

    sample1 = al_load_sample("./sound/burp_sound.wav");
    burp_sound  = al_create_sample_instance(sample1);
    al_set_sample_instance_playmode(burp_sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(burp_sound, al_get_default_mixer());

    sample2 = al_load_sample("./sound/eating_sound.wav");
    eat_sound  = al_create_sample_instance(sample2);
    al_set_sample_instance_playmode(eat_sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(eat_sound, al_get_default_mixer());

     sample3 = al_load_sample("./sound/zombie_groan.wav");
    zombie_groan  = al_create_sample_instance(sample3);
    al_set_sample_instance_playmode(zombie_groan, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(zombie_groan, al_get_default_mixer());

     sample4 = al_load_sample("./sound/drink_sound.wav");
    drink_sound  = al_create_sample_instance(sample4);
    al_set_sample_instance_playmode(drink_sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(drink_sound, al_get_default_mixer());

      sample5 = al_load_sample("./sound/cash.wav");
    cash_sound  = al_create_sample_instance(sample5);
    al_set_sample_instance_playmode(cash_sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(cash_sound, al_get_default_mixer());

      sample6 = al_load_sample("./sound/ding.wav");
    ding_sound  = al_create_sample_instance(sample6);
    al_set_sample_instance_playmode(ding_sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(ding_sound, al_get_default_mixer());
}
void cust_init (Customer *c){
    c->wait_time = 0;
    int t = rand() % 4;
    c->skin_id = t;
    c->state = STANDBY;
    c->path_dir = true;
    c->food_state = order;
    c->walk_time = 0;

}
void character_init(){
    load_image();
    load_sound();
    fontDisplay = al_load_ttf_font("./font/old.ttf", 40, 0);

    srand(time(NULL));
    cur_score = 0;

    // initial the geometric information of character
 /*   chara.width = al_get_bitmap_width(chara.img_move[0]);
    chara.height = al_get_bitmap_height(chara.img_move[0]);

    */

    cust_init(&customer1);
    cust_init(&customer2);
    cust_init(&customer3);
    cust_init(&customer4);

   load_coor();

    customer1.facing_dir = R;
    customer2.facing_dir = R;
    customer3.facing_dir = L;
    customer4.facing_dir = L;

    chara.dir = false;
    chara.state = L;
    chara.chara_move = MV0;
    chara.food_state = order;
    chara_speed = chara_default_speed;

    potion_ct = 0;
    missed_cust = 0;

    // initial the animation component
   // chara.state = STOP;
    chara.anime = 0;
    chara.anime_time = 30;

}

void custGo(int r) {

    if (r == 1) {
        if (customer1.state == STANDBY) {
            customer1.state = WALK;
        }
    }
    else if (r == 2) {
        if (customer2.state == STANDBY) {
            customer2.state = WALK;
        }
    }
    else if (r == 3) {
        if (customer3.state == STANDBY) {
            customer3.state = WALK;
        }
    }
    else if (r == 0) {
        if (customer4.state == STANDBY) {
            customer4.state = WALK;
        }
    }

}
void character_process(ALLEGRO_EVENT event){
    // process the animation
    if( event.type == ALLEGRO_EVENT_TIMER ){
        if( event.timer.source == fps ){
            chara.anime++;
            chara.anime %= chara.anime_time;
        }
    // process the keyboard event
    }else if( event.type == ALLEGRO_EVENT_KEY_DOWN ){
        key_state[event.keyboard.keycode] = true;
        chara.anime = 0;
    }else if( event.type == ALLEGRO_EVENT_KEY_UP ){
        key_state[event.keyboard.keycode] = false;
    }


     // check if click mouse & determine path

        if (cur_time_game % cust_gen_speed == 1) {
            int r = rand()%4;
            custGo(r);
        }
        if (cur_time_game % 1000 == 500) al_play_sample_instance((zombie_groan));

     if (chara.chara_move == MV0) {
        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
        mouse_x = event.mouse.x;
        mouse_y = event.mouse.y;
        printf("%d %d\n", mouse_x, mouse_y);
        for (int i = 1; i <= 10; i++) {
            if (clickButton(tables[i-1], mouse_x, mouse_y)) {
                chara.move_dir = true;
                chara.chara_move = i;
                printf("MV%d\n", chara.chara_move);
                break;
            }
        }
        }
     }
}
void cust_update(Customer *c) {
    if (c->state == WALK) {
        c->walk_time++;
        c->walk_time %= cust_walk;
        if (c->path_dir) {
            if (c->facing_dir == R) {
                c->x += cust_speed;
                if (c->x > c->wait_x) {
                c->path_dir = false;
                c->facing_dir = L;
                c->state = WAIT;
                }

            }
            else if (c->facing_dir == L) {
                c->x -= cust_speed;
                if (c->x < c->wait_x) {
                    c->path_dir = false;
                    c->facing_dir = R;
                    c->state = WAIT;
                }
            }
        }

        else {
            if (c->facing_dir == L) {
                c->x -= cust_speed;
                if (c->x < -100) {
                    c->path_dir = true;
                    c->facing_dir = R;
                    c->state = STANDBY;
                }
            }
            else if (c->facing_dir == R) {
                c->x += cust_speed;
                if (c->x > 1400) {
                    c->path_dir = true;
                    c->facing_dir = L;
                    c->state = STANDBY;
                }
            }
        }
    }
    else if (c->state == STANDBY) {
        cust_init(c);
    }
    else if (c->state == WAIT && c->food_state != pay && c->food_state != gift) {
        c->wait_time++;
        if (c->wait_time < patience * .5) c->heart = FULL;
        else if (c->wait_time < patience * .9) c->heart = HALF;
        else c->heart = EMPTY;

        if (c->wait_time > patience) {
            c->state = WALK;
            c->food_state = order;
            missed_cust ++;
        }
    }
}


void serve (Customer *c) {
    if (c->state == WAIT) {
        int react = reaction(chara.food_state, c->food_state);
        if (react == DISH) {
            c->food_state = genFood();
        }
        else if (react == MONEY) {
            if (c->food_state == F4 ) al_play_sample_instance(drink_sound);
            else  al_play_sample_instance(eat_sound);
            chara.food_state = order;
            c->food_state =  pay;
        }
        else if (react == POTION) {
            chara.food_state = order;
            c->food_state = gift;
        }
        else if (react == LEAVE) {
            if (c->heart == FULL) {
                al_play_sample_instance(cash_sound);
            }
            else if (c->heart == HALF) al_play_sample_instance(ding_sound);
            cur_score += price_heart [c->heart];
          //  total_score += price_heart[c->heart];
            if (cur_score > max_score) max_score = cur_score;
            c->food_state = order;
            c->state = WALK;
            if (sound_ct) {
                al_play_sample_instance(burp_sound);
                sound_ct = 1;
            } else sound_ct = 0;
        }
        else if(react == LEAVE2) {
            al_play_sample_instance(drink_sound);
            c->food_state = order;
            c->state = WALK;
        }

    }
}
void character_update(){

    // use the idea of finite state machine to deal with different state
    if( key_state[ALLEGRO_KEY_W] ){
        chara.y -= 5;
        chara.state = MOVE;
    }else if( key_state[ALLEGRO_KEY_A] ){
        chara.dir = false;
        chara.x -= 5;
        chara.state = MOVE;
    }else if( key_state[ALLEGRO_KEY_S] ){
        chara.y += 5;
        chara.state = MOVE;
    }else if( key_state[ALLEGRO_KEY_D] ){
        chara.dir = true;
        chara.x += 5;
        chara.state = MOVE;
    }else if( key_state[ALLEGRO_KEY_SPACE] ){
        chara.state = ATK;
    }else if( chara.anime == chara.anime_time-1 ){
        chara.anime = 0;
        chara.state = STOP;
    }else if ( chara.anime == 0 ){
        chara.state = STOP;
    }


    cust_update(&customer1);
    cust_update(&customer2);
    cust_update(&customer3);
    cust_update(&customer4);

// move chara
    if (chara.chara_move == MV0) {
        chara.x = chara.init_x;
        chara.y = chara.init_y;
        chara.state = R;
    }
    else if (chara.chara_move == MV1) {
        if (chara.move_dir) {
            chara.state = L;
            chara.x -= 5 * chara_speed;
            chara.y -= 1 * chara_speed;
            if (chara.x < 300) {
                serve(&customer1);
                chara.move_dir = false;
            }
        }
        else {
            chara.state = R;
            chara.x += 5 * chara_speed;
            chara.y += 1 * chara_speed;
            if (chara.x > chara.init_x)
                chara.chara_move = MV0;
        }
    }

    else if (chara.chara_move == MV2) {
        if (chara.move_dir) {
            chara.state = L;
            chara.x -= 5 * chara_speed;
            chara.y += 1 * chara_speed;
            if (chara.x < 330) {
                serve(&customer2);
                chara.move_dir = false;
            }
        }
        else {
            chara.state = R;
            chara.x += 5 * chara_speed;
            chara.y -= 1 * chara_speed;
            if (chara.x > 600)
                chara.chara_move = MV0;
        }


    }
    else if (chara.chara_move == MV3) {
        if (chara.move_dir) {
            chara.state = R;
            chara.x += 5 * chara_speed;
            chara.y -= 1 * chara_speed;
            if (chara.x > 900) {
                serve(&customer3);
                chara.move_dir = false;
            }

        }
        else {
            chara.state = L;
            chara.x -= 5 * chara_speed;
            chara.y += 1 * chara_speed;
            if (chara.x < chara.init_x)
                chara.chara_move = MV0;

        }

    }
    else if (chara.chara_move == MV4) {
        if (chara.move_dir) {
            chara.state = R;
            chara.x += 5 * chara_speed;
            chara.y += 1 * chara_speed;
            if (chara.x > 900){
                serve(&customer4);
                chara.move_dir = false;
            }
        }
        else {
            chara.state = L;
            chara.x -= 5 * chara_speed;
            chara.y -= 1 * chara_speed;
            if (chara.x < chara.init_x)
                chara.chara_move = MV0;

        }

    }
    else if (chara.chara_move == MV5) {
        if (chara.move_dir) {
            chara.state = L;
            chara.x -= 4 * chara_speed;
            chara.y -= 3 * chara_speed;
            if (chara.x < 400)
                chara.move_dir = false;
        }
        else {
            chara.state = R;
            chara.food_state = F1;
            chara.x += 4 * chara_speed;
            chara.y += 3 * chara_speed;
            if (chara.x > 600)
                chara.chara_move = MV0;
        }

    }
    else if (chara.chara_move == MV6) {
        if (chara.move_dir) {
            chara.state = R;
            //chara.x += 4 * chara_speed;
            chara.y -= 4 * chara_speed;
            if (chara.y < 150)
                chara.move_dir = false;
        }
        else {
            chara.state = L;
            chara.food_state = F2;
            //chara.x -= 4 * chara_speed;
            chara.y += 4 * chara_speed;
            if (chara.y > chara.init_y)
                chara.chara_move = MV0;
        }

    }
    else if (chara.chara_move == MV7) {
       /*
        if (chara.move_dir) {
            chara.state = L;
            chara.x -= 3 * chara_speed;
            chara.y += 2 * chara_speed;
            if (chara.y > 400)
                chara.move_dir = false;
        }
        else {
            chara.state = R;
            chara.food_state = F3;
            chara.x += 3 * chara_speed;
            chara.y -= 2 * chara_speed;
            if (chara.y < 350)
                chara.chara_move = MV0;
        }
        */
         if (chara.move_dir) {
            chara.state = R;
            chara.x += 4 * chara_speed;
            chara.y -= 4 * chara_speed;
            if (chara.x > 770)
                chara.move_dir = false;
        }
        else {
            chara.state = L;
            chara.food_state = F3;
            chara.x -= 4 * chara_speed;
            chara.y += 4 * chara_speed;
            if (chara.x < 600)
                chara.chara_move = MV0;
        }

    }
    else if (chara.chara_move == MV8) {
            /*
        if (chara.move_dir) {
            chara.state = L;
            chara.y += 4 * chara_speed;
            if (chara.y > 400)
                chara.move_dir = false;
        }
        else {
             chara.state = R;
            chara.food_state = F1;
            chara.y -= 4 * chara_speed;
            if (chara.y < 350)
                chara.chara_move = MV0;
        }
        */
        if (chara.move_dir) {
            chara.state = L;
            chara.x -= 3 * chara_speed;
            chara.y += 2 * chara_speed;
            if (chara.y > 370)
                chara.move_dir = false;
        }
        else {
            chara.state = R;
            chara.food_state = F4;
            chara.x += 3 * chara_speed;
            chara.y -= 2 * chara_speed;
            if (chara.y < chara.init_y)
                chara.chara_move = MV0;
        }
    }
    else if (chara.chara_move == MV9) {
        /*
        if (chara.move_dir) {
            chara.state = R;
            chara.x += 4 * chara_speed;
            chara.y += 3 * chara_speed;
            if (chara.y > 420)
                chara.move_dir = false;
        }
        else {
            chara.state = L;
            chara.food_state = F5;
            chara.x -= 4 * chara_speed;
            chara.y -= 3 * chara_speed;
            if (chara.y < 350)
                chara.chara_move = MV0;
        }
        */
         if (chara.move_dir) {
            chara.state = L;
            chara.y += 4 * chara_speed;
            if (chara.y > 400)
                chara.move_dir = false;
        }
        else {
             chara.state = R;
            chara.food_state = F5;
            chara.y -= 4 * chara_speed;
            if (chara.y < chara.init_y)
                chara.chara_move = MV0;
        }

    }
    else if (chara.chara_move == MV10) {
        /*
        if (chara.move_dir) {
            chara.state = R;
            chara.x += 5 * chara_speed;
            chara.y -= 3 * chara_speed;
            if (chara.x > 1000)
                chara.move_dir = false;
        }
        else {
            chara.state = L;
            chara.food_state = F0;
            chara.x -= 5 * chara_speed;
            chara.y += 3 * chara_speed;
            if (chara.x < 600)
                chara.chara_move = MV0;
        }
        */
          if (chara.move_dir) {
            chara.state = R;
            chara.x += 4 * chara_speed;
            chara.y += 3 * chara_speed;
            if (chara.y > 410)
                chara.move_dir = false;
        }
        else {
            chara.state = L;
            chara.food_state = F6;
            chara.x -= 4 * chara_speed;
            chara.y -= 3 * chara_speed;
            if (chara.y < chara.init_y)
                chara.chara_move = MV0;
        }
    }

}
void customer_draw (Customer c) {
    if (c.state == WALK) {
        if (c.facing_dir == L) {
            if (c.walk_time < cust_walk / 2) al_draw_bitmap(cust_imgs[c.skin_id][0], c.x, c.y, ALLEGRO_FLIP_HORIZONTAL);
            else al_draw_bitmap(cust_imgs[c.skin_id][1], c.x, c.y, ALLEGRO_FLIP_HORIZONTAL);

        }
        else if (c.facing_dir == R) {
             if (c.walk_time < cust_walk / 2) al_draw_bitmap(cust_imgs[c.skin_id][0], c.x, c.y, 0);
            else al_draw_bitmap(cust_imgs[c.skin_id][1], c.x, c.y, 0);

        }

    }
    else if (c.state == WAIT) {
        if (c.facing_dir == L) {
         al_draw_bitmap(cust_imgs[c.skin_id][0], c.x, c.y, 0);
         if(c.food_state != gift) {
            al_draw_bitmap(cust_msg[ c.food_state ], c.msg_x, c.msg_y, ALLEGRO_FLIP_HORIZONTAL);
            if (c.food_state != pay) {
                al_draw_bitmap(cust_heart[ c.heart ], c.wait_x + 30, c.wait_y - 50, ALLEGRO_FLIP_HORIZONTAL);
            }
         } else {
            al_draw_bitmap(potion[0], c.msg_x, c.msg_y, ALLEGRO_FLIP_HORIZONTAL );
         }
        }

        else if (c.facing_dir == R) {
            al_draw_bitmap(cust_imgs[c.skin_id][0], c.x, c.y, ALLEGRO_FLIP_HORIZONTAL);
           if (c.food_state != gift) {
            al_draw_bitmap(cust_msg[ c.food_state ],  c.msg_x, c.msg_y, 0);
            if (c.food_state != pay) {
                al_draw_bitmap(cust_heart[ c.heart ], c.wait_x + 30, c.wait_y - 50, ALLEGRO_FLIP_HORIZONTAL);
            }
           } else {
                al_draw_bitmap(potion[0], c.msg_x, c.msg_y, 0);
         }
        }
    }
}

void character_draw(){
/*
    char revenue_str[20];
    sprintf(revenue_str, "revenue: %4d$", revenue);
    REVENUE = revenue;
    char time_str[20];
    sprintf(time_str, "time: %d", (time_max - cur_time_game) / 60);

    al_draw_text(fontDisplay, al_map_rgb(153,0,0), 20, 20, 0, revenue_str);
    al_draw_text(fontDisplay, al_map_rgb(153,0,0), 20, 60, 0, time_str);
    */
    // with the state, draw corresponding image
    /*
    if( chara.state == STOP ){
        if( chara.dir )
            al_draw_bitmap(chara.img_move[0], chara.x, chara.y, ALLEGRO_FLIP_HORIZONTAL);
        else
            al_draw_bitmap(chara.img_move[0], chara.x, chara.y, 0);
    }else if( chara.state == MOVE ){
        if( chara.dir ){
            if( chara.anime < chara.anime_time/2 ){
                al_draw_bitmap(chara.img_move[0], chara.x, chara.y, ALLEGRO_FLIP_HORIZONTAL);
            }else{
                al_draw_bitmap(chara.img_move[1], chara.x, chara.y, ALLEGRO_FLIP_HORIZONTAL);
            }
        }else{
            if( chara.anime < chara.anime_time/2 ){
                al_draw_bitmap(chara.img_move[0], chara.x, chara.y, 0);
            }else{
                al_draw_bitmap(chara.img_move[1], chara.x, chara.y, 0);
            }
        }
    }else if( chara.state == ATK ){
        if( chara.dir ){
            if( chara.anime < chara.anime_time/2 ){
                al_draw_bitmap(chara.img_atk[0], chara.x, chara.y, ALLEGRO_FLIP_HORIZONTAL);
            }else{
                al_draw_bitmap(chara.img_atk[1], chara.x, chara.y, ALLEGRO_FLIP_HORIZONTAL);
                al_play_sample_instance(chara.atk_Sound);
            }
        }else{
            if( chara.anime < chara.anime_time/2 ){
                al_draw_bitmap(chara.img_atk[0], chara.x, chara.y, 0);
            }else{
                al_draw_bitmap(chara.img_atk[1], chara.x, chara.y, 0);
                al_play_sample_instance(chara.atk_Sound);
            }
        }
    }
    */

// draw customer

    customer_draw(customer1);
    customer_draw(customer2);
    customer_draw(customer3);
    customer_draw(customer4);

// draw waiter
    if (chara.chara_move == MV0) {
        if (chara.state == L) {
             if (chara.food_state != order) {
                al_draw_bitmap(chara_img[1], chara.x, chara.y, ALLEGRO_FLIP_HORIZONTAL);
                al_draw_bitmap(plate[0], chara.x - 50, chara.y + 40, ALLEGRO_FLIP_HORIZONTAL);
                al_draw_bitmap(chara_food[ chara.food_state - 1 ], chara.x - 40, chara.y - 10, 0);
             }
             else al_draw_bitmap(chara_img[0], chara.x, chara.y, ALLEGRO_FLIP_HORIZONTAL);
        }
        else if (chara.state == R) {
            if (chara.food_state != order) {
                al_draw_bitmap(chara_img[1], chara.x, chara.y, 0);
                al_draw_bitmap(plate[0], chara.x + 50, chara.y + 40, ALLEGRO_FLIP_HORIZONTAL);
                if (chara.food_state == F4) al_draw_bitmap(chara_food[ chara.food_state - 1 ], chara.x + 70, chara.y - 10, ALLEGRO_FLIP_HORIZONTAL);
                else al_draw_bitmap(chara_food[ chara.food_state - 1 ], chara.x + 50, chara.y - 10, ALLEGRO_FLIP_HORIZONTAL);
            }
            else al_draw_bitmap(chara_img[0], chara.x, chara.y, 0);
        }
    }

    else if (chara.state == L) {
        if (cur_time_game % 10 < 5) al_draw_bitmap(chara_img[1], chara.x, chara.y, ALLEGRO_FLIP_HORIZONTAL);
        else  al_draw_bitmap(chara_img[2], chara.x, chara.y, ALLEGRO_FLIP_HORIZONTAL);
        if (chara.food_state != order) {
            al_draw_bitmap(plate[0], chara.x - 50, chara.y + 40, ALLEGRO_FLIP_HORIZONTAL);
             al_draw_bitmap(chara_food[ chara.food_state - 1 ], chara.x - 40, chara.y - 10, 0);
        }
    }
     else if (chara.state == R) {
        if (cur_time_game % 10 < 5) al_draw_bitmap(chara_img[1], chara.x, chara.y, 0);
        else  al_draw_bitmap(chara_img[2], chara.x, chara.y, 0);
        if (chara.food_state != order) {
            al_draw_bitmap(plate[0], chara.x + 50, chara.y + 40, ALLEGRO_FLIP_HORIZONTAL);
            if (chara.food_state == F4)  al_draw_bitmap(chara_food[ chara.food_state - 1 ], chara.x + 70, chara.y - 10, ALLEGRO_FLIP_HORIZONTAL);
            else al_draw_bitmap(chara_food[ chara.food_state - 1 ], chara.x + 50, chara.y - 10, ALLEGRO_FLIP_HORIZONTAL);
        }
    }

}
void character_destory(){
   /*
    al_destroy_bitmap(chara.img_atk[0]);
    al_destroy_bitmap(chara.img_atk[1]);
    al_destroy_sample_instance(chara.atk_Sound);
    for (int i = 0; i < 4; i++) {
        al_destroy_bitmap(chara.img_move[i]);
    }
*/

    al_destroy_sample_instance(burp_sound);
    al_destroy_sample_instance(drink_sound);
    al_destroy_sample_instance(eat_sound);
    al_destroy_sample_instance(zombie_groan);

    for (int i = 0; i < 2; i++) {
        al_destroy_bitmap(potion[i]);
    }

    for (int i = 0; i < 5; i++) {
        al_destroy_bitmap(chara_food[i]);
    }

   for(int i = 0; i < 7; i ++) {
    al_destroy_bitmap(cust_msg[i]);
   }
   for (int i = 0; i < 3; i++) {
    al_destroy_bitmap(cust_heart[i]);
   }

   for(int i = 0; i < 5; i++) {
    for (int j = 0; j < 2; j++) {
        al_destroy_bitmap(cust_imgs[i][j]);
    }
   }

}
