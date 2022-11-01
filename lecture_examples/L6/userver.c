/* userver.c - Simple Unix Domain Socket server */

/* Waits for a connection on the ./sample-socket Unix domain
   socket. Once a connection has been established, copy data
   from the socket to stdout until the other end closes the
   connection, and then wait for another connection to the 
   socket. */

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
  int sock, conn;
  socklen_t addrLength;

  assert((sock = socket(PF_UNIX, SOCK_STREAM, 0)) >= 0);

  /* Remove any preexisting socket (or other file) */
  unlink("./sample-socket");

  address.sun_family = AF_UNIX;       /* Unix domain socket */
  strcpy(address.sun_path, "./sample-socket");

  /* The total length of the address includes the sun_family 
     element */
  addrLength = sizeof(address.sun_family) + 
               strlen(address.sun_path);

  assert(bind(sock, (struct sockaddr *) &address, addrLength) == 0);

  assert(listen(sock, 5) == 0);

  while ((conn = accept(sock, (struct sockaddr *) &address, 
                        &addrLength)) >= 0) {
    printf("---- getting data\n");
    copydata(conn,STDOUT_FILENO);
    printf("---- done\n");
    close(conn);
  }

  assert(conn >= 0);
    
  close(sock);
  return 0;
}
