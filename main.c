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

 L'idea è di usare l'algoritmo di Dijkstra con una lista per organizzare le visite successive 
*/

#define BUFFER 10000

struct Nodo{
    int numeroOrdine;
    long int valore;
    struct Nodo *next;
};

typedef struct Nodo Elem;
typedef Elem *Lista;

struct NodoGrafo{
    int inizio;
    int fine;
    long int costo;
    struct NodoGrafo *next;
};

typedef struct NodoGrafo Pezzo;
typedef Pezzo *ListaPrioritaria;

ListaPrioritaria inserimento(ListaPrioritaria Queue, int riga, int j, long int valore, long int peso){
    ListaPrioritaria p;
    if(Queue==NULL){
        Queue = malloc(sizeof(struct NodoGrafo));
        Queue->inizio = riga;
        Queue->fine = j;
        Queue->costo = valore + peso;
        Queue->next = NULL;
        return Queue;
    }
    if (valore + peso < Queue->costo) {
        p = malloc (sizeof(struct NodoGrafo));
        p->inizio = riga;
        p->fine = j;
        p->costo = valore + peso;
        p->next = Queue;
        return p;
    }    
    Queue->next = inserimento(Queue->next,riga, j, valore, peso);
    return Queue;
}

void stampaLista(ListaPrioritaria Queue){
    ListaPrioritaria Trash;
    while (Queue!=NULL){
        //printf("Inizio: %d, Fine: %d, Costo: %ld\n",Queue->inizio, Queue->fine, Queue->costo);
        Trash = Queue;
        Queue = Queue->next;
        free(Trash);
    }
}

ListaPrioritaria successivo(ListaPrioritaria Queue,int d, long int Tabella[d][d], int *riga){
    ListaPrioritaria Trash = NULL;
    while(Queue!=NULL){
        Trash = Queue;
        if(Tabella[Queue->fine][0]==(d+1)){
            Tabella[Queue->fine][0] = Queue->fine;
            Tabella[Queue->fine][1] = Queue->inizio;
            Tabella[Queue->fine][2] = Queue->costo;
            *riga = Queue->fine;
            Queue = Queue->next;
            //printf("Liberata da 1 -> Inizio: %d, Fine: %d, Costo: %ld\n",Trash->inizio, Trash->fine, Trash->costo);
            free(Trash);
            return Queue;
        }
        Queue = Queue->next;
        //printf("Liberata da 2 -> Inizio: %d, Fine: %d, Costo: %ld\n",Trash->inizio, Trash->fine, Trash->costo);
        free(Trash);
    }
    return Queue;
}

long int Dijkstra(int d, long int matrice[d][d], long int TabellaVisite[3][d]){
    int riga = 0;
    long int peso = 0;
    long int punteggio = 0;
    long int valore = 0;
    ListaPrioritaria Queue = NULL;
    for (int i = 1; i < d; i++)
    {
        TabellaVisite[i][0] = (d+1);
    }
    for(int i=0; i<d; i++){
        for(int j=0; j<d; j++){
            if(riga!=j && matrice[riga][j]!=0){
                valore = matrice[riga][j];
                //printf("%ld ",valore);
                Queue = inserimento (Queue, riga, j, valore, peso);
            }
        }
        //printf("\n");
        if(Queue!=NULL) Queue = successivo(Queue,d,TabellaVisite,&riga);
        else break;
        peso = TabellaVisite[riga][2];
    }
    //printf("\n");
    for (int i = 0; i < d; i++)
    {
        punteggio += TabellaVisite [i][2];
    }
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            //printf("%ld ",TabellaVisite[i][j]);
        }
        //printf("\n");
    }
    return punteggio;
}

long int AggiungiGrafo(FILE *fin, int d){
    long int matrice [d][d];
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            matrice[i][j] = 0;
        }
    }
    long int TabellaVisite[d][3];
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            TabellaVisite[i][j] = 0;
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
    return Dijkstra(d,matrice,TabellaVisite);    
}

Lista accodamento (Lista best, long int punteggio, int numeroOrdine){
    Lista p;
    if (punteggio==0) return best;
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

int main() {
    FILE *fin;
    Lista classifica = NULL;
    fin = fopen("input_1","r");
    int d,k, caratteriLetti, numeroOrdine = 0;
    char lettura[BUFFER];
    if (fscanf(fin, "%d ,", &d) == 0) printf("Errore nella prima lettura\n");
    if (fscanf(fin, "%d\n", &k) == 0) printf("Errore nella seconda lettura\n");
    while(fgets(lettura, BUFFER, fin) != NULL) 
    {
        caratteriLetti = strlen(lettura);
        if(lettura[caratteriLetti-1] == '\n') lettura[caratteriLetti-1]='\0';
        if (!strcmp(lettura,"AggiungiGrafo")){
            numeroOrdine++;
            classifica = accodamento(classifica, AggiungiGrafo(fin,d),numeroOrdine);
            //printf("Fine AggiungiGrafo\n");
        } 
        if (!strcmp(lettura,"TopK")) TopK(classifica, k);

    } 
    fclose (fin);
    return 0;
}