#include "game.h"
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
