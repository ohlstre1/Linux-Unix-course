/**
 * \file
 * Mockup bank client for Assignment #2
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define BUFSIZE 255

void* routine(){
    printf("test");
}


int main(int argc,char **argv) {
    // Initialize communication with the bank server? We'll just fake it.
    
    phtread_t t1,t2;
    assert(phtread_create(&t1,NULL, &routine, NULL) != 0 );
    
    assert(phtread_create(&t2,NULL, &routine, NULL) != 0);
    phteard_join(&t1,NULL);
    phteard_join(&t2,NULL);
    
    
    printf("Connecting to the bank, please wait.\n");
    
    //sleep(5);
    
    // Connect successful.



    printf("ready\n");
    int quit = 0;
    char *buf = malloc(BUFSIZE);
    if (buf == NULL) return -1;
    while (!quit) {
        // Read a command
        if (fgets(buf,BUFSIZE,stdin) == NULL) break;
        // Process the command
        switch (buf[0]) {	// First letter determines the command
        case 'q': // quit
            quit = 1;
            break;
        case 'l': { // List account value
            int accno = -1;
            if (sscanf(buf,"l %d",&accno) == 1) {
                printf("ok: %d\n",accno+10); // Let's just fake a value
            } else printf("fail: Error in command\n");
            break;
        }
        case 'w': { // Withdraw from an account
            int accno = -1, amnt = 0;
            if (sscanf(buf,"w %d %d",&accno,&amnt) == 2) {
                printf("ok: did it\n");
            } else printf("fail: something went wrong\n");
            break;
        }
        case 't': // Transfer from one account to another
            printf("ok: just pretending it worked\n");
            break;
        case 'd': // Deposit to an account
            printf("ok: trust us, your money is safe\n");
            break;
        default: // Unknown command
            printf("fail: Unknown command\n");
            break;
        }
    }
    // Clean up
    free(buf);
    return 0;
}

