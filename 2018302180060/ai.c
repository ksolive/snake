#include "ai.h"
int ans_x[225], ans_y[225], cnt = 1000;
int dx[] = { -1, 0, 0, 1 };
int dy[] = { 0, -1, 1, 0 };
int findpathlong(int visited[][20], game_t* game, int pathlong, int start_x, int start_y, int end_x, int end_y)
{
	if (start_x == end_x && start_y == end_y)
		return pathlong;
	else
	{
		int x = 0;
		visited[start_x][start_y] = 1;
		for (int i = 0; i < 4; i++)
		{
			int tx = start_x + dx[i];
			int ty = start_y + dy[i];
			int y = 0;
			if ((tx >= 0 && tx < game->board->rows) && (ty >= 0 && ty < game->board->cols) && (!visited[tx][ty]))
			{
				y = findpathlong(visited, game, pathlong + 1, tx, ty, end_x, end_y);
			}
			if (x == 0 || (y < x && y != 0))
				x = y;
		}
		visited[start_x][start_y] = 0;
		return x;
	}
}
void dfs(int start_x, int start_y, int *path_x, int *path_y, int top, int vis[][20], game_t* game, int passed) {
	if (top >= game->snake->growth + game->growth_per_food){
		if ((top < cnt || cnt == 0) && passed == 1){
			cnt = top;
			for (int i = 0; i < cnt; i++){
				ans_x[i] = path_x[i];
				ans_y[i] = path_y[i];
			}
			return;
		}
	}
	if (passed == 0 && start_x == game->snake->tail->row&&start_y == game->snake->tail->col && (cnt == 1000)){
		for (int i = 0; i < top; i++){
			ans_x[i] = path_x[i];
			ans_y[i] = path_y[i];
		}
	}
	for (int i = 0; i < 4; i++) {
		int tx = start_x + dx[i];
		int ty = start_y + dy[i];
		if ((tx >= 0 && tx < game->board->rows) && (ty >= 0 && ty < game->board->cols) && (!vis[tx][ty]) && (game->board->cells[tx*game->board->rows + ty] == CELL_OPEN || game->board->cells[tx*game->board->rows + ty] == CELL_FOOD) && findpathlong(vis, game, 0, tx, ty, game->snake->tail->row, game->snake->tail->col) >= 0){
			vis[tx][ty] = 1;
			path_x[top] = tx;
			path_y[top] = ty;
			dfs(tx, ty, path_x, path_y, top + 1, vis, game, (game->board->cells[tx*game->board->rows + ty] == CELL_FOOD));
			vis[tx][ty] = 0;
		}
	}
	return;
}
direction_t get_action(game_t* game) {
	int  vis[20][20] = { 0 };
	vis[game->snake->head->row][game->snake->head->col] = 1;
	int path_x[225] = { game->snake->head->row }, path_y[225] = { game->snake->head->col };
	ans_x[0] = game->snake->head->row;
	ans_y[0] = game->snake->head->col;
	ans_x[1] = game->snake->head->row;
	ans_y[1] = game->snake->head->col;
	cnt = 1000;
	int top = 1;
	int passed = 0;
	for (int i = 0; i < game->board->rows; i++) {
		for (int j = 0; j < game->board->cols; j++) {
			if (game->board->cells[i*game->board->rows + j] == CELL_OPEN || game->board->cells[i*game->board->rows + j] == CELL_FOOD) {
				vis[i][j] = 0;
			}
			else {
				vis[i][j] = 1;
			}
		}
	}
	dfs(game->snake->head->row, game->snake->head->col, path_x, path_y, top, vis, game, passed);
	int vis_flag = 0;
	while (vis_flag == 0) {
		if (ans_x[1] == ans_x[0]) {
			if (ans_y[1] > ans_y[0])
				return EAST;
			else if (ans_y[1] < ans_y[0])
				return WEST;
			else {
				for (int i = 0; i < 4; i++) {
					ans_x[1] = ans_x[0] + dx[i];
					ans_y[1] = ans_y[0] + dy[i];
					if ((ans_x[1] >= 0 && ans_x[1] < game->board->rows) && (ans_y[1] >= 0 && ans_y[1] < game->board->cols)) {
						if (vis[ans_x[1]][ans_y[1]] != 0) {
							vis_flag = 1;
						}
					}
				}
			}
		}
		else {
			if (ans_x[1] > ans_x[0]) {
				return SOUTH;
			}
			else {
				return NORTH;
			}
			break;
		}
	}
	
}