#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int main(void) {
    int iterations = 101; // test 10 different block sizes: 10, 20, ... 100

    for (int i = 1; i <= iterations; i++) {
        int block_size = 10 * i;
        char blockSizeStr[12];
        sprintf(blockSizeStr, "%d", block_size);

        struct timespec start, end;
        // Record start time.
        if (clock_gettime(CLOCK_MONOTONIC, &start) == -1) {
            perror("clock_gettime start");
            exit(EXIT_FAILURE);
        }

        pid_t pid = fork();
        if (pid < 0) {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }
        if (pid == 0) {
            // Child process: execute ForkCopy with input.txt, output.txt and the block size.
            execl("./ForkCopy", "ForkCopy", "input.txt", "output.txt", blockSizeStr, (char *)NULL);
            perror("execl failed");
            exit(EXIT_FAILURE);
        } else {
            // Parent process waits for the child to complete.
            wait(NULL);
            if (clock_gettime(CLOCK_MONOTONIC, &end) == -1) {
                perror("clock_gettime end");
                exit(EXIT_FAILURE);
            }

            double elapsed = (end.tv_sec - start.tv_sec) + 
                             (end.tv_nsec - start.tv_nsec) / 1e9;
            printf("Block size: %d bytes, Elapsed time: %f seconds\n", block_size, elapsed);
        }
    }

    return 0;
}