#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

FILE* in;
int** mat1;
int** mat2;
int** mat3;
int** mat4;
int n1;
int m1;
int n2;
int m2;
typedef struct element{
    int* rowm1;
    int* colm2;
    int result;
}element;
typedef struct row{
    int* rowm1;
    int* result;
}row;
void read();
void mule();
void* elmul(void* args);
void mulr();
void* rmul(void* args);

int main(){
    int i, j;
    FILE* out = fopen("output.txt", "w");
    if(out == NULL){
        perror("Error in opening the file");
        exit(1);
    }
    clock_t start1, start2, end1, end2;
    read();
    start1 = clock();
    mule();
    end1 = clock();
    for(i = 0; i < n1; i++){
        for(j = 0; j < m2; j++){
            fprintf(out, "%d ", mat3[i][j]);
        }
        fprintf(out, "\n");
    }
    fprintf(out, "Time taken: %lf\n", ((double) (end1 - start1)) / CLOCKS_PER_SEC);
    fprintf(out, "\n");
    start2 = clock();
    mulr();
    end2 = clock();
    for(i = 0; i < n1; i++){
        for(j = 0; j < m2; j++){
            fprintf(out, "%d ", mat4[i][j]);
        }
        fprintf(out, "\n");
    }
    fprintf(out, "Time taken: %lf\n", ((double) (end2 - start2)) / CLOCKS_PER_SEC);
    fclose(out);
    return 0;
}

void read(){
    int i, j;
    in = fopen("input.txt", "r");
    if(in == NULL){
        perror("Error in opening the file");
        exit(1);
    }
    fscanf(in, "%d %d\n", &n1, &m1);
    mat1 = malloc(sizeof(int*) * n1);
    for(i = 0; i < n1; i++){
        if(feof(in))
            break;
        mat1[i] = malloc(sizeof(int) * m1);
        for(j = 0; j < m1; j++){
            if(feof(in))
                break;
            fscanf(in, "%d", &mat1[i][j]);
        }
        fscanf(in, "\n");
    }
    fscanf(in, "%d %d\n", &n2, &m2);
    mat2 = malloc(sizeof(int*) * n2);
    for(i = 0; i < n2; i++){
        if(feof(in))
            break;
        mat2[i] = malloc(sizeof(int) * m2);
        for(j = 0; j < m2; j++){
            if(feof(in))
                break;
            fscanf(in, "%d", &mat2[i][j]);
        }
        fscanf(in, "\n");
    }
    mat3 = malloc(sizeof(int*) * n1);
    mat4 = malloc(sizeof(int*) * n1);
    for(i = 0; i < n1; i++){
        mat3[i] = malloc(sizeof(int) * m2);
        mat4[i] = malloc(sizeof(int) * m2);
    }
    fclose(in);
}

void mule(){
    int i, j, k;
    pthread_t threads[n1][m2];
    element*** el = malloc(sizeof(element**) * n1);
    for(i = 0; i < n1; i++){
        el[i] = malloc(sizeof(element*) * m2);
        for(j = 0; j < m2; j++){
            el[i][j] = (element*)malloc(sizeof(element));
            el[i][j]->rowm1 = malloc(sizeof(int) * m1);
            el[i][j]->colm2 = malloc(sizeof(int) * n2);
        }
    }

    for(i = 0; i < n1; i++){
        for(j = 0; j < m2; j++){
            for(k = 0; k < m1; k++){
                el[i][j]->rowm1[k] = mat1[i][k];
                el[i][j]->colm2[k] = mat2[k][j];
            }
            pthread_create(&threads[i][j], NULL, elmul, (void*)(el[i][j]));
        }
    }
    for(i = 0; i < n1; i++){
        for(j = 0; j < m2; j++){
            pthread_join(threads[i][j], NULL);
        }
    }
    for(i = 0; i < n1; i++){
        for(j = 0; j < m2; j++){
            mat3[i][j] = el[i][j]->result;
        }
    }
}

void* elmul(void* args){
    int i;
    ((element*)args)->result = 0;
    for(i = 0; i < m1; i++){
        ((element*)args)->result += (((element*)args)->rowm1[i] * ((element*)args)->colm2[i]);
    }
}

void mulr(){
    int i, j;
    pthread_t threads[n1];
    row** r = malloc(sizeof(row*) * n1);
    for(i = 0; i < n1; i++){
        r[i] = (row*)malloc(sizeof(row));
        r[i]->rowm1 = malloc(sizeof(int) * m1);
        r[i]->result = malloc(sizeof(int) * m2);
    }

    for(i = 0; i < n1; i++){
        for(j = 0; j < m1; j++){
            r[i]->rowm1[j] = mat1[i][j];
        }
        pthread_create(&threads[i], NULL, rmul, (void*)(r[i]));
    }
    for(i = 0; i < n1; i++){
        pthread_join(threads[i], NULL);
    }
    for(i = 0; i < n1; i++){
        for(j = 0; j < m2; j++){
            mat4[i][j] = r[i]->result[j];
        }
    }
}

void* rmul(void* args){
    int i, j;
    for(i = 0; i < m2; i++){
        ((row*)args)->result[i] = 0;
        for(j = 0; j < m1; j++){
            ((row*)args)->result[i] += (((row*)args)->rowm1[j] * mat2[j][i]);
        }
    }
}
