/* uclient.c - Simple Unix Domain Socket client */

/* Connect to the ./sample-socket Unix domain socket, copy stdin
   into the socket, and then exit. */

#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <assert.h>

void copydata(int from,int to) {
  char buf[1024];
  int amount;

  while ((amount = read(from, buf, sizeof(buf))) > 0) {
    assert((write(to, buf, amount) == amount));
  }
  assert(amount >= 0);
}


int main(void) {
  struct sockaddr_un address;
  int sock;
  size_t addrLength;

  assert((sock = socket(PF_UNIX, SOCK_STREAM, 0)) >= 0);

  address.sun_family = AF_UNIX;    /* Unix domain socket */
  strcpy(address.sun_path, "./sample-socket");

  /* The total length of the address includes the sun_family 
     element */
  addrLength = sizeof(address.sun_family) + 
               strlen(address.sun_path);

  assert((connect(sock, (struct sockaddr *) &address, addrLength)) == 0);

  copydata(STDIN_FILENO,sock);

  close(sock);
    
  return 0;
}
