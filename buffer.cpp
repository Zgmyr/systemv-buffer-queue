/*
 * File: buffer.cpp
 * Author: Zachary Gmyr
 * Date: 02-12-2026
 *
 * Description:
 * Implements the circular buffer functions bufQueue and bufDequeue.
 * Operates on buffer* structs stored in shared memory
 */

#include "buffer.h"
#include <stdio.h>

// Add value to circular buffer
void bufQueue(buffer* b, int val) {
	int next_in = (b->in + 1) % BUFFER_SIZE;
	if (next_in == b->out) // prevent overwritting unread items
		fprintf(stderr, "Cannot add to queue; buffer FULL\n");
	else {
		b->queue[b->in] = val;  // add if there is room
		b->in = (b->in + 1) % BUFFER_SIZE; // increment in pointer
	}
}

// Remove value from circular buffer (returned through &val)
void bufDequeue(buffer* b, int* val) {
	if (b->out == b->in) // buffer empty
		fprintf(stderr, "Nothing to dequeue; buffer EMPTY\n");
	else {
		*val = b->queue[b->out]; // grab dequeued value
		b->out = (b->out + 1) % BUFFER_SIZE;
	}
}
