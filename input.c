#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFER 10000

void Dijkstra(int d, int matrice[d][d]){
    printf("Chiamata a Dijkstra\n");
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            printf("%d ",matrice[i][j]);
        }
        printf("\n");
    }
}

void AggiungiGrafo(FILE *fin, int d){
    int matrice [d][d];
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
               else matrice[i][colonna] = matrice[i][colonna]*10 + riga[j]-48;
           }
           if (riga[j]==44) colonna++;
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
    fin = fopen("input.txt","r");
    int d,k, caratteriLetti;
    char lettura[BUFFER];
    if (fscanf(fin, "%d ,", &d) == 0) printf("Errore nella prima lettura\n");
    if (fscanf(fin, "%d\n", &k) == 0) printf("Errore nella seconda lettura\n");
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
