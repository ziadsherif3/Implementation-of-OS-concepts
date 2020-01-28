#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

FILE* in;
int n;
int* ar;
typedef struct thread_args{
    int* arr;
    int* temp;
    int ls;
    int re;
}thread_args;
void read();
void mergeSort(int* arr);
void* sort(void* args);
//void sort(int* arr, int* temp, int ls, int re);
void mergeHalves(int* arr, int* temp, int ls, int re);
void arrayCopy(int* src, int srcPos, int* dest, int destPos, int len);

int main(){
    int i;
    clock_t start, end;
    read();
    printf("Array before sorting:\n");
    for(i = 0; i < n; i++)
        printf("%d ", ar[i]);
    printf("\n");
    start = clock();
    mergeSort(ar);
    end = clock();
    printf("Array after sorting:\n");
    for(i = 0; i < n; i++)
        printf("%d ", ar[i]);
    printf("\nTime taken: %lf\n", ((double) (end - start)) / CLOCKS_PER_SEC);
    free(ar);
    return 0;
}

void read(){
    int i;
    in = fopen("input.txt","r");
    if(in == NULL){
        perror("Error in opening the file");
        exit(1);
    }
    fscanf(in,"%d\n", &n);
    ar = malloc(sizeof(int) * n);
    for(i = 0; i < n; i++){
        if(feof(in))
            break;
        fscanf(in, "%d", &ar[i]);
    }
    fclose(in);
}

void mergeSort(int* arr){
    int* temp = malloc(sizeof(int) * n);
    thread_args* args1 = (thread_args*)malloc(sizeof(thread_args));
    thread_args* args2 = (thread_args*)malloc(sizeof(thread_args));
    /**Assigning values to args1 elements.*/
    args1->arr = arr;
    args1-> temp = temp;
    args1->ls = 0;
    args1->re = (n - 1) / 2;
    /**Assigning values to args2 elements.*/
    args2->arr = arr;
    args2-> temp = temp;
    args2->ls = ((n - 1) / 2) + 1;
    args2->re = n - 1;
    /**Creating the threads and waiting until they terminate.*/
    pthread_t thread1;
    pthread_t thread2;
    pthread_create(&thread1, NULL, sort, (void*)args1);
    pthread_create(&thread2, NULL, sort, (void*)args2);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    mergeHalves(arr, temp, 0, n - 1);
    //sort(arr, temp, 0, n - 1);
    free(temp);
    free(args1);
    free(args2);
}

void* sort(void* args){
    if(((thread_args*)args)->ls >= ((thread_args*)args)->re){
        return ((void*)NULL);
    }
    int mid = (((thread_args*)args)->ls + ((thread_args*)args)->re) / 2;
    thread_args* args1 = (thread_args*)malloc(sizeof(thread_args));
    thread_args* args2 = (thread_args*)malloc(sizeof(thread_args));
    /**Assigning values to args1 elements.*/
    args1->arr = ((thread_args*)args)->arr;
    args1->temp = ((thread_args*)args)->temp;
    args1->ls = ((thread_args*)args)->ls;
    args1->re = mid;
    /**Assigning values to args2 elements.*/
    args2->arr = ((thread_args*)args)->arr;
    args2->temp = ((thread_args*)args)->temp;
    args2->ls = mid + 1;
    args2->re = ((thread_args*)args)->re;
    /**Creating the threads and waiting until they terminate.*/
    pthread_t thread1;
    pthread_t thread2;
    pthread_create(&thread1, NULL, sort, (void*)args1);
    pthread_create(&thread2, NULL, sort, (void*)args2);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    //sort(arr, temp, ls, mid);
    //sort(arr, temp, mid + 1, re);
    mergeHalves(((thread_args*)args)->arr, ((thread_args*)args)->temp, ((thread_args*)args)->ls, ((thread_args*)args)->re);
    free(args1);
    free(args2);
}

void mergeHalves(int* arr, int* temp, int ls, int re){
    int le = (ls + re) / 2;
    int rs = le + 1;
    int size = re - ls + 1;

    int index = ls;
    int left = ls;
    int right = rs;

    while(left <= le && right <= re){
        if(arr[left] < arr[right]){
            temp[index] = arr[left];
            left++;
        }else{
            temp[index] = arr[right];
            right++;
        }
        index++;
    }
    arrayCopy(arr, left, temp, index, le - left + 1);
    arrayCopy(arr, right, temp, index, re - right + 1);
    arrayCopy(temp, ls, arr, ls, size);
}

void arrayCopy(int* src, int srcPos, int* dest, int destPos, int len){
    int i;
    for(i = 0; i < len; i++){
        dest[destPos++] = src[srcPos++];
    }
}
