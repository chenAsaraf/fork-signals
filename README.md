
This C code written as part of a task in Operating-System course, Ariel Univeraity.
Uses system calls to generate processes and to create communication between processes.

md5pipe:
A program that produces two pipes and two processes. The parent receives a string from the user (no longer than 20 characters).
The son process encrypts the string and sends it back to the parent. CPP directory  “md5” is used.	

signals:
This program runs 5 processes which sends to each other signals using the kill command.
The signals activate the procedure sigCathcher which sends a signal to the next process- his sibling.
Each child process exit after receiving the signal. The parent collects all the zombies and then exits.

