//Name: Duc Tran
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

//function returns a random element from 1 -> 6
int rollDice(void) {
  int number = rand();
  return number % 6 + 1;
}

// fills the array from index start to end with random dice values
void initialization(int *arr, int start, int end) {
  for (int i = start; i < end; i++) {
    arr[i] = rollDice();
  }
}

// prints all elements of the array
void printArr(int *arr, int len) {
  for (int i = 0; i < len; i++) {
    printf("%d ", arr[i]);
  }
}

// removes selected dice, shifts remaining values left, and rerolls new dice
void reArrange(int *arr, int len, int *choices, int len_choices) {
  // mark the selected dice for removal by setting them to 0
  for (int i = 0; i < len_choices; i++) {
    arr[choices[i] - 1] = 0;
  }
  printf("\nRemoved dices: ");
  printArr(arr, len);
  
  int shift = 0;
  // shift non-zero values to the left and fill the rest with new dice rolls
  for (int i = 0; i < len; i++) {
    if (arr[i] != 0) {
      arr[shift++] = arr[i];
    }
  }
  // fill the remaining positions with new dice rolls
  for (int i = shift; i < len; i++) {
    arr[i] = rollDice();
  }
  printf("\nNew set of dices: ");
  // print the new set of dice 
  printArr(arr, len);
  printf("\n");
}

// handles reroll logic based on user choice and remaining rolls
void reRoll(int *arr, int len, int *remaining_rolls) {
  printf("\nReroll options: \n");
  if (*remaining_rolls == len) {
    printf("1. All dices\n");
  }
  printf("2. None of the dice\n");
  printf("3. Choose a subset less than %d numbers\n", *remaining_rolls);
  printf("Your choice: ");
  int option;
  // validate user input for reroll options
  while (1) {
    int valid = scanf("%d", &option);
    if (valid != 1 || option < 1 || option > 3) {
      printf("Invalid option. Please choose again: ");
      continue;
    } else if (*remaining_rolls < 5 && option == 1) {
      printf("You don't have enough rolls for this. Please choose again: ");
      continue;
    }
    break;
  }
  printf("\n");
  int choices[5] = {0, 0, 0, 0, 0};
  int count = 0;
  // execute the reroll based on the user's choice
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
      if (*remaining_rolls == 0) {
        printf("You don't have any rolls left. Please choose again: ");
        return;
      }

      printf("How many dice do you want to change? ");
      // validate the user's input for how many dice to reroll
      while (1) {
        int valid = scanf("%d", &count);
        while (getchar() != '\n');
        if (valid != 1 || count < 1 || count > 5) {
          printf("Invalid option. Please choose again: ");
          continue;
        } else if (count > *remaining_rolls) {
          printf("You don't have enough rolls for this. Please choose again: ");
          continue;
        } 
        break;
      }
      int valid_sequence;
      // validate the user's input for which dice to reroll
      do {
          valid_sequence = 1;
          printf("Please type %d numbers:\n", count);
          // read the positions of the dice to reroll
          for (int i = 0; i < count; i++) {
              printf("Dice at position?: ");

              if (scanf("%d", &choices[i]) != 1) {
                printf("Invalid input. Please enter a number.\n");
                valid_sequence = 0;
                while (getchar() != '\n');   
                break;
              } else if (choices[i] < 1 || choices[i] > 5) {
                  printf("Invalid number at position %d: %d\n", i+1, choices[i]);
                  valid_sequence = 0;
                  while (getchar() != '\n');
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

// checks if the chosen scoreline is still available
bool valid_scoreline(int *score_sheet, int len, int option) {
  return (score_sheet[option - 1] == -1);
}

// displays the current scoreboard with defined and undefined scores
void scoreboard(int *score_sheet) {
  printf("\nCurrent Scoreboard:\n");
  // print the current scores for each number, showing "Undefined" for those not yet scored
  for (int i = 0; i < 6; i++) {
    if (score_sheet[i] != -1) {
      printf("%d: %d\n", i+1, score_sheet[i]);
    } else {
      printf("%d: Undefined\n", i+1);
    }
  }
}

// calculates the score for the chosen number and updates the scoreboard
void update_scoreboard(int *dices, int len, int *score_sheet, int option) {
  int sum = 0;
  // sum up the values of the dice that match the chosen number
  for (int i = 0; i < len; i++) {
    if (dices[i] == option) {
      sum += dices[i];
    }
  }
  score_sheet[option-1] = sum;
}

int main() {
  srand(time(NULL));
  // initialize the dice array and the score sheet, where -1 indicates an unscored line
  int dices[5] = {0, 0, 0, 0, 0};
  int score_sheet[6] = {-1, -1, -1, -1, -1, -1};
  // main game loop for 6 rounds, allowing the player to roll and score each round
  for (int i = 0; i < 6; i ++) {
    printf("\nAttempt %d\n", i + 1);
    int remaining_rolls = 5;
    printf("First roll: ");
    initialization(dices, 0, 5);    
    printArr(dices, 5);
    // allow the player to reroll up to 2 times, or choose to score after the first roll
    while (1) {
      printf("\nReroll?\n");
      printf("Remaining roll = %d\n", remaining_rolls);
      printf("1. Yes\n");
      printf("2. No\n");
      printf("Your choice: ");
      int option;
      // validate user input 
      while (1) {
        int valid = scanf("%d", &option);
        if (valid != 1 || option < 1 || option > 2) {
          printf("\nInvalid option. Please choose again: ");
          while (getchar() != '\n');
          continue;
        }
        break;
      }
      if (option == 1) {  // if the player chooses to reroll, reroll; 
        reRoll(dices, 5, &remaining_rolls);
      } else {        // otherwise, proceed to scoring
        scoreboard(score_sheet);
        printf("Available score sheet: ");
        for (int i = 0; i < 6; i++) {
          if (score_sheet[i] == -1) {
            printf("%d ", i+1);
          }
        }
        printf("\nChoose a scoreline: ");
        // validate the user's input for scoring options, ensuring they choose an available scoreline
        while (1) {
          int valid = scanf("%d", &option);
          while (getchar() != '\n');
          if (valid != 1 || option < 1 || option > 6 || !valid_scoreline(score_sheet, 6, option)) {
            printf("\nInvalid option. Please choose again: ");
            continue;
          }
          break;
        }
        // calculate and update the score for the chosen scoreline
        update_scoreboard(dices, 5, score_sheet, option);
        scoreboard(score_sheet);
        break;
      }
    }
  } 
  //total score:
  int total_score = 0;
  // sum up the scores from all scored lines to calculate the total score
  for (int i = 0; i <6; i++) {
    total_score += score_sheet[i];
  }
  printf("Congratulations! You scored: %d\n", total_score);
  return 0;
}
