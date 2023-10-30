
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "morse.c"
#include <time.h>

#define BLOCKSIZE 4096
#include <signal.h>


time_t start_time, current_time;
volatile sig_atomic_t flag = 0;

void handle_signal(int signo) {
    if (signo == SIGUSR1) {
        flag = 1;
    } else if (signo == SIGUSR2) {
        flag = 2;
    } else if (signo == SIGALRM) {
        flag = 3;
    } else if (signo == SIGHUP) {
        flag = 4;
    } else if(signo ==  SIGINT){
        write(STDOUT_FILENO, "Program interupted..exiting\n", 29); 
    }else{
        write(STDOUT_FILENO, "Unkown signal\n", 15);    }
}

/**
 * Sends Morse code as signals to the parent process.
 * 
 * @param morseBuf The Morse code string to be sent.
 * @param log_file File pointer to the log file for logging errors.
 * @return 0 on success, -1 on failure.
 */
int sendMorseAsSignals(const char *morseBuf, FILE *log_file) {
    for (int i = 0; i < strlen(morseBuf)-1 ; i++) {
        usleep(1000);
        if(morseBuf[i] == '.'){
            kill(getppid(), SIGUSR1);
        }else if(morseBuf[i] == '-'){
            kill(getppid(), SIGUSR2);
        }else if(morseBuf[i] == '/'){
            kill(getppid(), SIGALRM);
        }else if (morseBuf[i] == ' '){
            kill(getppid(), SIGHUP);
        }else{
            write(STDOUT_FILENO, "Unkown char\n", 13);
        }
    }
    return 0;
}

void setString(char c, char* str) {
    time(&start_time);
    int len = strlen(str);
    str[len] = c;
    str[len + 1] = '\0';
}


int main(int argc,char **argv) {
    int ifd, ofd; // idf: Stands for input file descriptor. & ofd: Stands for output file descriptor.
    
    FILE *log_file;
    log_file  = fopen(".log", "w");
    fprintf(log_file, "[%d]Starting program\n", getpid());

    if (argc == 2) { // Only input file, output stdout
        ofd = STDOUT_FILENO;
        if (strcmp(argv[1],"-") == 0) {
            ifd = STDIN_FILENO;
        } else {
            ifd = open(argv[1],O_RDONLY);
            if (ifd < 0) {

                fprintf(log_file, "Opening input file (%s) failed\n",argv[1]);
                return -1;
            }
        }
    } else if (argc == 3) { // Both input and output file given
        if (strcmp(argv[1],"-") == 0) {
            ifd = STDIN_FILENO;
        } else {
            ifd = open(argv[1],O_RDONLY);
            if (ifd < 0) {
                fprintf(log_file,"Opening input file (%s) failed\n",argv[1]);

                return -1;
            }


        }
        if (strcmp(argv[2],"-") == 0) {
            ofd = STDOUT_FILENO;
        } else {
            ofd = open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,0644);
            if (ofd < 0) {
                fprintf(log_file,"Creating output file (%s) failed\n",argv[2]);
                return -1;
            }
        }
    } else {
        fprintf(log_file,"Usage: %s [input|-] [output|-]\n",argv[0]);
        return -1;
    }

   

    char *morseBuf = malloc(BLOCKSIZE * 5); // Assuming worst case, every character is converted to a Morse string of length 4 (e.g., "-...") plus one space.
    if (morseBuf == NULL){ fprintf(log_file,"Malloc Error in morseBuf"); return -1;}       

    char *buf2 = malloc(BLOCKSIZE);
    if(buf2 == NULL) {fprintf(log_file,"Malloc Error in buf2"); return -1;}
    // Allocate buffer
    char *buf = malloc(BLOCKSIZE);
    if (buf == NULL){ fprintf(log_file,"Malloc Error in buf"); return -1;}
    
 
    int pid = fork();
    if( pid == -1 ) {
        fprintf(log_file, "Fork Error"); return -1;
    }else    fprintf(log_file,"[%d]Forking\n",getpid());

    
    if(pid == 0){ //child process
        fprintf(log_file,"[%d]Child process started\n",getpid());

        int s = read(ifd,buf,BLOCKSIZE);
        if (s < 0) {fprintf(log_file, "[%d]Read Error", getpid()); return -1;}
        if (s == 0) {fprintf(log_file, "Input closed"); return -1;} // input closed
        sleep(2);
        stringToMorse(buf, morseBuf);

        fprintf(log_file, "[%d]String to Encode:\"%s\"\n", getpid(), buf);
        fprintf(log_file, "[%d]Morse string:\"%s\"\n",getpid(),morseBuf);
        fprintf(log_file, "[%d]Sending string through signals..\n",getpid());
        
        if(sendMorseAsSignals(morseBuf, log_file) != 0) return -1;

        fprintf(log_file,"[%d]Signals sent\n",getpid());
   
        fprintf(log_file,"[%d]Child process finished\n",getpid());

    }else{ // parent
        fprintf(log_file,"[%d]Parent process started\n", getpid());
        struct sigaction sa;
        sa.sa_handler = &handle_signal;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;

        
        if (sigaction(SIGUSR1, &sa, NULL) == -1){ fprintf(log_file,"sigaction");}
        if (sigaction(SIGUSR2, &sa, NULL) == -1){ fprintf(log_file,"sigaction");}
        if (sigaction(SIGALRM, &sa, NULL) == -1) {fprintf(log_file,"sigaction");}
        if (sigaction(SIGHUP, &sa, NULL) == -1) {fprintf(log_file,"sigaction");}


        fprintf(log_file, "[%d]Listening for signals until no new signals in 10 seconds\n", getpid());
        time(&start_time);



        while (1) {

            time(&current_time); // Update the current time in each iteration
            if (difftime(current_time, start_time) >= 10) { // Check if 10 seconds have passed
                fprintf(log_file, "[%d| No Signals in last 10 seconds\n", getpid());
                break;
            }

        if (flag == 1) {
            flag = 0;
            setString('.', morseBuf);
        }else if(flag == 2){
            setString('-', morseBuf);
            flag=0;
        }else if(flag == 3){
            setString('/', morseBuf);
            flag=0;
        }else if(flag == 4){
            setString(' ', morseBuf);
            flag=0;
        }

        }

        fprintf(log_file, "[%d]Recived encoded string is:\"%s\"\n", getpid(), morseBuf);
        morseToString(morseBuf, buf2);
        fprintf(log_file, "[%d]Recived string is:\"%s\"\n",getpid(),buf2); 



        size_t to_write = strlen(buf2);
        ssize_t written = 0;
        size_t r = 0;


        while (r < to_write) {
            written = write(ofd, buf2 + r, to_write - r);
            if (written < 0) {
                fprintf(log_file, "[%d] Write Error:\n", getpid());
                return -1;
            }
            r += written;
        }


        // while (r < strlen(buf2)) {
        //     ssize_t result = write(ofd, buf2 + r, strlen(buf2) - r);
        //     if (result < 0) {
        //         fprintf(log_file, "[%d] Write Error", getpid());
        //         return -1;}
        //     r += result;
        //     }


        fprintf(log_file,"[%d]Parent process finished\n",getpid()); 
    }

    fclose(log_file);
    free(buf2);
    free(morseBuf);
    free(buf);
    close(ifd);
    close(ofd);

    return 0;
}
