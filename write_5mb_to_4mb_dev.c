#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define FILE_SIZE 5000000


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

    //fwrite(content, sizeof(char), FILE_SIZE, fourf);
    //fwrite(content, sizeof(char), FILE_SIZE, logf);

    int LEN = 1000000;
    for (int i = 0; i < FILE_SIZE; i+=LEN) {
        int count = fwrite(content+i, sizeof(char), LEN, fourf);
	printf ("fwrite count: %d\n", count);
	//fwrite(content+i, sizeof(char), 10000, logf);
    }
    fclose(fourf);
    fclose(logf);
    return 0;
}


