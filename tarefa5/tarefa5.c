#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// compilar: clang -Xpreprocessor -fopenmp -I$(brew --prefix libomp)/include -L$(brew --prefix libomp)/lib -lomp tarefa5.c -o tarefa5

// Função que verifica se um número é primo
int is_prime(int num) {
    if (num <= 1) return 0;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) return 0;
    }
    return 1;
}

int main() {
    int n = 5000000; // Altere este valor para testar (ex: 1000000, 5000000, 10000000)
    double inicio, fim;
    int contador;

    printf("Buscando primos de 2 ate %d...\n\n", n);

    // ==========================================
    // 1. VERSÃO SEQUENCIAL
    // ==========================================
    contador = 0;
    inicio = omp_get_wtime();
    for (int i = 2; i <= n; i++) {
        if (is_prime(i)) {
            contador++;
        }
    }
    fim = omp_get_wtime();
    printf("[Sequencial]\nResultado: %d primos\nTempo: %f segundos\n\n", contador, fim - inicio);

    // ==========================================
    // 2. VERSÃO PARALELA (INGÊNUA - COM ERRO)
    // Apenas com o #pragma omp parallel for
    // ==========================================
    contador = 0;
    inicio = omp_get_wtime();
    #pragma omp parallel for
    for (int i = 2; i <= n; i++) {
        if (is_prime(i)) {
            contador++; // CONDIÇÃO DE CORRIDA AQUI!
        }
    }
    fim = omp_get_wtime();
    printf("[Paralelo Ingenuo]\nResultado: %d primos (ERRO! Condicao de corrida)\nTempo: %f segundos\n\n", contador, fim - inicio);

    // ==========================================
    // 3. VERSÃO PARALELA CORRIGIDA
    // Com Reduction e Dynamic Scheduling
    // ==========================================
    contador = 0;
    inicio = omp_get_wtime();
    #pragma omp parallel for reduction(+:contador) schedule(dynamic)
    for (int i = 2; i <= n; i++) {
        if (is_prime(i)) {
            contador++; // Seguro! O reduction cuida disso.
        }
    }
    fim = omp_get_wtime();
    printf("[Paralelo Corrigido]\nResultado: %d primos (Correto!)\nTempo: %f segundos\n\n", contador, fim - inicio);

    return 0;
}