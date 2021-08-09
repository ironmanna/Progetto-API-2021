#include <stdio.h>
#include <stdlib.h>

/*
GraphRanker
L'obiettivo del progetto di quest'anno è la gestione di una classifica tra grafi diretti pesati
La classifica tiene traccia dei k "migliori" grafi.Il programma da realizzare riceve in ingresso due parametri, 
una sola volta (sulla prima riga del file, separati da spazio)
    d:ilnumerodinodideigrafi
    k:lalunghezzadellaclassifica
E una sequenza di comandi tra
    AggiungiGrafo [matrice-di-adiacenza] 
    TopK

d, k e il numero di grafi sono rappresentabili con interi a 32 bit.


AggiungiGrafo(FILE *fin)
    Crea un array di interi con d-1 celle dove memorizza il valore del cammino minimo a quel nodo, la funzione
    ritorna la somma dei cammini minimi 
TopK()
    Stampa i valori minimi dalla lista

 L'idea è di usare l'algoritmo di Dijkstra con un heap per organizzare le visite successive 
*/

#define BUFFER 10000
#define INT_MAX 2147483647

struct Nodo{
    int numeroOrdine;
     int valore;
    struct Nodo *next;
};

typedef struct Nodo Elem;
typedef Elem *Lista;

struct Heap{
    int occupati;
    int capacita;
    int *arrayValori;
    int *arrayHeapToGrafo;
    int *arrayGrafoToHeap;
};
typedef struct Heap Heap;

int compare(char *str1, char *str2);

extern size_t strlen (const char *__s)
     __THROW __attribute_pure__ __nonnull ((1));

#ifdef	__USE_XOPEN2K8
/* Find the length of STRING, but scan at most MAXLEN characters.
   If no '\0' terminator is found in that many characters, return MAXLEN.  */
extern size_t strnlen (const char *__string, size_t __maxlen)
     __THROW __attribute_pure__ __nonnull ((1));
#endif

Heap *CreaHeap(int capacita);
void inserisci(Heap *h, int valore, int *distanza, int indice, int peso);
void heapifyBassoAlto(Heap *h,int indice);
void heapifyAltoBasso(Heap *h, int indicePadre);
void aggiornamento(Heap *h, int valore, int peso, int index);
int PopMin(Heap *h,int *indice);
int Dijkstra(int d, int matrice[d][d]);
int AggiungiGrafo(FILE *fin, int d);
Lista accodamento (Lista best,  int punteggio, int numeroOrdine);
void TopK(Lista best, int k);
void libera(Lista best);

int main() {
    FILE *fin;
    Lista classifica = NULL;
    fin = fopen("input_2","r");
    int d = 0, k = 0, caratteriLetti = 0, numeroOrdine = 0, scelta = 0;
    char lettura[BUFFER];
    if (fgets(lettura, BUFFER, fin) == NULL) printf("Errore nella prima lettura\n");
    caratteriLetti = strlen(lettura);
    if(lettura[caratteriLetti-1] == '\n') lettura[caratteriLetti-1]='\0';
    for(int i=0; i<caratteriLetti; i++){
        if (lettura[i] != 32 && lettura[i] != 0){
            if (scelta == 0) d = d*10 + (lettura[i] - 48);
            else k = k*10 + (lettura[i] - 48);
        }
        if (lettura[i] == 32) scelta = 1;
    }
    while(fgets(lettura, BUFFER, fin) != NULL) 
    {
        caratteriLetti = strlen(lettura);
        if(lettura[caratteriLetti-1] == '\n') lettura[caratteriLetti-1]='\0';
        if (!compare(lettura,"AggiungiGrafo")){
            classifica = accodamento(classifica, AggiungiGrafo(fin,d),numeroOrdine);
            numeroOrdine++;
            //printf("Fine AggiungiGrafo\n");
        } 
        if (!compare(lettura,"TopK")) TopK(classifica, k);
    }
    libera(classifica);
    fclose (fin);
    return 0;
}

void libera(Lista best){
    Lista trash;
    while(best!=NULL){
        trash = best;
        best = best->next;
        free(trash);
    }
}

int compare(char *str1, char *str2){
    int indice = 0;
    while (str1[indice]==str2[indice] && (str1[indice]!='\0' && str2[indice]!='\0'))
    {
        indice++;
        //printf("Sono in compare\n");
    }
    if(str1[indice] == str2[indice]) return 0;
    else return str1[indice]-str2[indice];
}

Lista accodamento (Lista best,  int punteggio, int numeroOrdine){
    Lista p;
    if(best==NULL){
        best = malloc(sizeof(struct Nodo));
        best->numeroOrdine = numeroOrdine;
        best->valore = punteggio;
        best->next = NULL;
        return best;
    }
    if (punteggio < best->valore) {
        p = malloc (sizeof(struct Nodo));
        p->numeroOrdine = numeroOrdine;
        p->valore = punteggio;
        p->next = best;
        return p;
    }    
    best->next = accodamento(best->next,punteggio,numeroOrdine);
    return best;
}

void TopK(Lista best, int k){
    while (best!=NULL && k!=0){
        printf("%d ",best->numeroOrdine);
        best = best->next;
        k--;

    }
    printf("\n");
}

Heap *CreaHeap(int capacita){
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
    //printf("Valore aggiornato %d a indice %d\n",h->arrayValori[h->arrayGrafoToHeap[index-1]], h->arrayGrafoToHeap[index-1]);
    if(h->arrayGrafoToHeap[index-1]!=0) heapifyBassoAlto(h,h->arrayGrafoToHeap[index-1]);
    //printf("Fine aggiornamento di %d a indice %d\n",h->arrayValori[h->arrayGrafoToHeap[index-1]], h->arrayGrafoToHeap[index-1]);
    for (int i = 0; i < 2; i++)
    {
        //printf("%d %d %d | ",h->arrayValori[i],h->arrayHeapToGrafo[i],h->arrayGrafoToHeap[i]);
    }
    //printf("\n");
    printf("Fine aggiornamento\n");
}

void insert(Heap *h, int key,int *distanza,int index, int peso){
    if( h->occupati < h->capacita ){
        h->arrayValori[h->occupati] = key + peso;
        h->arrayHeapToGrafo[h->occupati] = index;
        h->arrayGrafoToHeap[index-1] = h->occupati;
        //printf("Inserito %d in %d\n",(key+peso),h->occupati);
        heapifyBassoAlto(h, h->occupati);
        for (int i = 0; i < 2; i++)
        {
            //printf("%d %d %d | ",h->arrayValori[i],h->arrayHeapToGrafo[i],h->arrayGrafoToHeap[i]);
        }
        //printf("Fine heap\n");
        h->occupati++;
    }
}

void heapifyBassoAlto(Heap *h,int index){
    int temp;
    int parent_node = (index-1)/2;
    //printf("Heapify Bottom Top\n");

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
        heapifyBassoAlto(h,parent_node);
    }
}

void heapifyAltoBasso(Heap *h, int parent_node){ //da rifare
    int sinistro = INT_MAX;
    int destro = INT_MAX;
    int min;
    int temp;
    int indexMin, indexParent;

    if((parent_node*2+1) < h->occupati && parent_node!=INT_MAX) sinistro = parent_node*2 + 1;
    if((parent_node*2+2) < h->occupati && parent_node!=INT_MAX) destro = parent_node*2 + 2;

    if(sinistro !=INT_MAX  && parent_node!=INT_MAX){
        min = parent_node;
        if(h->arrayValori[sinistro] < h->arrayValori[min]) min = sinistro;
        if(destro!=INT_MAX) if(h->arrayValori[destro] < h->arrayValori[min]) min = destro;

        if(min!=parent_node){
            temp = h->arrayValori[min];
            h->arrayValori[min] = h->arrayValori[parent_node];
            h->arrayValori[parent_node] = temp;

            temp = h->arrayHeapToGrafo[min];
            h->arrayHeapToGrafo[min] = h->arrayHeapToGrafo[parent_node];
            h->arrayHeapToGrafo[parent_node] = temp;

            indexMin = h->arrayHeapToGrafo[parent_node] - 1;
            indexParent = h->arrayHeapToGrafo[min] - 1;
            printf("%d %d %d %d \n",indexMin,indexParent,min,parent_node);
            h->arrayGrafoToHeap[indexMin] = parent_node;
            h->arrayGrafoToHeap[indexParent] = min; 
            heapifyAltoBasso(h,min);
        }
    }
}

int PopMin(Heap *h, int *indice){
    int pop;
    int nodo;
    printf("Sono topMin\n");
    if(h->occupati==0){
        printf("\n__Heap is Empty__\n");
        return -1;
    }
    pop = h->arrayValori[0];
    nodo = h->arrayHeapToGrafo[0];
    for (int i = 0; i < 2; i++)
    {
        printf("%d %d %d | ",h->arrayValori[i],h->arrayHeapToGrafo[i],h->arrayGrafoToHeap[i]);
    }
    printf("Sono problema? %d\n",(nodo-1));
    h->arrayGrafoToHeap[(nodo-1)] = INT_MAX;
    *indice = nodo;
    h->arrayValori[0] = h->arrayValori[h->occupati-1];
    h->arrayValori[h->occupati-1] = INT_MAX;
    h->arrayHeapToGrafo[0] = h->arrayHeapToGrafo[h->occupati-1];
    h->arrayHeapToGrafo[h->occupati-1] = INT_MAX;
    h->occupati--;
    nodo = h->arrayHeapToGrafo[0];
    for (int i = 0; i < 2; i++)
    {
        //printf("%d %d %d | ",h->arrayValori[i],h->arrayHeapToGrafo[i],h->arrayGrafoToHeap[i]);
    }
    //printf("Nodo: %d, occupati: %d\n",nodo-1,h->occupati);
    if(nodo != INT_MAX) h->arrayGrafoToHeap[nodo-1] = 0;
    
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
        //printf("%d %d %d | ",h->arrayValori[i],h->arrayHeapToGrafo[i],h->arrayGrafoToHeap[i]);
    }
    
    heapifyAltoBasso(h, 0);
    //printf("Dopo heapifyTopBottom\n");
    for (int i = 0; i < 2; i++)
    {
        //printf("%d %d %d | ",h->arrayValori[i],h->arrayHeapToGrafo[i],h->arrayGrafoToHeap[i]);
    } 
    return pop;
}

int AggiungiGrafo(FILE *fin, int d){
     int matrice [d][d];
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            matrice[i][j] = 0;
        }
    }
    int colonna=0;
    int caratteriLetti;
    char riga[BUFFER];
    for (int i = 0; i < d; i++)
    {
        if(fgets(riga, BUFFER, fin) == NULL) printf("Problema 1\n");
        caratteriLetti = strlen(riga);
        if(riga[caratteriLetti-1] == '\n') riga[caratteriLetti-1]='\0';
        for(int j=0;j<caratteriLetti;j++){
           if (riga[j] != 44 && riga[j] != 0){
               if(matrice[i][colonna]==0) matrice[i][colonna]=riga[j]-48;
               else matrice[i][colonna] = matrice[i][colonna]*10 + riga[j]-48;
           }
           if (riga[j]==44) colonna++;
        }
        colonna = 0;
    }
    return Dijkstra(d,matrice);    
}

int Dijkstra(int d, int matrice[d][d]){
    int capacita = d-1;
    int *distanza = malloc(d*sizeof(int));
    for (int i = 0; i < d; i++)
    {
        distanza[i]=INT_MAX;
    }
    distanza[0]=0;

    Heap *heap = CreaHeap(capacita);
    if( heap == NULL ){
        //printf("Errore nella funzione di creazione dell'Heap\n");
        return -1;
    }

    int peso = 0;
    int indice = 0;

    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if(indice!=j && matrice[indice][j]!=0){ //Cammino da considerare
                if(distanza[j]==INT_MAX) { //Nodo non ancora visitato
                    if (heap->arrayGrafoToHeap[j-1] == INT_MAX) //Nessun cammino per il nodo 
                    {
                        insert(heap, matrice[indice][j],distanza, j, peso);
                    }
                    else if((matrice[indice][j]+peso) < heap->arrayValori[heap->arrayGrafoToHeap[j-1]]) aggiornamento(heap,matrice[indice][j],peso,j); //aggiorno il valore presente nell'heapp
                }
            }
        }
        peso = PopMin(heap,&indice);
        //printf("Pop di valore: %d e indice: %d\n",peso,indice);
        if((distanza[indice] > peso || distanza[indice]==INT_MAX) && peso!=INT_MAX && peso!=-1){
            distanza[indice] = peso;
            //printf("Aggiorno distanza \n");
        }
        for (int k = 0; k < d; k++)
        {
            //printf("Nodo: %d, Distanza: %d\n",k,distanza[k]);
        }    
    }

    int somma = 0;
    for (int i = 0; i < d; i++)
    {
        if(distanza[i]!=INT_MAX) somma+=distanza[i];
    }
    free(heap->arrayHeapToGrafo);
    free(heap->arrayGrafoToHeap);
    free(heap->arrayValori);
    free(heap);
    free(distanza);
    return somma;
}