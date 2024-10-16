/*
    Implement a "ping pong" using send and receive
    
    Define the rules of the game!
    One or more itetarions? What data to exchange?
    Exit condition? Visual representation?


    !! Number guessing game !!

    > process 0 (p0) thinks of a number between 0 and 100 
    > process 1 (p1) produces a guess 
    > p1 tells its guess to p0 
    > p0 tells p1 if it guessed the number is smaller, greater, or equal to the original one 
    > if the number was guessed, the game finishes 
    > otherwise, p1 tries another guess, and so on

    I will use two MPI tags:
    - 0 for p0 to send the suggestion
    - 1 for p1 to send its guess
*/

#include <mpi.h>
#include <stdio.h>

#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#include <stdbool.h>

typedef enum {
    GO_HIGHER,
    GO_LOWER,
    GOT_IT
} GUESS_RES;

int main() {

    int comm_sz;
    int my_rank;

    MPI_Init(NULL, NULL);

    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if (comm_sz != 2) {
        if (my_rank == 0) {
            fprintf(stderr, "This game needs 2 players.\n");
        }
        
        MPI_Finalize();
        return 1;
    }

    int value;
    int guess;

    bool guessed = false;
    GUESS_RES guess_result;

    int pid = getpid();
    srand(time(NULL) | pid);

    if (my_rank == 0) {
        
        // Thinking about a number
        value = rand() % 100;

        // Reacting to p1's guesses

        do {
            MPI_Recv(&guess, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("[%d] I received the guess %d\n", my_rank, guess);

            if (guess == value) {

                guess_result = GOT_IT;

                printf("[%d] You got it! The number was %d!\n", my_rank, value);
                MPI_Send(&guess_result, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

                guessed = true;
            }
            else if (guess > value) {

                guess_result = GO_LOWER;
                
                printf("[%d] Wrong! Try a lower value!\n", my_rank);
                MPI_Send(&guess_result, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            }
            else {

                guess_result = GO_HIGHER;

                printf("[%d] Wrong! Try a higher value!\n", my_rank);
                MPI_Send(&guess_result, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            }
        } while(!guessed);
    }
    else if (my_rank == 1) {

        int low_limit = 0;
        int high_limit = 99;

        // Thinking of the first guess
        guess = rand() % 100;

        // Going on until I win
        do {
            printf("[%d] I'm sending the guess %d\n", my_rank, guess);
            MPI_Send(&guess, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);

            MPI_Recv(&guess_result, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            switch (guess_result) {
                case GO_HIGHER:

                    if (guess >= low_limit) {
                        low_limit = guess;
                    }

                    do {
                        guess = rand()  % 100;
                    } while (guess <= low_limit || guess >= high_limit);

                    break;
                case GO_LOWER:

                    if (guess <= high_limit) {
                        high_limit = guess;
                    }

                    do {
                        guess = rand()  % 100;
                    } while (guess >= high_limit || guess <= low_limit);

                    break;
                case GOT_IT:

                    printf("[%d] Oh! I got it! Yay!!\n", my_rank);
                    guessed = true;

                    break;
            }

        } while (!guessed);
    }

    MPI_Finalize();

    return 0;
}