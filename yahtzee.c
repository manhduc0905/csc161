#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

//function returns a random element from 1 -> 6
int rollDice(void) {
  int number = rand();
  return number % 6 + 1;
}

void initialization(int *arr, int start, int end) {
  for (int i = start; i < end; i++) {
    arr[i] = rollDice();
  }
}

void printArr(int *arr, int len) {
  for (int i = 0; i < len; i++) {
    printf("%d ", arr[i]);
  }
}

void reArrange(int *arr, int len, int *choices, int len_choices) {
  for (int i = 0; i < len_choices; i++) {
    arr[choices[i] - 1] = 0;
  }
  printf("\nRemoved dices: ");
  printArr(arr, len);
  
  int shift = 0;
  for (int i = 0; i < len; i++) {
    if (arr[i] != 0) {
      arr[shift++] = arr[i];
    }
  }
  for (int i = shift; i < len; i++) {
    arr[i] = rollDice();
  }
  printf("\nNew set of dices: ");
  printArr(arr, len);
  printf("\n");
}

void reRoll(int *arr, int len, int *remaining_rolls) {
  printf("\nReroll options: \n");
  if (*remaining_rolls == len) {
    printf("1. All dices\n");
  }
  printf("2. None of the dice\n");
  printf("3. Choose a subset less than %d numbers\n", *remaining_rolls);
  printf("Your choice: ");
  int option;
  while (1) {
    int valid = scanf("%d", &option);
    if (valid != 1 || valid < 1 || valid > 3) {
      printf("Invalid option. Please choose again: ");
      continue;
    }
    else if (*remaining_rolls < 5 && option == 1) {
      printf("You don't have enough rolls for this. Please choose again: ");
      continue;
    }
    break;
  }
  printf("\n");
  int choices[5] = {0, 0, 0, 0, 0};
  int count = 0;

  switch(option){
    case 1:
      *remaining_rolls -= 5;
      initialization(arr, 0, len);
      printf("New set of dices: ");
      printArr(arr, len);
      break;
    case 2:
      return;
    case 3: 
      printf("How many dice do you want to change? ");
      while (1) {
        int valid = scanf("%d", &count);
        if (valid != 1 || valid < 1 || valid > 5) {
          scanf("%*s");
          printf("Invalid option. Please choose again: ");
          continue;
        }
        else if (count > *remaining_rolls) {
          printf("You don't have enough rolls for this. Please choose again: ");
          continue;
        }
        break;
      }
      int valid_sequence;
      do {
          valid_sequence = 1;
          printf("Please type %d numbers:\n", count);

          for (int i = 0; i < count; i++) {
              printf("Dice at position?: ");
              scanf("%d", &choices[i]);
              if (choices[i] < 1 || choices[i] > 5) {
                  printf("Invalid number at position %d: %d\n", i+1, choices[i]);
                  valid_sequence = 0;
                  scanf("%*s");
                  break; 
              }
          }
      } while (!valid_sequence);
      *remaining_rolls -= count;
      reArrange(arr, 5, choices, count);
      break;
    default:
      return;
  }
}

bool valid_scoreline(int *score_sheet, int len, int option) {
  return (score_sheet[option - 1] == -1);
}

void scoreboard(int *score_sheet) {
  printf("\nCurrent Scoreboard:\n");
  for (int i = 0; i < 6; i++) {
    if (score_sheet[i] != -1) {
      printf("%d: %d\n", i+1, score_sheet[i]);
    }
    else {
      printf("%d: Undefined\n", i+1);
    }
  }
}

void update_scoreboard(int *dices, int len, int *score_sheet, int option) {
  int sum = 0;
  for (int i = 0; i < len; i++) {
    if (dices[i] == option) {
      sum += dices[i];
    }
  }
  score_sheet[option-1] = sum;
}

int main() {
  srand(time(NULL));
  int dices[5] = {0, 0, 0, 0, 0};
  int score_sheet[6] = {-1, -1, -1, -1, -1, -1};
  for (int i = 0; i < 6; i ++) {
    printf("\nAttempt %d\n", i + 1);
    int remaining_rolls = 5;
    printf("First roll: ");
    initialization(dices, 0, 5);    
    printArr(dices, 5);
    while (1) {
      printf("\nReroll?\n");
      printf("Remaining roll = %d\n", remaining_rolls);
      printf("1. Yes\n");
      printf("2. No\n");
      printf("Your choice: ");
      int option;
      while (1) {
        int valid = scanf("%d", &option);
        if (valid != 1 || valid < 1 || valid > 2) {
          printf("\nInvalid option. Please choose again: ");
          scanf("%*s");
          continue;
        }
        break;
      }
      if (option == 1) {
        reRoll(dices, 5, &remaining_rolls);
      }
      else {
        scoreboard(score_sheet);
        printf("Available score sheet: ");
        for (int i = 0; i < 6; i++) {
          if (score_sheet[i] == -1) {
            printf("%d ", i+1);
          }
        }
        printf("\nChoose a scoreline: ");
        while (1) {
          int valid = scanf("%d", &option);
          if (valid != 1 || option < 1 || option > 6 || !valid_scoreline(score_sheet, 6, option)) {
            printf("\nInvalid option. Please choose again: ");
            scanf("%*s");
            continue;
          }
          break;
        }
        update_scoreboard(dices, 5, score_sheet, option);
        scoreboard(score_sheet);
        break;
      }
    }
  } 
  //total score:
  int total_score = 0;
  for (int i = 0; i < 5; i++) {
    total_score += score_sheet[i];
  }
  printf("Congratulations! You scored: %d\n", total_score);
  return 0;
}
