#ifndef HELLOLIB
#define HELLOLIB

struct hello {
	char *target;
	char *day;
	int language;
};


void printhello2(struct hello *hello);
void printhello(void);

#endif
