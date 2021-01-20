#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include "timer.h"

// ---Macro para a mensagem de erro---

#define MENSAGEM mensagem(args[0])

// ---Prototipo para a funcao mensagem---

void mensagem(char *arg);
static int comparador(const void * a, const void * b);
double somaSequencial(long long int n);
void * somaMatriz(void * arg);

// ---Variaveis globais---

int nthreads;
long long int elementos;
double *vetor;

// ---Fluxo principal do programa---

int main(int argc, char *args[]) {
    double somaSeq = 0; //soma sequencial
    double somaConc = 0; //soma concorrente
    double inicio, fim;
    double *retorno;
    pthread_t *tid;

    if(argc < 3){
        MENSAGEM;
        return 1;
    }

    elementos = atoll(args[1]);
    nthreads = atoi(args[2]);

    // ---Soma sequencial---
    vetor = (double *) malloc(sizeof(double) * elementos);
    if(!vetor){
        fprintf(stderr, "Erro alocando o vetor\n");
        return 2;
    }
    somaSeq = somaSequencial(elementos);
    printf("Valor aproximado pela funcao sequencial: %.15lf\n", somaSeq);
    printf("\nValor exato de pi: %.15lf\n", M_PI);

    /*printf("\nVetor original de elementos do somatorio:\n");
    for(long int i = 0; i<elementos; i++){
        printf("%.15lf ", vetor[i]);
    }
    printf("\n");

    qsort(vetor, elementos, sizeof(double), comparador);

    printf("Vetor ordenado de elementos do somatorio:\n");
    for(long int i = 0; i<elementos; i++){
        printf("%.15lf ", vetor[i]);
    }*/
    printf("\n");

    //ate aqui tudo certo

    // ---Soma concorrente---
    GET_TIME(inicio);
    tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
    if(!tid) {
        fprintf(stderr, "Erro na alocacao do vetor de threads\n");
        return 2;
    }
    for(long int i = 0; i < nthreads; i++){
        if(pthread_create(tid+i, NULL, somaMatriz, (void *) i)){
            fprintf(stderr, "Erro na criacao das threads\n");
            return 3;
        }
    }
    for(long int i = (nthreads - 1); i >= 0; i--){
        if(pthread_join(*(tid+i), (void **) &retorno)){
            fprintf(stderr, "Erro no retorno das threads\n");
            return 3;
        }
        //printf("Valor retornado pela thread %d: %.15lf\n", (i+1), *retorno);
        somaConc += *retorno;
        free(retorno);
    }
    GET_TIME(fim);
    printf("Tempo da aproximacao concorrente: %lf segundos\n", fim-inicio);

    free(vetor);
    free(tid);
    printf("Valor aproximado pela soma concorrente: %.15lf\n\n", somaConc);

    return 0;
}

// ---Funcoes---

//mensagem de erro da inicializacao do programa
void mensagem(char *arg){
    fprintf(stderr, "Digite: %s <numero de elementos da serie> <numero de threads>", arg);
}
// funcao de comparacao para ordenar os valores do somatorio no vetor
static int comparador (const void * a, const void * b){
    if (*(double*)a > *(double*)b) return 1;
    else if (*(double*)a < *(double*)b) return -1;
    else return 0;
}

//funcao executada por cada thread
void * somaMatriz(void * arg){
    long int id = (long int) arg;
    double *somaLocal;
    long int tamBloco = elementos/nthreads;
    long int inicio = id * tamBloco;
    long int fim = inicio + tamBloco;
    if(id == nthreads - 1) fim = elementos;

    somaLocal = (double *) malloc(sizeof(double));
    if(!somaLocal){
        fprintf(stderr, "Erro na alocacao da variavel soma local\n");
        exit(1);
    }
    *somaLocal = 0;
    for(long int i = inicio; i < fim; i++){
        *somaLocal += vetor[i];
    }
    pthread_exit((void *) somaLocal);
}

//soma sequencial
double somaSequencial(long long int n){
    double resultado = 0;
    double resultadoInvertido = 0;
    double termo;
    double inicio, fim;

    GET_TIME(inicio);
    for(long long int i = 1; i < n+1; i++){
        //se o numero é impar, o termo é positivo
        if(i % 2 != 0){
            termo = 4 * (1.0/((2*i) - 1));
            resultado += termo;  //adiciona o valor ao resultado
            vetor[i-1] = termo;
        }
            // se o numero é par, o termo é negativo
        else{
            termo = (-1 * 4) * (1.0/((2*i) - 1));
            resultado += termo;  //adiciona o valor ao resultado
            vetor[i-1] = termo;
        }
    }
    printf("\n");
    GET_TIME(fim);
    printf("Tempo da aproximacao sequencial: %lf segundos\n", fim-inicio);

    //soma invertida
    for(long int i = elementos - 1; i >= 0; i--){
        resultadoInvertido += vetor[i];
    }
    if(fabs(M_PI - resultadoInvertido) < fabs(M_PI - resultado)){
        printf("O valor da soma invertida é mais exato\n");
    }
    printf("Valor aproximado pela funcao invertida: %.15lf\n", resultadoInvertido);

    return (resultado);
}
