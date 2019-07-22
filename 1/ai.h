#ifndef AI_H
#define AI_H
#include "game.h"
#include "file.h"
typedef struct{
    int x;
    int y;
    int pre;
    char dir;
    int t;
}point;
typedef struct{
	point data[1000];
    int front,rear;
}Queue;
int empty_Queue(Queue *q);

point de_Queue(Queue*q);

void en_Queue_back(Queue *q, point e);

Queue* init_Queue(void);

direction_t get_action(game_t* game);

#endif // AI_H
