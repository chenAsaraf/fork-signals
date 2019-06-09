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


int childrens[5];
int j;

void sigCathcher(int sig_num){
    signal(SIGTERM, sigCathcher);
    printf("PID %d caught one\n", getpid());
    
    if (j>-1)    
        kill(childrens[j], SIGTERM);    
}


int main(){

    signal(SIGTERM,sigCathcher);
    int i, pid, zombie, status;

    for(i =0 ; i<5; ++i) { // loop will run n times (n=5) 
        if((pid = fork()) == 0) { 
            printf("PID %d ready\n",getpid()); 
            j = i-1;
            pause(); //wait for any signals
            exit(0); //become zombie
        }
        else//father updating the childrens array
            childrens[i] = pid;
    } 
    
    sleep(1);

    kill(childrens[4], SIGTERM);

     sleep(1);

    //collect all the zombies:
    for(i = 0; i<5; i++){
        zombie = wait(&status);
        printf("%d is dead\n", zombie);
    }
    
  
    return 0;
}

