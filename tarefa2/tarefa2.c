#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void executar_testes(int N) {
    // Aloca o vetor dinamicamente para suportar tamanhos grandes
    double *vetor = malloc(N * sizeof(double));
    if (vetor == NULL) {
        printf("Erro de alocação de memória.\n");
        return;
    }

    clock_t inicio, fim;
    double soma_total;

    printf("=== Tamanho do vetor: %d elementos ===\n", N);

    // ---------------------------------------------------------
    // 1. Inicialização do vetor (Cálculo simples)
    // ---------------------------------------------------------
    inicio = clock();
    for (int i = 0; i < N; i++) {
        vetor[i] = i * 1.5; 
    }
    fim = clock();
    printf("1. Inicializacao      : %f segundos\n", (double)(fim - inicio) / CLOCKS_PER_SEC);

    // ---------------------------------------------------------
    // 2. Soma com dependência (Acumulativa)
    // ---------------------------------------------------------
    soma_total = 0.0;
    inicio = clock();
    for (int i = 0; i < N; i++) {
        soma_total += vetor[i]; // Dependência: a iteração i+1 precisa esperar a i terminar
    }
    fim = clock();
    printf("2. Soma (Dependencia) : %f segundos (Soma: %.2f)\n", (double)(fim - inicio) / CLOCKS_PER_SEC, soma_total);

    // ---------------------------------------------------------
    // 3. Quebra de dependência (Múltiplos acumuladores para ILP)
    // ---------------------------------------------------------
    double s0 = 0.0, s1 = 0.0, s2 = 0.0, s3 = 0.0;
    inicio = clock();
    // Processamos de 4 em 4 para quebrar a dependência
    for (int i = 0; i < N; i += 4) {
        s0 += vetor[i];
        s1 += vetor[i+1];
        s2 += vetor[i+2];
        s3 += vetor[i+3];
    }
    soma_total = s0 + s1 + s2 + s3;
    fim = clock();
    printf("3. Soma (ILP / 4 vars): %f segundos (Soma: %.2f)\n\n", (double)(fim - inicio) / CLOCKS_PER_SEC, soma_total);

    free(vetor);
}

int main() {
    // Testa com 10 milhões, 50 milhões e 100 milhões de elementos
    executar_testes(10000000);
    executar_testes(50000000);
    executar_testes(100000000);
    return 0;
}