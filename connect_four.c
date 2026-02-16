#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define BOARD_WIDTH 7
#define BOARD_HEIGHT 6
#define INPUT_LENGTH 10
#define WINNING_CONDITION 4

// Constants to print in bold red text, bold yellow text, and to reset text style
#define START_RED "\033[1m\033[31m"
#define START_YELLOW "\033[1m\033[33m"
#define RESET_COLOR "\033[0m"

// Fills the entire board with empty spaces.
void initialization(char board[]){
  for (int i = 0; i < BOARD_HEIGHT; i++) {
    for (int j = BOARD_WIDTH * i; j < BOARD_WIDTH * (i + 1); j++) {
      board[j] = ' ';
    }
  }
}

// Prints the current board state with colored pieces.
void print_board(char board[]){
  char col_char[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G'};
  for (int j = 0; j < BOARD_WIDTH; j++) {
    printf(" %c", col_char[j]);
  }
  printf("\n");
  for (int i = 0; i < BOARD_HEIGHT; i++) {
    for (int j = BOARD_WIDTH * i; j < BOARD_WIDTH * (i + 1); j++) {
      
      if (board[j] == 'R')
        printf("|" START_RED "R" RESET_COLOR);
      else if (board[j] == 'Y')
        printf("|" START_YELLOW "Y" RESET_COLOR);
      else
        printf("| ");
    }
    printf("|\n");
  }
  printf("---------------\n");
}

// Returns true if the selected column still has at least one empty slot.
bool check_col(char board[], int col_idx){
  for (int i = 0; i < BOARD_HEIGHT; i++){
    int position = col_idx + BOARD_WIDTH*i;
    if (board[position] == ' '){
      return true;
    }
  }
  return false;
}

// Prompts the user to enter a valid column (A–G) and returns its index.
int get_column(char board[]){
  char input[INPUT_LENGTH];
  while(1){
    printf("Please type in the column character: ");
      if (fgets(input, INPUT_LENGTH, stdin) == NULL) {
        fprintf(stderr, "Failed to read input from user.\n");
        continue;
      }
      if (strlen(input) == 2 && strcmp(input, "A\n") >= 0 && strcmp(input, "G\n") <= 0) {
        int col_idx = input[0] - 'A'; 
        if (check_col(board, col_idx) == false) {
          printf("Column filled! Please choose another position.\n");
          continue;
        }
        return col_idx;
      }
      printf("Wrong input type, please type a character between A-G\n");
      printf("Character should be written in uppercase!\n");
  }
}

// Gets the move and fills in the chosen column.
void get_move(char board[], int *height, char turn){
  print_board(board);
  printf("What move do you want to make?\n");
  int col_idx = get_column(board);
  int position = col_idx + (BOARD_HEIGHT - height[col_idx] - 1)*BOARD_WIDTH;
  printf("%d %d \n", position, col_idx);
  board[position] = turn;
  printf("%c\n", board[position]);
  height[col_idx]++;
  print_board(board);
}

// Returns true if the board is completely full
bool check_tie(char board[]){
  for (int i = 0; i < BOARD_HEIGHT*BOARD_WIDTH; i++){
    if (board[i] == ' '){
      return false;
    }
  }
  return true;
}

// Returns true if four given characters are identical.
bool diagonal_check(char x1, char x2, char x3, char x4){
  return (x1 == x2 && x2 == x3 && x3 == x4);
}

// Checks the board for any horizontal, vertical, or diagonal win.
bool check_win(char board[]){
  //horizontal check
  for (int i = 0; i < BOARD_HEIGHT; i++){
    int now_pos = BOARD_WIDTH*i;
    for (int j = now_pos; j < now_pos + BOARD_WIDTH - WINNING_CONDITION + 1; j++){
      char cur_char = board[j];
      if (cur_char == ' '){
        continue;
      }
      int score = 1;
      for (int k = j + 1; k < j + WINNING_CONDITION; k++){
        score += (cur_char == board[k]);
        if (score == WINNING_CONDITION){
          return true;
        }
      }
    }
  }
  //vertical check
  for (int i = 0; i < BOARD_WIDTH; i++){
    for (int j = 0; j < BOARD_HEIGHT - WINNING_CONDITION + 1; j++){
      int now_pos = i + j*BOARD_WIDTH;
      char cur_char = board[now_pos];
      if (cur_char == ' '){
        continue;
      }
      int score = 1;
      for (int k = now_pos + BOARD_WIDTH; k < now_pos + BOARD_WIDTH * WINNING_CONDITION ; k+= BOARD_WIDTH){
        score += (cur_char == board[k]);
        if (score == WINNING_CONDITION){
          return true;
        }
      }
    }
  }
  //diagonal check
  for (int i = 0; i < BOARD_HEIGHT; i++){
    for (int j = 0; j < BOARD_WIDTH; j++){
      int cur_pos = i*BOARD_WIDTH + j;
      if (board[cur_pos] == ' '){
        continue;
      }
      bool right_diag = false;
      bool left_diag = false;
      if (i + WINNING_CONDITION - 1 < BOARD_HEIGHT){
        if (j + WINNING_CONDITION - 1 < BOARD_WIDTH){
          char x1 = board[cur_pos];
          char x2 = board[cur_pos + 1*(BOARD_WIDTH + 1)];
          char x3 = board[cur_pos + 2*(BOARD_WIDTH + 1)];
          char x4 = board[cur_pos + 3*(BOARD_WIDTH + 1)];
          right_diag = diagonal_check(x1, x2, x3, x4);
        }
        if (j - WINNING_CONDITION + 1 >= 0){
          char x1 = board[cur_pos];
          char x2 = board[cur_pos + 1*(BOARD_WIDTH - 1)];
          char x3 = board[cur_pos + 2*(BOARD_WIDTH - 1)];
          char x4 = board[cur_pos + 3*(BOARD_WIDTH - 1)];
          left_diag = diagonal_check(x1, x2, x3, x4);
        }
      }
      if (right_diag || left_diag){
        return true;
      }
    }
  }
  return false;
}

int main() {
  char board[BOARD_WIDTH * BOARD_HEIGHT + 1] = {' '};
  int height[BOARD_WIDTH + 1] = {0};
  int player_turn = 0;
  initialization(board);
  //print_board(board);
  while (1){
    char turn = (player_turn == 0) ?  'R' : 'Y' ;
    char *color_turn = (player_turn == 0) ? START_RED "R" RESET_COLOR : START_YELLOW "Y" RESET_COLOR;
    bool win = false;
    bool tie = false;
    printf("It's player %s turn\n", color_turn);
  
    get_move(board, height, turn);
    win = check_win(board);
    tie = check_tie(board);
    if (win){
      printf("Player %s win!\n", color_turn);
      break;
    }
    else if (tie){
      printf("It's a tie\n");
      break;
    }
    player_turn = (player_turn + 1)%2;
    //print_board(board, true);
  }
  return 0;
}



// char row_num[INPUT_LENGTH];
//           printf("Column %c selected!\nPlease type in the row number: ", input[0]);
//           while (1) {
//             if (fgets(row_num, INPUT_LENGTH, stdin) == NULL) {
//               fprintf(stderr, "Failed to read input from user.\n");
//               continue;
//             }
//             if (strlen(row_num) == 2 && strcmp(row_num, "1\n") >= 0 && strcmp(row_num, "6\n") <= 0){
//               position = col_idx + BOARD_HEIGHT*(atoi(row_num)-1);
//               if (board[position] != ' '){
//                 printf("Position taken! Please choose another position.\n");
//                 continue;
//               }
//               return position;
//             }
//             printf("Wrong input type, please type a number between 1-6\n");
//           }