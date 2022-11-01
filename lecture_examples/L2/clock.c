#include <stdio.h>
#include <time.h>

/*
int clock_getres(clockid_t clockid, struct timespec *res);

int clock_gettime(clockid_t clockid, struct timespec *tp);
 */

long long timediff(struct timespec *t1,struct timespec *t2) {
    long long nsec;
    nsec = (t2->tv_sec - t1->tv_sec)*1000000000;
    nsec += (t2->tv_nsec - t1->tv_nsec);
    return nsec;
}

int main(void) {


    struct timespec t1,t2,res;
    clock_getres(CLOCK_MONOTONIC,&res);
    clock_gettime(CLOCK_MONOTONIC,&t1);

    // here something
    int i;
    for (i = 0; i < 10000; i++) ;

    clock_gettime(CLOCK_MONOTONIC,&t2);
    long long nsecs = timediff(&t1,&t2);
    printf("nanoseconds %lld\n",nsecs);
    printf("timestamp resolution %d.%9.9d\n",res.tv_sec,res.tv_nsec);

    return 0;
}
