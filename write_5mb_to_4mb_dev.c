#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

#define FILE_SIZE 5000000


int main(int argc, char * argv)
{
    int fourf = open("/dev/four", O_RDWR);
    printf ("open result: %d\n", fourf);
    int logf = open("log.txt", O_RDWR|O_CREAT);
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

    int LEN = 1000000;
    for (int i = 0; i < FILE_SIZE; i+=LEN) {
        int result = write(fourf, content+i, (ssize_t) LEN);
	printf ("write result: %d\n", result);
        write(logf, content+i, (ssize_t) LEN);
    }

    close(fourf);
    close(logf);
    return 0;
}


