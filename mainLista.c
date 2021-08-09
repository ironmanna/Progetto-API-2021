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

void Dijkstra(int d, long int matrice[d][d]){
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

void AggiungiGrafo(FILE *fin, int d){
    long int matrice [d][d];
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            matrice[i][j] = 0;
        }
    }
    int colonna=0;
    printf("Chiamata a AggiungiGrafo\n");
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
               else{
                   matrice[i][colonna] = matrice[i][colonna]*10 + riga[j]-48;
               }
               printf("%ld ", matrice[i][colonna]);
           }
           if (riga[j]==44 || riga[j]==0) {
               colonna++;
               printf("\n");
           }
        }
        colonna = 0;
    }
    Dijkstra(d,matrice);    
}

void TopK(){
    printf("Sono TopK\n");
}

int main(){
    FILE *fin;
    fin = fopen("input_1","r");
    int d = 0,k = 0, caratteriLetti = 0, scelta = 0;
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
    printf("%d %d\n",d,k);
    while(fgets(lettura, BUFFER, fin) != NULL) 
    {
        caratteriLetti = strlen(lettura);
        if(lettura[caratteriLetti-1] == '\n') lettura[caratteriLetti-1]='\0';
        if (!strcmp(lettura,"AggiungiGrafo")) AggiungiGrafo(fin,d);
        if (!strcmp(lettura,"TopK")) TopK();

    } 
    fclose (fin);
    return 0;
}