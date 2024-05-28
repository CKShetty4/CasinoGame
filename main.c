#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h> // for Sleep function

#define MIN_BET 10
#define MAX_BET 10000
#define MIN_DEPOSIT 101
#define MIN_WITHDRAWAL 100
#define MIN_PLAY_PERCENTAGE 0.8

typedef struct {
    int balance;
    int total_deposit;
} Player;

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
    /*
    if (player->total_deposit > 0 && player->balance - amount < player->total_deposit * MIN_PLAY_PERCENTAGE) {
        printf("You must play at least %d%% of your deposit before withdrawal.\n", (int)(MIN_PLAY_PERCENTAGE * 100));
        printf("Press any key to continue...\n");
        getchar(); // read a character
        getchar(); // wait for Enter key
        return;
    }*/
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

    srand(time(NULL));//seed random number, else same series will be generated
    secret_number = rand() % 10 + 1;

    printf("Guess a number between 1 and 10.\n");
    scanf("%d", &user_guess);

    if (user_guess == secret_number) {
        player->balance += bet * 10;
        printf("Congratulations! You won %d. Current balance: %d\n", bet * 10, player->balance);
        return 1;
    } else {
        player->balance -= bet;
        printf("Sorry, you lost. Current balance: %d\n", player->balance);
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

void game3(){
printf("Game is under Development\n");
}
int main() {
    Player player1 = {0, 0};
    Player player2 = {0, 0};
    int choice, bet, player_choice;

    printf("\nWelcome to the Casino!\n");
    while (1) {
        system("cls");
        printf("\n 1. Deposit\n");
        printf(" 2. Withdraw\n");
        printf(" 3. Play Number Hunt\n");
        printf(" 4. Play Number Ambush\n");
        printf(" 5. Play Coded Danger\n");
        printf(" 6. Exit\n");
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
                game3();
                printf("Press any key to continue...\n");
                getchar(); // read a character
                getchar(); // wait for Enter key
                break;
            case 6:
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
