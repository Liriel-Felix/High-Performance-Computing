#include <stdio.h>
#include <math.h>
#include <omp.h>

// compilar: clang -Xpreprocessor -fopenmp -I$(brew --prefix libomp)/include -L$(brew --prefix libomp)/lib -lomp tarefa6.c -o tarefa6

// Função f(x) = x^2
double f(double x) {
    return x * x;
}

int main() {
    double a = 0.0;
    double b = 10.0;
    // Baterias de testes com diferentes valores de N
    int testes_n[] = {1000000, 5000000, 10000000}; 
    
    for (int t = 0; t < 3; t++) {
        int n = testes_n[t];
        double h = (b - a) / n;
        double extremos = (f(a) + f(b)) / 2.0;
        double soma, integral;
        double inicio, fim;

        printf("=== TESTE COM n = %d ===\n", n);

        // 1. SEQUENCIAL
        soma = 0.0;
        inicio = omp_get_wtime();
        for (int i = 1; i < n; i++) {
            soma += f(a + i * h);
        }
        integral = h * (extremos + soma);
        fim = omp_get_wtime();
        printf("1. Sequencial      | Res: %f | Tempo: %f s\n", integral, fim - inicio);

        // 2. PARALELA (SEM TRATAMENTO - ERRO)
        soma = 0.0;
        inicio = omp_get_wtime();
        #pragma omp parallel for
        for (int i = 1; i < n; i++) {
            soma += f(a + i * h); // Condição de corrida!
        }
        integral = h * (extremos + soma);
        fim = omp_get_wtime();
        printf("2. Sem Tratamento  | Res: %f | Tempo: %f s\n", integral, fim - inicio);

        // 3. PARALELA COM CRITICAL
        soma = 0.0;
        inicio = omp_get_wtime();
        #pragma omp parallel for
        for (int i = 1; i < n; i++) {
            double val = f(a + i * h);
            #pragma omp critical
            {
                soma += val;
            }
        }
        integral = h * (extremos + soma);
        fim = omp_get_wtime();
        printf("3. Com Critical    | Res: %f | Tempo: %f s\n", integral, fim - inicio);

        // 4. PARALELA COM ATOMIC
        soma = 0.0;
        inicio = omp_get_wtime();
        #pragma omp parallel for
        for (int i = 1; i < n; i++) {
            double val = f(a + i * h);
            #pragma omp atomic
            soma += val;
        }
        integral = h * (extremos + soma);
        fim = omp_get_wtime();
        printf("4. Com Atomic      | Res: %f | Tempo: %f s\n", integral, fim - inicio);

        // 5. PARALELA COM REDUCTION
        soma = 0.0;
        inicio = omp_get_wtime();
        #pragma omp parallel for reduction(+:soma)
        for (int i = 1; i < n; i++) {
            soma += f(a + i * h);
        }
        integral = h * (extremos + soma);
        fim = omp_get_wtime();
        printf("5. Com Reduction   | Res: %f | Tempo: %f s\n\n", integral, fim - inicio);
    }

    return 0;
}