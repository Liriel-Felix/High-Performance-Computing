#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include <sys/mman.h>
#include <semaphore.h>
#include <fcntl.h> // Necessário para as configurações do sem_open no Mac

#define NUM_PROCESSOS 4
#define INCREMENTOS 1000000

int main() {
    // Aloca a memória compartilhada APENAS para o contador
    int *contador = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    
    if (contador == MAP_FAILED) {
        perror("Erro na memória compartilhada");
        exit(EXIT_FAILURE);
    }

    *contador = 0; // Inicializa o contador com zero

    // Limpa o semáforo caso o programa tenha parado de forma inesperada antes
    sem_unlink("/meu_semaforo"); 
    
    // Cria o semáforo no Mac. O "1" no final significa que ele começa destravado.
    sem_t *semaforo = sem_open("/meu_semaforo", O_CREAT, 0644, 1);

    printf("Contador inicial: %d\n", *contador);

    for (int i = 0; i < NUM_PROCESSOS; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("Erro ao criar processo");
            exit(EXIT_FAILURE);
        }
        if (pid == 0) {
            // Código executado pelo processo filho
            for (int j = 0; j < INCREMENTOS; j++) {
                sem_wait(semaforo); // Pede a vez (Trava)
                (*contador)++;      // Soma
                sem_post(semaforo); // Passa a vez (Destrava)
            }
            printf("Filho %d terminou. Contador atual = %d\n", getpid(), *contador);
            exit(EXIT_SUCCESS);
        }
    }

    // Processo pai espera todos os filhos terminarem
    for (int i = 0; i < NUM_PROCESSOS; i++) {
        wait(NULL);
    }

    printf("\nContador no processo pai: %d\n", *contador);

    // Limpeza: fecha e apaga o semáforo e libera a memória
    sem_close(semaforo);
    sem_unlink("/meu_semaforo");
    munmap(contador, sizeof(int));

    return 0;
}