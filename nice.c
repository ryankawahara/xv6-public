
#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]) {
    int pid, value, old_value;

    if (argc < 2) {
        printf(2, "Usage: nice <pid> <value> or nice <value>\n");
        exit();
    }

    if (argc == 2) {
        pid = getpid();  // Assume current process if only value is given
        value = atoi(argv[1]);
    } else {
        pid = atoi(argv[1]);
        value = atoi(argv[2]);
    }

    old_value = nice(pid, value);  // Call the system call
    if (old_value < 0) {
        printf(2, "Error: Failed to set nice value.\n");
    } else {
        printf(1, "%d %d\n", pid, old_value);
    }

    exit();
}