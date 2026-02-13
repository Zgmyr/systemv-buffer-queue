# System V (Shared Memory) Buffer Queue

**Author:** Zachary Gmyr  
**Date:** 02-12-2026

---

## Project Motivation

I built this project while concurrently taking **Operating Systems** at the University of Missouri-St. Louis. My goal was to learn and practice **fork/exec** and **System V shared memory**. This project demonstrates a **shared memory circular buffer queue**, illustrating the **producer-consumer relationship** between a client and a server.

> **Note:** This was an independent project for practice — not assigned coursework.  
> All development was performed in a command-line environment: I an SSH connection to the campus Linux server & used standard terminal tools for editing, compiling, and running the programs.

---

## Project Description

The project consists of three main components:

| Component        | Purpose |
|-----------------|---------|
| `buffer.h`       | Defines the circular buffer struct and function prototypes for enqueue/dequeue. |
| `buffer.cpp`     | Implements the circular buffer logic (`bufQueue` and `bufDequeue`) for use in shared memory. |
| `server.cpp`     | Creates the shared memory segment, initializes the buffer, prompts the user to enter values, forks a child, waits for client completion, and prints updated buffer contents. |
| `client.cpp`     | Attaches to the existing shared memory segment, may read the buffer, allows the user to add new items, and detaches on exit. |

### Circular Buffer Layout

The shared memory buffer is a fixed-size circular queue:

| Field  | Description |
|--------|------------|
| `in`   | Index where the next item will be added. |
| `out`  | Index of the next item to remove. |
| `queue`| Integer array of size `BUFFER_SIZE` (10) storing the queued values. |

---

### Workflow

1. **Server** initializes the buffer and prompts the user to enqueue values until full.  
2. Server **forks a child** process to run the client program.  
3. **Client** attaches to the shared memory, may read current contents, and can enqueue new values.  
4. After the client exits, the **server** prints the current buffer contents.  
5. Shared memory is detached and removed before the server terminates.

---

### Concurrency Notes

- The project avoids race conditions by having the server **wait** for the client to finish before accessing the buffer again.
- In a real concurrent system, **semaphores** or other synchronization primitives would be needed to prevent simultaneous access.

---

## Compilation

The provided **Makefile** can be used to build the project:

```bash
make
```
This produces two executables:
- `server` → the main program to be run
- `client` → executed by the server during fork/exec

To clean compiled objects and executables:
```bash
make clean
```
---

### Usage
1. Run the server:
```bash
./server
```
2. Enter integers to enqueue in the buffer. Enter `-1` to stop input.
3. The server forks a client, which may read current buffer queue.
4. Client prompts user to enter integers to be enqueued in the buffer. Enter `-1` to stop input.
5. Once the client exits, the server reads the current buffer queue.
6. The server displays entire final buffer contents.

---
### Additional Notes
- Implements **System V shared memory** and **fork/exec** on Linux.
- Demonstrates a basic **producer-consumer flow** with a circular buffer.
- Error handling is implemented using `perror()` for system calls.
- Designed for single-child access; multiple clients would require proper synchronization.
