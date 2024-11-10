
#include "types.h"
#include "stat.h"
#include "user.h"

int invalid_pid() {
    // this function shows nice behaving properly
    int pid = 100;
    int value = 2;
    int old_value = nice(pid, value);
    if (old_value != -1) {
        printf(1, "Changed process %d from a priority of %d to %d\n", pid, old_value, value);
    }
    else {
        return 1;
    }
    
    return 0;
}

int invalid_nice() {
    // this function shows nice changing the same pid multiple times
    int pid = 2;
    int value_1 = -2;
    int old_value_1 = nice(pid, value_1);
    int value_2 = 9;
    int old_value_2 = nice(pid, value_2);

    if (old_value_1 != -1) {
        printf(1, "Changed process %d from a priority of %d to %d\n", pid, old_value_1, value_1);
    }
    if (old_value_2 != -1) {
        printf(1, "Changed process %d from a priority of %d to %d\n", pid, old_value_2, value_2);
    }

    if (old_value_1 < 0 || old_value_2 < 0) {
        return 1;
    }
    
    return 0; 
}


int main(int argc, char *argv[]) {

    invalid_pid();
    invalid_nice();

    exit();
}