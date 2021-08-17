#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
GraphRanker
L'obiettivo del progetto è la gestione di una classifica tra grafi diretti pesati
La classifica tiene traccia dei k "migliori" grafi.Il programma da realizzare riceve in ingresso due parametri, 
una sola volta (sulla prima riga del file, separati da spazio)
    d: il numero di nodi dei grafi
    k: la lunghezza della classifica
E una sequenza di comandi tra
    AggiungiGrafo [matrice-di-adiacenza] 
    TopK
d,k e il numero di grafi sono rappresentabili con interi a 32 bit.
AggiungiGrafo()
    Copia la matrice, invoca la funzione che esegue l'algoritmo di Dijkstra con un MinHeap per la gestione della coda e ritorna il punteggio
    del grafo
TopK()
    Stampa i valori minimi da un MaxHeap
*/

#define BUFFER 1000000
#define INT_MAX 2147483647

struct Heap{
    int occupati; //tiene traccia delle caselle occupate
    int capacita;
    unsigned int *arrayValori; //contiene i punteggi
    int *arrayNodi; //contiene il numero d'ordine
};
typedef struct Heap Heap;

struct NodoGrafo{
    unsigned int distanza; 
    int indice;
};
typedef struct NodoGrafo NodoGrafo;

int compare(char *str1, char *str2);
unsigned int Dijkstra(int d,unsigned int grafo[d][d],Heap *Q,NodoGrafo *u);
Heap *CreateHeap(int capacita);
NodoGrafo *CreaNodoGrafo();
void CostruisciMinHeap(Heap *h);
void CostruisciMaxHeap(Heap *h);
void MinHeapify(Heap *h, int indice);
void MaxHeapify(Heap *h, int indice);
void CancellaMin(Heap *h,NodoGrafo *u);
unsigned int AggiungiGrafo(int d,unsigned int matrice [d][d],Heap *Q, NodoGrafo *u);
void TopK(int k, int best[k]);

int main() {
    //dichiarazione delle variabili

    int d = 0, k = 0, caratteriLetti, numeroOrdine = 0, scelta = 0;
    unsigned int punteggio = 0;
    char lettura[BUFFER];

    //lettura di d e k

    if (fgets(lettura, BUFFER, stdin) == NULL) printf("Errore nella prima lettura\n");
    caratteriLetti = strlen(lettura);
    if(lettura[caratteriLetti-1] == '\n') lettura[caratteriLetti-1]='\0';
    for(int i=0; i<caratteriLetti; i++){
        if (lettura[i] != 32 && lettura[i] != 0){
            if (scelta == 0) d = d*10 + (lettura[i] - 48);
            else k = k*10 + (lettura[i] - 48);
        }
        if (lettura[i] == 32) scelta = 1;
    }

    //creazione delle strutture dati necessarie che verrano poi passate come parametri

    scelta = 0;
    Heap *classifica = CreateHeap(k);
    Heap *Q = CreateHeap(d);
    unsigned int matrice [d][d];
    NodoGrafo *u = CreaNodoGrafo();
    while(fgets(lettura, BUFFER, stdin) != NULL) 
    {
        caratteriLetti = strlen(lettura);
        if(lettura[caratteriLetti-1] == '\n') lettura[caratteriLetti-1]='\0';
        if (!compare(lettura,"AggiungiGrafo")){
            punteggio = AggiungiGrafo(d,matrice,Q,u);
            if(classifica->occupati < k){
                //inserisco nel MaxHeap il punteggio del grafo
                classifica->arrayNodi[classifica->occupati] = numeroOrdine;
                classifica->arrayValori[classifica->occupati] = punteggio;
                classifica->occupati++;
                if(classifica->occupati == k) CostruisciMaxHeap(classifica); //se ho raggiunto il limite di capacita ordino il MaxHeap
            }
            else{
                if(punteggio < classifica->arrayValori[0] && punteggio != 526997){ //controllo che il valore del punteggio debba essere inserito
                    classifica->arrayValori[0] = punteggio;
                    classifica->arrayNodi[0] = numeroOrdine;
                    MaxHeapify(classifica,0); //dopo aver sostituito il nuovo valore ricostruisco l'Heap (l'ultima volta non mi serve)
                }
            }
            numeroOrdine++;
        }
        if (!compare(lettura,"TopK")) TopK(k,classifica->arrayNodi);
    }

    free(Q->arrayNodi);
    free(Q->arrayValori);
    free(Q);
    free(classifica->arrayNodi);
    free(classifica->arrayValori);
    free(classifica);
    free(u);
    return 0;
}

int compare(char *str1, char *str2){ //cicla sulle due stringe e ritorna 0 se sono uguali
    int indice = 0;
    while (str1[indice]==str2[indice] && (str1[indice]!='\0' && str2[indice]!='\0')) indice++;
    if(str1[indice] == str2[indice]) return 0;
    else return str1[indice]-str2[indice];
}

void TopK(int k,int best[k]){ //stampa l'array contente gli indici nel formato riconosciuto  
    for (int *pArr = best ; pArr < best+k; pArr++)
    {
        if(pArr!=best+k-1){
            if(*pArr != -1){
                if(*(pArr+1) != -1) printf("%d ",*pArr);
                else{
                    printf("%d",*pArr);
                    pArr = best + k-1;
                }
            }
            else pArr = best + k-1;
        }
        else {
            if(*pArr != -1) printf("%d",*pArr);
        }
    }
    printf("\n");
}

Heap *CreateHeap(int capacita){ //crea l'Heap di dimensione data
    Heap *h = (Heap * ) malloc(sizeof(Heap));

    if(h == NULL){
        printf("Errore nella creazione dell'Heap\n");
        return NULL;
    }

    h->occupati=0;
    h->capacita = capacita;
    h->arrayValori = (unsigned int *) malloc(capacita*sizeof(int));
    h->arrayNodi = (int *) malloc(capacita*sizeof(int));

    for (int i = 0; i < capacita; i++)
    {
        h->arrayValori[i] = INT_MAX;
        h->arrayNodi[i] = -1;
    }
    
    if ( h->arrayValori == NULL || h->arrayNodi == NULL ){
        printf("Errore nella creazione degli array dell'Heap\n");
        return NULL;
    }
    return h;
}

NodoGrafo *CreaNodoGrafo(){ //crea la struct NodoGrafo
    NodoGrafo *h = (NodoGrafo * ) malloc(sizeof(NodoGrafo));

    if(h == NULL){
        printf("Errore nella creazione del NodoGrafo");
        return NULL;
    }
    h->distanza = 0;
    h->indice = 0;

    return h;
}

void CostruisciMinHeap(Heap *h){ //identifica i nodi non foglia e richiama MinHeapify su di essi
    int heapsize = h->occupati;

    for (int i = (heapsize-1)/2; i >= 0; i--)
    {
        MinHeapify(h,i);
    }
}

void CostruisciMaxHeap(Heap *h){ //identifica i nodi non foglia e richiama MaxHeapify su di essi
    int heapsize = h->occupati;

    for (int i = (heapsize-1)/2; i >= 0; i--)
    {
        MaxHeapify(h,i);
    }
}

void MinHeapify(Heap *h, int indice){ //ordina il MinHeap
    int sinistro = INT_MAX, destro = INT_MAX, min = indice, temp = 0;

    if((indice*2+1) < h->occupati) sinistro = indice*2 + 1;
    if((indice*2+2) < h->occupati) destro = indice*2 + 2;

    if(sinistro !=INT_MAX) if(h->arrayValori[sinistro] < h->arrayValori[min]) min = sinistro;
    if(destro !=INT_MAX) if(h->arrayValori[destro] < h->arrayValori[min]) min = destro;
    
    if(min != indice){
        temp = h->arrayValori[min];
        h->arrayValori[min] = h->arrayValori[indice];
        h->arrayValori[indice] = temp;

        temp = h->arrayNodi[min];
        h->arrayNodi[min] = h->arrayNodi[indice];
        h->arrayNodi[indice] = temp;

        MinHeapify(h,min);
    }
}

void MaxHeapify(Heap *h, int indice){ //ordina il MaxHeap
    int sinistro = INT_MAX, destro = INT_MAX, max = indice, temp = 0;

    if((indice*2+1) < h->occupati) sinistro = indice*2 + 1;
    if((indice*2+2) < h->occupati) destro = indice*2 + 2;

    if(sinistro !=INT_MAX) if(h->arrayValori[sinistro] > h->arrayValori[max]) max = sinistro;
    if(destro !=INT_MAX) if(h->arrayValori[destro] > h->arrayValori[max]) max = destro;
    
    if(max != indice){
        temp = h->arrayValori[max];
        h->arrayValori[max] = h->arrayValori[indice];
        h->arrayValori[indice] = temp;

        temp = h->arrayNodi[max];
        h->arrayNodi[max] = h->arrayNodi[indice];
        h->arrayNodi[indice] = temp;

        MaxHeapify(h,max);
    }
}

void CancellaMin(Heap *h,NodoGrafo *u){ //cancella il valore minimo dal MinHeap e lo riordina
    if(h->occupati < 1) printf("Heap vuoto\n");

    u->distanza = h->arrayValori[0];
    u->indice = h->arrayNodi[0];
    
    h->arrayValori[0] = h->arrayValori[h->occupati-1];
    h->arrayNodi[0] = h->arrayNodi[h->occupati-1];

    h->occupati--;
    MinHeapify(h,0);
}

unsigned int Dijkstra(int d, unsigned int grafo[d][d], Heap *Q,NodoGrafo *u){ //esegue l'algoritmo di Dijkstra (per pseudocodice vedere slide) 
    Q->occupati=0;
    int i;
    for (i = 0; i < Q->capacita; i++)
    {
        Q->arrayValori[i] = INT_MAX;
        Q->arrayNodi[i] = -1;
    }
    u->distanza = 0;
    u->indice = 0;
    unsigned int Visite[d];
    Visite[0] = 0;
    int ndis = 0, indiceAgg = 0;
    int corrispondenze[d];
    for (i = 0; i < d; i++)
    {
        if(i!=0){
            Visite[i] = INT_MAX;
        }
        if(Q->occupati < Q->capacita){
            Q->arrayNodi[Q->occupati] = i;
            Q->arrayValori[Q->occupati] = Visite[i];
            Q->occupati++;
        }   
    }
    while(Q->occupati!=0){
        CancellaMin(Q,u);
        for (int i = 0; i < d; i++)
        {
            corrispondenze[i] = -1;
        }
        for (int i = Q->occupati-1; i >= 0; i--)
        {
            corrispondenze[Q->arrayNodi[i]] = i;
        }
        Visite[u->indice] = u->distanza;
        for (i = 0; i < d; i++)
        {
            if(grafo[u->indice][i]!=0){
                ndis = u->distanza + grafo[u->indice][i];
                indiceAgg = corrispondenze[i];
                if(indiceAgg !=-1){
                    if (ndis < Q->arrayValori[indiceAgg]) {
                        Q->arrayValori[indiceAgg] = ndis;
                    }
                }
            }
        }
        CostruisciMinHeap(Q); 
    }

    unsigned int punteggio = 0;
    for (i = 0; i < d; i++)
    {
        if(Visite[i] != INT_MAX) punteggio+=Visite[i];
    }
    return punteggio;
}

unsigned int AggiungiGrafo(int d,unsigned int matrice [d][d], Heap *Q,NodoGrafo *u){ //memorizza la matrice con la correzione del valore ascii e invoca Dijkstra
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            matrice[i][j] = 0;
        }
    }
    unsigned int colonna=0;
    unsigned int caratteriLetti;
    char riga[BUFFER];
    for (int i = 0; i < d; i++)
    {
        if(fgets(riga, BUFFER, stdin) == NULL) printf("Problema 1\n");
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
    return Dijkstra(d,matrice,Q,u);    
}