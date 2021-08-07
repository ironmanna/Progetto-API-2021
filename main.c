#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    int *arrayCorrispondenze;
};
typedef struct Heap Heap;

int compare(char *str1, char *str2);

Heap *CreaHeap(int capacita);
void inserisci(Heap *h, int valore, int *distanza, int indice, int peso);
void heapifyBassoAlto(Heap *h,int indice);
void heapifyAltoBasso(Heap *h, int indicePadre);
int PopMin(Heap *h,int *indice);
int Dijkstra(int d, int matrice[d][d]);
int AggiungiGrafo(FILE *fin, int d);
Lista accodamento (Lista best,  int punteggio, int numeroOrdine);
void TopK(Lista best, int k);
void libera(Lista best);

int main() {
    FILE *fin;
    Lista classifica = NULL;
    fin = fopen("input_4","r");
    int d = 0,k = 0, caratteriLetti = 0, numeroOrdine = 0, scelta = 0;
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

Heap *creaHeap(int capacita){
    Heap *h = (Heap * ) malloc(sizeof(Heap));

    if(h == NULL){
        //printf("Errore nella creazione dell'Heap");
        return NULL;
    }
    h->occupati=0;
    h->capacita = capacita;
    h->arrayValori = (int *) malloc(capacita*sizeof(int));
    h->arrayCorrispondenze = (int *) malloc(capacita*sizeof(int));

    if ( h->arrayValori == NULL || h->arrayCorrispondenze == NULL){
        //printf("Errore nella creazione degli array");
        return NULL;
    }
    return h;
}

void inserisci(Heap *h, int valore, int *distanza,int indice, int peso){
    if( h->occupati < h->capacita){
        h->arrayValori[h->occupati] = valore + peso;
        h->arrayCorrispondenze[h->occupati] = indice;
        heapifyBassoAlto(h, h->occupati);
        h->occupati++;
    }
}

void heapifyBassoAlto(Heap *h,int indice){
    int temp;
    int indicePadre = (indice-1)/2;

    if(h->arrayValori[indicePadre] > h->arrayValori[indice]){
        temp = h->arrayValori[indicePadre];
        h->arrayValori[indicePadre] = h->arrayValori[indice];
        h->arrayValori[indice] = temp;
        temp = h->arrayCorrispondenze[indicePadre];
        h->arrayCorrispondenze[indicePadre] = h->arrayCorrispondenze[indice];
        h->arrayCorrispondenze[indice] = temp;
        heapifyBassoAlto(h,indicePadre);
    }
}

void heapifyAltoBasso(Heap *h, int indicePadre){
    int sinistro = indicePadre*2+1;
    int destro = indicePadre*2+2;
    int min;
    int temp;

    if(sinistro >= h->occupati || sinistro <0)
        sinistro = -1;
    if(destro >= h->occupati || destro <0)
        destro = -1;

    if(sinistro != -1 && h->arrayValori[sinistro] < h->arrayValori[indicePadre])
        min = sinistro;
    else
        min = indicePadre;
    if(destro != -1 && h->arrayValori[destro] < h->arrayValori[min])
        min = destro;

    if(min != indicePadre){
        temp = h->arrayValori[min];
        h->arrayValori[min] = h->arrayValori[indicePadre];
        h->arrayValori[indicePadre] = temp;
        temp = h->arrayCorrispondenze[min];
        h->arrayCorrispondenze[min] = h->arrayCorrispondenze[indicePadre];
        h->arrayCorrispondenze[indicePadre] = temp;

        heapifyAltoBasso(h, min);
    }
}

int PopMin(Heap *h, int *indice){
    int pop;
    if(h->occupati==0){
        //printf("Heap vuoto\n");
        return -1;
    }
    pop = h->arrayValori[0];
    h->arrayValori[0] = h->arrayValori[h->occupati-1];
    *indice = h->arrayCorrispondenze[0];
    h->arrayCorrispondenze[0] = h->arrayCorrispondenze[h->occupati-1];
    h->occupati--;
    heapifyAltoBasso(h, 0);
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
    int capacita = d * (d-1);
    int *distanza = malloc(d*sizeof(int));
    for (int i = 0; i < d; i++)
    {
        distanza[i]=-1;
    }
    distanza[0]=0;

    Heap *heap = creaHeap(capacita);
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
            if(indice!=j && matrice[indice][j]!=0){
                if(distanza[j]==-1 ) inserisci(heap, matrice[indice][j],distanza, j, peso);
            } 
        }
        peso = PopMin(heap,&indice);
        //printf("Pop di valore: %d e indice: %d\n",peso,indice);
        if(peso!=-1) if(distanza[indice] > peso || distanza[indice]==-1) distanza[indice] = peso;
    }

    int somma = 0;
    for (int i = 0; i < d; i++)
    {
        if(distanza[i]!=-1) somma+=distanza[i];
    }
    free(heap->arrayCorrispondenze);
    free(heap->arrayValori);
    free(heap);
    free(distanza);
    return somma;
}