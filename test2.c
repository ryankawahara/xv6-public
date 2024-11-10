#include "types.h"
#include "stat.h"
#include "user.h"

// Function to calculate Fibonacci number
int fibonacci(int n) {
    if (n <= 1) return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

// Function for each child process to calculate Fibonacci numbers for a fixed duration
void calculate_fibonacci(int duration) {
    int count = 0;
    int i = 1;
    int end_time = uptime() + duration; 

    while (uptime() < end_time) {
        fibonacci(i % 20);  
        count++;
        i++;
    }
    printf(1, "Process %d calculated Fibonacci %d times.\n", getpid(), count);
}

int main() {
    int num_children = 3;
    int nice_values[] = {1, 3, 5};  
    int duration = 600;  
    int i;

    for (i = 0; i < num_children; i++) {
        int pid = fork();
        if (pid == 0) {
            // Child process
            int old_nice = nice(getpid(), nice_values[i]); 
            printf(1, "Child %d with old nice value %d and new nice value %d started...\n", getpid(), old_nice, nice_values[i]);
            calculate_fibonacci(duration);
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