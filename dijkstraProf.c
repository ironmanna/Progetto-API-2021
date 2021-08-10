#include <stdio.h>
#include <stdlib.h>

#define INT_MAX 2147483647

struct Heap{
    int occupati;
    int capacita;
    int *arrayValori;
    int *arrayPrecedenti;
    int *arrayNodi;
};
typedef struct Heap Heap;

struct NodoGrafo{
    int distanza;
    int indice;
    int predecessore;
};
typedef struct NodoGrafo NodoGrafo;

int Dijkstra(int d, int grafo[d][d]);
Heap *CreateHeap(int capacita);
void HeapifyDalBasso(Heap *h,int indice);
void HeapifyDallAlto(Heap *h, int indice);
void Accoda(Heap *p,int indice, int distanza);
NodoGrafo *CancellaMin(Heap *h);
void PopMin(Heap *h);
int trovaIndice (Heap *h, int nodo);

int main(){
    int d = 3;
    int Grafo[d][d];
    Grafo[0][0] = 3;
    Grafo[0][1] = 1;
    Grafo[0][2] = 8;
    Grafo[1][0] = 0;
    Grafo[1][1] = 0;
    Grafo[1][2] = 5;
    Grafo[2][0] = 0;
    Grafo[2][1] = 9;
    Grafo[2][2] = 0;
    
    printf("Punteggio: %d\n",Dijkstra(d,Grafo));
    return 0;
}

Heap *CreateHeap(int capacita){
    Heap *h = (Heap * ) malloc(sizeof(Heap));

    if(h == NULL){
        printf("Memory Error!");
        return NULL;
    }
    h->occupati=0;
    h->capacita = capacita;
    h->arrayValori = (int *) malloc(capacita*sizeof(int));
    h->arrayPrecedenti = (int *) malloc(capacita*sizeof(int));
    h->arrayNodi = (int *) malloc(capacita*sizeof(int));

    for (int i = 0; i < capacita; i++)
    {
        h->arrayValori[i] = INT_MAX;
        h->arrayNodi[i] = -1;
        h->arrayPrecedenti[i] = -1;
    }
    

    if ( h->arrayValori == NULL || h->arrayNodi == NULL || h->arrayPrecedenti == NULL){
        printf("Memory Error!");
        return NULL;
    }
    return h;
}

void HeapifyDalBasso(Heap *h,int indice){
    int temp;
    int parent_node = (indice-1)/2;

    if(h->arrayValori[parent_node] > h->arrayValori[indice]){
        temp = h->arrayValori[parent_node];
        h->arrayValori[parent_node] = h->arrayValori[indice];
        h->arrayValori[indice] = temp;
        temp = h->arrayPrecedenti[parent_node];
        h->arrayPrecedenti[parent_node] = h->arrayPrecedenti[indice];
        h->arrayPrecedenti[indice] = temp;
        temp = h->arrayNodi[parent_node];
        h->arrayNodi[parent_node] = h->arrayNodi[indice];
        h->arrayNodi[indice] = temp;
        HeapifyDalBasso(h,parent_node);
    }
}

void Accoda(Heap *h,int indice, int distanza){
    h->arrayValori[h->occupati] = distanza;
    h->arrayNodi[h->occupati] = indice;
    HeapifyDalBasso(h,h->occupati);
    h->occupati++;
}

void HeapifyDallAlto(Heap *h, int indice){
    int sinistro = INT_MAX;
    int destro = INT_MAX;
    int min;
    int temp;

    if((indice*2+1) < h->occupati) sinistro = indice*2 + 1;
    if((indice*2+2) < h->occupati) destro = indice*2 + 2;

    if(sinistro !=INT_MAX){
        min = indice;
        if(h->arrayValori[sinistro] < h->arrayValori[min]) min = sinistro;
        if(destro !=INT_MAX) if(h->arrayValori[destro] < h->arrayValori[min]) min = destro;

        if(min!=indice){
            temp = h->arrayValori[min];
            h->arrayValori[min] = h->arrayValori[indice];
            h->arrayValori[indice] = temp;

            temp = h->arrayPrecedenti[min];
            h->arrayPrecedenti[min] = h->arrayPrecedenti[indice];
            h->arrayPrecedenti[indice] = temp;

            temp = h->arrayNodi[min];
            h->arrayNodi[min] = h->arrayNodi[indice];
            h->arrayNodi[indice] = temp;
 
            HeapifyDallAlto(h,min);
        }
    } 
}

void PopMin(Heap *h){
    if(h->occupati==0){
        printf("\n__Heap is Empty__\n");
    }
    h->arrayValori[0] = h->arrayValori[h->occupati-1];
    h->arrayValori[h->occupati-1] = INT_MAX;
    h->arrayPrecedenti[0] = h->arrayValori[h->occupati-1];
    h->arrayPrecedenti[h->occupati-1] = -1;
    h->arrayNodi[0] = h->arrayNodi[h->occupati-1];
    h->arrayNodi[h->occupati-1] = -1;
    h->occupati--;

    HeapifyDallAlto(h, 0);
}

NodoGrafo *CancellaMin(Heap *h){
    NodoGrafo *temp = malloc(sizeof(NodoGrafo));
    temp->distanza = h->arrayValori[0];
    temp->indice = h->arrayNodi[0];
    temp->predecessore = h->arrayPrecedenti[0];
    PopMin(h);
    return temp;
}

int trovaIndice (Heap *h,int nodo){
    int i = 0;
    for (i = 0; i < h->occupati; i++)
    {
        if(h->arrayNodi[i] == nodo) return i;
    }
    return -1;
}

int Dijkstra(int d, int grafo[d][d]){
    Heap *Q = CreateHeap(d);
    NodoGrafo *u = malloc(sizeof(NodoGrafo));
    int Visite[2][d];
    Visite[0][0] = 0;
    Visite[1][0] = 0;
    int ndis = 0, indiceAgg = 0;
    for (int i = 0; i < d; i++)
    {
        if(i!=0){
            Visite[0][i] = INT_MAX;
            Visite[1][i] = -1;
        }
        Accoda(Q,i,Visite[0][i]);
    }
    
    while(Q->occupati!=0){
        u = CancellaMin(Q);
        Visite[0][u->indice] = u->distanza;
        Visite[1][u->indice] = u->predecessore;
        for (int i = 0; i < d; i++)
        {
            if(grafo[u->indice][i]!=0 && Visite[1][i] == -1 && u->indice != i){
                ndis = u->distanza + grafo[u->indice][i];
                indiceAgg = trovaIndice(Q,i);
                if(indiceAgg !=-1){
                    if (ndis < Q->arrayValori[indiceAgg]) {
                        Q->arrayValori[indiceAgg] = ndis;
                        Q->arrayPrecedenti[indiceAgg] = u->indice;
                        HeapifyDalBasso(Q,indiceAgg);
                    }
                }
            }
        }
    }

    free(Q);
    free(u);

    int punteggio = 0;
    for (int i = 0; i < d; i++)
    {
        if(Visite[0][i] != INT_MAX) punteggio+=Visite[0][i];
        printf("Nodo %d, Distanza %d\n",i,Visite[0][i]);
    }
    return punteggio;
}