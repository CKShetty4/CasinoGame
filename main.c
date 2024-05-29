#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h> // for Sleep function

#define MIN_BET 10
#define MAX_BET 10000
#define MIN_DEPOSIT 101
#define MIN_WITHDRAWAL 100
#define MIN_PLAY_PERCENTAGE 0.8
#define ROWS 9
#define COLS 9
#define MINES 10

typedef struct {
    int balance;
    int total_deposit;
} Player;
typedef struct {
    int isMine;
    int isRevealed;
    int adjacentMines;
} Cell;

void deposit(Player *player, int amount) {
    if (amount < MIN_DEPOSIT) {
        printf("Minimum deposit is %d credits.\n", MIN_DEPOSIT);
        return;
    }
    player->balance += amount;
    player->total_deposit += amount;
    printf("Deposit successful. Current balance: %d\n", player->balance);
}

void withdraw(Player *player, int amount) {
    if (amount < MIN_WITHDRAWAL) {
        printf("Minimum withdrawal is %d credits.\n", MIN_WITHDRAWAL);
        printf("Press any key to continue...\n");
        getchar(); // read a character
        getchar(); // wait for Enter key
        return;
    }
    if (player->balance < amount) {
        printf("Insufficient balance. Current balance: %d\n", player->balance);
        printf("Press any key to continue...\n");
        getchar(); // read a character
        getchar(); // wait for Enter key
        return;
    }
    player->balance -= amount;
    printf("Withdrawal successful. Current balance: %d\n", player->balance);
    printf("Press any key to continue...\n");
        getchar(); // read a character
        getchar(); // wait for Enter key
}

int game1(Player *player, int bet) {
    int secret_number, user_guess;

    if (bet < MIN_BET) {
        printf("Minimum bet is %d credits.\n", MIN_BET);
        printf("Press any key to continue...\n");
        getchar(); // read a character
        getchar(); // wait for Enter key
        return 0;
    }
    if (bet > MAX_BET) {
        printf("Maximum bet is %d credits.\n", MAX_BET);
        printf("Press any key to continue...\n");
        getchar(); // read a character
        getchar(); // wait for Enter key
        return 0;
    }
    if (player->balance < bet) {
        printf("Insufficient balance. Current balance: %d\n", player->balance);
        printf("Press any key to continue...\n");
        getchar(); // read a character
        getchar(); // wait for Enter key
        return 0;
    }

    srand(time(NULL));//seed random number, else same series will be generated
    secret_number = rand() % 10 + 1;

    printf("Guess a number between 1 and 10.\n");
    scanf("%d", &user_guess);

    if (user_guess == secret_number) {
        player->balance += bet * 10;
        printf("Congratulations! You won %d. Current balance: %d\n", bet * 10, player->balance);
        printf("Press any key to continue...\n");
        getchar(); // read a character
        getchar(); // wait for Enter key
        return 1;
    } else {
        player->balance -= bet;
        printf("Sorry, you lost. The number was %d. Current balance: %d\n",secret_number, player->balance);
        printf("Press any key to continue...\n");
        getchar(); // read a character
        getchar(); // wait for Enter key
        return 1;
    }
}

int game2(Player *player, int bet) {
    int n, count = 1;
    char input;

    if (bet < MIN_BET) {
        printf("Minimum bet is %d credits.\n", MIN_BET);
        return 0;
    }
    if (bet > MAX_BET) {
        printf("Maximum bet is %d credits.\n", MAX_BET);
        return 0;
    }
    if (player->balance < bet) {
        printf("Insufficient balance. Current balance: %d\n", player->balance);
        return 0;
    }

    srand(time(NULL));
    n = rand() % 100 + 1; // generate a random number between 1 and 100

    printf("Press any key to stop the count. If you fail to stop, you will lose your bet.\n");
    printf("Counting starts...\n");

    while (count <= n) {
        printf("%d ", count);
        fflush(stdout); // force the output to be printed immediately
        if (kbhit()) { // check if a key is pressed
            scanf(" %c", &input); // consume the input
            player->balance += bet * count;
            //*balance += bet * count;
            printf("\nCongratulations! You won %d. Current balance: %d\n", bet * count, player->balance);
            return;
        }
        Sleep(10); // wait for 10 milliseconds
        count++;
    }

    player->balance -= bet;
    printf("\nSorry, you lost. Current balance: %d\n", player->balance);
    return;
}

void initializeBoard(Cell board[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            board[i][j].isMine = 0;
            board[i][j].isRevealed = 0;
            board[i][j].adjacentMines = 0;
        }
    }
}

void placeMines(Cell board[ROWS][COLS]) {
    srand(time(NULL));
    int placedMines = 0;
    while (placedMines < MINES) {
        int r = rand() % ROWS;
        int c = rand() % COLS;
        if (!board[r][c].isMine) {
            board[r][c].isMine = 1;
            placedMines++;
        }
    }
}

void calculateAdjacentMines(Cell board[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (board[i][j].isMine) continue;
            int mineCount = 0;
            for (int r = -1; r <= 1; r++) {
                for (int c = -1; c <= 1; c++) {
                    int newRow = i + r;
                    int newCol = j + c;
                    if (newRow >= 0 && newRow < ROWS && newCol >= 0 && newCol < COLS && board[newRow][newCol].isMine) {
                        mineCount++;
                    }
                }
            }
            board[i][j].adjacentMines = mineCount;
        }
    }
}

void printBoard(Cell board[ROWS][COLS], int reveal) {
    printf("    ");
    for (int i = 0; i < COLS; i++) printf("%d ", i);
    printf("\n   ");
    for (int i = 0; i < COLS; i++) printf("--");
    printf("\n");

    for (int i = 0; i < ROWS; i++) {
        printf("%d | ", i);
        for (int j = 0; j < COLS; j++) {
            if (reveal || board[i][j].isRevealed) {
                if (board[i][j].isMine) printf("* ");
                else printf("%d ", board[i][j].adjacentMines);
            } else {
                printf("# ");
            }
        }
        printf("\n");
    }
}

void revealCell(Cell board[ROWS][COLS], int row, int col) {
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS || board[row][col].isRevealed) return;
    board[row][col].isRevealed = 1;

    if (board[row][col].adjacentMines == 0) {
        for (int r = -1; r <= 1; r++) {
            for (int c = -1; c <= 1; c++) {
                revealCell(board, row + r, col + c);
            }
        }
    }
}

int game3(Player *player, int bet) {
    Cell board[ROWS][COLS];
    initializeBoard(board);
    placeMines(board);
    calculateAdjacentMines(board);

    int gameOver = 0;
    int reward = 0;
    while (!gameOver) {
        system("cls");
        printBoard(board, 0);
        int row, col;
        printf("Enter row and column to reveal: ");
        scanf("%d%d", &row, &col);

        if (row < 0 || row >= ROWS || col < 0 || col >= COLS) {
            printf("Invalid input. Try again.\n");
            continue;
        }

        if (board[row][col].isMine) {
            printf("Game Over! You hit a mine.\n");
            //
            printBoard(board, 1);
            player->balance -= bet;
            printf("You lost %d credits. Your current balance is %d credits.\n", bet, player->balance);
            return;
        } else {
            revealCell(board, row, col);
            reward += bet*5;
            printf("You won %d credits.", bet*5);
            printf("Do you want to withdraw or continue? (0 to withdraw, 1 to continue): ");
            int choice;
            scanf("%d", &choice);
            if (choice == 0) {
                player->balance += reward;
                player->total_deposit += bet;
                printf("You have withdrawn %d credits. Your current balance is %d credits.\n", reward, player->balance);
                return;
            }
        }
    }
}

int game4(Player* player1, Player* player2, int bet1, int bet2) {
  int player1_guess, player2_guess, target_number;
  if (bet1 < MIN_BET||bet2<MIN_BET) {
        printf("Minimum bet is %d credits.\n", MIN_BET);
        return 0;
    }
  if (bet1 > MAX_BET||bet2> MAX_BET) {
        printf("Maximum bet is %d credits.\n", MAX_BET);
        return 0;
    }
  if (player1->balance < bet1) {
        printf("Insufficient balance in player1 accoount. Current balance: %d\n", player1->balance);
        return 0;
    }
  if (player2->balance < bet2) {
        printf("Insufficient balance in player2 account. Current balance: %d\n", player2->balance);
        return 0;
    }
  printf("Welcome to High Card!\n");
  srand(time(NULL));
  target_number = rand() % (100 - 1 + 1) + 1;
  printf("\nPlayer 1, guess a number between 1 and 100: ");
  scanf("%d", &player1_guess);
  printf("\nPlayer 2, guess a number between 1 and 100: ");
  scanf("%d", &player2_guess);
  int player1_diff = abs(player1_guess - target_number);
  int player2_diff = abs(player2_guess - target_number);

  if (player1_diff < player2_diff) {
    printf("\nTarget number: %d\n", target_number);
    printf("Player 1 wins!\n");
    printf("Reward: %d\n", (int)(0.9 * bet2));
    player1->balance += (int)(0.9 * bet2);
    printf("Player 1's new balance: %d\n", player1->balance);
  } else if (player1_diff > player2_diff) {
    printf("\nTarget number: %d\n", target_number);
    printf("Player 2 wins!\n");
    printf("Reward: %d\n", (int)(0.9 * bet2));
    player2->balance += (int)(0.9 * bet2);
    printf("Player 2's new balance: %d\n", player2->balance);
  } else {
    printf("\nTarget number: %d\n", target_number);
    printf("It's a tie!\n");
    printf("Both player win 90%% of their bet");
    player1->balance += (int)(0.9 * bet1);
    player2->balance += (int)(0.9 * bet2);
    printf("Player 1's new balance: %d\n", player1->balance);
    printf("Player 2's new balance: %d\n", player2->balance);
  }
}

int main() {
    Player player1 = {0, 0};
    Player player2 = {0, 0};
    int bet1, bet2;
    int choice, bet, player_choice;

    printf("\nWelcome to the Casino!\n");
    while (1) {
        system("cls");
        printf("\n 1. Deposit\n");
        printf(" 2. Withdraw\n");
        printf(" 3. Play Number Hunt\n");
        printf(" 4. Play Number Ambush\n");
        printf(" 5. Play Coded Danger\n");
        printf(" 6. Play High Card(PVP)\n");
        printf(" 7. Exit\n");
        printf("Player 1 Balance: %d\n", player1.balance);
        printf("Player 2 Balance: %d\n", player2.balance);
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter the player number (1 or 2): ");
                scanf("%d", &player_choice);
                if (player_choice == 1) {
                    printf("Enter the deposit amount: ");
                    scanf("%d", &bet);
                    deposit(&player1, bet);
                } else if (player_choice == 2) {
                    printf("Enter the deposit amount: ");
                    scanf("%d", &bet);
                    deposit(&player2, bet);
                } else {
                    printf("Invalid player number. Please try again.\n");
                }
                printf("Press any key to continue...\n");
                getchar(); // read a character
                getchar(); // wait for Enter key
                break;
            case 2:
                printf("Enter the player number (1 or 2): ");
                scanf("%d", &player_choice);
                if (player_choice == 1) {
                    printf("Enter the withdrawal amount: ");
                    scanf("%d", &bet);
                    withdraw(&player1, bet);
                } else if (player_choice == 2) {
                    printf("Enter the withdrawal amount: ");
                    scanf("%d", &bet);
                    withdraw(&player2, bet);
                } else {
                    printf("Invalid player number. Please try again.\n");
                }
                printf("Press any key to continue...\n");
                getchar(); // read a character
                getchar(); // wait for Enter key
                break;
            case 3:
                printf("Enter the player number (1 or 2): ");
                scanf("%d", &player_choice);
                if (player_choice == 1) {
                    if (player1.balance < MIN_BET) {
                        printf("Insufficient balance. Current balance: %d\n", player1.balance);
                        printf("Press any key to continue...\n");
                        getchar(); // read a character
                        getchar(); // wait for Enter key
                        break;
                    }
                    printf("Enter the bet amount (minimum %d): ", MIN_BET);
                    scanf("%d", &bet);
                    while (game1(&player1, bet)) {
                        printf("Do you want to play again? (y/n): ");
                        char play_again;
                        scanf(" %c", &play_again);
                        if (play_again == 'y' || play_again == 'Y') {
                            if (player1.balance < MIN_BET) {
                                printf("Insufficient balance. Current balance: %d\n", player1.balance);
                                printf("Press any key to continue...\n");
                                getchar(); // read a character
                                getchar(); // wait for Enter key
                                break;
                            }
                            printf("Enter the bet amount (minimum %d): ", MIN_BET);
                            scanf("%d", &bet);
                            continue;
                        }
                        else
                            break;
                    }
                } else if (player_choice == 2) {
                    if (player2.balance < MIN_BET) {
                        printf("Insufficient balance. Current balance: %d\n", player2.balance);
                        printf("Press any key to continue...\n");
                        getchar(); // read a character
                        getchar(); // wait for Enter key
                        break;
                    }
                    printf("Enter the bet amount (minimum %d): ", MIN_BET);
                    scanf("%d", &bet);
                    while (game1(&player2, bet)) {
                        printf("Do you want to play again? (y/n): ");
                        char play_again;
                        scanf(" %c", &play_again);
                        if (play_again == 'y' || play_again == 'Y') {
                            if (player2.balance < MIN_BET) {
                                printf("Insufficient balance. Current balance: %d\n", player2.balance);
                                printf("Press any key to continue...\n");
                                getchar(); // read a character
                                getchar(); // wait for Enter key
                                break;
                            }
                            printf("Enter the bet amount (minimum %d): ", MIN_BET);
                            scanf("%d", &bet);
                            continue;
                        }
                        else
                            break;
                    }
                } else {
                    printf("Invalid player number. Please try again.\n");
                }
                printf("Press any key to continue...\n");
                getchar(); // read a character
                getchar(); // wait for Enter key
                break;
            case 4:
                printf("Enter the player number (1 or 2): ");
                scanf("%d", &player_choice);
                if (player_choice == 1) {
                    if (player1.balance < MIN_BET) {
                        printf("Insufficient balance. Current balance: %d\n", player1.balance);
                        printf("Press any key to continue...\n");
                        getchar(); // read a character
                        getchar(); // wait for Enter key
                        break;
                    }
                    printf("Enter the bet amount (minimum %d): ", MIN_BET);
                    scanf("%d", &bet);
                    while (game2(&player1, bet)) {
                        printf("Do you want to play again? (y/n): ");
                        char play_again;
                        scanf(" %c", &play_again);
                        if (play_again == 'y' || play_again == 'Y') {
                            if (player1.balance < MIN_BET) {
                                printf("Insufficient balance. Current balance: %d\n", player1.balance);
                                printf("Press any key to continue...\n");
                                getchar(); // read a character
                                getchar(); // wait for Enter key
                                break;
                            }
                            printf("Enter the bet amount (minimum %d): ", MIN_BET);
                            scanf("%d", &bet);
                            continue;
                        }
                        else
                            break;
                    }
                } else if (player_choice == 2) {
                    if (player2.balance < MIN_BET) {
                        printf("Insufficient balance. Current balance: %d\n", player2.balance);
                        printf("Press any key to continue...\n");
                        getchar(); // read a character
                        getchar(); // wait for Enter key
                        break;
                    }
                    printf("Enter the bet amount (minimum %d): ", MIN_BET);
                    scanf("%d", &bet);
                    while (game2(&player2, bet)) {
                        printf("Do you want to play again? (y/n): ");
                        char play_again;
                        scanf(" %c", &play_again);
                        if (play_again == 'y' || play_again == 'Y') {
                            if (player2.balance < MIN_BET) {
                                printf("Insufficient balance. Current balance: %d\n", player2.balance);
                                printf("Press any key to continue...\n");
                                getchar(); // read a character
                                getchar(); // wait for Enter key
                                break;
                            }
                            printf("Enter the bet amount (minimum %d): ", MIN_BET);
                            scanf("%d", &bet);
                            continue;
                        }
                        else
                            break;
                    }
                } else {
                    printf("Invalid player number. Please try again.\n");
                }
                printf("Press any key to continue...\n");
                getchar(); // read a character
                getchar(); // wait for Enter key
                break;
            case 5:
                /*
                printf("You will play Coded Danger. The rules are as follows:\n");
                printf("- The game has a grid of 8x8 cells. 10 mines are placed randomly.\n");
                printf("- You will start with 0 revealed cells. Each unrevealed cell contains the number of adjacent mines.\n");
                printf("- Your goal is to reveal all non-mine cells. If you reveal a mine, you loose %d credits.\n", bet);
                printf("- You will have the option to withdraw or continue after each move.\n");
                printf("- Your bet for each game is same.\n");
                printf("- If you choose to withdraw, you will lose the current bet and withdrawal amount.\n");
                */
                printf("Enter the player number (1 or 2): ");
                scanf("%d", &player_choice);
                if (player_choice == 1) {
                    if (player1.balance < MIN_BET) {
                        printf("Insufficient balance. Current balance: %d\n", player1.balance);
                        printf("Press any key to continue...\n");
                        getchar(); // read a character
                        getchar(); // wait for Enter key
                        break;
                    }
                    printf("Enter the bet amount (minimum %d): ", MIN_BET);
                    scanf("%d", &bet);
                    while (game3(&player1, bet)) {
                        printf("Do you want to play again? (y/n): ");
                        char play_again;
                        scanf(" %c", &play_again);
                        if (play_again == 'y' || play_again == 'Y') {
                            if (player1.balance < MIN_BET) {
                                printf("Insufficient balance. Current balance: %d\n", player1.balance);
                                printf("Press any key to continue...\n");
                                getchar(); // read a character
                                getchar(); // wait for Enter key
                                break;
                            }
                            printf("Enter the bet amount (minimum %d): ", MIN_BET);
                            scanf("%d", &bet);
                            continue;
                        }
                        else
                            break;
                    }
                } else if (player_choice == 2) {
                    if (player2.balance < MIN_BET) {
                        printf("Insufficient balance. Current balance: %d\n", player2.balance);
                        printf("Press any key to continue...\n");
                        getchar(); // read a character
                        getchar(); // wait for Enter key
                        break;
                    }
                    printf("Enter the bet amount (minimum %d): ", MIN_BET);
                    scanf("%d", &bet);
                    while (game3(&player2, bet)) {
                        printf("Do you want to play again? (y/n): ");
                        char play_again;
                        scanf(" %c", &play_again);
                        if (play_again == 'y' || play_again == 'Y') {
                            if (player2.balance < MIN_BET) {
                                printf("Insufficient balance. Current balance: %d\n", player2.balance);
                                printf("Press any key to continue...\n");
                                getchar(); // read a character
                                getchar(); // wait for Enter key
                                break;
                            }
                            printf("Enter the bet amount (minimum %d): ", MIN_BET);
                            scanf("%d", &bet);
                            continue;
                        }
                        else
                            break;
                    }
                } else {
                    printf("Invalid player number. Please try again.\n");
                }
                printf("Press any key to continue...\n");
                getchar(); // read a character
                getchar(); // wait for Enter key
                break;
            case 6:
                 if (player1.balance < MIN_BET) {
                        printf("Insufficient balance. Current balance: %d\n", player1.balance);
                        printf("Press any key to continue...\n");
                        getchar(); // read a character
                        getchar(); // wait for Enter key
                        break;
                    }
                printf("Enter player 1's bet: ");
                scanf("%d", &bet1);
                 if (player2.balance < MIN_BET) {
                        printf("Insufficient balance. Current balance: %d\n", player2.balance);
                        printf("Press any key to continue...\n");
                        getchar(); // read a character
                        getchar(); // wait for Enter key
                        break;
                    }
                printf("Enter player 2's bet: ");
                scanf("%d", &bet2);
                while (game4(&player1, &player2, bet1, bet2)) {
                        printf("Do you want to play again? (y/n): ");
                        char play_again;
                        scanf(" %c", &play_again);
                        if (play_again == 'y' || play_again == 'Y') {
                            if (player1.balance < MIN_BET) {
                                printf("Insufficient balance. Current balance: %d\n", player1.balance);
                                printf("Press any key to continue...\n");
                                getchar(); // read a character
                                getchar(); // wait for Enter key
                                break;
                    }
                        printf("Enter player 1's bet: ");
                        scanf("%d", &bet1);
                         if (player2.balance < MIN_BET) {
                                printf("Insufficient balance. Current balance: %d\n", player2.balance);
                                printf("Press any key to continue...\n");
                                getchar(); // read a character
                                getchar(); // wait for Enter key
                                break;
                            }
                        printf("Enter player 2's bet: ");
                        scanf("%d", &bet2);
                        continue;
                        }
                        else
                            break;
                    }
                printf("Press any key to continue...\n");
                getchar(); // read a character
                getchar(); // wait for Enter key
                break;
            case 7:
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
                printf("Press any key to continue...\n");
                getchar(); // read a character
                getchar(); // wait for Enter key
        }
    }

    return 0;
}
