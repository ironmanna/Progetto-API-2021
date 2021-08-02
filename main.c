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

 L'idea è di usare l'algoritmo di Dijkstra con una struttura a Heap di Fibonacci per ottenere le prestazioni 
 temporali e spaziali migliori
*/

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

Lista inserisci_ordine_ricorsivo1 (Lista l, int valore);

ListaPrioritaria inserisci_ordine_ricorsivo2 (ListaPrioritaria l, int costo, int inizio, int peso, int fine);

int AggiungiGrafo(FILE *fin, int d);

void TopK (Lista l, int k);

int SceltaSuccessivo(ListaPrioritaria Queue, int d, int TabellaVisite[3][d]);

int main() {
    int d, k, punteggio, count=0;
    char lettura[16];
    FILE *fin;
    Lista valori_finali = NULL;
    fin = fopen ("input.txt", "r");
    if (fin == NULL) {
        printf("La lettura non é andata a buon fine"); 
        exit(1);
    }
    if (fscanf(fin, "%d ,", &d) == 0) printf("Errore nella prima lettura\n");
    if (fscanf(fin, "%d\n", &k) == 0) printf("Errore nella seconda lettura\n");
    while(!feof(fin)){
        printf("%d\n",count);
        if (fgets(lettura,16,fin) == NULL) printf("Errore nella fgets %s\n",lettura);
        if (strcmp(lettura,"AggiungiGrafo\n")==0){ //Che cazzo ho scritto? -> Segmentation Fault 11 qui
            printf("Sono qui 2\n"); 
            punteggio = AggiungiGrafo(fin, d);
            printf("Sono qui 3\n");
            inserisci_ordine_ricorsivo1(valori_finali, punteggio);
        }
        if (strcmp(lettura,"TopK")==0) TopK(valori_finali, k );
        count++;
    }
    fclose(fin);
    return 0;
}

void TopK (Lista l, int k){
    for (int i = 0; i<k; i++){
        printf("%d ", l->valore);
        l = l->next;
    }
}

Lista inserisci_ordine_ricorsivo1 (Lista l, int valore){
    Lista p;
    if (l == NULL){
        p = malloc (sizeof(struct Nodo));
        p->valore = valore;
        p->next = NULL;
        return p;
    }
    if (valore < l->valore) {
        p = malloc (sizeof(struct Nodo));
        p->valore = valore;
        p->next = l;
        return p;
    }    
    l->next = inserisci_ordine_ricorsivo1(l->next,valore);
    return l;
}

ListaPrioritaria inserisci_ordine_ricorsivo2 (ListaPrioritaria l, int costo, int inizio, int peso, int fine){
    ListaPrioritaria p;
    if (l == NULL){
        p = malloc (sizeof(struct NodoGrafo));
        p->costo = costo + peso;
        p->inizio = inizio;
        p->fine = fine;
        p->next = NULL;
        return p;
    }
    if ((costo + peso) < l->costo) {
        p = malloc (sizeof(struct NodoGrafo));
        p->costo = costo + peso;
        p->inizio = inizio;
        p->fine = fine;
        p->next = l;
        return p;
    }

    l->next = inserisci_ordine_ricorsivo2(l->next,costo,inizio,peso,fine);
    return l;
}

int SceltaSuccessivo(ListaPrioritaria Queue,int d, int TabellaVisite[3][d]){
    if(Queue == NULL){
        return 0;
    }
    TabellaVisite[0][Queue->fine] = Queue->fine;
    TabellaVisite[1][Queue->fine] = Queue->inizio;
    TabellaVisite[2][Queue->fine] = Queue->costo;
    int selezione = Queue->fine;
    ListaPrioritaria Trash = Queue;
    Queue = Queue->next;
    free(Trash);
    return selezione;
}


int AggiungiGrafo(FILE *fin, int d) {
    int matrice [d][d];
    for (int i = 0; i<d; i++){
        if(fscanf(fin,"%d", &matrice[i][0])==0) printf("Errore nella lettura della prima cella\n");
        for (int j = 1; j<d; j++){
            if(fscanf(fin,", %d", &matrice[i][j])==0) printf("Errore nella lettura delle celle\n");; //attenzione alle virgole
        }
    }
    int TabellaVisite[3][d-1];
    TabellaVisite[0][0] = 0;
    TabellaVisite[1][0] = 0;
    TabellaVisite[2][0] = 0;
    ListaPrioritaria Queue = NULL;
    int selezione = 0;
    int peso = 0;
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j<d; j++){
            if (matrice[selezione][j] != 0 && selezione != j){
                Queue = inserisci_ordine_ricorsivo2(Queue,matrice[selezione][j],selezione,peso,j);
            }
        }
        selezione = SceltaSuccessivo(Queue,d,TabellaVisite);
        if (i>0 && selezione == 0) break;
    }
    int somma = 0;
    for(int i = 0; i<d; i++){
        somma+=TabellaVisite[2][i];
    }
    return somma;
}