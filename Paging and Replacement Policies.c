#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct clockPage{
    int pn;
    int use;
}clockPage;
void OPTIMAL(int nPages, int* pRefs, int s);
void FIFO(int nPages, int* pRefs, int s);
void LRU(int nPages, int* pRefs, int s);
void CLOCK(int nPages, int* pRefs, int s);

int main()
{
    int i;
    int nPages;
    char* alg;
    int p;
    int s = 0;
    int* pRefs;
    scanf("%d", &nPages);
    getchar();
    scanf("%m[^\n]s", &alg);
    getchar();
    scanf("%d", &p);
    while(p != -1){
        if(s == 0){
            pRefs = malloc(sizeof(int));
        }
        else{
            pRefs = realloc(pRefs, (s + 1) * sizeof(int));
        }
        pRefs[s++] = p;
        scanf("%d", &p);
    }
    //for(i = 0; i < s; i++){
        //printf("%d", pRefs[i]);
    //}
    //printf("\n");
    //printf("%d\n", nPages);
    if(!strcmp(alg, "OPTIMAL")){
        OPTIMAL(nPages, pRefs, s);
    }
    else if(!strcmp(alg, "FIFO")){
        FIFO(nPages, pRefs, s);
    }
    else if(!strcmp(alg, "LRU")){
        LRU(nPages, pRefs, s);
    }
    else if(!strcmp(alg, "CLOCK")){
        CLOCK(nPages, pRefs, s);
    }
    free(alg);
    free(pRefs);
    return 0;
}

void OPTIMAL(int nPages, int* pRefs, int s){
    int i, j, z, found, nPageF = 0, pap = 0, full = 0, lnf;
    int* aPages = malloc(sizeof(int) * nPages);
    printf("Replacement Policy = OPTIMAL\n");
    printf("-------------------------------------\n");
    printf("Page   Content of Frames\n");
    printf("----   -----------------\n");
    for(i = 0; i < s; i++){
        if(!full){
            found = 0;
            for(j = 0; j < pap; j++){
                if(aPages[j] == pRefs[i]){
                    found = 1;
                    break;
                }
            }
            if(!found){
                aPages[pap] = pRefs[i];
                pap++;
            }
            if((pRefs[i] / 10) == 0){
                printf("0");
            }
            printf("%d     ", pRefs[i]);
            for(j = 0; j < pap; j++){
                if((aPages[j] / 10) == 0){
                    printf("0");
                }
                printf("%d ", aPages[j]);
            }
            printf("\n");
            if(pap == nPages){
                full = 1;
            }
        }
        else{
            found = 0;
            for(j = 0; j < nPages; j++){
                if(aPages[j] == pRefs[i]){
                    found = 1;
                    break;
                }
            }
            if((pRefs[i] / 10) == 0){
                printf("0");
            }
            if(!found){
                nPageF++;
                lnf = 0;
                pap = 0;
                for(z = 0; z < nPages; z++){
                    found = 0;
                    for(j = i + 1; j < s; j++){
                        if(aPages[z] == pRefs[j]){
                            found = 1;
                            break;
                        }
                    }
                    if(found){
                        if(j - i > lnf){
                            lnf = j - i;
                            pap = z;
                        }
                    }
                    else{
                        lnf = -1;
                        pap = z;
                        break;
                    }
                }
                aPages[pap] = pRefs[i];
                printf("%d F   ", pRefs[i]);
            }
            else{
                printf("%d     ", pRefs[i]);
            }
            for(j = 0; j < nPages; j++){
                if((aPages[j] / 10) == 0){
                    printf("0");
                }
                printf("%d ", aPages[j]);
            }
            printf("\n");
        }
    }
    printf("-------------------------------------\n");
    printf("Number of page faults = %d\n", nPageF);
}

void FIFO(int nPages, int* pRefs, int s){
    int i, j, found, nPageF = 0, pap = 0, full = 0;
    int* aPages = malloc(sizeof(int) * nPages);
    printf("Replacement Policy = FIFO\n");
    printf("-------------------------------------\n");
    printf("Page   Content of Frames\n");
    printf("----   -----------------\n");
    for(i = 0; i < s; i++){
        if(!full){
            found = 0;
            for(j = 0; j < pap; j++){
                if(aPages[j] == pRefs[i]){
                    found = 1;
                    break;
                }
            }
            if(!found){
                aPages[pap] = pRefs[i];
                pap++;
            }
            if((pRefs[i] / 10) == 0){
                printf("0");
            }
            printf("%d     ", pRefs[i]);
            for(j = 0; j < pap; j++){
                if((aPages[j] / 10) == 0){
                    printf("0");
                }
                printf("%d ", aPages[j]);
            }
            printf("\n");
            if(pap == nPages){
                full = 1;
            }
        }
        else{
            found = 0;
            for(j = 0; j < nPages; j++){
                if(aPages[j] == pRefs[i]){
                    found = 1;
                    break;
                }
            }
            if((pRefs[i] / 10) == 0){
                printf("0");
            }
            if(!found){
                nPageF++;
                aPages[pap % nPages] = pRefs[i];
                pap++;
                printf("%d F   ", pRefs[i]);
            }
            else{
                printf("%d     ", pRefs[i]);
            }
            for(j = 0; j < nPages; j++){
                if((aPages[j] / 10) == 0){
                    printf("0");
                }
                printf("%d ", aPages[j]);
            }
            printf("\n");
        }
    }
    printf("-------------------------------------\n");
    printf("Number of page faults = %d\n", nPageF);
}

void LRU(int nPages, int* pRefs, int s){
    int i, j, z, found, nPageF = 0, pap = 0, full = 0, lpf;
    int* aPages = malloc(sizeof(int) * nPages);
    printf("Replacement Policy = LRU\n");
    printf("-------------------------------------\n");
    printf("Page   Content of Frames\n");
    printf("----   -----------------\n");
    for(i = 0; i < s; i++){
        if(!full){
            found = 0;
            for(j = 0; j < pap; j++){
                if(aPages[j] == pRefs[i]){
                    found = 1;
                    break;
                }
            }
            if(!found){
                aPages[pap] = pRefs[i];
                pap++;
            }
            if((pRefs[i] / 10) == 0){
                printf("0");
            }
            printf("%d     ", pRefs[i]);
            for(j = 0; j < pap; j++){
                if((aPages[j] / 10) == 0){
                    printf("0");
                }
                printf("%d ", aPages[j]);
            }
            printf("\n");
            if(pap == nPages){
                full = 1;
            }
        }
        else{
            found = 0;
            for(j = 0; j < nPages; j++){
                if(aPages[j] == pRefs[i]){
                    found = 1;
                    break;
                }
            }
            if((pRefs[i] / 10) == 0){
                printf("0");
            }
            if(!found){
                nPageF++;
                lpf = 0;
                pap = 0;
                for(z = 0; z < nPages; z++){
                    for(j = i - 1; j >= 0; j--){
                        if(aPages[z] == pRefs[j]){
                            if(i - j > lpf){
                                lpf = i - j;
                                pap = z;
                            }
                            break;
                        }
                    }
                }
                aPages[pap] = pRefs[i];
                printf("%d F   ", pRefs[i]);
            }
            else{
                printf("%d     ", pRefs[i]);
            }
            for(j = 0; j < nPages; j++){
                if((aPages[j] / 10) == 0){
                    printf("0");
                }
                printf("%d ", aPages[j]);
            }
            printf("\n");
        }
    }
    printf("-------------------------------------\n");
    printf("Number of page faults = %d\n", nPageF);
}

void CLOCK(int nPages, int* pRefs, int s){
int i, j, z, found, nPageF = 0, pap = 0, full = 0;
    clockPage** aPages = malloc(sizeof(clockPage*) * nPages);
    for(i = 0; i < nPages; i++){
        aPages[i] = malloc(sizeof(clockPage));
    }
    printf("Replacement Policy = CLOCK\n");
    printf("-------------------------------------\n");
    printf("Page   Content of Frames\n");
    printf("----   -----------------\n");
    for(i = 0; i < s; i++){
        if(!full){
            found = 0;
            for(j = 0; j < pap; j++){
                if(aPages[j]->pn == pRefs[i]){
                    found = 1;
                    aPages[j]->use = 1;
                    break;
                }
            }
            if(!found){
                aPages[pap]->pn = pRefs[i];
                aPages[pap]->use = 1;
                pap++;
            }
            if((pRefs[i] / 10) == 0){
                printf("0");
            }
            printf("%d     ", pRefs[i]);
            for(j = 0; j < pap; j++){
                if(((aPages[j]->pn) / 10) == 0){
                    printf("0");
                }
                printf("%d ", aPages[j]->pn);
            }
            printf("\n");
            if(pap == nPages){
                full = 1;
                pap = 0;
            }
        }
        else{
            found = 0;
            for(j = 0; j < nPages; j++){
                if(aPages[j]->pn == pRefs[i]){
                    found = 1;
                    aPages[j]->use = 1;
                    break;
                }
            }
            if((pRefs[i] / 10) == 0){
                printf("0");
            }
            if(!found){
                nPageF++;
                if(pap == nPages){
                    pap = 0;
                }
                while(1){
                    if(pap == nPages){
                        pap = 0;
                    }
                    if(aPages[pap]->use == 1){
                        aPages[pap]->use = 0;
                        pap++;
                    }
                    else{
                        aPages[pap]->pn = pRefs[i];
                        aPages[pap]->use = 1;
                        pap++;
                        break;
                    }
                }
                printf("%d F   ", pRefs[i]);
            }
            else{
                printf("%d     ", pRefs[i]);
            }
            for(j = 0; j < nPages; j++){
                if(((aPages[j]->pn) / 10) == 0){
                    printf("0");
                }
                printf("%d ", aPages[j]->pn);
            }
            printf("\n");
        }
    }
    printf("-------------------------------------\n");
    printf("Number of page faults = %d\n", nPageF);
}
