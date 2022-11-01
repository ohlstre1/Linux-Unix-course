/* mpx-fork.c - Displays input from two pipes using fork */

#include <fcntl.h>
#include <stdio.h>
#include <sys/select.h>
#include <unistd.h>

int main(void) {
    int fds[2];
    char buf[4096];
    int rc;

    /* open both pipes */
    if ((fds[0] = open("p1", O_RDONLY)) < 0) {
        perror("open p1");
        return 1;
    }

    if ((fds[1] = open("p2", O_RDONLY)) < 0) {
        perror("open p2");
        return 1;
    }
 
    /* start off reading from both file descriptors */
    int pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }
    
    // Decide which process handles which FD
    int myfd;
    if (pid == 0) { // child
        myfd = fds[0];
    } else {
        myfd = fds[1];
    }

    while (1) {
        // Do blocking read from myfd
        rc = read(myfd, buf, sizeof(buf) - 1);
        if (rc < 0) {
            perror("read");
            return 1;
        } else if (!rc) {
            /* this pipe has been closed, we can exit. */
            close(myfd);
            return 0;
        } else {
            buf[rc] = '\0';
            // Note: output writes are not synchronized between processes
            printf("read: %s", buf);
        }
    }

    return 0;
}
