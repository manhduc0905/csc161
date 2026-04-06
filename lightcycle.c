#include <ncurses.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#define block "  "
// You can adjust the frame rate to change the game speed. The demo runs at 4 frames per second.
#define FRAME_RATE 200
#define WIDTH 2


/**
 * Sleep for a given number of milliseconds
 * \param   ms  The number of milliseconds to sleep for
 */
void sleep_ms(size_t ms) {
  struct timespec ts;
  size_t rem = ms % 1000;
  ts.tv_sec = (ms - rem) / 1000;
  ts.tv_nsec = rem * 1000000;

  // Sleep repeatedly as long as nanosleep is interrupted
  while (nanosleep(&ts, &ts) != 0) {}
}

typedef enum {
  UP,
  DOWN,
  LEFT,
  RIGHT
} Direction;

typedef struct {
  int x;
  int y;
  Direction dir;
  bool alive;
} coordinate;

int player_move[8] = {0, -1, 0, 1, -2, 0, 2, 0};

//BOARD_IDX: returns 2d coordinates to 1d coordinate
int BOARD_IDX(int x, int y, int r_width){
  return y*(r_width) + x;
}

//Set the initial border
void border_set(int* board, int max_w, int max_h) {
  int r_width = (max_w + 1) * WIDTH + 1;

  for (int x = 0; x < r_width; x++){
      board[BOARD_IDX(x, 0, r_width)] = 3;
      board[BOARD_IDX(x, max_h + 1, r_width)] = 3;
  }

  for (int y = 0; y <= max_h + 1; y++){
      board[BOARD_IDX(0, y, r_width)] = 3; 
      board[BOARD_IDX(r_width - 1, y, r_width)] = 3;
  }
}

//Print the game state
void board_print(int* board, int max_w, int max_h){
  int r_width = (max_w+1)*WIDTH + 1;
  for (int y = 0; y <= max_h + 1; y++){
    for (int x = 0; x < r_width; x+=WIDTH){
        int idx = BOARD_IDX(x, y, r_width);
        attron(COLOR_PAIR(board[idx]));
        mvprintw(y, x, block);
        attroff(COLOR_PAIR(board[idx]));
    }
  }
}

//Check if the coordinate is within the bound
bool inbound(int x, int y, int max_w, int max_h){
  return (x > 0 && x < (max_w + 1) * WIDTH && y > 0 && y < max_h + 1);
}

//check if the player collides with anything in the board
bool collide(int idx, int* board){
  return (board[idx] != 0);
}
//update the position of the player's current state and whether the player is alive
void update_state(coordinate* p1, coordinate* p2, int max_w, int max_h, int* board){
  int r_width = (max_w+1)*WIDTH + 1;
  int temp1_x = p1->x + player_move[p1->dir*2];
  int temp1_y = p1->y + player_move[p1->dir*2 + 1];

  int temp2_x = p2->x + player_move[p2->dir*2];
  int temp2_y = p2->y + player_move[p2->dir*2 + 1];
  if (temp1_x == temp2_x && temp1_y == temp2_y){
    board[BOARD_IDX(temp1_x, temp1_y, r_width)] = 4;
    p1->alive = false;
    p2->alive = false;
    return;
  } 

  if (collide(BOARD_IDX(temp1_x, temp1_y, r_width), board)) {
    board[BOARD_IDX(temp1_x, temp1_y, r_width)] = 4;
    p1->alive = false;
  }  

  if (collide(BOARD_IDX(temp2_x, temp2_y, r_width), board)) {
    board[BOARD_IDX(temp2_x, temp2_y, r_width)] = 4;
    p2->alive = false;
  }

  if (p1->alive){
    p1->x = temp1_x;
    p1->y = temp1_y;
    board[BOARD_IDX(p1->x, p1->y, r_width)] = 1;
  }

  if (p2->alive){
    p2->x = temp2_x;
    p2->y = temp2_y;
    board[BOARD_IDX(p2->x, p2->y, r_width)] = 2;
  }

  return;
}

//Print the end screen
void endscreen(int condition, int max_w, int max_h){
  int r_width = (max_w+1)*WIDTH + 1;
  char* msg;
  char* msg_exit = "Press space to exit.";

  switch(condition){
    case 1:
      msg = "Tie game!";
      break;
    case 2:
      msg = "Blue wins!";
      break;
    case 3:
      msg = "Yellow wins!";
      break;
  }

  attron(COLOR_PAIR(5));

  // Rectangle dimensions
  int rect_w = 20;   
  int rect_h = 5;    

  // Center position
  int start_x = (r_width - rect_w * WIDTH) / WIDTH;
  int start_y = (max_h + 2 - rect_h) / WIDTH;

  attron(COLOR_PAIR(5));

  // Draw filled rectangle
  for (int y = 0; y < rect_h; y++) {
      for (int x = 0; x < rect_w; x++) {
          mvprintw(start_y + y, start_x + x*WIDTH, block);
      }
  }
  
  mvprintw(start_y + 1, start_x + (rect_w * WIDTH - strlen(msg)) / WIDTH, "%s", msg);
  mvprintw(start_y + 3, start_x + (rect_w * WIDTH - strlen(msg_exit)) / WIDTH, "%s", msg_exit);

  attroff(COLOR_PAIR(5));
}

int main() {
  // Set up ncurses
  initscr();              // Initialize the standard window
  noecho();               // Don't display keys when they are typed
  nodelay(stdscr, true);  // Set up non-blocking input with getch()
  keypad(stdscr, true);   // Allow arrow keys
  curs_set(false);        // Hide the cursor

    
  // Tell ncurses to initialize color support
  start_color();

  // Color pair 0 is white text on a black background
  init_pair(0, COLOR_WHITE, COLOR_BLACK);

  // Color pair 1 is white text on a blue background
  init_pair(1, COLOR_WHITE, COLOR_BLUE);

  // Color pair 2 is white text on a yellow background
  init_pair(2, COLOR_WHITE, COLOR_YELLOW);

  // Color pair 3 is white text on a white background
  init_pair(3, COLOR_WHITE, COLOR_WHITE);

  // Color pair 4 is white text on a RED background
  init_pair(4, COLOR_WHITE, COLOR_RED);

  // Color pair 5 is black text on a WHITE background
  init_pair(5, COLOR_BLACK, COLOR_WHITE);

  // Game settings:
  int grid_width = 35;
  int grid_height = 25;

  //Starting position
  coordinate player1 = {(grid_width/WIDTH + 1)*WIDTH, 0, DOWN, true};
  coordinate player2 = {(grid_width/WIDTH + 1)*WIDTH, grid_height+1, UP, true};

  //game board
  int board[(grid_height+2) * ((grid_width+1)*WIDTH + 1)];
  memset(board, 0, sizeof(board));
  //Playable area
  border_set(board, grid_width, grid_height);

  bool running = true;
  bool winner = false;
  // TODO: Implement the lightcycle game using a game loop
  while(running){
    //clear();
    int input;

    while ((input = getch()) != ERR) {
      switch(input) {
        // player 1
        case KEY_UP:  
          if (player1.dir != DOWN){
            player1.dir = UP;
          }
          break;
        case KEY_DOWN:    
          if (player1.dir != UP){
            player1.dir = DOWN;
          }
          break;
        case KEY_LEFT:  
          if (player1.dir != RIGHT){  
            player1.dir = LEFT;
          }
          break;
        case KEY_RIGHT:    
          if (player1.dir != LEFT){
            player1.dir = RIGHT;
          }
          break;
        //player 2
        case 'w':  
          if (player2.dir != DOWN){
            player2.dir = UP;
          }
          break;
        case 's':    
          if (player2.dir != UP){
            player2.dir = DOWN;
          }
          break;
        case 'a':    
          if (player2.dir != RIGHT){  
            player2.dir = LEFT;
          }
          break;
        case 'd':    
          if (player2.dir != LEFT){
            player2.dir = RIGHT;
          }
          break;
        case ' ':
          if (winner){
            running = false;
          }
          break;
      }
    }
    //print the game board
    board_print(board, grid_width, grid_height);

    //only update if theres no winner yet
    if (!winner) {
      update_state(&player1, &player2, grid_width, grid_height, board);
    }

    //endgame condition
    if (!player1.alive && !player2.alive ) {
      // Both crashed in the same frame
      //show_end_screen("Tie game!"); 
      endscreen(1, grid_width, grid_height);
      winner = true;
    } else if (!player1.alive){
      endscreen(3, grid_width, grid_height);
      winner = true;
    } else if (!player2.alive){
      endscreen(2, grid_width, grid_height);
      winner = true;
    }
  
    refresh();
    sleep_ms(FRAME_RATE);
  }
  // Shut down
  endwin();
}
