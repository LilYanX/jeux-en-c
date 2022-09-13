#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int nombreMystere()       // génération d'un nombre mystere
{
    int nombreMystere;
    const int MAX = 6, MIN = 1;
    srand(time(NULL));
    nombreMystere = (rand() % (MAX - MIN + 1)) + MIN;
    return nombreMystere;
}
int nombreMystere2()
{
    int nombreMystere;
    const int MAX = 699, MIN = 100;
    srand(time(NULL));
    nombreMystere = (rand() % (MAX - MIN + 1)) + MIN;
    return nombreMystere/100;
}

void regles() {
    clear();
    printf("----------------------------Les Regles----------------------------\n");
    printf("Le baggamon est un jeu de plateaux en 1 contre 1.\nIci le but du joueur 1(B) est de mettre tous ses pions sur la case 0\net pour le joueur 2(R) son but sera de mettre tous ses pions dans la case 25\n");
    printf("Chaque joueur a 15 pions pre-pose avant le lancement de la partie\n");
    printf("A chaque tour le joueur lancera 2 des, ces des seront le moyen de deplacements des pions\n");
    printf("Un double avec les des permettra au joueur d'effectuer 2 fois les 2 des\n");
    printf("Les joueurs peuvent manger un pion lorsque par exemple un pion(B) tombe sur une case ou il n'y a qu'un SEUL pion(R)\n");
    printf("Ce pion se retrouvera en prison et lors du prochain tour,le joueur 2(R) est oblige de joueur le pion en prison\n");
}

int interdiction(int tour, char team[], int choix, int de1, int de2, int colonne[]) {        // on empêche l'utilisateur de selectionner un pion qu'il ne pourra pas déplacer
    if(tour%2==0) {
        if((team[choix-1]!='1') || (((team[choix-1-de1]=='2') && (colonne[choix-de1]>1)) && ((team[choix-de2-1]=='2') && (colonne[choix-de2]>1))) || ((choix-de1<0) && (choix-de2<0))) {
            printf("Choix non valide...\n");
            return 1;
        }
    }
    else {
        if((team[choix-1]!='2') || (((team[choix-1+de1]=='1') && (colonne[choix+de1]>1)) && ((team[choix+de2-1]=='1') && (colonne[choix+de2]>1))) || ((choix+de1>25) && (choix+de2>25))) {
            printf("Choix non valide...\n");
            return 1;
        }
    }
    return 0;
}

int interdiction2(int tour, char team[], int choix, int move, int de1, int de2, int colonne[]) {        // on empêche l'utilisateur de placer un pion à un endroit impossible
    if(tour%2==0) {
        if(((move-choix != de1) && (move-choix != de2)) || ((((team[choix-1-de1]=='2') && (colonne[move-de1-1]>1)) && ((team[choix-de2-1]=='2') && (colonne[move-de2-1]>1))) || (choix<0))) {
            printf("Choix non valide...\n");
            return 1;
        }
    }
    else {
        if(((choix-move != de1) && (choix-move != de2)) || ((((team[choix+de1-1]=='1') && (colonne[choix+de1-1]>1)) && ((team[choix+de2-1]=='1') && (colonne[choix+de2-1]>1))) || (choix>25))) {
            printf("Choix non valide...\n");
            return 1;
        }
    }
    return 0;
}

int passetour(int tour, char team[], int de1, int de2, int colonne[]) {
    int t=0,i;
    if(tour%2==0) {
        for(i=24;i>0;i--) {
            if((team[i-1]!='1') || (((team[i-1-de1]=='2') && (colonne[i-de1]>1)) && ((team[i-de2-1]=='2') && (colonne[i-de2]>1))))
                t++;
        }
    }
    else {
        for(i=1;i<25;i++) {
            if((team[i-1]!='2') || (((team[i-1+de1]=='1') && (colonne[i+de1]>1)) || ((team[i+de2-1]=='1') && (colonne[i+de2]>1))))
                t++;
        }
    }
    return t;
}

void verif(int colonne[], char team[], int choix) {       // on réinitialise les equipes pour les emplacements impactés durant le tour
    if(colonne[choix]==0) {
        team[choix-1]='N';
    }
}

void verif2(char team[], int choix, int tour) {        // on initialise les nouvelles équipes des emplacements impactés
    if(team[choix-1]=='N') {
        if(tour%2==0) {
            team[choix-1]='1';
        }
        else {
            team[choix-1]='2';
        }
    }
}

int dereste (int colonne, int choix, int de, int de2, int tour) {       // on recherche le deuxième dé non utilisé
    if(tour%2==0) {
        if(choix-colonne==de) {
            return de2;
        }
        else {
            return de;
        }
    }
    else {
        if(colonne-choix==de) {
            return de2;
        }
        else {
            return de;
        }
    }
}

int tourjoueur(int tour) {           // retourne le numéro du joueur actuel
    return tour%2+1;
}

void manger(int colonne[], char team[], int choix, int tour, int prison[]) {
    if((tour%2==0) && (team[choix-1]=='2')) {
        colonne[choix]--;
        team[choix-1]='N';
        prison[1]++;
    }
    if((tour%2!=0) && (team[choix-1]=='1')) {
        colonne[choix]--;
        team[choix-1]='N';
        prison[0]++;
    }
}

int prisonplay(int tour, char team[], int de1, int de2, int colonnes[], int prison[]) { // oblige le joueur a jouer son pion en prison durant ce tour
    int move, choixcolonne,i=1;
    printf("Aie... Vous avez un pion en prison\nVeuillez donc saisir la case ou vous souhaitez le deplacer : ");
    if(tourjoueur(tour)==1)
        move=25;
    else
        move=0;
    do{
        scanf("%d",&choixcolonne);
        i=interdiction2(tour,team,choixcolonne,move,de1,de2, colonnes);
    }while(i==1);   // Il est obligé de placer son jeton sur une case valide
    manger(colonnes,team,choixcolonne,tour,prison);
    colonnes[choixcolonne]++;
    prison[tourjoueur(tour)-1]--;
    verif2(team,choixcolonne,tour);
    return de1=dereste(choixcolonne,move,de1,de2,tour);
}

int jeu(int tour, char team[], int de1, int de2, int colonnes[], int prison[]) {    // réalise le tour d'un joueur et renvoie le dé restant
    int choixcolonne, i=1, move;
    do{
        scanf("%d",&choixcolonne);
        i=interdiction(tour,team,choixcolonne,de1,de2,colonnes);
    }while(i==1);   // Il est obligé de jouer un jeton valide
    colonnes[choixcolonne]--;
    verif(colonnes,team,choixcolonne);
    move=choixcolonne;
    printf("\nVeuillez saisir la colonne qui accueillera le jeton : ");
    do{
        scanf("%d",&choixcolonne);
        i=interdiction2(tour,team,choixcolonne,move,de1,de2, colonnes);
    }while(i==1);   // Il est obligé de placer son jeton sur une case valide
    manger(colonnes,team,choixcolonne,tour,prison);
    colonnes[choixcolonne]++;
    verif2(team,choixcolonne,tour);
    return de1=dereste(choixcolonne,move,de1,de2,tour);
}

void plateau(int tour, char team[], int colonnes[], int prison[])   // crée le plateau et indique le tour du joueur
{
    char a[24];
    int i;
    for(i=0;i<24;i++) {
        if(team[i]=='N'){
            a[i]=' ';
        }
        else{
            if(team[i]=='1')
                a[i]='B';
            else
                a[i]='R';
        }
    }
    if(tour<11)
        printf("\n                                                      %c%c%c%c%c%c%c%c%c%c%c%c\n                                                      %c  TOUR %i  %c\n                                                      %c%c%c%c%c%c%c%c%c%c%c%c\n\n",201,205,205,205,205,205,205,205,205,205,205,187,186,tour-1,186,200,205,205,205,205,205,205,205,205,205,205,188);
    else
        printf("\n                                                      %c%c%c%c%c%c%c%c%c%c%c%c%c\n                                                      %c  TOUR %i  %c\n                                                      %c%c%c%c%c%c%c%c%c%c%c%c%c\n\n",201,205,205,205,205,205,205,205,205,205,205,205,187,186,tour-1,186,200,205,205,205,205,205,205,205,205,205,205,205,188);

    printf("    13       14       15       16       17       18                 19       20       21       22       23       24\n");
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",201,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,203,205,205,205);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",203,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,187);
    printf("%c  %c %c %c    %c %c %c    %c %c %c    %c %c %c    %c %c %c    %c %c %c    %c   %c    %c %c %c    %c %c %c    %c %c %c    %c %c %c    %c %c %c    %c %c %c %c\n",186,179,a[12],179,179,a[13],179,179,a[14],179,179,a[15],179,179,a[16],179,179,a[17],179,179,179,179,a[18],179,179,a[19],179,179,a[20],179,179,a[21],179,179,a[22],179,179,a[23],179,186);
    printf("%c  %c %i %c    %c %i %c    %c %i %c    %c %i %c    %c %i %c    %c %i %c    %c   %c    %c %i %c    %c %i %c    %c %i %c    %c %i %c    %c %i %c    %c %i %c %c\n",186,179,colonnes[13],179,179,colonnes[14],179,179,colonnes[15],179,179,colonnes[16],179,179,colonnes[17],179,179,colonnes[18],179,179,179,179,colonnes[19],179,179,colonnes[20],179,179,colonnes[21],179,179,colonnes[22],179,179,colonnes[23],179,179,colonnes[24],179,186);
    printf("%c  %c   %c    %c   %c    %c   %c    %c   %c    %c   %c    %c   %c    %c   %c    %c   %c    %c   %c    %c   %c    %c   %c    %c   %c    %c   %c %c\n",186,92,47,92,47,92,47,92,47,92,47,92,47,179,179,92,47,92,47,92,47,92,47,92,47,92,47,186);
     printf("%c   %c%c%c      %c%c%c      %c%c%c      %c%c%c      %c%c%c      %c%c%c     %c   %c     %c%c%c      %c%c%c      %c%c%c      %c%c%c      %c%c%c      %c%c%c  %c\n",186,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,179,179,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,186);
    printf("%c                                                       %c  %i  %c                                                      %c\n",186,186,prison[0],186,186);
    printf("%c                                                       %c  B  %c                                                      %c\n",186,186,186,186);
    printf("%c                                                       %c  R  %c                                                      %c\n",186,186,186,186);
    printf("%c                                                       %c  %i  %c                                                      %c\n",186,186,prison[1],186,186);
    printf("%c   %c%c%c      %c%c%c      %c%c%c      %c%c%c      %c%c%c      %c%c%c     %c   %c     %c%c%c      %c%c%c      %c%c%c      %c%c%c      %c%c%c      %c%c%c  %c\n",186,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,179,179,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,186);
    printf("%c  %c   %c    %c   %c    %c   %c    %c   %c    %c   %c    %c   %c    %c   %c    %c   %c    %c   %c    %c   %c    %c   %c    %c   %c    %c   %c %c \n",186,47,92,47,92,47,92,47,92,47,92,47,92,179,179,47,92,47,92,47,92,47,92,47,92,47,92,186);
    printf("%c  %c %i %c    %c %i %c    %c %i %c    %c %i %c    %c %i %c    %c %i %c    %c   %c    %c %i %c    %c %i %c    %c %i %c    %c %i %c    %c %i %c    %c %i %c %c \n",186,179,colonnes[12],179,179,colonnes[11],179,179,colonnes[10],179,179,colonnes[9],179,179,colonnes[8],179,179,colonnes[7],179,179,179,179,colonnes[6],179,179,colonnes[5],179,179,colonnes[4],179,179,colonnes[3],179,179,colonnes[2],179,179,colonnes[1],179,186);
    printf("%c  %c %c %c    %c %c %c    %c %c %c    %c %c %c    %c %c %c    %c %c %c    %c   %c    %c %c %c    %c %c %c    %c %c %c    %c %c %c    %c %c %c    %c %c %c %c \n",186,179,a[11],179,179,a[10],179,179,a[9],179,179,a[8],179,179,a[7],179,179,a[6],179,179,179,179,a[5],179,179,a[4],179,179,a[3],179,179,a[2],179,179,a[1],179,179,a[0],179,186);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",200,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,202,205,205,205);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",202,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,188);
    printf("    12       11       10        9        8        7                 6        5        4        3        2        1\n\n");
    if(tour%2==0)
        printf(" %c Joueur 1 (B)\n\n   Joueur 2 (R)\n\n",26);
    else
        printf("   Joueur 1 (B)\n\n %c Joueur 2 (R)\n\n",26);
}

int victoire(int colonnes[]){   // permet l'arrêt du jeu lorsque la condition de victoire est remplie
    if((colonnes[0]==15) || (colonnes[25]==15))
        return 1;
    else
        return 0;
}

void clear(){
    printf("\n");
    system("pause");
    system("cls");
}

int main()
{
    int de1;
    int de2;
    int colonnes[26]={0};
    int win=0;
    int i=0;
    char team[24];
    int tour=2;
    int prison[2]={0};
    int menu;

    // On initialise le plateau

    for(i=0;i<24;i++){
        team[i]='N';
    }

    colonnes[24]+=2;
    team[23]='1';
    colonnes[13]+=5;
    team[12]='1';
    colonnes[8]+=3;
    team[7]='1';
    colonnes[6]+=5;
    team[5]='1';

    colonnes[1]+=2;
    team[0]='2';
    colonnes[12]+=5;
    team[11]='2';
    colonnes[17]+=3;
    team[16]='2';
    colonnes[19]+=5;
    team[18]='2';

    printf("-------------------MENU-------------------\n");
    printf("Veuillez choisir ce que vous voulez faire :\n");
    printf("1. Jouer\n");
    printf("2. Regles\n");
    printf("3. Quitter\n");
    scanf("%d",&menu);
    switch(menu) {
        case 1 :
            clear();
            while(win==0) {
                plateau(tour,team,colonnes,prison);
                de1=nombreMystere();
                de2=nombreMystere2();
                if(de1!=de2){       // Si les 2 dés sont différents
                    printf("C'est au tour du joueur %d et vous avez un %d et un %d.\nVeuillez choisir la colonne a laquelle vous souhaitez deplacer un pion : ",tourjoueur(tour),de1,de2);
                    if(prison[tourjoueur(tour)-1]!=0) {
                        if(passetour(tour,team,de1,de2, colonnes)!=24) {
                            de1=prisonplay(tour,team,de1,de2,colonnes,prison);
                            clear();
                            if(prison[tourjoueur(tour)-1]!=0) {
                                plateau(tour,team,colonnes,prison);
                                de1=prisonplay(tour,team,de1,de2,colonnes,prison);
                                tour++;
                                win=victoire(colonnes);
                                clear();
                            }
                            else {
                                plateau(tour,team,colonnes,prison);
                                printf("Il vous reste un de de %d.\nVeuillez choisir la colonne a laquelle vous souhaitez deplacer un pion : \n",de1);
                                de1=jeu(tour,team,de1,de2,colonnes,prison);
                                tour++;
                                win=victoire(colonnes);
                                clear();
                            }
                        }
                        else {
                            printf("Vous ne pouvez pas jouer...\nVous passez donc votre tour\n");
                            tour++;
                            win=victoire(colonnes);
                            clear();
                        }
                        if(prison[tourjoueur(tour)-1]!=0) {
                            plateau(tour,team,colonnes,prison);
                            de1=prisonplay(tour,team,de1,de2,colonnes,prison);
                            win=victoire(colonnes);
                            tour++;
                            clear();
                        }
                    }
                    else {
                    if(passetour(tour,team,de1,de2, colonnes)!=24) {  // Si le joueur n'a aucune possibilité, il passe son tour
                        de1=jeu(tour,team,de1,de2,colonnes,prison);
                        clear();
                        printf("Il vous reste un de de %d.\nVeuillez choisir la colonne a laquelle vous souhaitez deplacer un pion : \n",de1);
                        plateau(tour,team,colonnes,prison);
                        if(passetour(tour,team,de1,de1,colonnes)!=24) {  // Si le joueur n'a aucune possibilité, il passe son tour
                            de1=jeu(tour,team,de1,de2,colonnes,prison);
                            tour++;
                            win=victoire(colonnes);
                            clear();
                        }
                        else{
                            printf("Vous ne pouvez pas jouer...\nVous passez donc votre tour\n");
                            tour++;
                            win=victoire(colonnes);
                            clear();
                        }
                    }
                    else{
                        printf("Vous ne pouvez pas jouer...\nVous passez donc votre tour\n");
                        tour++;
                        win=victoire(colonnes);
                        clear();
                    }
                }
                }
                else {          // Si le joueur a fait un double
                    printf("C'est au tour du joueur %d et vous avez fait un double %d.\nVeuillez choisir la colonne a laquelle vous souhaitez deplacer un pion : ",tourjoueur(tour),de1);
                    if(prison[tourjoueur(tour)-1]!=0) {
                        if(passetour(tour,team,de1,de2, colonnes)!=24) {
                            de1=prisonplay(tour,team,de1,de2,colonnes,prison);
                            win=victoire(colonnes);
                            clear();
                            if(prison[tourjoueur(tour)-1]!=0) {
                                printf("Il vous reste 3 de de %d.\nVeuillez choisir la colonne a laquelle vous souhaitez deplacer un pion : \n",de1);
                                plateau(tour,team,colonnes,prison);
                                de1=prisonplay(tour,team,de1,de2,colonnes,prison);
                                win=victoire(colonnes);
                                clear();
                                if(prison[tourjoueur(tour)-1]!=0) {
                                    printf("Il vous reste 2 de de %d.\nVeuillez choisir la colonne a laquelle vous souhaitez deplacer un pion : \n",de1);
                                    plateau(tour,team,colonnes,prison);
                                    de1=prisonplay(tour,team,de1,de2,colonnes,prison);
                                    win=victoire(colonnes);
                                    clear();
                                    if(prison[tourjoueur(tour)-1]!=0) {
                                        printf("Il vous reste un de de %d.\nVeuillez choisir la colonne a laquelle vous souhaitez deplacer un pion : \n",de1);
                                        plateau(tour,team,colonnes,prison);
                                        de1=prisonplay(tour,team,de1,de2,colonnes,prison);
                                        win=victoire(colonnes);
                                        clear();
                                    }
                                    else {
                                        printf("Il vous reste un de de %d.\nVeuillez choisir la colonne a laquelle vous souhaitez deplacer un pion : \n",de1);
                                        if(passetour(tour,team,de1,de2, colonnes)!=24) {  // Si le joueur n'a aucune possibilité, il passe son tour
                                            plateau(tour,team,colonnes,prison);
                                            de1=jeu(tour,team,de1,de2,colonnes,prison);
                                            tour++;
                                            win=victoire(colonnes);
                                            clear();
                                        }
                                        else{
                                        printf("Vous ne pouvez pas jouer...\nVous passez donc votre tour\n");
                                        tour++;
                                        win=victoire(colonnes);
                                        clear();
                                        }
                                    }
                                }
                                else {
                                    printf("Il vous reste 2 de de %d.\nVeuillez choisir la colonne a laquelle vous souhaitez deplacer un pion : \n",de2);
                                    if(passetour(tour,team,de1,de2, colonnes)!=24) {  // Si le joueur n'a aucune possibilité, il passe son tour
                                        plateau(tour,team,colonnes,prison);
                                        de1=jeu(tour,team,de1,de2,colonnes,prison);
                                        win=victoire(colonnes);
                                        clear();
                                        printf("Il vous reste un de de %d.\nVeuillez choisir la colonne a laquelle vous souhaitez deplacer un pion : \n",de1);
                                        if(passetour(tour,team,de1,de2, colonnes)!=24) {  // Si le joueur n'a aucune possibilité, il passe son tour
                                            plateau(tour,team,colonnes,prison);
                                            de1=jeu(tour,team,de1,de2,colonnes,prison);
                                            tour++;
                                            win=victoire(colonnes);
                                            clear();
                                        }
                                        else{
                                        printf("Vous ne pouvez pas jouer...\nVous passez donc votre tour\n");
                                        tour++;
                                        win=victoire(colonnes);
                                        clear();
                                        }
                                    }
                                    else{
                                        printf("Vous ne pouvez pas jouer...\nVous passez donc votre tour\n");
                                        tour++;
                                        win=victoire(colonnes);
                                        clear();
                                    }
                                }
                            }
                            else {
                                printf("Il vous reste 3 de de %d.\nVeuillez choisir la colonne a laquelle vous souhaitez deplacer un pion : \n",de1);
                                if(passetour(tour,team,de1,de2, colonnes)!=24) {  // Si le joueur n'a aucune possibilité, il passe son tour
                                    plateau(tour,team,colonnes,prison);
                                    de1=jeu(tour,team,de1,de2,colonnes,prison);
                                    win=victoire(colonnes);
                                    clear();
                                    printf("Il vous reste 2 de de %d.\nVeuillez choisir la colonne a laquelle vous souhaitez deplacer un pion : \n",de2);
                                    if(passetour(tour,team,de1,de2, colonnes)!=24) {  // Si le joueur n'a aucune possibilité, il passe son tour
                                        plateau(tour,team,colonnes,prison);
                                        de1=jeu(tour,team,de1,de2,colonnes,prison);
                                        win=victoire(colonnes);
                                        clear();
                                        printf("Il vous reste un de de %d.\nVeuillez choisir la colonne a laquelle vous souhaitez deplacer un pion : \n",de1);
                                        if(passetour(tour,team,de1,de2, colonnes)!=24) {  // Si le joueur n'a aucune possibilité, il passe son tour
                                            plateau(tour,team,colonnes,prison);
                                            de1=jeu(tour,team,de1,de2,colonnes,prison);
                                            tour++;
                                            win=victoire(colonnes);
                                            clear();
                                        }
                                        else{
                                        printf("Vous ne pouvez pas jouer...\nVous passez donc votre tour\n");
                                        tour++;
                                        win=victoire(colonnes);
                                        clear();
                                        }
                                    }
                                    else{
                                        printf("Vous ne pouvez pas jouer...\nVous passez donc votre tour\n");
                                        tour++;
                                        win=victoire(colonnes);
                                        clear();
                                    }
                                }
                                else{
                                    printf("Vous ne pouvez pas jouer...\nVous passez donc votre tour\n");
                                    tour++;
                                    win=victoire(colonnes);
                                    clear();
                                }
                            }
                        }
                    }
                    else {
                        if(passetour(tour,team,de1,de2, colonnes)!=24) {
                            de1=jeu(tour,team,de1,de2,colonnes,prison);
                            win=victoire(colonnes);
                            clear();
                            printf("Il vous reste 3 de de %d.\nVeuillez choisir la colonne a laquelle vous souhaitez deplacer un pion : \n",de1);
                            if(passetour(tour,team,de1,de2, colonnes)!=24) {  // Si le joueur n'a aucune possibilité, il passe son tour
                                plateau(tour,team,colonnes,prison);
                                de1=jeu(tour,team,de1,de2,colonnes,prison);
                                win=victoire(colonnes);
                                clear();
                                printf("Il vous reste 2 de de %d.\nVeuillez choisir la colonne a laquelle vous souhaitez deplacer un pion : \n",de2);
                                if(passetour(tour,team,de1,de2, colonnes)!=24) {  // Si le joueur n'a aucune possibilité, il passe son tour
                                    plateau(tour,team,colonnes,prison);
                                    de1=jeu(tour,team,de1,de2,colonnes,prison);
                                    win=victoire(colonnes);
                                    clear();
                                    printf("Il vous reste un de de %d.\nVeuillez choisir la colonne a laquelle vous souhaitez deplacer un pion : \n",de1);
                                    if(passetour(tour,team,de1,de2, colonnes)!=24) {  // Si le joueur n'a aucune possibilité, il passe son tour
                                        plateau(tour,team,colonnes,prison);
                                        de1=jeu(tour,team,de1,de2,colonnes,prison);
                                        tour++;
                                        win=victoire(colonnes);
                                        clear();
                                    }
                                    else{
                                    printf("Vous ne pouvez pas jouer...\nVous passez donc votre tour\n");
                                    tour++;
                                    win=victoire(colonnes);
                                    clear();
                                    }
                                }
                                else{
                                    printf("Vous ne pouvez pas jouer...\nVous passez donc votre tour\n");
                                    tour++;
                                    win=victoire(colonnes);
                                    clear();
                                }
                            }
                            else{
                                printf("Vous ne pouvez pas jouer...\nVous passez donc votre tour\n");
                                tour++;
                                win=victoire(colonnes);
                                clear();
                            }
                        }
                        else{
                            printf("Vous ne pouvez pas jouer...\nVous passez donc votre tour\n");
                            tour++;
                            win=victoire(colonnes);
                            clear();
                        }
                    }
                }
            }
            printf("Bravo ! \nLe joueur %d a gagne ! \n",tourjoueur(tour-1));
            clear();
            main();
            break;
        case 2 :
            regles();
            clear();
            main();
            break;
        case 3 :
            return 0;
        default :
            printf("Veuillez choisir une valeur correcte...\n");
            clear();
            main();
    }
}
