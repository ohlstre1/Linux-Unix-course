/* mpx-aiosig.c - Displays input from two pipes using asynchronous I/O with signals */

#include <fcntl.h>
#include <stdio.h>
#include <sys/poll.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <aio.h>

// Initialize read async event, requesting a signal notification
int init_read(struct aiocb *cb,int fd,char *buf,int bufsize) {
    cb->aio_fildes = fd;
    cb->aio_buf = buf;
    cb->aio_nbytes = bufsize;
    cb->aio_offset = 0;
    cb->aio_sigevent.sigev_notify=SIGEV_SIGNAL;
    cb->aio_sigevent.sigev_signo=SIGIO;
    return aio_read(cb);
}

int quit = 0;
int sigio = 0;
void sig_int(int signo) {
    if (signo == SIGINT) quit=1;
    else if (signo == SIGIO) sigio=1;
    else printf("signal %d\n",signo);
}

int main(void) {
    struct aiocb cbs[2];
    char buf[2][4096];
    int fds[2];
    struct sigaction sig;
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

    /* Setup signal handler. */
    sigemptyset(&sig.sa_mask);
    sig.sa_flags=0;
    sig.sa_handler = sig_int;
    sigaction(SIGINT,&sig,NULL);
    sigaction(SIGIO,&sig,NULL);

    /* Clear aiocbs */
    memset(&cbs,0,sizeof(struct aiocb)*2);

    /* Initialize AIO functions */
    if (init_read(&cbs[0],fds[0],buf[0],4096) < 0)
        perror("aio_read");
    if (init_read(&cbs[1],fds[1],buf[1],4096) < 0)
        perror("aio_read");

    /* Repeat forever */
    while (1) {
        // Wait for a signal
        pause();
        if (quit > 0) break;
        if (sigio < 1) continue;

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
