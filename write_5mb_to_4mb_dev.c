#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FILE_SIZE 2000000


int main(int argc, char * argv)
{
    FILE* fourf = fopen("/dev/four", "w+");
    FILE* logf = fopen("log.txt", "w+");
    char content[FILE_SIZE];
    char number[100];
    int num = 1;
    for (int i = 0; i < FILE_SIZE; ){
        sprintf(number, " %d", num);
	strcat(content, number);
	i += strlen(number);
	num++;
    }
    content[FILE_SIZE-1] = '\0';

    for (int i = 0; i < FILE_SIZE; i+=1000) {
        fwrite(content+i, sizeof(char), 1000, fourf);
	fwrite(content+i, sizeof(char), 1000, logf);
    }

    fclose(fourf);
    fclose(logf);
    return 0;
}


