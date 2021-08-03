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
    int valore;
    struct Nodo *next;
};

typedef struct Nodo Elem;
typedef Elem *Lista;

struct NodoGrafo{
    int inizio;
    int fine;
    int costo;
    struct NodoGrafo *next;
};

typedef struct NodoGrafo Pezzo;
typedef Pezzo *ListaPrioritaria;

ListaPrioritaria inserimento(ListaPrioritaria Queue, int riga, int j, int valore, int peso){
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
        printf("Inizio: %d, Fine: %d, Costo: %d\n",Queue->inizio, Queue->fine, Queue->costo);
        Trash = Queue;
        Queue = Queue->next;
        free(Trash);
    }
}

ListaPrioritaria successivo(ListaPrioritaria Queue,int d, int Tabella[d][d], int *riga){
    ListaPrioritaria Trash = NULL;
    while(Queue!=NULL){
        Trash = Queue;
        if(Tabella[Queue->fine][0]==(d+1)){
            Tabella[Queue->fine][0] = Queue->fine;
            Tabella[Queue->fine][1] = Queue->inizio;
            Tabella[Queue->fine][2] = Queue->costo;
            *riga = Queue->fine;
            Queue = Queue->next;
            printf("Liberata da 1 -> Inizio: %d, Fine: %d, Costo: %d\n",Trash->inizio, Trash->fine, Trash->costo);
            free(Trash);
            return Queue;
        }
        Queue = Queue->next;
        printf("Liberata da 2 -> Inizio: %d, Fine: %d, Costo: %d\n",Trash->inizio, Trash->fine, Trash->costo);
        free(Trash);
    }
    return Queue;
}

int Dijkstra(int d, int matrice[d][d]){
    int riga = 0,valore = 0, peso = 0, punteggio = 0;
    ListaPrioritaria Queue = NULL;
    int TabellaVisite[d][3];
    TabellaVisite[0][0] = 0;
    TabellaVisite[0][1] = 0;
    TabellaVisite[0][2] = 0;
    for (int i = 1; i < d; i++)
    {
        TabellaVisite[i][0] = (d+1);
    }
    for(int i=0; i<d; i++){
        for(int j=0; j<d; j++){
            if(riga!=j && matrice[riga][j]!=0){
                valore = matrice[riga][j];
                printf("%d ",valore);
                Queue = inserimento (Queue, riga, j, valore, peso);
            }
        }
        printf("\n");
        if(Queue!=NULL) Queue = successivo(Queue,d,TabellaVisite,&riga);
        else break;
        peso = TabellaVisite[riga][2];
    }
    printf("\n");
    for (int i = 0; i < d; i++)
    {
        punteggio += TabellaVisite [i][2];
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

Lista accodamento (Lista best, int punteggio){
    Lista p;
    if(best==NULL){
        best = malloc(sizeof(struct Nodo));
        best->valore = punteggio;
        best->next = NULL;
        return best;
    }
    if (punteggio < best->valore) {
        p = malloc (sizeof(struct Nodo));
        p->valore = punteggio;
        p->next = best;
        return p;
    }    
    best->next = accodamento(best->next,punteggio);
    return best;
}

void TopK(Lista best, int k){
    Lista Trash = NULL;
    while(best!=NULL || k!=0){
        printf("%d ",best->valore);
        k--;
        Trash = best;
        best = best->next;
        free(Trash);
    }
}

int main() {
    FILE *fin;
    Lista classifica = NULL;
    fin = fopen("input.txt","r");
    int d,k, caratteriLetti;
    char lettura[BUFFER];
    if (fscanf(fin, "%d ,", &d) == 0) printf("Errore nella prima lettura\n");
    if (fscanf(fin, "%d\n", &k) == 0) printf("Errore nella seconda lettura\n");
    while(fgets(lettura, BUFFER, fin) != NULL) 
    {
        caratteriLetti = strlen(lettura);
        if(lettura[caratteriLetti-1] == '\n') lettura[caratteriLetti-1]='\0';
        if (!strcmp(lettura,"AggiungiGrafo")) classifica = accodamento(classifica, AggiungiGrafo(fin,d));
        if (!strcmp(lettura,"TopK")) TopK(classifica, k);

    } 
    fclose (fin);
    return 0;
}