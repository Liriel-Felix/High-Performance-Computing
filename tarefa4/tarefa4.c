#include <stdio.h>
#include <math.h>
#include <omp.h> 

// compilar: clang -Xpreprocessor -fopenmp -I$(brew --prefix libomp)/include -L$(brew --prefix libomp)/lib -lomp tarefa4.c -o tarefa4

// Função f(x) = x^2
double f(double x) {
    return x * x;
}

int main() {
    double a = 0.0;     
    double b = 10.0;    
    int n = 1000000;    
    
    double h = (b - a) / n; 
    double soma, integral;
    
    // Extremos da fórmula (fora do laço, calculado apenas uma vez)
    soma = (f(a) + f(b)) / 2.0;
    
    // ==========================================
    // PARALELIZAÇÃO DO LAÇO PRINCIPAL
    // Usamos reduction(+:soma) para que cada thread 
    // some seus trapézios parciais com segurança.
    // ==========================================
    #pragma omp parallel for reduction(+:soma)
    for (int i = 1; i < n; i++) {
        double x_i = a + i * h;
        soma += f(x_i);
    }
    
    integral = h * soma;
    
    printf("O valor da integral de %f a %f com %d subdivisões eh: %f\n", a, b, n, integral);
    
    return 0;
}