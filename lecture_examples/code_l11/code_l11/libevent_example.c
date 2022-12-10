/*
 * Source: http://tjake.posterous.com/libevent-webserver-in-40-lines-of-c 
 * Modified to report event mechanism.
 * Select event method using environmental variables:
 * "Additional Notes
 * It is possible to disable support for epoll, kqueue, devpoll, poll or
 * select by setting the environment variable EVENT_NOEPOLL, EVENT_NOKQUEUE,
 * EVENT_NODEVPOLL, EVENT_NOPOLL or EVENT_NOSELECT, respectively. By setting
 * the environment variable EVENT_SHOW_METHOD, libevent displays the kernel
 * notification method that it uses."
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/queue.h>

#include <err.h>
#include <event.h>
#include <evhttp.h>

void generic_handler(struct evhttp_request *req, void *arg) {
  struct evbuffer *buf;
  buf = evbuffer_new();

  if (buf == NULL)
    err(1, "failed to create response buffer");

  evbuffer_add_printf(buf, "Requested: %sn", evhttp_request_uri(req));
  evhttp_send_reply(req, HTTP_OK, "OK", buf);
}

int main(int argc, char **argv) {
  struct evhttp *httpd;

  event_init();
  httpd = evhttp_start("0.0.0.0", 8080);

  /* Set a callback for requests to "/specific". */
  /* evhttp_set_cb(httpd, "/specific", another_handler, NULL); */

  /* Set a callback for all other requests. */
  evhttp_set_gencb(httpd, generic_handler, NULL);

  printf("Starting with libevent version %s, using method %s\n",event_get_version(),event_get_method());

  event_dispatch();

  /* Not reached in this code as it is now. */
  evhttp_free(httpd);

  return 0;
}
