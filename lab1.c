/*O programa segue uma logica de, uma vez escolhido o tamanho do vetor, este tera seus valores
  inicias preenchidos de 0 ate N-1. Assim, depois que as threads agirem, os valores finais do
  vetor irao de 1 até N*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 2

int *pt; //ponteiro global para modificar os valores do vetor de dentro das threads

typedef struct{
    int valor1, valor2; //valor1 e valor2 indicam, respectivamente, o começo e o fim da metade que será preenchida
    int thread_id;
}Valores;

void *incrementar(void *arg){
    Valores *valores = (Valores *) arg;
    printf("\nThread %d criada\n\n", valores->thread_id);
    for(int i = valores->valor1; i <= valores->valor2; i++){
        printf("A thread %d esta incrementando o vetor\n", valores->thread_id);
        pt[i] = pt[i] + 1;
    }
    pthread_exit(NULL);
}

int main(void){

    pthread_t tid[NTHREADS]; //identificador da thread no sistema
    int n; //variavel para indicar o tamanho do vetor
    printf("Escolha um valor para n no intervalo 10 < n < 100: ");
    while(1){
        scanf("%d", &n);
        if(n < 100 && n > 10)
            break;
        else
            printf("Por favor, insira um valor no intervalo 10 < n < 100\n");
    }
    printf("\n");
    int vetor[n];
    for(int i = 0; i < n; i++){
        vetor[i] = i;
    }
    printf("Valores iniciais do vetor:\n");
    for(int i = 0; i < n; i++){
        printf("%d ", vetor[i]);
    }
    printf("\n");
    printf("\n");

    //ponteiro para struct Valores, assim sao passados argumentos por referencia às threads, importante passar por referencia para ocorrer
    //a modificacao dos valores do vetor
    Valores *valores;
    Valores *valores2;

    valores = malloc(sizeof(Valores));
    valores2 = malloc(sizeof(Valores));
    //fazendo o ponteiro global apontar para o vetor
    pt = vetor;
    if (valores == NULL) {
        printf("--ERRO: malloc()\n"); exit(-1);
    }

    int inicio;
    int fim;
    int metade = n / 2;

    printf("Criando as threads que irao incrementar os valores no vetor.\n\n");
    //na primeira iteracao é criada a thread responsavel pela primeira metade, na segunda é criada a
    //thread responsavel pela segunda metade, apos isso o sistema operacional irá escolher a ordem em
    //que as thredas sao escalonadas
    for(int i = 0; i <NTHREADS; i++){
        if(i == 0){
            inicio = 0;
            fim = metade;
            valores->valor1 = inicio;
            valores->valor2 = fim;
            valores->thread_id = i + 1;
            if (pthread_create(&tid[0], NULL, incrementar, (void *) valores)){
                printf("ERRO -- pthread_create\n");
            }
        }
        else{
            inicio = metade + 1;
            fim = n - 1;
            valores2->valor1 = inicio;
            valores2->valor2 = fim;
            valores2->thread_id = 2;
            if (pthread_create(&tid[1], NULL, incrementar, (void *) valores2)){
                printf("ERRO -- pthread_create\n");
            }
        }
    }
    //a funcao pthread_join é usada aqui para garantir que o vetor so sera printado apos todas os incrementos terem sido efetivados
    for(int i = 0; i < NTHREADS; i++){
        if (pthread_join(tid[i], NULL)){
            printf("ERRO -- pthread_create\n");
        }
    }
    printf("\nTermino da thread main\n\n");
    free(valores);
    free(valores2);
    printf("Valores finais no vetor:\n");
    for(int i = 0; i < n; i++){
        printf("%d ", vetor[i]);
    }
    return 0;
}
