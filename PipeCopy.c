#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define DEFAULT_BUFF_SIZE 64

void readinFile(const char *srcName, int pipefd[2], const size_t bufferSize) {
    FILE *src = fopen(srcName, "rb");
    if(src == NULL){
        perror("Error opening source file");
        exit(EXIT_FAILURE);
    }

    close(pipefd[0]);
    //close the read end of the pipe

    char buffer[bufferSize];
    size_t bytesRead;

    while((bytesRead = fread(buffer, 1, sizeof(buffer), src) ) > 0){
        if(write(pipefd[1],buffer, bytesRead) != (ssize_t)bytesRead){
            perror("Error writing to pipe");
            fclose(src);
            exit(EXIT_FAILURE);
        }
    } // write in to the pipe

    
    fclose(src);
    // Close the write end to signal end-of-file.
    close(pipefd[1]);

    exit(EXIT_SUCCESS);
}

void writeFile(const char *destName, int pipefd[2], const size_t bufferSize) {
    FILE *dest = fopen(destName, "w");
    if (dest == NULL) {
        perror("Error opening destination file");
        exit(EXIT_FAILURE);
    }

    // Close unused write end in the writer process.
    close(pipefd[1]);

    char buffer[bufferSize];
    size_t bytesRead;
    while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
        if (fwrite(buffer, 1, bytesRead, dest) != bytesRead) {
            perror("Error writing to destination file");
            fclose(dest);
            exit(EXIT_FAILURE);
        }
    }

    fclose(dest);
    close(pipefd[0]);

    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]){
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <source_file> <destination_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }else if(argc > 4){
        fprintf(stderr, "Usage: %s <source_file> <destination_file> <buffer size>\n", argv[0]);
    }

    size_t bufferSize;
    if(argc == 4){
        bufferSize = atoi(argv[3]);
    }else {
        bufferSize = DEFAULT_BUFF_SIZE;
    }  // init the buffer size



    int pipefd[2]; // pipefd[0]: read end, pipefd[1]: write end

    if (pipe(pipefd) < 0) {
        perror("Pipe creation failed");
        exit(EXIT_FAILURE);
    }

    // Fork the first child for reading from source file.
    pid_t pid_read = fork();
    pid_t pid_write = fork();
    if (pid_read < 0) {
        perror("Fork failed for reader process");
        exit(EXIT_FAILURE);
    }
    if (pid_read == 0) {
        readinFile(argv[1], pipefd, bufferSize);
    }

    // Fork the second child for writing to destination file.
    if (pid_write < 0) {
        perror("Fork failed for writer process");
        exit(EXIT_FAILURE);
    }
    if (pid_write == 0) {
        writeFile(argv[2], pipefd, bufferSize);
    }

    // Wait for both child processes to complete.
    wait(NULL);
    wait(NULL);   

    // Parent process: Close both ends of the pipe as they're not used here.
    close(pipefd[0]);
    close(pipefd[1]);

    return EXIT_SUCCESS;
}