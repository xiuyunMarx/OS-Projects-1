#include <stddef.h>
#include <stdio.h>
#include "stdlib.h"

void copy(FILE *src, FILE *dest){
    char inputBuffer[10];
    size_t readBytes;
    size_t block_size = 1,block_count = 1;
    while((readBytes = fread(inputBuffer,block_size,block_count, src))){
        if(readBytes > 0){
            fwrite(inputBuffer, block_size, readBytes, dest);
        }else { 
            //no complete block read, read in char
            char ch;
            ch = fgetc(src);
            while(ch != EOF){
                fputc(ch, dest);
                ch = fgetc(src);
            }
            fputc(ch, dest);
            break;
        }
    }
    
    return ;
}
int main(int argc, char *argv[]){
    FILE *src, *dest;
    src = fopen(argv[1], "r");
    dest = fopen(argv[2], "w+");
    if(src == NULL){
        printf("File open error : file %s", argv[1]);
        exit(1);
    }
    if(dest ==NULL){
        printf("File open error : file %s", argv[2]);
        exit(1);
    }    
    copy(src, dest);
    fclose(src);
    fclose(dest);
    return 0;
}
