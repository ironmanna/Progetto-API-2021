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

int compare(char *str1, char *str2);

extern size_t strlen (const char *__s)
     __THROW __attribute_pure__ __nonnull ((1));

#ifdef	__USE_XOPEN2K8
/* Find the length of STRING, but scan at most MAXLEN characters.
   If no '\0' terminator is found in that many characters, return MAXLEN.  */
extern size_t strnlen (const char *__string, size_t __maxlen)
     __THROW __attribute_pure__ __nonnull ((1));
#endif

int Dijkstra(int d, int grafo[d][d]);
Heap *CreateHeap(int capacita);
void HeapifyDalBasso(Heap *h,int indice);
void HeapifyDallAlto(Heap *h, int indice);
void Accoda(Heap *p,int indice, int distanza);
NodoGrafo *CancellaMin(Heap *h);
void PopMin(Heap *h);
int trovaIndice (Heap *h, int nodo);
int AggiungiGrafo(FILE *fin, int d);
Lista accodamento (Lista best,  int punteggio, int numeroOrdine);
void TopK(Lista best, int k);
void libera(Lista best);

int main() {
    FILE *fin;
    Lista classifica = NULL;
    fin = fopen("input_3","r");
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
    NodoGrafo *trash = malloc(sizeof(NodoGrafo));
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
        free(trash);
        trash = u;
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

    free(trash);
    free(Q->arrayNodi);
    free(Q->arrayValori);
    free(Q->arrayPrecedenti);
    free(Q);
    free(u);

    int punteggio = 0;
    for (int i = 0; i < d; i++)
    {
        if(Visite[0][i] != INT_MAX) punteggio+=Visite[0][i];
        //printf("Nodo %d, Distanza %d\n",i,Visite[0][i]);
    }
    return punteggio;
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