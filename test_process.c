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

// Function for each child process to print primes for a fixed duration
void print_primes(int duration) {
    int count = 0;
    int i = 2;
    int end_time = uptime() + duration;  // Run for `duration` ticks

    while (uptime() < end_time) {
        if (is_prime(i)) {
            count++;
        }
        i++;
    }
    printf(1, "Process %d found %d primes.\n", getpid(), count);
}

int main() {
    int num_children = 3;
    int nice_values[] = {1, 3, 5};  // Nice values within the allowed range [1, 5]
    int duration = 100;  // Duration in clock ticks for each child to run
    int i;

    for (i = 0; i < num_children; i++) {
        int pid = fork();
        if (pid == 0) {
            // Child process
            int old_nice = nice(getpid(), nice_values[i]);  // Set the nice value
            printf(1, "Child %d with old nice value %d and new nice value %d started...\n", getpid(), old_nice, nice_values[i]);
            print_primes(duration);
            printf(1, "Child %d with nice value %d finished.\n", getpid(), nice_values[i]);
            exit();
        } else if (pid < 0) {
            printf(2, "fork failed\n");
            exit();
        }
    }

    // Parent process waits for all children
    for (i = 0; i < num_children; i++) {
        wait();
    }

    printf(1, "All children have finished.\n");
    exit();
}
