#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// compilar: clang -Xpreprocessor -fopenmp -I$(brew --prefix libomp)/include -L$(brew --prefix libomp)/lib -lomp tarefa7.c -o tarefa7

// Função que verifica se um número é primo
int is_prime(int num) {
    if (num <= 1) return 0;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) return 0;
    }
    return 1;
}

int main() {
    int n_fixo = 1000000;
    int contador;
    double inicio, fim;

    printf("=== PARTE 1: TESTE DE ESCOPO DE VARIAVEIS (n = %d) ===\n", n_fixo);

    // 1. Apenas PRIVATE
    contador = 0;
    #pragma omp parallel for private(contador)
    for (int i = 2; i <= n_fixo; i++) {
        if (is_prime(i)) contador++; // contador começa com lixo de memória
    }
    printf("1. private(contador)          -> Resultado: %d (Incorreto)\n", contador);

    // 2. Apenas FIRSTPRIVATE
    contador = 0;
    #pragma omp parallel for firstprivate(contador)
    for (int i = 2; i <= n_fixo; i++) {
        if (is_prime(i)) contador++; // começa em 0, mas valor não é retornado ao global
    }
    printf("2. firstprivate(contador)     -> Resultado: %d (Incorreto)\n", contador);

    // 3. Apenas LASTPRIVATE
    contador = 0;
    #pragma omp parallel for lastprivate(contador)
    for (int i = 2; i <= n_fixo; i++) {
        if (is_prime(i)) contador++; 
    }
    printf("3. lastprivate(contador)      -> Resultado: %d (Incorreto/Lixo)\n", contador);

    // 4. FIRSTPRIVATE e LASTPRIVATE
    contador = 0;
    #pragma omp parallel for firstprivate(contador) lastprivate(contador)
    for (int i = 2; i <= n_fixo; i++) {
        if (is_prime(i)) contador++; 
    }
    printf("4. first & lastprivate        -> Resultado: %d (Incorreto - Conta apenas da ultima thread)\n", contador);

    // 5. DEFAULT(NONE) com REDUCTION
    contador = 0;
    #pragma omp parallel for default(none) shared(n_fixo) reduction(+:contador)
    for (int i = 2; i <= n_fixo; i++) {
        if (is_prime(i)) contador++;
    }
    printf("5. default(none) + reduction  -> Resultado: %d (Correto)\n", contador);

    // 6. REDUCTION
    contador = 0;
    #pragma omp parallel for reduction(+:contador)
    for (int i = 2; i <= n_fixo; i++) {
        if (is_prime(i)) contador++;
    }
    printf("6. Apenas reduction           -> Resultado: %d (Correto)\n\n", contador);


    printf("=== PARTE 2: TESTE DE ESCALONADORES (SCHEDULE) ===\n");
    int testes_n[] = {1000000, 5000000, 10000000};
    
    // Configura o runtime schedule para dynamic por padrão para o teste
    omp_set_schedule(omp_sched_dynamic, 1);

    for (int t = 0; t < 3; t++) {
        int n = testes_n[t];
        printf("\n--- Testando para N = %d ---\n", n);

        // STATIC
        contador = 0; inicio = omp_get_wtime();
        #pragma omp parallel for reduction(+:contador) schedule(static)
        for (int i = 2; i <= n; i++) { if (is_prime(i)) contador++; }
        fim = omp_get_wtime();
        printf("STATIC  : %f segundos\n", fim - inicio);

        // DYNAMIC
        contador = 0; inicio = omp_get_wtime();
        #pragma omp parallel for reduction(+:contador) schedule(dynamic)
        for (int i = 2; i <= n; i++) { if (is_prime(i)) contador++; }
        fim = omp_get_wtime();
        printf("DYNAMIC : %f segundos\n", fim - inicio);

        // GUIDED
        contador = 0; inicio = omp_get_wtime();
        #pragma omp parallel for reduction(+:contador) schedule(guided)
        for (int i = 2; i <= n; i++) { if (is_prime(i)) contador++; }
        fim = omp_get_wtime();
        printf("GUIDED  : %f segundos\n", fim - inicio);

        // AUTO
        contador = 0; inicio = omp_get_wtime();
        #pragma omp parallel for reduction(+:contador) schedule(auto)
        for (int i = 2; i <= n; i++) { if (is_prime(i)) contador++; }
        fim = omp_get_wtime();
        printf("AUTO    : %f segundos\n", fim - inicio);

        // RUNTIME
        contador = 0; inicio = omp_get_wtime();
        #pragma omp parallel for reduction(+:contador) schedule(runtime)
        for (int i = 2; i <= n; i++) { if (is_prime(i)) contador++; }
        fim = omp_get_wtime();
        printf("RUNTIME : %f segundos\n", fim - inicio);
    }

    return 0;
}