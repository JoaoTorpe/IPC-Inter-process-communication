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
    const char *producer_string = "SOnoIFPE";

    int shm_fd;
    void *ptr;

    // Criar o segmento de memória compartilhada
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        printf("Falha na criação da memória compartilhada\n");
        exit(EXIT_FAILURE);
    }

    // Configurar o tamanho da memória compartilhada
    if (ftruncate(shm_fd, SIZE) == -1) {
        printf("Falha ao configurar o tamanho da memória\n");
        exit(EXIT_FAILURE);
    }

    // Mapear a memória compartilhada no espaço de endereçamento do processo
    ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        printf("Falha no mapeamento\n");
        exit(EXIT_FAILURE);
    }

    // Preencher a memória compartilhada com 512 cópias de "ifeeeee"
    for (int i = 0; i < NUM_SLOTS; i++) {
        memcpy(ptr + i * SLOT_SIZE, initial_string, SLOT_SIZE);
    }

    printf("Memória compartilhada inicializada com 512 cópias de 'ifeeeee'\n");

    // Produzir 1000 strings "SOnoIFPE" e escrevê-las nos slots livres
    for (int i = 0; i < 1000; i++) {
        int slot_found = 0;

        // Procurar um slot livre para escrever
        for (int j = 0; j < NUM_SLOTS; j++) {
            if (strncmp(ptr + j * SLOT_SIZE, initial_string, SLOT_SIZE) == 0) {
                memcpy(ptr + j * SLOT_SIZE, producer_string, SLOT_SIZE);
                slot_found = 1;
                break;
            }
        }

        if (!slot_found) {
            printf("Não há slots livres disponíveis\n");
            break;
        }
    }

    // Manter o processo aberto até que o consumidor termine
    getchar();

    // Desmapear e fechar a memória compartilhada
    munmap(ptr, SIZE);
    close(shm_fd);
    shm_unlink(name);

    return 0;
}