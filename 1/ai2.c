#include "ai.h"
#define MAX 1000
Queue* init_Queue(void) {
	Queue*q;
	q=(Queue*)malloc(sizeof(Queue));
	q->front=q->rear=0;
	return q;
}
void en_Queue_back(Queue *q, point e) {

    q->rear=(q->rear+1)%MAX;
    q->data[q->rear]=e;
}
point de_Queue(Queue*q)
{
    point e;
    q->front=(q->front+1)%MAX;
    e=q->data[q->front];
    return e;
}
int empty_Queue(Queue *q) {
    if(q->front==q->rear) return 1;
    else return 0;
}
direction_t get_action(game_t* game) {

    int dir[4][2]={{1,0},{-1,0},{0,1},{0,-1}};
    char x[4]={'S','N','E','W'};
    int visited[game->board->rows+1][game->board->cols+1];
    for(int i=0;i<=game->board->rows;i++)
        for(int j=0;j<=game->board->rows;j++)
            visited[i][j]=0;
    Queue *q;
    point p;
    q=init_Queue();
    p.x=game->snake->head->row;p.y=game->snake->head->col;p.pre=-1;
   // printf("%d %d\n",p.x,p.y);
    en_Queue_back(q,p);

    visited[p.x][p.y]=1;
//printf("iro\n");
    empty_Queue(q);
    int cnt=0;
    //game->board->game->snake->tail->row,game->snake->col)=CELL_FOOD;
    for(int i=0;i<game->board->rows;i++){
        for(int j=0;j<game->board->cols;j++){
            if(*board_cell(game->board,i,j)==CELL_FOOD)
            {cnt++;}
        }
    }
    while(!empty_Queue(q)&&cnt){


        point temp;
        temp=de_Queue(q);
        //visited[temp.x][temp.y]=0;
        //printf("444\n");
        if(*board_cell(game->board,temp.x,temp.y)==CELL_FOOD)
        {
            //printf("vbbb\n");
            point tem=temp;
            //char path[100];
            char d;
            int i=0;
            while(tem.pre!=-1){
                d=tem.dir;
                tem=q->data[tem.pre];
                i++;
            }
            switch(d){
                case 'W':return WEST;
                case 'N':return NORTH;
                case 'S':{return SOUTH;}
                case 'E':return EAST;

            }
            //for(int j=i-1;j>=0;j--)printf("%c",path[j]);
            //printf("")
            break;
        }
        for(int i=0;i<4;i++)
        {
            point pp;
            pp.x=temp.x+dir[i][0];
            pp.y=temp.y+dir[i][1];
            if(visited[pp.x][pp.y]==0&&(*board_cell(game->board,pp.x,pp.y)==CELL_OPEN||*board_cell(game->board,pp.x,pp.y)==CELL_FOOD))
            {
                pp.dir=x[i];
                visited[pp.x][pp.y]=1;
                pp.pre=q->front;
             //   if(*board_cell(game->board,pp.x,pp.y)==CELL_FOOD)printf("ppp:%d %d\n",pp.x,pp.y);
                en_Queue_back(q,pp);
            }
            else continue;

        }

    }
   
    
        for(int i=0;i<4;i++)
        {
            point pp;
            pp.x=p.x+dir[i][0];
            pp.y=p.y+dir[i][1];
            if(*board_cell(game->board,pp.x,pp.y)==CELL_OPEN)
            {
                switch(i)
                {
                    case 0:return SOUTH;
                    case 1:return NORTH;
                    case 2:return EAST;
                    case 3:return WEST;
                }
            }
            else continue;
        }

 

}
