#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>

char* secret = "cBWTwTwC5eKAcDxpwx88";

void raise_alarm(unsigned long found, unsigned long expected) {
    printf("detected return address corruption!\n");
    printf("found: %lx, expected: %lx\n",found, expected);
    exit(0);

}

void *frame;
unsigned long expected = 0;
unsigned long found = 0;


void print_secret() {
	frame = __builtin_frame_address(0);
	found = frame;
    printf("Ah! %s\n",secret);
    exit(0);
}

char buffer[256];
int i;

void dump_frame(void* frame) {
	uint64_t* pt = (uint64_t*)frame;
	int i = 0;
	for (i = 0; i < 16; i += 1) {
		printf("[%p, %d]: %lx\n", pt-i, i, *(pt-i));
		fflush(stdout);
	}
}


void receive_input() {
    char buf[128];
    frame = __builtin_frame_address(0);
    dump_frame(frame);
 
    expected = frame;

	if (found != 0) raise_alarm(found, expected);

    frame = __builtin_frame_address(0);
    found = frame;

    fgets(buffer, 256, stdin);
    strcpy(buf, buffer);

    dump_frame(frame);
    frame =  __builtin_frame_address(0);
    found = frame;
    if (expected != found) raise_alarm(found, expected);
}

int run() {
    printf("Can you obtain the secret?\n");
    receive_input();
    printf("Returnning Normally\n");
}

int main() {
    run();
}
