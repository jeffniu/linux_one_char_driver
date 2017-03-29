#include <stdio.h>

#define FILE_SIZE 5242880

int main(int argc, char * argv)
{
    FILE* fourf = fopen("/dev/four", "w+");
    char content[FILE_SIZE];
    for (int i = 0; i < FILE_SIZE; i++) {
	content[i] = FILE_SIZE%(FILE_SIZE/26)+ 'a';
    }
    content[FILE_SIZE-1] = '\0';

    fwrite(content, sizeof(char), FILE_SIZE, fourf);
    fclose(fourf);
    return 0;
}
