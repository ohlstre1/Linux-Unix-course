#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#define BUFSIZE 128

#define FIFO_PERMS (S_IRWXU | S_IWGRP| S_IWOTH)

 
int main(int argc, char *argv[]) 
{
   char buf[BUFSIZE];
   int buflen;
   pid_t childpid;
   int fd; 

    /* name of server fifo is passed on the command line */
  if (argc != 2) 
  {
      fprintf(stderr, "Usage: %s fifoname\n", argv[0]);
      return 1; 
   }
                         /* create a named pipe to handle incoming requests */
   if ((mkfifo(argv[1], FIFO_PERMS) == -1) && (errno != EEXIST)) 
   {
       perror("Failed to create a FIFO");
       return 1; 
   }

    
   childpid = fork();
   if (childpid == -1) 
   {
      perror("Failed to fork"); 
      return 1;
   }
   if (childpid)                                       /* parent code */
   {
       /* open a write communication endpoint to the pipe */

        if ((fd = open(argv[1], O_WRONLY)) == -1) 
        {
          perror("Failed to open FIFO");
          return 1;
        }

	strcpy(buf,"hello my child");
	buflen=strlen(buf);
   	printf("Parent [%d]: sending \"%s\" to child...",getpid(),buf);
        write(fd, buf, buflen);
        printf("Done\n");
        wait(NULL);
        printf("Parent [%d]: child has terminated, exiting\n",getpid());
   }
   else                                                 /* child code */
   {
   /* open a read communication endpoint to the pipe */

   sleep(5);
   if ((fd = open(argv[1], O_RDONLY)) == -1) 
   {
       perror("Failed to open FIFO");
       return 1;
   }

      sleep(2);
      buflen = read(fd, buf, BUFSIZE);
      
      buf[buflen]='\0';
      if(buflen > 0)
        fprintf(stderr, "Child [%d]: parent said (%d) \"%s\"\n", getpid(), buflen, buf);
   }
   close(fd);
   return 0;
}
