#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// A xv6-riscv syscall can take up to six arguments.
#define max_args 6

// Print a help message.
void print_help(int argc, char **argv) {
    fprintf(2, "%s <options: pid or S/R/X/Z>%s\n",
             argv[0], argc > 7 ? ": too many args" : "");
}

// Function to check if a string represents a valid number.
int num_check(const char *str) {
    if (*str == '\0') {
        return 0; // Empty string is not a valid number.
    }
    for (; *str != '\0'; str++) {
        if (*str < '0' || *str > '9') {
            return 0; // Not a number if any character is not between '0' and '9'.
        }
    }
    return 1; // The string is a valid number.
}


int main(int argc, char **argv) {
    // Print a help message if there are too many arguments.
    if (argc > 7) {
        print_help(argc, argv);
        exit(1);
    }

    // Argument vector to pass to the pstate() system call.
    int args[max_args];
    memset(args, 0, max_args * sizeof(int));

    // Process the command-line arguments.
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == 'S' || argv[i][0] == 'R' || argv[i][0] == 'X' || argv[i][0] == 'Z') {
            // Ensure that the character argument is exactly one character.
            if (argv[i][1] != '\0') {
                // Invalid input, characters like "SZ" or "S2" are not allowed.
                print_help(argc, argv);
                exit(1);
            }
            // Map the character to a specific state value (negative for system states).
            switch (argv[i][0]) {
                case 'S':
                    args[i - 1] = -1;
                    break;
                case 'R':
                    args[i - 1] = -2;
                    break;
                case 'X':
                    args[i - 1] = -3;
                    break;
                case 'Z':
                    args[i - 1] = -4;
                    break;
            }
        } else {
            if(num_check(argv[i])){
              args[i - 1] = atoi(argv[i]);
            } else{
                print_help(argc, argv);
                exit(1);
            }
        }
    }
  // printf("argv : %d %d %d %d %d %d\n", args[0], args[1], args[2], args[3], args[4], args[5]);
    // Call the pstate() system call with the parsed arguments.
    int ret = pstate(args[0], args[1], args[2], args[3], args[4], args[5]);
    if (ret) {
        fprintf(2, "pstate failed\n");
        exit(1);
    }

    exit(0);
}
