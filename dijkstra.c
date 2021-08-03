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

ListaPrioritaria inserimento(ListaPrioritaria Queue, int riga, int j, int valore){
    ListaPrioritaria p;
    if(Queue==NULL){
        Queue = malloc(sizeof(struct NodoGrafo));
        Queue->inizio = riga;
        Queue->fine = j;
        Queue->costo = valore;
        Queue->next = NULL;
        return Queue;
    }
    if (valore < Queue->costo) {
        p = malloc (sizeof(struct NodoGrafo));
        p->costo = valore;
        p->inizio = riga;
        p->fine = j;
        p->next = Queue;
        return p;
    }    
    Queue->next = inserimento(Queue->next,riga, j, valore);
    return Queue;
}

void stampaLista(ListaPrioritaria Queue){
    while (Queue!=NULL){
        printf("Inizio: %d, Fine: %d, Costo: %d\n",Queue->inizio, Queue->fine, Queue->costo);
        Queue = Queue->next;
    }
}

int main(){
    int Grafo[3][3];
    srand(time(NULL));
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            Grafo[i][j] = rand()%100;
        }
    }
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            printf("%d ",Grafo[i][j]);
        }
        printf("\n");
    }
    int riga = 0,valore = 0;
    ListaPrioritaria Queue;
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            if(riga!=j){
                valore = Grafo[riga][j];
                printf("%d ",valore);
                Queue = inserimento (Queue, riga, j, valore);
            }
        }
        riga++;
    }
    printf("\n");
    stampaLista(Queue);
    return 0;
}