# OperatingSystemandSecurity

## Description
This repository contains my Operating Systems coursework (ST5004CEM) in C, covering multithreading, memory paging simulation, secure file management, and socket-based networking. Built and tested locally on Ubuntu.

## Folder Structure
### Task1
- Deadlock.c
- Round-Robin.c
- thread.c
- partD_race_condition_demo.c

### Task2
- FIFOPageReplacement.c
- LRUPageReplacement.c

### Task3
- auth_permission.c
- file_ops.c
- encryption.c

### Task4
- client.c
- server.c
- users.txt

## Requirements
- GCC Compiler
- Ubuntu/Linux Operating System

## How to Compile

### Deadlock
```bash
gcc Deadlock.c -o Deadlock -pthread
./Deadlock
```

### Round-Robin
```bash
gcc Round-Robin.c -o Round-Robin
./Round-Robin
```

### Thread
```bash
gcc thread.c -o thread -pthread
./thread
```

### Part D (Race Condition Demo)
```bash
gcc partD_race_condition_demo.c -o partD_race -pthread
./partD_race
```

### FIFO Page Replacement
```bash
gcc FIFOPageReplacement.c -o fifo
./fifo
```

### LRU Page Replacement
```bash
gcc LRUPageReplacement.c -o lru
./lru
```

### Auth & Permission
```bash
gcc auth_permission.c -o auth_permission
./auth_permission
```

### File Operations
```bash
gcc file_ops.c -o file_ops
./file_ops
```

### Encryption
```bash
gcc encryption.c -o encryption
./encryption
```

### Server (Task4 - run first, in its own terminal)
```bash
gcc server.c -o server -pthread
./server
```

### Client (Task4 - run in a second terminal, while server is running)
```bash
gcc client.c -o client
./client admin admin123
```

## Author
Pradeepta Shrestha
