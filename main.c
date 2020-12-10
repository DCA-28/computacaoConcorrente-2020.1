#include <stdio.h>
#include <stdlib.h>
#include <sys\timeb.h>

int **matrixGenerator(int n);
struct timeb start, end;
int diff;


int main() {
    int n; //inteiro que indica a ordem da matrix quadrada
    printf("Digite o numero da ordem da matrix: ");
    scanf("%d", &n);
    int *matrix[n]; //vetor de ponteiro para inteiros, onde cada ponteiro irá apontar para uma linha da matrix
    int *matrix2[n];
    void *linha;
    void *linha2;
    //aqui geramos as "linhas" da matrix, ou seja, os endereços para os quais cada ponteiro do vetor matrix ira apontar
    //cada endereço corresponde a 'n' espaços de inteiro, sendo então uma linha(vetor) da matrix
    ftime(&start);
    for (int i = 0; i < n; i++) {
        linha = (int *) malloc(sizeof(int) * n);
        linha2 = (int *) malloc(sizeof(int) * n);
        if(linha == NULL || linha2 == NULL){
            printf("Nao foi possivel gerar a matriz.");
            exit(-1);
        }
        matrix[i] = linha;
        matrix2[i] = linha2;
    }
    printf("Gerando as matrizes...\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = (i + j);
            matrix2[i][j] = (2 * (i +j));
        }
    }
    ftime(&end);
    diff = (int) (1000.0 * (end.time - start.time) + (end.millitm - start.millitm));
    /*printf("Elementos da matriz 1:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("Elementos da matriz 2:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", matrix2[i][j]);
        }
        printf("\n");
    }*/
    printf("Tempo de excucao da criacao de matrizes em milisegundos: %u", diff);
    return 0;
}

int **matrixGenerator(int n) {
    int *matrix3[n]; //vetor de ponteiro para inteiros, onde cada ponteiro irá apontar para uma linha da matrix
    void *posicao;
    //aqui geramos as "linhas" da matrix, ou seja, os endereços para os quais cada ponteiro do vetor matrix ira apontar
    //cada endereço corresponde a 'n' espaços de inteiro, sendo então uma linha(vetor) da matrix
    for (int i = 0; i < n; i++) {
        posicao = (int *) malloc(sizeof(int) * n);
        matrix3[i] = posicao;
    }
    printf("Gerando a matriz...\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix3[i][j] = (i + j);
        }
    }
    printf("Elementos da matriz:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", matrix3[i][j]);
        }
        printf("\n");
    }
    return matrix3;
}



