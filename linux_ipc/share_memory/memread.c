/** compilation: gcc -o memreader memreader.c -lrt -pthread**/
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
    
    int fd = shm_open("/memshare", O_RDWR, 0644);

    if (fd < 0) report_and_exit("Can't get file descriptor...");

    void *memptr = mmap(NULL, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if ((void*)-1 == memptr) report_and_exit("Can't access segment...");

    sem_t *semptr = sem_open("memshare_sem", O_CREAT, 0644, 0);

    if (semptr == (void*)-1) report_and_exit("sem_open");

    if (!sem_wait(semptr)) {
       
        printf("%s\n", (char*)memptr); 
        sem_post(semptr); 
    }

    munmap(memptr, 1024);
    close(fd);
    sem_close(semptr);

    return 0;
}
