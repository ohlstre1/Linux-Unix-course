/**
 * Mockup for ELEC-C7310 assignment 1.
 * This only writes data directly without doing a data transfer between processes.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <assert.h>
#include <ctype.h>
#include "morse.h"



void handle_sigint(int sig){
    printf("You cannot stop me\n");
}



int main(int argc,char **argv) {
    int ifd, ofd;

    int pipe_fd[2];
    assert(pipe(pipe_fd) == 0);
   
    struct sigaction sa = { 0 };
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = &handle_sigint;

    assert((sigaction(SIGINT, &sa, NULL)) == 0); 
    
    sleep(5);

    FILE *log_file;
    log_file  = fopen(".log", "w");
    fprintf(log_file, "Starting program\n");
    
    
    if (argc == 2) { // Only input file, output stdout
        
        ofd = STDOUT_FILENO;
        printf("\n%d\n", ofd);
        if (strcmp(argv[1],"-") == 0) {
            ifd = STDIN_FILENO;
        } else {
            ifd = open(argv[1],O_RDONLY);
            if (ifd < 0) {
                fprintf(stderr,"Opening input file failed\n");
                fprintf(log_file, "Opening input file failed\n");
                return -1;
            }
        }
    } else if (argc == 3) { // Both input and output file given
        if (strcmp(argv[1],"-") == 0) {
            ifd = STDIN_FILENO;
        } else {
            ifd = open(argv[1],O_RDONLY);
            if (ifd < 0) {
                fprintf(stderr,"Opening input file failed\n");
                fprintf(log_file, "Opening input file failed\n");
                return -1;
            }
        }
        if (strcmp(argv[2],"-") == 0) {
            ofd = STDOUT_FILENO;
        } else {
            ofd = open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,0666);
            if (ofd < 0) {
                fprintf(stderr,"Creating output file failed\n");
                fprintf(log_file, "Creating output file failed\n");
                return -1;
            }
        }
    } else {
        fprintf(stderr,"Usage: %s [input|-] [output|-]\n",argv[0]);
        fprintf(log_file, "Need input and output files");
        return -1;
    }
    
    // Allocate buffer
    char *buf = malloc(BLOCKSIZE);
    char buff[BLOCKSIZE]; 
    char *output = malloc(BLOCKSIZE*5);
    char *output1 = malloc(BLOCKSIZE*5);
    if (buf == NULL) return -1;

    int pid = fork();
    if( pid == -1 ) {
    fprintf(log_file, "Fork error\n"); return -1;
    }
   
    //child
    
    if(pid == 0){
       

        close(pipe_fd[1]);

        fprintf(log_file,"[Child] Program starting\n");
        sleep(5);
        fprintf(log_file,"[Child] Slept for 5 seconds\n");

        
        if(read(pipe_fd[0], &buff, BLOCKSIZE) == -1){
            fprintf(log_file, "[Child] Pipe Read Error");
        }
        
        close(pipe_fd[0]);
        fprintf(log_file, "[Child] Recived following= %s\n",buff);
        morseDecode(output, buff);
        fprintf(log_file, "[Child] Decoded following message=\n%s",output);
        
        write(ofd,output,strlen(output));

   
        

    //parrent process
    }else{
        


        fprintf(log_file,"[Parent] Program starting\n");
        
        close(pipe_fd[0]);
        int s;
        s = read(ifd,buf,BLOCKSIZE);
        if (s < 0) return -1;
        fprintf(log_file, "[Parent] Before encoding=\n%s",buf);
        morseEncode(output1, buf);
        fprintf(log_file, "[Parent] After encoding=\n%s",buf);
        if( write(pipe_fd[1], buf, BLOCKSIZE) == -1){
            fprintf(log_file, "[Parent] Pipe Write Error");  
        }
        close(pipe_fd[1]);

    }    
    
    free(output1);
    free(output);
    free(buf);
    close(ifd);
    close(ofd);
    fclose(log_file);
    
    return 0;
}
