#include "ai.h"
#define MAX 10000
const static int dx[] = {0, 1, 0, -1};
const static int dy[] = {1, 0, -1, 0};
int EatFood=0;
int NearEnemy=0;
int OccupiedDirection=0;
typedef struct{
    int x;
    int y;
    int pre;
    char dir;
    int t;
}point;
typedef struct{
	point data[10000];
    int front,rear;
}Queue;
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

direction_t get_action1(game_t* game) {

    int dir[4][2]={{1,0},{-1,0},{0,1},{0,-1}};
    char x[4]={'S','N','E','W'};
    int visited[game->board->rows+1][game->board->cols+1];
    
    for(int i=0;i<=game->board->rows;i++)
        for(int j=0;j<=game->board->rows;j++)
            visited[i][j]=0;
    Queue *q;
    point p;
	p.x=game->snake1->head->row;p.y=game->snake1->head->col;p.pre=-1;
    if(NearEnemy!=0)
	{
		if(p.x==game->snake2->head->row+1&&p.y==game->snake2->head->col+1&&NearEnemy==1)
			if(*board_cell(game->board,p.x-1,p.y)==CELL_OPEN)
			return NORTH;
        if(p.x==game->snake2->head->row+1&&p.y==game->snake2->head->col-1&&NearEnemy==2)
			if(*board_cell(game->board,p.x-1,p.y)==CELL_OPEN)
			return NORTH;
		if(p.x==game->snake2->head->row-1&&p.y==game->snake2->head->col+1&&NearEnemy==1)
			if(*board_cell(game->board,p.x+1,p.y)==CELL_OPEN)
			return SOUTH;
		if(p.x==game->snake2->head->row-1&&p.y==game->snake2->head->col-1&&NearEnemy==2)
			if(*board_cell(game->board,p.x+1,p.y)==CELL_OPEN)
			return SOUTH;
		if(p.x==game->snake2->head->row+1&&p.y==game->snake2->head->col+1&&NearEnemy==3)
			if(*board_cell(game->board,p.x,p.y-1)==CELL_OPEN)
			return WEST;
		if(p.x==game->snake2->head->row-1&&p.y==game->snake2->head->col+1&&NearEnemy==4)
			if(*board_cell(game->board,p.x,p.y-1)==CELL_OPEN)
			return WEST;
		if(p.x==game->snake2->head->row+1&&p.y==game->snake2->head->col+1&&NearEnemy==3)
			if(*board_cell(game->board,p.x,p.y+1)==CELL_OPEN)
			return EAST;
		if(p.x==game->snake2->head->row-1&&p.y==game->snake2->head->col+1&&NearEnemy==4)
			if(*board_cell(game->board,p.x,p.y+1)==CELL_OPEN)
			return EAST;
	}
    q=init_Queue();
    
   // printf("%d %d\n",p.x,p.y);
    en_Queue_back(q,p);

    visited[p.x][p.y]=1;
	
	//printf("InWhile");
	NearEnemy=0;
//printf("iro\n");
    empty_Queue(q);
    int cnt=1;
    //game->board->game->snake->tail->row,game->snake->col)=CELL_FOOD;
    if(game->snake1->head!=game->snake1->tail&&EatFood==0)
		EatFood=1;
	//printf("%d-%d ",OccupiedDirection,EatFood);
	int AimAttackX=game->snake2->head->row,AimAttackY=game->snake2->head->col;
    while(!empty_Queue(q)&&cnt){


        point temp;
        temp=de_Queue(q);
        if(*board_cell(game->board,temp.x,temp.y)==CELL_FOOD&&EatFood==0)
        {
            point tem=temp;
            char d;
            int i=0;
            while(tem.pre!=-1){
                d=tem.dir;
                tem=q->data[tem.pre];
				//d=tem.dir;
                i++;
            }
            switch(d){
                case 'W':return WEST;
                case 'N':return NORTH;
                case 'S':{return SOUTH;}
                case 'E':return EAST;

            }
            //break;
        }
		if(EatFood!=0)
		{
			//printf("InAttackWay");
			
CheckAgain:
			switch(OccupiedDirection)
			{
				case 0:
					
					//AimAttackY+=1;
					if(*board_cell(game->board,AimAttackX-1,AimAttackY+1)!=CELL_OPEN)
					{
						OccupiedDirection=1;
						goto CheckAgain;
					}
					if(temp.y==AimAttackY+1&&(temp.x==AimAttackX-1))
					{
						//printf("C1");
						NearEnemy=2;
						point tem=temp;
            char d;
            int i=0;
            while(tem.pre!=-1){
                d=tem.dir;
                tem=q->data[tem.pre];
                //d=tem.dir;
				i++;
            }
            switch(d){
                case 'W':return WEST;
                case 'N':return NORTH;
                case 'S':{return SOUTH;}
                case 'E':return EAST;

            }
            //break;
					}
					break;
				case 1:
					//AimAttackY-=1;
					if(*board_cell(game->board,AimAttackX-1,AimAttackY-1)!=CELL_OPEN)
					{
						OccupiedDirection=2;
						goto CheckAgain;
					}
					if(temp.y==AimAttackY-1&&(temp.x==AimAttackX-1))
					{
						//printf("C2");
						NearEnemy=2;
						point tem=temp;
            char d;
            int i=0;
            while(tem.pre!=-1){
                d=tem.dir;
                tem=q->data[tem.pre];
                //d=tem.dir;
				i++;
            }
            switch(d){
                case 'W':return WEST;
                case 'N':return NORTH;
                case 'S':{return SOUTH;}
                case 'E':return EAST;

            }
            //break;
					}
					break;
				case 2:
					//AimAttackX+=1;
					if(*board_cell(game->board,AimAttackX+1,AimAttackY+1)!=CELL_OPEN)
					{
						OccupiedDirection=3;
						goto CheckAgain;
					}
					if(temp.x==AimAttackX+1&&(temp.y==AimAttackY+1))
					{
						//printf("C3");
						NearEnemy=3;
						point tem=temp;
            char d;
            int i=0;
            while(tem.pre!=-1){
                d=tem.dir;
                tem=q->data[tem.pre];
                //d=tem.dir;
				i++;
            }
            switch(d){
                case 'W':return WEST;
                case 'N':return NORTH;
                case 'S':{return SOUTH;}
                case 'E':return EAST;

            }
            //break;
					}
					break;
				case 3:
					//AimAttackX+=1;
				if(*board_cell(game->board,AimAttackX+1,AimAttackY-1)!=CELL_OPEN)
					{
						OccupiedDirection=0;
					}	
				if(temp.x==AimAttackX+1&&(temp.y==AimAttackY-1))
					{
						//printf("C4");
						NearEnemy=4;
						point tem=temp;
            char d;
            int i=0;
            while(tem.pre!=-1){
                d=tem.dir;
                tem=q->data[tem.pre];
                //d=tem.dir;
				i++;
            }
            switch(d){
                case 'W':return WEST;
                case 'N':return NORTH;
                case 'S':{return SOUTH;}
                case 'E':return EAST;

            }
            //break;
					}
					break;
			}
			//printf("out");
		}
        for(int i=0;i<4;i++)
        {
            point pp;
            pp.x=temp.x+dir[i][0];
            pp.y=temp.y+dir[i][1];
            if(visited[pp.x][pp.y]==0&&(*board_cell(game->board,pp.x,pp.y)==CELL_OPEN||(*board_cell(game->board,pp.x,pp.y)==CELL_FOOD&&EatFood==0)))
            {
		if(pp.x==1||pp.x==game->board->rows-2||pp.y==1||pp.y==game->board->cols-2)
                      continue;
                pp.dir=x[i];
                visited[pp.x][pp.y]=1;
                pp.pre=q->front;
             //   if(*board_cell(game->board,pp.x,pp.y)==CELL_FOOD)printf("ppp:%d %d\n",pp.x,pp.y);
                en_Queue_back(q,pp);
            }
            else continue;

        }

    }
	//printf("rare%d",q->rear);
	/*   
   if(game->snake1->head->row==game->snake2->head->row&&game->snake1->head->col==game->snake2->head->col+1)
	   return WEST;
   if(game->snake1->head->row==game->snake2->head->row&&game->snake1->head->col==game->snake2->head->col-1)
	   return EAST;
   if(game->snake1->head->row==game->snake2->head->row+1&&game->snake1->head->col==game->snake2->head->col)
	   return NORTH;
   if(game->snake1->head->row==game->snake2->head->row-1&&game->snake1->head->col==game->snake2->head->col)
	   return SOUTH;*/
	free(q);
        for(int i=0;i<4;i++)
        {
            point pp;
            pp.x=p.x+dir[i][0];
            pp.y=p.y+dir[i][1];
            if(*board_cell(game->board,pp.x,pp.y)==CELL_OPEN)
            {
		if((pp.x==1||pp.x==game->board->rows-2)&&(pp.y==1||pp.y==game->board->cols-2))
                      continue;
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


