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
*/

// AggiungiGrafo(FILE *fin)
 /*crea un array di interi con d-1 celle dove memorizza il valore del cammino minimo a quel nodo, la funzione
 ritorna la somma dei cammini minimi */
// TopK()
 //Stampa i valori minimi dalla lista


/*
 L'idea è di usare l'algoritmo di Dijkstra con una struttura a Heap di Fibonacci per ottenere le prestazioni 
 temporali e spaziali migliori
*/

struct Nodo{
    int valore;
    struct Nodo *next;
};

typedef struct Nodo Elem;
typedef Elem *Lista;

Lista inserisci_ordine_ricorsivo (Lista l, int valore){
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
        p->next = NULL;
        return p;
    }

    l->next = inserisci_ordine_ricorsivo(l->next,valore);
    return l;
}

int AggiungiGrafo(FILE *fin);

void TopK (Lista l, int k);

int main() {
    int d, k, punteggio;
    FILE *fin ;
    Lista valori_finali;
    fin = fopen ("input.txt", "r");
    if (fin == NULL) {
        printf("La lettura non é andata a buon fine"); 
        exit(1);
    }
    fscanf(fin, "%d", &d);
    fscanf(fin, "%d", &k);
    while (fgetc(fin)==65){
        punteggio = AggiungiGrafo(fin);
        inserisci_ordine_ricorsivo(valori_finali, punteggio);
    }
    TopK(valori_finali, k);
    fclose(fin);
    return 0;
}

void TopK (Lista l, int k){
    for (int i = 0; i<k; i++){
        printf("%d ", l->valore);
        l = l->next;
    }
}