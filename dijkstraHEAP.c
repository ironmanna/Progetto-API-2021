#include <stdio.h>
#include <stdlib.h>

struct Heap{
    int occupati;
    int capacita;
    int *arrayValori;
    int *arrayCorrispondenze;
};
typedef struct Heap Heap;

Heap *CreateHeap(int capacita);
void insert(Heap *h, int key, int *distanza, int index, int peso);
void print(Heap *h);
void heapify_bottom_top(Heap *h,int index);
void heapify_top_bottom(Heap *h, int parent_node);
int PopMin(Heap *h,int *indice);

int main(){
    int d = 4;
    int Grafo[d][d];
    Grafo[0][0] = 0;
    Grafo[0][1] = 7;
    Grafo[0][2] = 5;
    Grafo[0][3] = 12;
    Grafo[1][0] = 9;
    Grafo[1][1] = 0;
    Grafo[1][2] = 4;
    Grafo[1][3] = 6;
    Grafo[2][0] = 8;
    Grafo[2][1] = 14;
    Grafo[2][2] = 0;
    Grafo[2][3] = 2;
    Grafo[3][0] = 50;
    Grafo[3][1] = 1;
    Grafo[3][2] = 3;
    Grafo[3][3] = 0;
    int capacita = d * (d-1);
    int *distanza = malloc(d*sizeof(int));
    for (int i = 0; i < d; i++)
    {
        distanza[i]=-1;
    }
    distanza[0]=0;

    Heap *heap = CreateHeap(capacita);
    if( heap == NULL ){
        printf("__Memory Issue____\n");
        return -1;
    }

    int peso = 0;
    int indice = 0;

    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if(indice!=j && Grafo[indice][j]!=0){
                if(distanza[j]==-1 ) insert(heap, Grafo[indice][j],distanza, j, peso);
            } 
        }
        peso = PopMin(heap,&indice);
        printf("Pop di valore: %d e indice: %d\n",peso,indice);
        if(distanza[indice] > peso || distanza[indice]==-1) distanza[indice] = peso;
    }

    int somma = 0;
    for (int i = 0; i < d; i++)
    {
        if(distanza[i]!=-1) somma+=distanza[i];
        printf("Nodo: %d, Distanza: %d\n",i,distanza[i]);
    }
    printf("Punteggio: %d\n",somma);

    return 0;
}

Heap *CreateHeap(int capacita){
    Heap *h = (Heap * ) malloc(sizeof(Heap)); //one is number of heap

    //check if memory allocation is fails
    if(h == NULL){
        printf("Memory Error!");
        return NULL;
    }
    h->occupati=0;
    h->capacita = capacita;
    h->arrayValori = (int *) malloc(capacita*sizeof(int)); //size in bytes
    h->arrayCorrispondenze = (int *) malloc(capacita*sizeof(int));

    //check if allocation succeed
    if ( h->arrayValori == NULL || h->arrayCorrispondenze == NULL){
        printf("Memory Error!");
        return NULL;
    }
    return h;
}

void insert(Heap *h, int key,int *distanza,int index, int peso){
    if( h->occupati < h->capacita){
        h->arrayValori[h->occupati] = key + peso;
        h->arrayCorrispondenze[h->occupati] = index;
        heapify_bottom_top(h, h->occupati);
        h->occupati++;
    }
}

void heapify_bottom_top(Heap *h,int index){
    int temp;
    int parent_node = (index-1)/2;

    if(h->arrayValori[parent_node] > h->arrayValori[index]){
        //swap and recursive call
        temp = h->arrayValori[parent_node];
        h->arrayValori[parent_node] = h->arrayValori[index];
        h->arrayValori[index] = temp;
        temp = h->arrayCorrispondenze[parent_node];
        h->arrayCorrispondenze[parent_node] = h->arrayCorrispondenze[index];
        h->arrayCorrispondenze[index] = temp;
        heapify_bottom_top(h,parent_node);
    }
}

void heapify_top_bottom(Heap *h, int parent_node){
    int left = parent_node*2+1;
    int right = parent_node*2+2;
    int min;
    int temp;

    if(left >= h->occupati || left <0)
        left = -1;
    if(right >= h->occupati || right <0)
        right = -1;

    if(left != -1 && h->arrayValori[left] < h->arrayValori[parent_node])
        min=left;
    else
        min =parent_node;
    if(right != -1 && h->arrayValori[right] < h->arrayValori[min])
        min = right;

    if(min != parent_node){
        temp = h->arrayValori[min];
        h->arrayValori[min] = h->arrayValori[parent_node];
        h->arrayValori[parent_node] = temp;
        temp = h->arrayCorrispondenze[min];
        h->arrayCorrispondenze[min] = h->arrayCorrispondenze[parent_node];
        h->arrayCorrispondenze[parent_node] = temp;

        // recursive  call
        heapify_top_bottom(h, min);
    }
}

int PopMin(Heap *h, int *indice){
    int pop;
    if(h->occupati==0){
        printf("\n__Heap is Empty__\n");
        return -1;
    }
    // replace first node by last and delete last
    pop = h->arrayValori[0];
    h->arrayValori[0] = h->arrayValori[h->occupati-1];
    *indice = h->arrayCorrispondenze[0];
    h->arrayCorrispondenze[0] = h->arrayCorrispondenze[h->occupati-1];
    h->occupati--;
    heapify_top_bottom(h, 0);
    return pop;
}


void print(Heap *h){
    int i;
    printf("____________Print Heap_____________\n");
    for(i=0;i< h->occupati;i++){
        printf("-> %d %d",h->arrayValori[i],h->arrayCorrispondenze[i]);
    }
    printf("->__/\\__\n");
}