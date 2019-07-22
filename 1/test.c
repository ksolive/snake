#include <stdio.h>
#include<stdlib.h>
#include <time.h>
#include<unistd.h>
//#include<curses.h>
#define MAX 1000
typedef enum {
    CELL_OPEN,
    CELL_FOOD,
    CELL_WALL,
    CELL_SNAKE,
} cell_t;

typedef enum {
    EAST,
    SOUTH,
    WEST,
    NORTH,
} direction_t;
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


/*! A node of a snake (each node occupies a cell)
    This is like a node in a linked list. */
typedef struct SnakeNode {
    /* Row of the cell the node occupies */
    int row;
    /* Column of the cell the node occupies */
    int col;
    /* Pointer to the next node, in the direction of snake movement */
    struct SnakeNode *next;
} snake_node_t;

/*! Defines a snake, which is like a linked-list with the
    head and tail reversed. */
typedef struct Snake {
    /* The front node of the snake, or NULL if snake has no nodes */
    snake_node_t *head;
    /* The rear node of the snake, or NULL if snake has no nodes */
    snake_node_t *tail;
    /* The direction the snake head is going to move on the next iteration */
    direction_t heading;
    /* Snake growth counter indicating how many segments the snake should add */
    int growth;
} snake_t;

/*! Defines a board, which is a just a grid of cells. */
typedef struct Board {
    /* Number of rows in the grid */
    int rows;
    /* Number of columns in the grid */
    int cols;
    /* Pointer to the beginning of the board cell data.  The cells are organized
       in a row-major layout ( http://en.wikipedia.org/wiki/Row-major_order ) */
    cell_t *cells;
} board_t;

/*! Defines the game data structure, which consists of the board, the two
   snakes, and some game parameters. */
typedef struct Game {
    /* The game board */
    board_t *board;
    /* The human controlled snake */
    snake_t *snake;
    /* Time delay between each game iteration, in seconds */
    float time_step;
    /* Probability of one food cell appeared in each game iteration */
    float food_probability;
    /* Segments that snakes grow for each food cell that they eat */
    int growth_per_food;
} game_t;

/* create a board with `rows` * `cols` */
board_t *create_board(int rows, int cols);

/* free a board */
void destroy_board(board_t *board);

/* get cell in specific position */
cell_t *board_cell(board_t *board, int row, int col);

/* create a snake */
snake_t *create_snake(board_t *board, int row, int col, direction_t heading);

/* free a snake */
void destroy_snake(snake_t *snake);
void display(game_t *game);

/* add food to board with probalility */
void randomly_add_food(board_t *board, float probability);

/* set direction */
void set_direction(snake_t *snake, direction_t d);

int update_snake_head(snake_t *snake, board_t *board, int growth_per_food);
void update_snake_tail(snake_t *snake, board_t *board);

/* run step */
int next_frame(game_t *cur_game);

int empty_Queue(Queue *q);
void en_Queue_back(Queue *q, point e);
Queue* init_Queue(void);
point de_Queue(Queue*q);

game_t *create_game(const char *file_name) {
	FILE* input_filep;
	game_t* cur_game;
	int row, col, grow_per_food; float food_p, time_gap;
	int rs, cs;
	char tem, dir;
	cur_game = (game_t*)malloc(sizeof(game_t));
	input_filep = fopen(file_name, "r");
	fscanf(input_filep, "%d%d%f%f%d", &row, &col, &time_gap, &food_p, &grow_per_food);
	cur_game->board = create_board(row, col);
    fscanf(input_filep,"%c", &tem);
    fscanf(input_filep,"%c", &tem);
	for (int i = 0; i < row; i++) {
		for (int j = 0; j <= col; j++) {
            fscanf(input_filep,"%c", &tem);
            if(tem=='\n') continue;

			if (tem == 'X') {
				*board_cell(cur_game->board, i, j) = CELL_WALL;
			}
			else if (tem == '-') {
				*board_cell(cur_game->board, i, j) = CELL_OPEN;
			}
			else {
				*board_cell(cur_game->board, i, j) = CELL_SNAKE;
				rs = i; cs = j;
				dir = tem;
			}
		}

	}
	switch (dir)
		{
		case 'N':cur_game->snake=create_snake(cur_game->board, rs, cs, NORTH); break;
		case 'S':cur_game->snake=create_snake(cur_game->board, rs, cs, SOUTH); break;
		case 'W':cur_game->snake=create_snake(cur_game->board, rs, cs, WEST); break;
		case 'E':cur_game->snake=create_snake(cur_game->board, rs, cs, EAST); break;
		}
		cur_game->food_probability = food_p;
		cur_game->growth_per_food = grow_per_food;
		cur_game->time_step = time_gap;
	fclose(input_filep);
	return cur_game;
}

void destroy_game(game_t *game) {
	destroy_board(game->board);
	destroy_snake(game->snake);
	free(game);
}

direction_t get_action(game_t* game) {

    int dir[4][2]={{1,0},{-1,0},{0,1},{0,-1}};
    char x[4]={'S','N','E','W'};
    int visited2[game->board->rows+1][game->board->cols+1];
    int visited[game->board->rows+1][game->board->cols+1][3*game->board->rows+1];
    int time[game->board->rows+1][game->board->cols+1];
    for(int i=0;i<=game->board->rows;i++)
        for(int j=0;j<=game->board->rows;j++)
        {
            time[i][j]=10000;visited2[i][j]=0;
            for(int k=0;k<=3*game->board->rows;k++)
                visited[i][j][k]=0;
        }

    snake_node_t *nd;
    nd=game->snake->tail;
    int t=0;
    while(nd)
    {
        time[nd->row][nd->col]=t;
        t++;
        nd=nd->next;
    }
    Queue *q;
    point p;
    q=init_Queue();
    p.x=game->snake->head->row;
    p.y=game->snake->head->col;
    p.pre=-1;p.t=0;
    printf("%d %d\n",p.x,p.y);
    en_Queue_back(q,p);

    visited[p.x][p.y][p.t]=1;
    visited2[p.x][p.y]=1;
//printf("iro\n");
    empty_Queue(q);
    display(game);
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
        //printf("%d\n",temp.t);
        if(*board_cell(game->board,temp.x,temp.y)==CELL_FOOD)
        {
            printf("vbbb\n");
            point tem=temp;
            //char path[100];
            char d;
            int i=0;
            while(tem.pre!=-1){
                d=tem.dir;
                tem=q->data[tem.pre];
                i++;
                //printf("%d\n",tem.pre);
            }
            switch(d){
                case 'W':return WEST;
                case 'N':return NORTH;
                case 'S':{printf("aaa\n");return SOUTH;}
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
            pp.t=temp.t+1;
            if(pp.x<game->board->rows&&pp.y<game->board->cols
               &&(pp.t>=time[pp.x][pp.y]||time[pp.x][pp.y]==10000)
               &&visited[pp.x][pp.y][pp.t]==0&&visited2[pp.x][pp.y]==0
               &&(*board_cell(game->board,pp.x,pp.y)==CELL_OPEN||*board_cell(game->board,pp.x,pp.y)==CELL_FOOD))
            {
                pp.dir=x[i];
                visited[pp.x][pp.y][pp.t]=1;
                visited2[pp.x][pp.y]=1;
                pp.pre=q->front;
                //printf("pre:%d\n",pp.pre);
                if(*board_cell(game->board,pp.x,pp.y)==CELL_FOOD)printf("ppp:%d %d\n",pp.x,pp.y);
                en_Queue_back(q,pp);
            }
            else continue;

        }

    }
    if(cnt==0)
    {
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
    return SOUTH;

}

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
const char grid[] = "-+X*";

void display(game_t *game) {
    system("cls");
    board_t *board = game->board;
    int i, j;
    for (i = 0; i < board->rows; i++) {
        for (j = 0; j < board->cols; j++) {
            printf("%c", grid[*board_cell(board, i, j)]);
        }
        puts("");
    }

}

int main(int argc, char *argv[]) {
    srand(time(0));
    if (argc != 2) {
      printf("Usage: ./main file_name\n");
        exit(-1);
    }

    game_t *game = create_game(argv[1]);

    while (1) {

        direction_t d = get_action(game);
        set_direction(game->snake, d);

        snake_node_t *nd;
        nd=game->snake->tail;
        int t=0;
        while(nd)
        {
            //time[nd->row][nd->col]=t;
            t++;
            nd=nd->next;
        }
        printf("score:%d\n",t);
        if (next_frame(game) != 0) {
            break;
        }
        usleep(game->time_step * 10000);
    }
    destroy_game(game);
    return 0;
}
board_t *create_board(int rows, int cols) {
	cell_t *cells; board_t* board;
	cells = (cell_t*)malloc(sizeof(cell_t)*rows*cols);
	board = (board_t*)malloc(sizeof(board_t));
	for (int i = 0; i < rows*cols; i++) {
		cells[i] = CELL_OPEN;
	}
	board->cells = cells;
	board->cols = cols;
	board->rows = rows;
	return board;
}

void destroy_board(board_t *board) {
	free(board->cells);
	free(board);
	return;
}

cell_t *board_cell(board_t *board, int row, int col) {
	if (row < 0 || col < 0 || row >= board->rows || col > board->cols) {
		printf("wrong position1");
		return NULL;
	}
	cell_t *cell;

	cell = board->cells + (row*board->cols + col);
	return cell;
}

snake_t *create_snake(board_t *board, int row, int col, direction_t heading) {
	if (row < 0 || col < 0 || row >= board->rows || col >= board->cols) {
		printf("wrong position2");
		return NULL;
	}
	snake_t* snake;
	snake = (snake_t*)malloc(sizeof(snake_t));
	snake_node_t*head, *tail;
	head = (snake_node_t*)malloc(sizeof(snake_node_t));
	tail = (snake_node_t*)malloc(sizeof(snake_node_t));

	head->col = col; head->row = row;
	tail->col = col; tail->row = row;
	tail->next = head; head->next = NULL;

	snake->head = head;
	snake->heading = heading;
	snake->tail = tail;
	snake->growth = 0;
	return snake;
}

void destroy_snake(snake_t *snake) {
	snake_node_t* tem;

	tem = snake->tail;
	while (snake->tail != NULL) {
		tem = snake->tail;
		snake->tail = snake->tail->next;
		free(tem);
	}
	free(snake);
	return;
}

void randomly_add_food(board_t *board, float probability) {
	static int flag_srand = 0;
	if (flag_srand == 0) {
		srand(time(NULL));
		flag_srand = 1;
	}

	float k = (float)(rand() % 1000) / 1000;
	if (k > probability) {
		return;
	}
	int p = rand() % (board->rows*board->cols);
	if (board->cells[p] != CELL_OPEN) {
		for (int i = 0; i + p < board->rows*board->cols; i++) {
			if (board->cells[p + i] == CELL_OPEN) {
				board->cells[p + i] = CELL_FOOD;
				return;
			}
		}
		for (int i = 0; p - i < board->rows*board->cols; i++) {
			if (board->cells[p - i] == CELL_OPEN) {
				board->cells[p - i] = CELL_FOOD;
				return;
			}
		}
	}
	board->cells[p] = CELL_FOOD;
	return;
}

int next_frame(game_t *cur_game) {
	if (update_snake_head(cur_game->snake, cur_game->board, cur_game->growth_per_food) == 1) {
		return 1;
	}
	update_snake_tail(cur_game->snake, cur_game->board);
	randomly_add_food(cur_game->board, cur_game->food_probability);
	return 0;
}

int update_snake_head(snake_t *snake, board_t *board, int growth_per_food) {
	cell_t *tem_cell; int dir = -1;
	tem_cell = NULL;
	switch (snake->heading) {
	case EAST: {
		tem_cell = board_cell(board, snake->head->row, snake->head->col + 1);
		break;
	}
	case SOUTH: {
		tem_cell = board_cell(board, snake->head->row + 1, snake->head->col);
		break;
	}
	case WEST: {
		tem_cell = board_cell(board, snake->head->row, snake->head->col - 1);
		break;
	}
	case NORTH: {
		tem_cell = board_cell(board, snake->head->row - 1, snake->head->col);
		break;
	}
	}
	if (*tem_cell == CELL_OPEN) {
		snake_node_t *tem_snake_node;
		tem_snake_node = (snake_node_t*)malloc(sizeof(snake_node_t));

		switch (snake->heading) {
		case EAST: {
			tem_snake_node->row = snake->head->row;
			tem_snake_node->col = snake->head->col + 1;
			tem_snake_node->next = NULL;
			snake->head->next = tem_snake_node;
			snake->head = tem_snake_node;
			*board_cell(board, tem_snake_node->row, tem_snake_node->col) = CELL_SNAKE;
			break;
		}
		case SOUTH: {
			tem_snake_node->row = snake->head->row + 1;
			tem_snake_node->col = snake->head->col;
			tem_snake_node->next = NULL;
			snake->head->next = tem_snake_node;
			snake->head = tem_snake_node;
			*board_cell(board, tem_snake_node->row, tem_snake_node->col) = CELL_SNAKE;
			break;
		}
		case WEST: {
			tem_snake_node->row = snake->head->row;
			tem_snake_node->col = snake->head->col - 1;
			tem_snake_node->next = NULL;
			snake->head->next = tem_snake_node;
			snake->head = tem_snake_node;
			*board_cell(board, tem_snake_node->row, tem_snake_node->col) = CELL_SNAKE;
			break;
		}
		case NORTH: {
			tem_snake_node->row = snake->head->row - 1;
			tem_snake_node->col = snake->head->col;
			tem_snake_node->next = NULL;
			snake->head->next = tem_snake_node;
			snake->head = tem_snake_node;
			*board_cell(board, tem_snake_node->row, tem_snake_node->col) = CELL_SNAKE;
			break;
		}
		}
		return 0;
	}
	else if (*tem_cell == CELL_FOOD) {
		snake_node_t *tem_snake_node;
		tem_snake_node = (snake_node_t*)malloc(sizeof(snake_node_t));

		snake->growth += growth_per_food;
		switch (snake->heading) {
		case EAST: {
			tem_snake_node->row = snake->head->row;
			tem_snake_node->col = snake->head->col + 1;
			tem_snake_node->next = NULL;
			snake->head->next = tem_snake_node;
			snake->head = tem_snake_node;
			*board_cell(board, tem_snake_node->row, tem_snake_node->col) = CELL_SNAKE;
			break;
		}
		case SOUTH: {
			tem_snake_node->row = snake->head->row + 1;
			tem_snake_node->col = snake->head->col;
			tem_snake_node->next = NULL;
			snake->head->next = tem_snake_node;
			snake->head = tem_snake_node;
			*board_cell(board, tem_snake_node->row, tem_snake_node->col) = CELL_SNAKE;
			break;
		}
		case WEST: {
			tem_snake_node->row = snake->head->row;
			tem_snake_node->col = snake->head->col - 1;
			tem_snake_node->next = NULL;
			snake->head->next = tem_snake_node;
			snake->head = tem_snake_node;
			*board_cell(board, tem_snake_node->row, tem_snake_node->col) = CELL_SNAKE;
			break;
		}
		case NORTH: {
			tem_snake_node->row = snake->head->row - 1;
			tem_snake_node->col = snake->head->col;
			tem_snake_node->next = NULL;
			snake->head->next = tem_snake_node;
			snake->head = tem_snake_node;
			*board_cell(board, tem_snake_node->row, tem_snake_node->col) = CELL_SNAKE;
			break;
		}
		}
		return 0;
	}
	else {
		printf("wasted");
		return 1;
	}
}

void update_snake_tail(snake_t *snake, board_t *board) {
	if (snake->growth > 0) {
		snake->growth--;
	}
	else {
		snake_node_t *tem_snake_node;
		tem_snake_node = snake->tail;
		snake->tail = tem_snake_node->next;
		*board_cell(board, tem_snake_node->row, tem_snake_node->col) = CELL_OPEN;
		free(tem_snake_node);
		tem_snake_node = NULL;
	}
	return;
}

void set_direction(snake_t *snake, direction_t direction) {
	snake->heading = direction;
	return;
}


