/* 
 * File: server.cpp
 * Author: Zachary Gmyr
 * Date: 02-12-2026
 *
 * Description:
 * Implements the server process for a shared-memory buffer queue.
 * Creates and initializes the shared memory segment, accpets input
 * to fill the buffer queue, forks a client process, waits for child
 * completion then reads the buffer for any changes made by the client.
 *
 * Build:
 * 	g++ server.cpp buffer.cpp -o server
 *
 * Notes:
 * Uses System V shared memory and fork/exec model.
 */

#include <stdio.h>
#include <sys/shm.h>   // shmget(), shmat(), ftok()
#include <stdlib.h>    // exit()
#include <unistd.h>    // fork(), execlp(), getpid()
#include <sys/types.h> // pid_t
#include <sys/wait.h>  // wait(), WIFEXITED()
#include <errno.h>     // perror()
#include "buffer.h"

int main(int argc, char* argv[]) {

	// create shared memory segment & validate
	const key_t sh_key = ftok("server.cpp", 0);
	int shm_id = shmget(sh_key, sizeof(buffer), IPC_CREAT | 0700);

	if (shm_id == -1) {  // validate segment created
		perror("shmget");
		exit(1);
	}

	// attach to shm & validate
	buffer* buf = (buffer*) shmat(shm_id, NULL, 0);

	if (buf == (void*) -1) {  // validate attach succeeded
		perror("shmat");
		exit(1);
	}

	// initialize buffer
	buf->in = 0;
	buf->out = 0;
	for (int i=0; i<BUFFER_SIZE; i++)
		buf->queue[i] = 0; // buffer begins empty

	// prompt user to queue items
	printf("Server [pid=%d]: enter values to add to queue (-1 to stop)\n", getpid());
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
	
	// fork child
	pid_t clientPid = fork();

	if (clientPid < 0) {  // fork failed
		perror("fork");
		
		// clean up shared memory & exit
		shmdt(buf);
		shmctl(shm_id, IPC_RMID, NULL);

		exit(1);
	}
	else if (clientPid == 0) { // child process
		// shm_id to pass to child for attaching
		char shmIdStr[16];
		snprintf(shmIdStr, sizeof(shmIdStr), "%d", shm_id); // int -> str

		// exec child to client program
		execlp("./client", "./client", shmIdStr, (char*)0 );

		// terminate if exec fails
		perror("execlp");
		exit(1);
	}

	// wait for child to return
	int status;
	wait(&status);

	if (WIFEXITED(status))
		printf("Child exited with NORMAL status: %d\n", WEXITSTATUS(status));
	else
		printf("Child did NOT exit with normal status\n");


	// print new contents
	int r_queue;
	printf("Server [pid=%d]: updated with new buffer queue from Client [pid=%d]\n", getpid(), clientPid);

	while (buf->out != buf->in) {
                bufDequeue(buf, &r_queue);
                printf("%3d|", r_queue);
        }
	printf("\n");

	// print buffer contents (from 0->BUFFER_SIZE)
	sleep(2);
	printf("Server [pid=%d]: final contents of buffer queue are as follows...\n", getpid());
	for (int i=0; i<BUFFER_SIZE; i++) {
		printf("[%d]: %d\n", i, buf->queue[i]);
	}
	printf("\n");

	// free shared memory & finish
	
	shmdt(buf);  // detach
	shmctl(shm_id, IPC_RMID, NULL);  // free
	
	return 0;
}
