#include "file.h"
#include "ai.h"
#include "game.h"
#include "view.h"
#include <unistd.h>


int main(int argc, char* argv[]) {
  srand(time(0));
  if (argc != 2) {
    printf("Usage: ./main file_name");
    exit(-1);
  }
  game_t* game = create_game(argv[1]);
  while (1) {
    display(game);
    direction_t d = get_action(game);
    set_direction(game->snake, d);
    if (next_frame(game) != 0) {
      break;
    
    }
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
   usleep(game->time_step * 1000000);
  }
  return 0;
}
