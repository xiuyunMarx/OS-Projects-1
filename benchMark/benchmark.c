#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <the program to benchmark>\n", argv[0]);
        exit(1);
    }

    fprintf(stdout, "The program to benchmark is: %s\n", argv[1]);
    struct timespec start, end;
    double total_time = 0.0;

    char pathToProgram[200] = "/home/xyma/MarxFiles/Homework/project1/";
    // Append the program name to the "./" directory
    strcat(pathToProgram, argv[1]);
    // printf("Executable path: %s\n", pathToProgram);

    // Benchmark the program 10 times.
    for (int i = 1; i < 102; i++) {
        if (clock_gettime(CLOCK_MONOTONIC, &start) == -1) {
            fprintf(stderr, "Error getting start time\n");
            exit(1);
        }

        pid_t pid = fork();
        if (pid < 0) {
            fprintf(stderr, "fork failed\n");
            exit(1);
        }
        
        if (pid == 0) { // Child process
            char numStr[12];
            sprintf(numStr, "%d", 10 * i);
            // execl: first argument is the executable path,
            // second argument is conventionally the program name,
            // followed by parameters.
            execl(pathToProgram, argv[0],  "/home/xyma/MarxFiles/Homework/project1/input.txt",
                "/home/xyma/MarxFiles/Homework/project1/benchMark/output.txt", numStr, (char *)NULL);
            perror("execution failed in benchmark");
            exit(1);
        } else { // Parent process

            wait(NULL);
            if (clock_gettime(CLOCK_MONOTONIC, &end) == -1) {
                fprintf(stderr, "Error getting end time\n");
                exit(1);
            }

            double elapsed = (end.tv_sec - start.tv_sec) +
                             (end.tv_nsec - start.tv_nsec) / 1e9;
            printf("with block size = %d Iteration %d elapsed time: %f seconds\n",10*i, i, elapsed);
            total_time += elapsed;
        }
    }

    printf("Average elapsed time: %f seconds\n", total_time / 10);
    return 0;
}