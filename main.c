#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MIN_BET 1

void deposit(int *balance, int amount) {
    *balance += amount;
    printf("Deposit successful. Current balance: %d\n", *balance);
}

void withdraw(int *balance, int amount) {
    if (*balance >= amount) {
        *balance -= amount;
        printf("Withdrawal successful. Current balance: %d\n", *balance);
    } else {
        printf("Insufficient balance. Current balance: %d\n", *balance);
    }
}

int game1(int *balance, int bet) {
    int secret_number, user_guess;

    if (*balance < bet) {
        printf("Insufficient balance. Current balance: %d\n", *balance);
        return 0;
    }

    srand(time(NULL));
    secret_number = rand() % 10 + 1;

    printf("Guess a number between 1 and 10.\n");
    scanf("%d", &user_guess);

    if (user_guess == secret_number) {
        *balance += bet * 10;
        printf("Congratulations! You won %d. Current balance: %d\n", bet * 10, *balance);
        return;
    } else {
        *balance -= bet;
        printf("Sorry, you lost. Current balance: %d\n", *balance);
        return;
    }
}

void game2(void) {
    printf("Game 2: Under development.\n");
}

int main() {
    int balance = 0, amount, bet, choice;

    while (1) {
        printf("\nWelcome to the Casino!\n");
        printf("1. Deposit\n");
        printf("2. Withdraw\n");
        printf("3. Play Game 1\n");
        printf("4. Play Game 2\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter the deposit amount: ");
                scanf("%d", &amount);
                deposit(&balance, amount);
                break;
            case 2:
                printf("Enter the withdrawal amount: ");
                scanf("%d", &amount);
                withdraw(&balance, amount);
                break;
            case 3:
                if (balance < MIN_BET) {
                    printf("Insufficient balance. Current balance: %d\n", balance);
                    break;
                }
                printf("Enter the bet amount (minimum %d): ", MIN_BET);
                scanf("%d", &bet);
                //game1(&balance, bet);
                while (game1(&balance, bet)) {
                    printf("Do you want to play again? (y/n): ");
                    char play_again;
                    scanf(" %c", &play_again);
                    if (play_again == 'y' || play_again == 'Y') {
                        continue;
                    }
                    else
                        break;
                }
                /*printf("Do you want to play again? (yes/no): ");
                    char play_again;
                    scanf(" %c", &play_again);
                    if (play_again == 'y' || play_again == 'Y'||play_again == 'yes' || play_again == 'YES'||play_again == 'y' || play_again == 'Yes') {
                        game1(&balance, bet);
                    }*/
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
