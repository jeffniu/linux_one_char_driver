#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FILE_SIZE 5000000


int main(int argc, char * argv)
{
    FILE* fourf = fopen("/dev/four", "w+");
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

    fwrite(content, sizeof(char), FILE_SIZE, fourf);

    fclose(fourf);
    return 0;
}


