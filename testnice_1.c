
#include "types.h"
#include "stat.h"
#include "user.h"

int regular_nice() {
    // this function shows nice behaving properly
    int pid = 1;
    int value = 2;
    int old_value = nice(pid, value);
    printf(1, "Changed process %d from a priority of %d to %d\n", pid, old_value, value);
    
    return 0;
}

int change_multiple_nice() {
    // this function shows nice changing the same pid multiple times
    int pid = 2;
    int value_1 = 2;
    int value_2 = 1;
    int old_value_1 = nice(pid, value_1);
    int old_value_2 = nice(pid, value_2);

    printf(1, "Changed process %d from a priority of %d to %d\n", pid, old_value_1, value_1);
    printf(1, "Changed process %d from a priority of %d to %d\n", pid, old_value_2, value_2);

    
    return 0; // Return 0 on success
}


int main(int argc, char *argv[]) {

    regular_nice();
    change_multiple_nice();

    exit();
}