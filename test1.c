#include "types.h"
#include "stat.h"
#include "user.h"

// Function to check if a number is prime
int is_prime(int num) {
    int j;
    if (num < 2) return 0;
    for (j = 2; j * j <= num; j++) {
        if (num % j == 0) return 0;
    }
    return 1;
}

// Function for each child process to count primes up to a fixed limit
void count_primes(int max_count, int nice_val) {
    int count = 0;
    int i = 2; 

    while (count < max_count) {
        if (is_prime(i)) {
            count++;
        }
        i++;
    }
    printf(1, "Process %d counted %d primes with nice value %d.\n", getpid(), count, nice_val);
}

int main(void) {
    int num_children = 3;
    int nice_values[] = {1, 3, 5};  
    int prime_count = 100000*1.5;  
    int i;

    for (i = 0; i < num_children; i++) {
        int pid = fork();
        if (pid == 0) {
            // Child process
            nice(getpid(), nice_values[i]);  // Set the nice value for priority
            printf(1, "Child process %d started with nice value %d\n", getpid(), nice_values[i]);
            count_primes(prime_count, nice_values[i]);  // Each child counts a fixed number of primes
            printf(1, "Child process %d with nice value %d finished counting primes.\n", getpid(), nice_values[i]);
            exit();
        } else if (pid < 0) {
            printf(2, "fork failed\n");
            exit();
        }
    }

    // Parent process waits for all children to finish
    for (i = 0; i < num_children; i++) {
        wait();
    }

    printf(1, "All child processes have finished.\n");
    exit();
}
