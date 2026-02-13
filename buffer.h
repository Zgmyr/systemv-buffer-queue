/*
 * File: buffer.h
 * Author: Zachary Gmyr
 * Date: 02-12-2026
 *
 * Description:
 * Defines the shared-memory circular buffer structure and the
 * function prototypes for enqueue (bufQueue) and dequeue (bufDequeue).
 * This header is included by server.cpp and client.cpp.
 */

#ifndef BUFFER_H
#define BUFFER_H

#define BUFFER_SIZE 10

// shared memory buffer struct
struct buffer  {
	int in;
	int out;
	int queue[BUFFER_SIZE];
};

// function definitions
void bufQueue(buffer* b, int val); // adds val to buffer queue

void bufDequeue(buffer* b, int* val); // removes value from buffer queue

#endif
