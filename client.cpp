/*
 * File: client.cpp
 * Author: Zachary Gmyr
 * Date: 02-12-2026
 *
 * Description:
 * Client process for shared-memory buffer queue demo.
 * Attaches to an existing shared memory segment using shm_id
 * passed as a command-line argument, reads items from the buffer,
 * adds new items, and detaches on exit.
 *
 * Usage:
 * 	./client <shm_id>
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include "buffer.h"

int main(int argc, char* argv[]) {

	// validate usage (requires shm_id argument)
	if (argc < 2) {
		fprintf(stderr, "Usage: %s <shm_id>\n", argv[0]);
		exit(1);
	}

	// attach to shared memory & validate
	buffer* buf = (buffer*) shmat(atoi(argv[1]), NULL, 0);

	if (buf == (void*) -1) {
		fprintf(stderr, "Shared memory attach failed\n");
		exit(1);
	}
	
	// read buffer until out == in
	int r_queue;
	
	printf("Client [pid=%d]: reading shared memory buffer created by Server [pid=%d]\n|", getpid(), getppid());

	while (buf->out != buf->in) {
		bufDequeue(buf, &r_queue);
		printf("%3d|", r_queue);
	}
	printf("\n");

	sleep(2); // wait after read

	// add new contents to buffer queue
        printf("Client [pid=%d]: enter values to add to queue (-1 to stop)\n", getpid());
        int value, c;
        do {
                printf("> ");
                // get input
                if (scanf("%d", &value) != 1) { // validate integer
                        fprintf(stderr, "Invalid input; try again.\n");
                        while ( (c = getchar()) != '\n' && c != EOF); // clear buffer
                        continue;
                }
                while ( (c = getchar()) != '\n' && c != EOF); // clear buffer

                if (value == -1) // input over
                        break;

                bufQueue(buf, value); // add to queue
        } while (true);

	printf("Client [pid=%d]: returning control to Server [pid=%d]\n", getpid(), getppid());
	
	shmdt(buf); // detach from shared memory
	
	return 0;
}
