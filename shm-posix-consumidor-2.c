#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

#define SIZE 4096
#define SLOT_SIZE 8
#define NUM_SLOTS 512

int main() {
    const char *name = "OS";
    const char *initial_string = "ifeeeee";

    int shm_fd;
    void *ptr;

    // Abrir o segmento de memória compartilhada
    shm_fd = shm_open(name, O_RDWR, 0666);
    if (shm_fd == -1) {
        printf("Falha ao abrir a memória compartilhada\n");
        exit(EXIT_FAILURE);
    }

    // Mapear a memória compartilhada no espaço de endereçamento do processo
    ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        printf("Falha no mapeamento\n");
        exit(EXIT_FAILURE);
    }

    // Consumir e substituir strings na memória compartilhada
    for (int i = 0; i < 1000; i++) {
        for (int j = 0; j < NUM_SLOTS; j++) {
            if (strncmp(ptr + j * SLOT_SIZE, initial_string, SLOT_SIZE) != 0) {
                printf("Consumindo: %s\n", (char *)(ptr + j * SLOT_SIZE));
                sleep(1); // Pausa de 1 segundo
                memcpy(ptr + j * SLOT_SIZE, initial_string, SLOT_SIZE);
            }
        }
    }

    // Desmapear e fechar a memória compartilhada
    munmap(ptr, SIZE);
    close(shm_fd);
    shm_unlink(name);

    return 0;
}