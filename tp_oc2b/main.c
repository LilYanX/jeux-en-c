#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "dico.h"

char lireCaractere()
{
    char caractere = 0;

    caractere = getchar(); // On lit le premier caractère ; // On met la lettre en majuscule si elle ne l'est pas déjà

    caractere=toupper(caractere); // On met le caractère en majuscule pour que le joueur puisse écrire sa lettre et en majuscule et en minuscule
    // On lit les autres caractères mémorisés un à un jusqu'au \n (pour les effacer)
    while (getchar() != '\n') ;

    return caractere; // On retourne le premier caractère qu'on a lu
}
int piocherMot(char *motPioche)
{
    FILE* dico = NULL;
    int nombreMots = 0, numMotChoisi = 0;
    int caractereLu = 0;
    dico=fopen("dico.txt","r");
    if(dico==NULL) {
        printf("\nL'ouverture n'a pas marché...");
        return 0;
    }
    do {
        caractereLu=fgetc(dico);
        if(caractereLu=='\n') {
            nombreMots++;
        }
    } while(caractereLu != EOF);
    numMotChoisi=nombreAleatoire(nombreMots);

    rewind(dico);
    while(numMotChoisi!=0) {
        caractereLu=fgetc(dico);
        if(caractereLu=='\n') {
            numMotChoisi--;
        }
    }
    fgets(motPioche,100,dico);
    motPioche[strlen(motPioche) - 1] = '\0';
    fclose(dico);
    return 1; // Tout s'est bien passé, on retourne 1
}

int nombreAleatoire(int nombreMax)
{
    srand(time(NULL));
    return (rand() % nombreMax);
}

int main()
{
    int i,c=0,t=0;
    char maLettre;
    char motSecret[100]={0};
    if (!piocherMot(motSecret))
    exit(0);
    int *lettreTrouvee=NULL;
    int tailleMot=strlen(motSecret),cpt=10;
    lettreTrouvee = malloc(tailleMot*sizeof(int));
    if(lettreTrouvee==NULL) {
        exit(0);
    }
    for(i=0;i<tailleMot;i++) {
        lettreTrouvee[i]=0;
    }
    printf("Bienvenue dans le jeu du Pendu !\n");
    while(cpt>0) {
        printf("Il vous reste %d coups a jouer\nQuel est le mot secret ?",cpt);
        for(i=0;i<tailleMot;i++) {
            if(lettreTrouvee[i]==1) {
                printf("%c",motSecret[i]);
            }
            else {
                printf("*");
            }
        }
        printf("\n");
        maLettre=lireCaractere();
        for(i=0;i<tailleMot;i++) {
            if(maLettre==motSecret[i]) {
                c++;
                t++;
                lettreTrouvee[i]=1;
            }
        }
        if(c==0) {
            cpt--;
        }
        c=0;
        if(t==tailleMot) {
            printf("Gagne ! Le mot secret etait bien : %s",motSecret);
            exit(0);
        }
        if(cpt==0) {
            printf("Perdu ! Le mot secret etait : %s",motSecret);
            exit(0);
        }
    }
    return 0;
}

