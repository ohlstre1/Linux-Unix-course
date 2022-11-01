#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>

#define DAEMON_FIFO "/tmp/daemon_fifo"
#define FIFO_PERMS (S_IRWXU | S_IWGRP| S_IWOTH)


/* closeall() -- close all FDs >= a specified value */
void closeall(void) {
	int fd = 0;
	int fdlimit = sysconf(_SC_OPEN_MAX);
	while (fd < fdlimit)
		close(fd++);
}

/**
 * Daemonize the process - detach process from user and disappear into the
 * background.
 *
 * Returns -1 on failure, but you can't do much except exit in
 * that case since we may already have forked.  This is based on the BSD
 * version, so the caller is responsible for things like the umask, etc.
 * It's believed to work on all Posix systems. */

int turn_into_daemon(void) {
	// Fork again to sever ties to parent.
	switch (fork())  {
		case 0:  break;     /* We're the child, ok. */
		case -1: return -1; /* Fork failed! */
		default: _exit(0);  /* Parent exits. */
	}

	printf("My pid %d, my ppid %d, my gpid %d\n",getpid(),getppid(),getpgrp());

	// Get a new session.
	assert(setsid() > 0);               /* shouldn't fail */

	/* Comment out this switch if you want to acquire a control tty in
	 * the future -- not normally advisable for daemons. */
	switch (fork()) {
		case 0:  break;
		case -1: return -1;
 		default: _exit(0);
	}

	printf("My pid %d, my ppid %d, my gpid %d\n",getpid(),getppid(),getpgrp());

	/* Get to the root directory, just in case the current working
	 * directory needs to be unmounted at some point. */
	chdir("/");

	/* Close all open files. */
	closeall();

	/* open /dev/null. As all fds are closed, the following open
	 * will make stdin=/dev/null  */
	open("/dev/null",O_RDWR);
	dup(0);  /* Copy /dev/null also as stdout stream */
	dup(0);  /* Copy /dev/null also as stderr stream */

	// Now we're a daemon, a lonely avenger fighting for the cause.

	return 0;
}


/* No STDOUT anymore, we use syslog to report things */

/**
 * Reports a terminal error (and quits). */
void errexit(const char *str) {
	syslog(LOG_INFO, "%s failed: %s (%d)", str, strerror(errno),errno);
	exit(1);
}

/**
 * Sends a generic error message, not fatal. */
void errreport(const char *str) {
	syslog(LOG_INFO, "%s failed: %s (%d)", str, strerror(errno),errno);
}

/* This is the daemon's main work -- listen for messages and then do something */
void process(void) {
	char str[1024];
	int len, fd;

	openlog("my_daemon", LOG_PID, LOG_DAEMON);
	umask(0);
	
	if ((mkfifo(DAEMON_FIFO, FIFO_PERMS) == -1) && (errno != EEXIST)) {
		 errexit("Failed to create a FIFO");
	}
	
	if(errno == EEXIST) {
		struct stat stat_buf;
		  
		if(stat(DAEMON_FIFO,&stat_buf) != 0)
			errexit("stat");
		if(S_ISFIFO(stat_buf.st_mode))
			syslog(LOG_INFO,"File %s exists, and is a FIFO, not problem...",DAEMON_FIFO);
		else
			errexit("File exists, and is not a FIFO, exiting...");
	}

	if ((fd = open(DAEMON_FIFO, O_RDONLY)) == -1) 
		errexit("Failed to open FIFO");

	syslog(LOG_INFO,"My pid %d, my ppid %d, my gpid %d\n",getpid(),getppid(),getpgrp());

	for ( ; ; ) {
		len = read(fd,str,1024);
		if (len>0) {
			str[len]='\0';
			syslog(LOG_INFO, "Someone sent me a job to do: %s", str);
		}
	}
}

int main(void) {
	printf("My pid %d, my ppid %d, my gpid %d\n",getpid(),getppid(),getpgrp());

	// Turn into daemon
	if (turn_into_daemon() < 0) {
		perror("daemon");
		exit(2);
	}
	// Perform the task
	process();

	return 0;
}
