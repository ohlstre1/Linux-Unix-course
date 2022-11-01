#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>

#define BUFSIZE 128
 
int main(void) {
   char buf[BUFSIZE] = "Hello my child!";
   int bytes;
   pid_t childpid;  
   int fd[2]; 
    
   if (pipe(fd) == -1) {
      perror("Failed to create the pipe"); 
      return 1; 
   } 
   printf("Size of pipe buffer: %d\n",PIPE_BUF);
   bytes = strlen(buf);
   childpid = fork();
   if (childpid == -1) {
      perror("Failed to fork"); 
      return 1;
   }
   if (childpid)                                       /* parent code */
   {
   	close(fd[0]);
   	printf("Parent [%d]: sending \"%s\" to child...",getpid(),buf);
        write(fd[1], buf, bytes);
        printf("Done\n");
        wait(NULL);
        printf("Parent [%d]: child has terminated, exiting\n",getpid());
        
   }
   else                                                 /* child code */
   {
	close(fd[1]);  	
      sleep(2);
      bytes = read(fd[0], buf, BUFSIZE);
      buf[bytes]='\0';
      fprintf(stderr, "Child [%ld]: my mamma said \"%s\"\n",
           (long)getpid(), buf);
   }
   return 0;
}
