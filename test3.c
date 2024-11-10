#include "types.h"
#include "stat.h"
#include "user.h"

// Function to calculate factorial
int factorial(int n) {
    int result = 1;
    int i;
    for (i = 1; i <= n; i++) {
        result *= i;
    }
    return result;
}

// Function for each child process to calculate factorials for a fixed duration
void calculate_factorials(int duration) {
    int count = 0;
    int i = 1;
    int end_time = uptime() + duration;  // Run for duration ticks

    while (uptime() < end_time) {
        factorial(i);
        count++;
        i++;
    }
    printf(1, "Process %d calculated factorials %d times.\n", getpid(), count);
}

int main() {
    int num_children = 3;
    int nice_values[] = {1, 3, 5}; 
    int duration = 800; 
    int i;

    for (i = 0; i < num_children; i++) {
        int pid = fork();
        if (pid == 0) {
            // Child process
            int old_nice = nice(getpid(), nice_values[i]);  // Set the nice value
            printf(1, "Child %d with old nice value %d and new nice value %d started...\n", getpid(), old_nice, nice_values[i]);
            calculate_factorials(duration);
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