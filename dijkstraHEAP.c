#include <stdio.h>
#include <stdlib.h>

#define INT_MAX 2147483647

struct Heap{
    int occupati;
    int capacita;
    int *arrayValori;
    int *arrayHeapToGrafo;
    int *arrayGrafoToHeap;
};
typedef struct Heap Heap;

Heap *CreateHeap(int capacita);
void insert(Heap *h, int key, int *distanza, int index, int peso);
void aggiornamento(Heap *h, int valore, int peso, int index);
void heapify_bottom_top(Heap *h,int index);
void heapify_top_bottom(Heap *h, int parent_node);
int PopMin(Heap *h,int *indice);

int main(){
    int d = 3;
    int Grafo[d][d];
    Grafo[0][0] = 0;
    Grafo[0][1] = 0;
    Grafo[0][2] = 2;
    Grafo[1][0] = 7;
    Grafo[1][1] = 0;
    Grafo[1][2] = 4;
    Grafo[2][0] = 0;
    Grafo[2][1] = 1;
    Grafo[2][2] = 0;
    int capacita = d-1;
    int *distanza = malloc(d*sizeof(int));
    for (int i = 1; i < d; i++)
    {
        distanza[i]=INT_MAX;
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
        for (int j = 1; j < d; j++)
        {
            if(indice!=j && Grafo[indice][j]!=0){ //Cammino da considerare
                if(distanza[j]==INT_MAX) { //Nodo non ancora visitato
                    if (heap->arrayGrafoToHeap[j-1] == INT_MAX) //Nessun cammino per il nodo 
                    {
                        insert(heap, Grafo[indice][j],distanza, j, peso);
                    }
                    else if((Grafo[indice][j]+peso) < heap->arrayValori[heap->arrayGrafoToHeap[j-1]]) aggiornamento(heap,Grafo[indice][j],peso,j); //aggiorno il valore presente nell'heapp
                }
            } 
        }
        peso = PopMin(heap,&indice);
        printf("Pop di valore: %d e indice: %d\n",peso,indice);
        if((distanza[indice] > peso || distanza[indice]==INT_MAX) && peso!=INT_MAX && peso!=-1){
            distanza[indice] = peso;
            printf("Aggiorno distanza \n");
        }
        for (int k = 0; k < d; k++)
        {
            printf("Nodo: %d, Distanza: %d\n",k,distanza[k]);
        }
    }

    int somma = 0;
    for (int i = 0; i < d; i++)
    {
        if(distanza[i]!=INT_MAX) somma+=distanza[i];
        printf("Nodo: %d, Distanza: %d\n",i,distanza[i]);
    }
    printf("Punteggio: %d\n",somma);

    for (int i = 0; i < d-1; i++)
    {
        printf("%d ",heap->arrayValori[i]);
    }
    printf("\n");

    for (int i = 0; i < d-1; i++)
    {
        printf("%d ",heap->arrayHeapToGrafo[i]);
    }
    printf("\n");

    for (int i = 0; i < d-1; i++)
    {
        printf("%d ",heap->arrayGrafoToHeap[i]);
    }
    printf("\n");
    
   free(distanza);
   free(heap->arrayGrafoToHeap);
   free(heap->arrayHeapToGrafo);
   free(heap->arrayValori);
   free(heap);
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
    h->arrayHeapToGrafo = (int *) malloc(capacita*sizeof(int));
    h->arrayGrafoToHeap = (int *) malloc(capacita*sizeof(int));

    for (int i = 0; i < capacita; i++)
    {
        h->arrayValori[i] = INT_MAX;
        h->arrayHeapToGrafo[i] = INT_MAX;
        h->arrayGrafoToHeap[i] = INT_MAX;
    }
    

    if ( h->arrayValori == NULL || h->arrayGrafoToHeap == NULL || h->arrayHeapToGrafo == NULL){
        printf("Memory Error!");
        return NULL;
    }
    return h;
}

void aggiornamento(Heap *h, int valore, int peso, int index){
    printf("Sono aggiornamento %d %d %d\n ",(valore+peso),index,h->arrayGrafoToHeap[index-1]);
    h->arrayValori[h->arrayGrafoToHeap[index-1]] = valore + peso;
    printf("Valore aggiornato %d a indice %d\n",h->arrayValori[h->arrayGrafoToHeap[index-1]], h->arrayGrafoToHeap[index-1]);
    if(h->arrayGrafoToHeap[index-1]!=0) heapify_bottom_top(h,h->arrayGrafoToHeap[index-1]);
    printf("Fine aggiornamento di %d a indice %d\n",h->arrayValori[h->arrayGrafoToHeap[index-1]], h->arrayGrafoToHeap[index-1]);
    for (int i = 0; i < 2; i++)
    {
        printf("%d %d %d | ",h->arrayValori[i],h->arrayHeapToGrafo[i],h->arrayGrafoToHeap[i]);
    }
    printf("\n");
}

void insert(Heap *h, int key,int *distanza,int index, int peso){
    if( h->occupati < h->capacita ){
        h->arrayValori[h->occupati] = key + peso;
        h->arrayHeapToGrafo[h->occupati] = index;
        h->arrayGrafoToHeap[index-1] = h->occupati;
        printf("Inserito %d in %d\n",(key+peso),h->occupati);
        heapify_bottom_top(h, h->occupati);
        for (int i = 0; i < 2; i++)
        {
            printf("%d %d %d | ",h->arrayValori[i],h->arrayHeapToGrafo[i],h->arrayGrafoToHeap[i]);
        }
        printf("Fine heap\n");
        h->occupati++;
    }
}

void heapify_bottom_top(Heap *h,int index){
    int temp;
    int parent_node = (index-1)/2;
    printf("Heapify Bottom Top\n");

    if(h->arrayValori[parent_node] > h->arrayValori[index]){
        temp = h->arrayValori[parent_node];
        h->arrayValori[parent_node] = h->arrayValori[index];
        h->arrayValori[index] = temp;
        temp = h->arrayHeapToGrafo[parent_node];
        h->arrayHeapToGrafo[parent_node] = h->arrayHeapToGrafo[index];
        h->arrayHeapToGrafo[index] = temp;
        temp = h->arrayGrafoToHeap[index-1];
        h->arrayGrafoToHeap[index-1] = h->arrayGrafoToHeap[parent_node];
        h->arrayGrafoToHeap[parent_node] = temp;
        heapify_bottom_top(h,parent_node);
    }
}

void heapify_top_bottom(Heap *h, int parent_node){
    int sinistro = INT_MAX;
    int destro = INT_MAX;
    int min;
    int temp;
    int indexMin, indexParent;

    if((parent_node*2+1) < h->occupati) sinistro = parent_node*2 + 1;
    if((parent_node*2+2) < h->occupati) destro = parent_node*2 + 2;

    if((sinistro !=INT_MAX || destro !=INT_MAX) && parent_node!=INT_MAX){
        min = parent_node;
        if(h->arrayValori[sinistro] < h->arrayValori[min]) min = sinistro;
        if(h->arrayValori[destro] < h->arrayValori[min]) min = destro;

        if(min!=parent_node){
            temp = h->arrayValori[min];
            h->arrayValori[min] = h->arrayValori[parent_node];
            h->arrayValori[parent_node] = temp;

            temp = h->arrayHeapToGrafo[min];
            h->arrayHeapToGrafo[min] = h->arrayHeapToGrafo[parent_node];
            h->arrayHeapToGrafo[parent_node] = temp;

            indexMin = h->arrayHeapToGrafo[parent_node] - 1;
            indexParent = h->arrayHeapToGrafo[min] - 1;
            h->arrayGrafoToHeap[indexMin] = parent_node;
            h->arrayGrafoToHeap[indexParent] = min; 
            heapify_top_bottom(h,min);
        }
    }
    /*int left = parent_node*2+1;
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
        temp = h->arrayHeapToGrafo[min];
        h->arrayHeapToGrafo[min] = h->arrayHeapToGrafo[parent_node];
        h->arrayHeapToGrafo[parent_node] = temp;
        h->arrayGrafoToHeap[h->arrayHeapToGrafo[parent_node]-1] = parent_node;
        h->arrayGrafoToHeap[h->arrayHeapToGrafo[min]-1] = min;

        heapify_top_bottom(h, min);
    }*/
     
}

int PopMin(Heap *h, int *indice){
    int pop;
    int nodo;
    if(h->occupati==0){
        printf("\n__Heap is Empty__\n");
        return -1;
    }
    
    pop = h->arrayValori[0];
    nodo = h->arrayHeapToGrafo[0];
    h->arrayGrafoToHeap[(nodo-1)] = INT_MAX;
    *indice = nodo;
    h->arrayValori[0] = h->arrayValori[h->occupati-1];
    h->arrayValori[h->occupati-1] = INT_MAX;
    h->arrayHeapToGrafo[0] = h->arrayHeapToGrafo[h->occupati-1];
    h->arrayHeapToGrafo[h->occupati-1] = INT_MAX;
    nodo = h->arrayHeapToGrafo[0];
    for (int i = 0; i < 2; i++)
    {
        printf("%d %d %d | ",h->arrayValori[i],h->arrayHeapToGrafo[i],h->arrayGrafoToHeap[i]);
    }
    printf("Nodo: %d, occupati: %d\n",nodo-1,h->occupati);
    if(nodo != INT_MAX) h->arrayGrafoToHeap[nodo-1] = 0;
    h->occupati--;
    /*pop = h->arrayValori[0];
    h->arrayGrafoToHeap[h->arrayHeapToGrafo[0]-1] = -1;
    h->arrayValori[0] = h->arrayValori[h->occupati-1];
    *indice = h->arrayHeapToGrafo[0];
    h->arrayHeapToGrafo[0] = h->arrayHeapToGrafo[h->occupati-1];
    h->arrayGrafoToHeap[h->occupati-1] = h->arrayHeapToGrafo[0];
    h->arrayValori[h->occupati-1] = -1;
    h->arrayHeapToGrafo[h->occupati-1] = -1;
    h->occupati--;
    */
    for (int i = 0; i < 2; i++)
    {
        printf("%d %d %d | ",h->arrayValori[i],h->arrayHeapToGrafo[i],h->arrayGrafoToHeap[i]);
    }
    
    heapify_top_bottom(h, 0);
    printf("Dopo heapifyTopBottom\n");
    for (int i = 0; i < 2; i++)
    {
        printf("%d %d %d | ",h->arrayValori[i],h->arrayHeapToGrafo[i],h->arrayGrafoToHeap[i]);
    } 
    return pop;
}
