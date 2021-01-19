#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

float *mat;
float *mat2;
float *matsaida;
int nthreads; // numero de threads

typedef struct{
    int id;
    int dim;
    float *linha;
} tArgs;

//funcao que as thread executarao
void * multiplicar(void *arg){
    tArgs *args = (tArgs*) arg;
    int linhaMat2;
    //printf("Sou a Thread %d:\n", args->id);
    for (int i = args->id; i < args->dim; i+=nthreads) {
        linhaMat2 = 0;
        for (int j = 0; j < args->dim; j++) {
            //printf("Repeticoes do elemento da linha %d:\n", i);
            for(int k = 0; k < args->dim; k++){
                //printf("%.2f X %.2f = na posicao matsaida [%d][%d]\n", mat[(i*args->dim) + j], mat2[(linhaMat2*args->dim) + k], i, k); //pegar o "i" la de cima e o "k" daqui de baixo para ir modificando os elementos de matsaida
                matsaida[(i*args->dim) + k] += mat[(i*args->dim) + j] * mat2[(linhaMat2*args->dim) + k];
             }
            linhaMat2++;
            //printf("\n");
        }
        //printf("\n");
    }
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    int dim;
    pthread_t *tid; //identificar threads no sistema
    tArgs *args; //identificadores locais das threads e dimensao

    if (argc < 3) {
        printf("Digite %s <dimensao da matriz>, <numero de threads>\n", argv[0]);
        return 1;
    }
    dim = atoi(argv[1]);
    nthreads = atoi(argv[2]);
    if(nthreads > dim) nthreads = dim;
    //alocando as estruturas de dados
    mat = (float *) malloc(sizeof(float) * dim * dim);
    if (mat == NULL) {
        printf("Erro na alocacao de memoria\n");
        return 2;
    }
    mat2 = (float *) malloc(sizeof(float) * dim * dim);
    if (mat2 == NULL) {
        printf("Erro na alocacao de memoria\n");
        return 2;
    }
    matsaida = (float *) malloc(sizeof(float) * dim * dim);
    if (matsaida == NULL) {
        printf("Erro na alocacao de memoria\n");
        return 2;
    }

    //inicializando as estruturas de dados
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            mat[i * dim + j] = rand() % 100;
            mat2[i * dim + j] = rand() % 100;
            matsaida[i * dim + j] = 0;
        }
    }
    printf("Matriz numero 1: \n");
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            printf("%.2f ", mat[i * dim + j]);
        }
        printf("\n");
    }
    printf("Matriz numero 2: \n");
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            printf("%.2f ", mat2[i * dim + j]);
        }
        printf("\n");
    }
    printf("\n");
    //alocacao das estruturas
    tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
    if(tid == NULL){
        printf("Erro--malloc");
        return 2;
    }
    args = (tArgs*) malloc(sizeof(tArgs)*nthreads);
    if(args == NULL){
        printf("Erro--malloc");
        return 2;
    }
    //criacao das threads

    for(int i = 0; i < nthreads; i++){
        (args+i)->id = i;
        (args+i)->dim = dim;
        //preenchendo linha
        if(pthread_create(tid+i, NULL, multiplicar, (void*) (args+i))){
            printf("Erro em pthread_create()"); return 3;
        }
    }
    for(int i = 0; i < nthreads; i++){
        pthread_join(*(tid+i), NULL);
    }

    //resultados

    puts("Matriz de saida:");
    for(int i = 0; i < dim; i++){
        for(int j = 0; j < dim; j++){
            printf("%.2f ", matsaida[i * dim + j]);
        }
        printf("\n");
    }
    free(mat);
    free(mat2);
    free(matsaida);

    return 0;
}
