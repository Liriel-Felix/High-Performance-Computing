#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Função 1: Acesso por Linhas
void multiplica_linhas(int N, double A[N][N], double X[N], double Y[N]) {
    for (int i = 0; i < N; i++) {
        Y[i] = 0.0;
        for (int j = 0; j < N; j++) {
            Y[i] += A[i][j] * X[j];
        }
    }
}

// Função 2: Acesso por Colunas
void multiplica_colunas(int N, double A[N][N], double X[N], double Y[N]) {
    for (int i = 0; i < N; i++) {
        Y[i] = 0.0;
    }

    for (int j = 0; j < N; j++) {
        for (int i = 0; i < N; i++) {
            Y[i] += A[i][j] * X[j];
        }
    }
}

int main() {
    int N = 2000; // Tamanho da matriz

    // Alocação das matrizes/vetores
    double (*A)[N] = malloc(sizeof(double[N][N]));
    double *X = malloc(N * sizeof(double));
    double *Y = malloc(N * sizeof(double));

    // Preenchimento com valores iniciais
    for (int i = 0; i < N; i++) {
        X[i] = 1.0;
        for (int j = 0; j < N; j++) {
            A[i][j] = 2.0;
        }
    }

    // Teste 1: Linhas
    clock_t inicio = clock();
    multiplica_linhas(N, A, X, Y);
    clock_t fim = clock();
    double tempo_linhas = (double)(fim - inicio) / CLOCKS_PER_SEC;

    // Teste 2: Colunas
    inicio = clock();
    multiplica_colunas(N, A, X, Y);
    fim = clock();
    double tempo_colunas = (double)(fim - inicio) / CLOCKS_PER_SEC;

    // Exibição dos Resultados
    printf("--- Teste com N = %d ---\n", N);
    printf("Tempo por Linhas : %f segundos\n", tempo_linhas);
    printf("Tempo por Colunas: %f segundos\n", tempo_colunas);

    // Liberação de memória
    free(A);
    free(X);
    free(Y);

    return 0;
}