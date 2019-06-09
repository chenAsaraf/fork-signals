#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/fcntl.h>
#include "md5.h"

#define STRSIZE 20

int child_pid, parent_pid;
int nbytesReadHash;
//we need to use two pipes:
//first pipe to send input string from parent
int fd1[2];
//second pipe to send back hash string from the child
int fd2[2];

char inbuf[STRSIZE]; //Buffer for read()

//This function will be calld as a reaction to catching a signal.
void catchSigT(int sigNumber){
    signal(SIGTERM, catchSigT);
    close(fd2[1]);
    while ((nbytesReadHash = read(fd2[0], inbuf, 32)) > 0){
        if(nbytesReadHash == 32){
        printf("encrypted by process %d : %s\n", child_pid, inbuf);
        kill(child_pid, SIGINT);
        }
    }
    if (nbytesReadHash != 0){
        exit(2);
    } //zero indicates end of file
}

int main()
{

    /*Taking the input string from user:*/
    char userString[STRSIZE]; //the input use string
    printf("plain text: ");
    scanf("%s", userString);

    int nbytesRead; //return value from read()
    parent_pid = getpid();

    /*Defining the catch of  SIGTERM to activate the 'catchSigT':*/
    signal(SIGTERM, catchSigT);

    /*Making pipes:*/
    if ((pipe(fd1) < 0) || (pipe(fd2) < 0)){
        exit(1);
    }

    /*Continued: forking the proccess*/

    /*Parent process*/
    if ((child_pid = fork()) > 0){
        write(fd1[1], userString, STRSIZE);
        close(fd1[1]);
        wait(NULL); //wait for any child to terminate
    }
    /*Child process*/
    else{
        close(fd1[1]);
        while ((nbytesRead = read(fd1[0], inbuf, STRSIZE)) > 0){
            //hashing the function
            std::string hash= md5(inbuf);
            char _hash[32];
            int i = 0;
            for(char& c : hash){
                _hash[i] = c;
                i++;
            }
            //sending to father proccess
            write(fd2[1], _hash, sizeof(_hash));
            close(fd2[1]);
            //activate the signal hendler
            kill(parent_pid, SIGTERM);
        }
        if (nbytesRead != 0)
            exit(2);
    }

    return 0;
}
