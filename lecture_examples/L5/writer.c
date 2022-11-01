/* writer.c - pipes data from stdin to "outputfile". */

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(void) {
    int fd;
    char buf[4096];
    int i;

    if ((fd = open("outputfile", O_CREAT|O_TRUNC|O_WRONLY,0775)) < 0) {
        perror("open");
        return 1;
    }
    while (1) {
        i = read(STDIN_FILENO, buf, sizeof(buf));
        if (i < 0) {
            perror("read");
            return 1;
        } else if (!i) {
            printf("input closed\n");
            return 0;
        } 
        write(fd,buf,i);
    }
}
