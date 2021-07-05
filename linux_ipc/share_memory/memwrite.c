/** compilation: gcc -o memwriter memwriter.cpp -lrt -lpthread  **/
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>

void report_and_exit(const char *msg) {
    perror(msg);
    exit(-1);
}

int main(int argc, char *argv[]) {
    int fd = shm_open("/memshare", O_RDWR | O_CREAT, 0644); 

    if (fd < 0) report_and_exit("Can't open shared mem segment...");

    ftruncate(fd, 1024);   
    
    void* memptr = mmap(NULL, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); 
    
    if ((void*)-1 == memptr) report_and_exit("Can't get segment...");
    
    sem_t *semptr = sem_open("memshare_sem", O_CREAT, 0644, 0); 
    
    if(semptr == (void*)-1) report_and_exit("sem_open");
    
    strcpy(memptr, "this is message from memwriter!");

    if (sem_post(semptr) < 0) report_and_exit("sem_post");
   
    printf("sleep for 15...\n"); 
    sleep(15);

    /* clean up*/
    printf("clean up resource...\n");

    munmap(memptr, 1024);
    close(fd);
    sem_close(semptr);
    //shm_unlink("/memshare");
    return 0;
}
