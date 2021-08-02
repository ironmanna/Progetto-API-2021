#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFER 10000

void AggiungiGrafo(FILE *fin, int d){
    //int matrice [d][d];
    int cont=0,caratteriLetti;
    char riga[BUFFER];
    for (int i = 0; i < d; i++)
    {
        if(fgets(riga, BUFFER, fin) == NULL) printf("Problema 1\n");
        caratteriLetti = strlen(riga);
        if(riga[caratteriLetti-1] == '\n') riga[caratteriLetti-1]='\0';
        printf("%s\n",riga);
        for(int j=0;j<d;j++){
            //matrice[i][j] = riga[cont];
            cont++;
            if(riga[cont]==',') cont++;
            if(riga[cont]=='\0') cont++;
        }
    }
    /*for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            printf("%d ",matrice[i][j]);
        }
        printf("\n");
    }*/
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
