#include <stdio.h>
#include <stdlib.h>
#include "file.h"
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
  // fscanf(input_filep,"%c", &tem);
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

