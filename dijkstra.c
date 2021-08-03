#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

int main(){
    int d = 3;
    int Grafo[d][d];
    srand(time(NULL));
    /*for(int i=0; i<d; i++){
        for(int j=0; j<d; j++){
            Grafo[i][j] = rand()%100;
        }
    }
    for(int i=0; i<d; i++){
        for(int j=0; j<d; j++){
            printf("%d ",Grafo[i][j]);
        }
        printf("\n");
    }*/
    Grafo[0][0] = 3;
    Grafo[0][1] = 1;
    Grafo[0][2] = 8;
    Grafo[1][0] = 0;
    Grafo[1][1] = 0;
    Grafo[1][2] = 5;
    Grafo[2][0] = 0;
    Grafo[2][1] = 9;
    Grafo[2][2] = 0;
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
            if(riga!=j && Grafo[riga][j]!=0){
                valore = Grafo[riga][j];
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
    printf("Punteggio: %d\n",punteggio);
    return 0;
}