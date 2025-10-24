#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

volatile sig_atomic_t keep_running = 1;

void sigtstp_handler(int signum) {
    if (signum == SIGTSTP) {
        write(STDOUT_FILENO, "\nSIGTSTP received! Initiating graceful shutdown...\n", 50);
        keep_running = 0; 
    }
}

int main() {
    struct sigaction sa;
    sa.sa_handler = sigtstp_handler;    
    sigemptyset(&sa.sa_mask);           
    sa.sa_flags = 0;                    
    
    printf("Started with PID: %d\n", getpid());
    
    if (sigaction(SIGTSTP, &sa, NULL) == -1) {
        perror("Error setting up signal handler");
        return EXIT_FAILURE;
    }

    while (keep_running) {
        printf("Process is running. PID: %d\n", getpid());
        sleep(1); 
    }

    printf("Exiting gracefully after SIGTSTP signal.\n");
    return EXIT_SUCCESS;
}