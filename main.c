#include <allegro5/allegro.h>
#include <allegro5/keyboard.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <math.h>
#include "./headers/structs.h"

#define displayWidth 1280
#define displayHeight 720
#define PI 3.1415926535

char gameLoop=1;
char bot=1;
int initPlayersSpeed=6;
int initBallSpeed=6;

void drawRect(int x, int y, int w, int h, ALLEGRO_COLOR color){
    for (int i=0; i<h; i++){
        for (int j=0; j<w; j++){
            al_draw_pixel(x+j, y+i, color);
        }
    }
}

void drawCircle(int x, int y, int r, ALLEGRO_COLOR color){
    for (int ang=0; ang<360; ang++){
        al_draw_pixel(cos(ang)*r+x, sin(ang)*r+y, color);
    }
}

int main(){
    al_init();
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_keyboard();

    ALLEGRO_DISPLAY *display = al_create_display(displayWidth, displayHeight);
    al_set_window_position(display, 20, 20);
    al_set_window_title(display, "Pong by Cravo");
    ALLEGRO_EVENT_QUEUE *ev_queue = al_create_event_queue();
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60);
    al_register_event_source(ev_queue, al_get_display_event_source(display));
    al_register_event_source(ev_queue, al_get_keyboard_event_source());
    al_register_event_source(ev_queue, al_get_timer_event_source(timer));
    al_start_timer(timer);
    ALLEGRO_FONT *scoreFont = al_load_ttf_font("./fonts/score.ttf", 50, 0);

    // START GAME STRUCTS
    Player playerOne = {30, displayHeight/2-25, initPlayersSpeed, {10, 50}, 0, 0, al_map_rgb(255,255,255)};
    Player playerTwo = {displayWidth-30-15, displayHeight/2-25, initPlayersSpeed, {10, 50}, 0, 0, al_map_rgb(255,255,255)};
    Ball ball = {displayWidth/2, displayHeight/2, {20, 20}, {1,1}, initBallSpeed, al_map_rgb(230,20,20)};

    while (gameLoop){
        do{
            ALLEGRO_EVENT ev;
            al_wait_for_event(ev_queue, &ev);
            if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                gameLoop=0;
                break;
            }
            else if (ev.type == ALLEGRO_EVENT_KEY_DOWN){
                switch (ev.keyboard.keycode){
                    case ALLEGRO_KEY_W:
                        playerOne.direction=-1;
                        break;
                    case ALLEGRO_KEY_S:
                        playerOne.direction=1;
                        break;
                    
                    case ALLEGRO_KEY_UP:
                        playerTwo.direction=-1;
                        break;
                    case ALLEGRO_KEY_DOWN:
                        playerTwo.direction=1;
                        break;
                    
                    default:
                        break;
                };
            } else if(ev.type == ALLEGRO_EVENT_KEY_UP){
                if (ev.keyboard.keycode == ALLEGRO_KEY_W || ev.keyboard.keycode == ALLEGRO_KEY_S){
                    playerOne.direction=0;
                }
                if (ev.keyboard.keycode == ALLEGRO_KEY_UP || ev.keyboard.keycode == ALLEGRO_KEY_DOWN){
                    playerTwo.direction=0;
                }
            }
        } while(!al_is_event_queue_empty(ev_queue));

        if (bot){
            if (ball.x > displayWidth/2+20 && ball.x < playerTwo.x){
                playerTwo.direction=ball.y+ball.size[1]/2 < playerTwo.y+playerTwo.size[1]/2 ? -1 : 1;
                // if (ball.y-ball.size[1]/2 > playerTwo.y+playerTwo.size[1]){
                //     playerTwo.direction=1;
                // } else if(ball.y-ball.size[1]/2 < playerTwo.y+playerTwo.size[1]){
                //     playerTwo.direction=-1;
                // } else{
                //     playerTwo.direction=0;
                // }
            } else{
                playerTwo.direction=0;
            }
        }

        // HANDLE MOVIMENTS
        playerOne.y+=playerOne.direction*playerOne.speed;
        playerTwo.y+=playerTwo.direction*playerTwo.speed;
        ball.x+=ball.direction[0]*ball.speed;
        ball.y+=ball.direction[1]*ball.speed;
        if (ball.x+ball.size[0]/2 >= displayWidth || ball.x-ball.size[0]/2 <= 0){
            ball.direction[0]*=-1;
            ball.speed=initBallSpeed;
            playerOne.speed=initPlayersSpeed;
            playerTwo.speed=initPlayersSpeed;
            if (ball.x-ball.size[0]/2<=0){
                ball.x=ball.size[0]/2+1;
                playerTwo.points++;
            } else{
                ball.x=displayWidth-ball.size[0]/2-1;
                playerOne.points++;
            }
        } if (ball.y+ball.size[0]/2 >= displayHeight || ball.y <= 0){
            ball.direction[1]*=-1;
        }

        char colPlayerOne=ball.x-ball.size[0]/2 <= playerOne.x+playerOne.size[0] && ball.x-ball.size[0]/2 >= playerOne.x && ball.y+ball.size[0]/2 >= playerOne.y && ball.y-ball.size[0]/2 <= playerOne.y+playerOne.size[1];
        char colPlayerTwo=ball.x+ball.size[0]/2 >= playerTwo.x && ball.x+ball.size[0]/2 <= playerTwo.x+playerTwo.size[0] && ball.y+ball.size[0]/2 >= playerTwo.y && ball.y-ball.size[0]/2 <= playerTwo.y+playerTwo.size[1];

        if (colPlayerOne){
            ball.x=playerOne.x+playerOne.size[0]+1+ball.size[0]/2;
            if (playerOne.direction!=0){
                ball.direction[1]=playerOne.direction;
            }
        } else if(colPlayerTwo){
            ball.x=playerTwo.x-1-ball.size[0]/2;
            if (playerTwo.direction!=0){
                ball.direction[1]=playerTwo.direction;
            }
        }

        if (colPlayerOne || colPlayerTwo){
            ball.direction[0]*=-1;
            ball.speed+=0.5;
            playerOne.speed+=0.5;
            playerTwo.speed+=0.5;
        }

        // CLEAR TO BLACK
        al_clear_to_color(al_map_rgb(0,0,0));
        // SCORE BOARD
        al_draw_textf(scoreFont, al_map_rgb(255, 255, 255), displayWidth/2, 20, ALLEGRO_ALIGN_CENTER, "%d   %d", playerOne.points, playerTwo.points);
        // PLAYER AND BALL
        drawRect(playerOne.x, playerOne.y, playerOne.size[0], playerOne.size[1], playerOne.color);
        drawRect(playerTwo.x, playerTwo.y, playerTwo.size[0], playerTwo.size[1], playerTwo.color);
        drawCircle(ball.x, ball.y, ball.size[0]/2, ball.color);
        // UPDATE DISPLAY
        al_flip_display();
    }

    al_destroy_display(display);
    al_destroy_event_queue(ev_queue);
    al_destroy_timer(timer);
    al_destroy_font(scoreFont);

    return 0;
}