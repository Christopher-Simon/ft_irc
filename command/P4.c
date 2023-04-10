// #include "../Command.hpp"

// void Command::P4(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt)
// {

// }


// static double nb_aleatoire(void)
// {
//     return rand() / (RAND_MAX + 1.);
// }


// static int nb_aleatoire_entre(int min, int max)
// {
//     return nb_aleatoire() * (max - min + 1) + min;
// }

// char (*add_a_piece(char tab[6][7], unsigned a, unsigned b))[7]
// {
//     unsigned i=5;
//     a-=1;
//     while (tab[i][a]=='X' || tab[i][a]=='O')
//         i--;
//     (b==1)?(tab[i][a]='X'):(tab[i][a]='O');
    
//     return tab;
// }

// int affichage_tableau(char (*tab)[7])
// {
//     printf("  1   2   3   4   5   6   7  \n+---+---+---+---+---+---+---+\n");
//     for(int i=0; i<6; i++)
//     {
//         printf("| %c | %c | %c | %c | %c | %c | %c |\n+---+---+---+---+---+---+---+\n",tab[i][0], tab[i][1], tab[i][2], tab[i][3], tab[i][4], tab[i][5], tab[i][6]);
//     }
//     printf("  1   2   3   4   5   6   7  \n");
//     return 0;
// }


// int possibilites(char (*tab)[7], int p)
// {
//     int i=5;
//     p-=1;
//     while (tab[i][p]=='X' || tab[i][p]=='O')
//         i--;
//     if (i<0)
//     {
//         return 0;
//     }
//     //Colonnes
//     int compteur_colonne = 1;
//     int intermed=i+1;
//     char controller=' ';
//     if (intermed<6 && (tab[intermed][p]=='X' || tab[intermed][p]=='O'))
//     {
//         compteur_colonne++;
//         controller=tab[intermed][p];
//         intermed++;
//     }
//     while(intermed<6 && tab[intermed][p]==controller)
//     {
//         compteur_colonne++;
//         intermed++;
//     }
    
//     //Lignes
//     int compteur_ligne=1;
//     int compteur_ligne_left = 1;
//     intermed=p-1;
//     char controller_left='d';
//     if (intermed>=0 && (tab[i][intermed]=='X' || tab[i][intermed]=='O'))
//     {
//         compteur_ligne_left++;
//         controller_left=tab[i][intermed];
//         intermed--;
//     }
//     while(intermed>=0 && tab[i][intermed]==controller_left)
//     {
//         compteur_ligne_left++;
//         intermed--;
//     }
    
//     int compteur_ligne_right = 1;
//     intermed=p+1;
//     char controller_right='d';
//     if (intermed<7 && (tab[i][intermed]=='X' || tab[i][intermed]=='O'))
//     {
//         compteur_ligne_right++;
//         controller_right=tab[i][intermed];
//         intermed++;
//     }
//     while(intermed<7 && tab[i][intermed]==controller_right)
//     {
//         compteur_ligne_right++;
//         intermed++;
//     }
//     if (controller_right==controller_left)
//         compteur_ligne=compteur_ligne_left+compteur_ligne_right-1;
//     else
//         (compteur_ligne_left>compteur_ligne_right)?(compteur_ligne=compteur_ligne_left):(compteur_ligne=compteur_ligne_right);
//     int compteur_col_ligne=(compteur_ligne>compteur_colonne)?compteur_ligne:compteur_colonne;
    
//     //Diagonales
//     int compteur_diag=1;
//     int compteur_diag_droit=1;
//     int compteur_diag_gauche=1;
    
//     //Diagonale gauche haute
//     int compteur_diag_left_haut=1;
//     int intermed_col=p-1;
//     int intermed_ligne=i-1;
//     char controller_left_haut='d';
//     if (intermed_col>=0 && intermed_ligne>=0 && (tab[intermed_ligne][intermed_col]=='X' || tab[intermed_ligne][intermed_col]=='O'))
//     {
//         compteur_diag_left_haut++;
//         controller_left_haut=tab[intermed_ligne][intermed_col];
//         intermed_col--;
//         intermed_ligne--;
//     }
//     while(intermed>=0 && intermed_ligne>=0 && tab[intermed_ligne][intermed_col]==controller_left_haut)
//     {
//         compteur_diag_left_haut++;
//         intermed_col--;
//         intermed_ligne--;
//     }
//     //Diagonale gauche basse
//     int compteur_diag_left_bas=1;
//     intermed_col=p-1;
//     intermed_ligne=i+1;
//     char controller_left_bas='d';
//     if (intermed_col>=0 && intermed_ligne<6 && (tab[intermed_ligne][intermed_col]=='X' || tab[intermed_ligne][intermed_col]=='O'))
//     {
//         compteur_diag_left_bas++;
//         controller_left_bas=tab[intermed_ligne][intermed_col];
//         intermed_col--;
//         intermed_ligne++;
//     }
//     while(intermed>=0 && intermed_ligne<6 && tab[intermed_ligne][intermed_col]==controller_left_bas)
//     {
//         compteur_diag_left_bas++;
//         intermed_col--;
//         intermed_ligne++;
//     }
    
//     //Diagonale droite haute
//     int compteur_diag_right_haut=1;
//     intermed_col=p+1;
//     intermed_ligne=i-1;
//     char controller_right_haut='d';
//     if (intermed_col<7 && intermed_ligne>=0 && (tab[intermed_ligne][intermed_col]=='X' || tab[intermed_ligne][intermed_col]=='O'))
//     {
//         compteur_diag_right_haut++;
//         controller_right_haut=tab[intermed_ligne][intermed_col];
//         intermed_col++;
//         intermed_ligne--;
//     }
//     while(intermed<7 && intermed_ligne>=0 && tab[intermed_ligne][intermed_col]==controller_right_haut)
//     {
//         compteur_diag_right_haut++;
//         intermed_col++;
//         intermed_ligne--;
//     }
    
//     //Diagonale droite basse
//     int compteur_diag_right_bas=1;
//     intermed_col=p+1;
//     intermed_ligne=i+1;
//     char controller_right_bas='d';
//     if (intermed_col<7 && intermed_ligne<6 && (tab[intermed_ligne][intermed_col]=='X' || tab[intermed_ligne][intermed_col]=='O'))
//     {
//         compteur_diag_right_bas++;
//         controller_right_bas=tab[intermed_ligne][intermed_col];
//         intermed_col++;
//         intermed_ligne++;
//     }
//     while(intermed<7 && intermed_ligne<6 && tab[intermed_ligne][intermed_col]==controller_right_bas)
//     {
//         compteur_diag_right_bas++;
//         intermed_col++;
//         intermed_ligne++;
//     }
    
//     if (controller_right_bas==controller_left_haut)
//         compteur_diag_gauche=compteur_diag_left_haut+compteur_diag_right_bas-1;
//     else
//         (compteur_diag_left_haut>compteur_diag_right_bas)?(compteur_diag_gauche=compteur_diag_left_haut):(compteur_diag_gauche=compteur_diag_right_bas);
    
//     if (controller_right_haut==controller_left_bas)
//         compteur_diag_droit=compteur_diag_left_bas+compteur_diag_right_haut-1;
//     else
//         (compteur_diag_left_bas>compteur_diag_right_haut)?(compteur_diag_droit=compteur_diag_left_bas):(compteur_diag_droit=compteur_diag_right_haut);
//     (compteur_diag_droit>compteur_diag_gauche)?(compteur_diag=compteur_diag_droit):(compteur_diag=compteur_diag_gauche);
//     return (compteur_col_ligne>compteur_diag)?(compteur_col_ligne):(compteur_diag);
// }

// int meilleur_coup(char (*tab)[7])
// {
//     int score[7]={0,0,0,0,0,0,0};
//     int max=0;
//     for (int i=0;i<7;i++)
//     {
//         score[i]=possibilites(tab, i+1);
//         (score[i]>max)?(max=score[i]):(max=max);
//     }
//     printf("{%d} - {%d} - {%d} - {%d} - {%d} - {%d} - {%d}\n", score[0], score[1], score[2], score[3], score[4], score[5], score[6]);
//     printf("max : %d\n", max);
//     int n=0;
//     for (int j=0;j<7;j++)
//         {
//             if (score[j]==max)
//                 n++;
//         }
//     int possib[n];
//     int indice=0;
//     for (int j=0;j<7;j++)
//         {
//             if (score[j]==max)
//             {
//                 possib[indice]=j+1;
//                 indice++;
//             }
//         }
//     if (n==1)
//         return possib[0];
//     return possib[nb_aleatoire_entre(0,n-1)];
// }

// int verification_victoire(char (*tab)[7], int c)
// {
//     //printf("starfoullah");
//     if (c<7)
//         return 0;
    
//     //Colonnes;
//     for (int j=0; j<7; j++)
//     {
//         int i=5;
//         while (i>2)
//         {
//             if (tab[i][j]=='X' && tab[i-1][j]=='X' && tab[i-2][j]=='X' && tab[i-3][j]=='X')
//             {
//                 printf("Joueur 1 a gagné après %d coups\n", c);
//                 return 1;
//             }
//             if (tab[i][j]=='O' && tab[i-1][j]=='O' && tab[i-2][j]=='O' && tab[i-3][j]=='O')
//             {
//                 printf("Joueur 2 a gagné après %d coups\n", c);
//                 return 1;
//             }
//             i--;
//         }
//     }
    
//     //Lignes;
//     for (int i=5; i>=0; i--)
//     {
//         int j=0;
//         while (j<5)
//         {
//             if (tab[i][j]=='X' && tab[i][j+1]=='X' && tab[i][j+2]=='X' && tab[i][j+3]=='X')
//             {
//                 printf("Joueur 1 a gagné après %d coups\n", c);
//                 return 1;
//             }
//             if (tab[i][j]=='O' && tab[i][j+1]=='O' && tab[i][j+2]=='O' && tab[i][j+3]=='O')
//             {
//                 printf("Joueur 2 a gagné après %d coups\n", c);
//                 return 1;
//             }
//             j++;
//         }
//     }
    
//     //Diagonales
//     int comp=1;
//     char z='X';
//     while (comp<3)
//     {
//         //Diagonales de 4
//         int diag_4[4][4][2]={{{2,0},{3,1},{4,2},{5,3}},{{0,3},{1,4},{2,5},{3,6}},{{0,3},{1,2},{2,1},{3,0}},{{2,6},{3,5},{4,4},{5,3}}};
//         for(int s=0; s<4; s++)
//         {
//             if (tab[diag_4[s][0][0]][diag_4[s][0][1]]==z && tab[diag_4[s][1][0]][diag_4[s][1][1]]==z && tab[diag_4[s][2][0]][diag_4[s][2][1]]==z && tab[diag_4[s][3][0]][diag_4[s][3][1]]==z)
//             {
//                 printf("Joueur %d a gagné après %d coups\n",comp, c);
//                 return 1;
//             }
//         }
        
//         //Diagonales de 5
//         int diag_5[4][5][2]={{{1,0},{2,1},{3,2},{4,3},{5,4}},{{0,2},{1,3},{2,4},{3,5},{4,6}},{{0,4},{1,3},{2,2},{3,1},{4,0}},{{1,6},{2,5},{3,4},{4,3},{5,2}}};
//         int a=0;
//         while (a<2)
//         {
//             for(int s=0; s<4; s++)
//             {
//                 if (tab[diag_5[s][a][0]][diag_5[s][a][1]]==z && tab[diag_5[s][a+1][0]][diag_5[s][a+1][1]]==z && tab[diag_5[s][a+2][0]][diag_5[s][a+2][1]]==z && tab[diag_5[s][a+3][0]][diag_5[s][a+3][1]]==z)
//                 {
//                     printf("Joueur %d a gagné après %d coups\n",comp, c);
//                     return 1;
//                 }
//             }
//             a++;
//         }
        
//         //Diagonales de 6
//         int diag_6[4][6][2]={{{0,0},{1,1},{2,2},{3,3},{4,4},{5,5}},{{0,1},{1,2},{2,3},{3,4},{4,5},{5,6}},{{0,5},{1,4},{2,3},{3,2},{4,1},{5,0}},{{0,6},{1,5},{2,4},{3,3},{4,2},{5,1}}};
//         a=0;
//         while (a<3)
//         {
//             for(int s=0; s<4; s++)
//             {
//                 if (tab[diag_6[s][a][0]][diag_6[s][a][1]]==z && tab[diag_6[s][a+1][0]][diag_6[s][a+1][1]]==z && tab[diag_6[s][a+2][0]][diag_6[s][a+2][1]]==z && tab[diag_6[s][a+3][0]][diag_6[s][a+3][1]]==z)
//                 {
//                     printf("Joueur %d a gagné après %d coups\n",comp, c);
//                     return 1;
//                 }
//             }
//             a++;
//         }
//         comp++;
//         z='O';
//     }
    
//     return 0;
// }
    
// int main(void)
// {
//     char tableau [6][7];
//     for (int i=0; i<6; i++)
//         for (int j=0; j<7; j++)
//             tableau[i][j]=' ';
//     int a; int b = 1;
//     int compteur_coups=0;
//     printf("Let's play Puissance 4!, Est-ce que vous jouez à 1 ou 2 joueurs?\n");
//     int ia=scanf("%d", &ia);
//     affichage_tableau(tableau);
//     if (ia=='2')
//     {
//         while (verification_victoire(tableau, compteur_coups)==0)
//         {
//             printf("Joueur %d:\n", b);
//             scanf("%u", &a);
//             //if a>7... failure;
//             affichage_tableau(add_a_piece(tableau, a, b));
//             compteur_coups++;
//             if (b==1)
//                 b=2;
//             else
//                 b=1;
//         }
//         return 0;
//     }
//     else //Partie contre l'IA
//     {
//         time_t t;

//         if (time(&t) == (time_t)-1)
//         {
//                 fprintf(stderr, "Impossible d'obtenir la date courante\n");
//                 return EXIT_FAILURE;
//         }

//         srand((unsigned)t);
//         //nb_aleatoire_entre(0, 10)
    
//         while (verification_victoire(tableau, compteur_coups)==0)
//         {
//             printf("Joueur 1:\n");
//             scanf("%u", &a);
//             //if a>7... failure;
//             affichage_tableau(add_a_piece(tableau, a, 1));
//             compteur_coups++;
//             if (verification_victoire(tableau, compteur_coups)!=0)
//                 return 0;
//             printf("Au tour de l'ordinateur:\n");
//             affichage_tableau(add_a_piece(tableau, meilleur_coup(tableau), 2));
//             compteur_coups++;
//         }
//         return 0;
//     }
// }