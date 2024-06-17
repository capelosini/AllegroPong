#include <allegro5/allegro.h>

typedef struct Player Player;
struct Player{
    int x;
    int y;
    float speed;
    int size[2];
    char direction;
    int points;
    ALLEGRO_COLOR color;    
};

typedef struct Ball Ball;
struct Ball{
    int x;
    int y;
    int size[2];
    char direction[2];
    float speed;
    ALLEGRO_COLOR color;
};