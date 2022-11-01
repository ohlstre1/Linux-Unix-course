/* mpx-aio.c - Displays input from two pipes using asynchronous I/O */

#include <fcntl.h>
#include <stdio.h>
#include <sys/poll.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <aio.h>

// Initialize read async event
int init_read(struct aiocb *cb,int fd,char *buf,int bufsize) {
    cb->aio_fildes = fd;
    cb->aio_buf = buf;
    cb->aio_nbytes = bufsize;
    cb->aio_offset = 0;
    return aio_read(cb);
}


int main(void) {
    struct aiocb cbs[2];
    char buf[2][4096];
    int fds[2];
    int i;

    /* open both pipes */
    if ((fds[0] = open("p1", O_RDONLY)) < 0) {
        perror("open p1");
        return 1;
    }

    if ((fds[1] = open("p2", O_RDONLY)) < 0) {
        perror("open p2");
        return 1;
    }
    
    /* Clear aiocbs */
    memset(&cbs,0,sizeof(struct aiocb)*2);

    /* Initialize AIO functions */
    if (init_read(&cbs[0],fds[0],buf[0],4096) < 0)
        perror("aio_read");
    if (init_read(&cbs[1],fds[1],buf[1],4096) < 0)
        perror("aio_read");

    /* Repeat forever */
    while (1) {
        /* check to see which file descriptors are ready to be
           read from */
        for (i = 0; i < 2; i++) {
            int ret;
            // Check if this FD is still in progress
            if ((ret = aio_error(&cbs[i])) == EINPROGRESS)
                continue;
            ret = aio_return(&cbs[i]);
            if (ret > 0) {
                // We've read some data
                buf[i][ret] = '\0';
                printf("read: %s", buf[i]);
                // Restart read.
                if (init_read(&cbs[i],fds[i],buf[i],4096) < 0)
                    perror("aio_read");
            } else {
                perror("aio_return");
                return 1;
            }
        } 
    }

    return 0;
}
