#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h> // for Sleep function

#define MIN_DEPOSIT 101
#define MIN_WITHDRAWAL 100
#define MIN_BET 10
#define MAX_BET 10000

typedef struct {
    int balance;
    int total_deposit;
    int amount_played;
} Player;

void deposit(Player *player, int amount) {
    if (amount < MIN_DEPOSIT) {
        printf("Minimum deposit amount is %d credits. Deposit failed.\n", MIN_DEPOSIT);
    } else {
        player->balance += amount;
        player->total_deposit += amount;
        printf("Deposit successful. Current balance: %d\n", player->balance);
    }
}

void withdraw(Player *player, int amount) {
    if (amount < MIN_WITHDRAWAL) {
        printf("Minimum withdrawal amount is %d credits. Withdrawal failed.\n", MIN_WITHDRAWAL);
    } else if (player->balance >= amount && player->amount_played >= (player->total_deposit * 0.80)) {
        player->balance -= amount;
        printf("Withdrawal successful. Current balance: %d\n", player->balance);
    } else if (player->amount_played < (player->total_deposit * 0.80)) {
        printf("You must play at least 80%% of your total deposits before withdrawing.\n");
    } else {
        printf("Insufficient balance. Current balance: %d\n", player->balance);
    }
    printf("Press any key to continue...\n");
    getchar(); // read a character
    getchar(); // wait for Enter key
}

int game1(Player *player, int bet) {
    int secret_number, user_guess;

    if (bet < MIN_BET || bet > MAX_BET) {
        printf("Bet must be between %d and %d credits.\n", MIN_BET, MAX_BET);
        printf("Press any key to continue...\n");
        getchar(); // read a character
        getchar(); // wait for Enter key
        return 1;
    }

    if (player->balance < bet) {
        printf("Insufficient balance. Current balance: %d\n", player->balance);
        printf("Press any key to continue...\n");
        getchar(); // read a character
        getchar(); // wait for Enter key
        return 1;
    }

    srand(time(NULL)); // seed random number
    secret_number = rand() % 10 + 1;

    printf("Guess a number between 1 and 10.\n");
    scanf("%d", &user_guess);

    player->amount_played += bet;

    if (user_guess == secret_number) {
        player->balance += bet * 10;
        printf("Congratulations! You won %d. Current balance: %d\n", bet * 10, player->balance);
    } else {
        player->balance -= bet;
        printf("Sorry, you lost. Current balance: %d\n", player->balance);
    }
    printf("Press any key to continue...\n");
    getchar(); // read a character
    getchar(); // wait for Enter key
    return 1;
}

void game2(void) {
    printf("Game 2: Under development.\n");
    printf("Press any key to continue...\n");
    getchar(); // read a character
    getchar(); // wait for Enter key
}

int main() {
    Player players[2] = {{0, 0, 0}, {0, 0, 0}};
    int num_players, player_choice, amount, bet, choice;
    char play_again;

    printf("\nWelcome to the Casino!\n");
    printf("Enter number of players (1 or 2): ");
    scanf("%d", &num_players);

    while (1) {
        system("cls");
        printf("\nChoose a player (1 or 2): ");
        scanf("%d", &player_choice);
        if (player_choice < 1 || player_choice > num_players) {
            printf("Invalid player choice. Please try again.\n");
            continue;
        }

        Player *current_player = &players[player_choice - 1];
        printf("\n 1. Deposit\n");
        printf(" 2. Withdraw\n");
        printf(" 3. Play Game 1\n");
        printf(" 4. Play Game 2\n");
        printf(" 5. Exit\n");
        printf("Player %d Balance: %d\n", player_choice, current_player->balance);
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter the deposit amount: ");
                scanf("%d", &amount);
                deposit(current_player, amount);
                break;
            case 2:
                printf("Enter the withdrawal amount: ");
                scanf("%d", &amount);
                withdraw(current_player, amount);
                break;
            case 3:
                if (current_player->balance < MIN_BET) {
                    printf("Insufficient balance. Current balance: %d\n", current_player->balance);
                    printf("Press any key to continue...\n");
                    getchar(); // read a character
                    getchar(); // wait for Enter key
                    break;
                }
                printf("Enter the bet amount (minimum %d, maximum %d): ", MIN_BET, MAX_BET);
                scanf("%d", &bet);
                while (game1(current_player, bet)) {
                    printf("Do you want to play again? (y/n): ");
                    scanf(" %c", &play_again);
                    if (play_again == 'y' || play_again == 'Y') {
                        if (current_player->balance < MIN_BET) {
                            printf("Insufficient balance. Current balance: %d\n", current_player->balance);
                            printf("Press any key to continue...\n");
                            getchar(); // read a character
                            getchar(); // wait for Enter key
                            break;
                        }
                        printf("Enter the bet amount (minimum %d, maximum %d): ", MIN_BET, MAX_BET);
                        scanf("%d", &bet);
                        continue;
                    } else {
                        break;
                    }
                }
                break;
            case 4:
                game2();
                break;
            case 5:
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
