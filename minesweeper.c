/*
Projet de Démineur de Elie Rocamora 821
Début : 01/11/2021
Fin :
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define SCENE_NB_ROW_MIN (4)
#define SCENE_NB_COL_MIN (4)
#define SCENE_NB_ROW_MAX (30)
#define SCENE_NB_COL_MAX (30)
#define SCENE_MINE_PERCENT_MIN (5)
#define SCENE_MINE_PERCENT_MAX (70)
#define SCENE_MINE_VALUE (9)
#define SCENE_MINE_CHAR ('*')
#define SCENE_CELL_VOID_VALUE (0)
#define SCENE_CELL_VOID_CHAR (' ') //Caractère espace
#define SCENE_CELL_MASK_OFFSET (10)

void SceneDisplay(int sceneArray[][SCENE_NB_COL_MAX], int nbRow, int nbCol);
void SceneInit(int sceneArray[][SCENE_NB_COL_MAX], int nbRow, int nbCol, int percent);

int main(){
    int sceneArray[SCENE_NB_ROW_MAX][SCENE_NB_COL_MAX];

	printf("-------------ENTER APP--------------\n");


	int nbRow = 20;
	int nbCol = 20;
    int minePercent = 15;
    /*
	do{
		printf("Enter nbRow [%d ; %d]: ", SCENE_NB_ROW_MIN, SCENE_NB_ROW_MAX); 
		scanf("\n%i", &nbRow);
	}
	while((nbRow<SCENE_NB_ROW_MIN)||(nbRow>SCENE_NB_ROW_MAX));

	do{
		printf("Enter nbCol [%d ; %d]: ", SCENE_NB_COL_MIN, SCENE_NB_COL_MAX); 
		scanf("\n%i", &nbCol);
	}
	while((nbCol<SCENE_NB_ROW_MIN)||(nbCol>SCENE_NB_ROW_MAX));

    do{
		printf("Enter percent of mines [%d ; %d]: ", SCENE_MINE_PERCENT_MIN, SCENE_MINE_PERCENT_MAX); 
		scanf("\n%i", &minePercent);
	}
	while((minePercent<SCENE_MINE_PERCENT_MIN)||(minePercent>SCENE_MINE_PERCENT_MAX));

    */
    SceneInit(sceneArray, nbRow, nbCol, minePercent);
    SceneDisplay(sceneArray, nbRow, nbCol);

    return EXIT_SUCCESS;
}

void SceneDisplay(int sceneArray[][SCENE_NB_COL_MAX], int nbRow, int nbCol){
	int k, m;

	// Dizaines
	printf("   ");
	for(m=0;m<nbCol;m++){
		if(m%10==0) printf("%d ", m/10);
		else printf("  ");
	}
	printf("\n");
	//

	// Numéro de colonnes
	printf("   ");
	for(m=0;m<nbCol;m++){
		printf("%d ", m%10);
	}
	printf("\n");

    // Affichage des "-"
    printf("   ");
	for(m=0;m<nbCol;m++){
		printf("---");
	}
	printf("\n");

	for(k=0;k<nbRow;k++){
		// Numéro de lignes
		printf("%2d", k);
        // Affichage des "|"
        printf("|");
		// Affichage des éléments
		for(m=0;m<nbCol;m++){
			if (sceneArray[k][m]<SCENE_CELL_MASK_OFFSET){
				printf("%2d ", sceneArray[k][m]);
			}
			else{
				printf(" . ");
			}
			
		}
        // Affichage des "|"
        printf("|");
        // Numéro de lignes
		printf("%d\n",k);
	}

    // Affichage des "-"
    printf("   ");
	for(m=0;m<nbCol;m++){
		printf("---");
	}
	printf("\n");
	printf("   ");
	for(m=0;m<nbCol;m++){
		printf("%d ", m%10);
	}
	printf("\n");

	printf("   ");
	for(m=0;m<nbCol;m++){
		if(m%10==0) printf("%d ", m/10);
		else printf("  ");
	}
	printf("\n");
}

void SceneInit(int sceneArray[][SCENE_NB_COL_MAX], int nbRow, int nbCol, int percent){
    // On initialise toutes les valeurs du tableau à "SCENE_CELL_VOID_VALUE"
    for(int k=0;k<nbRow;k++){
		for(int m=0;m<nbCol;m++){
			sceneArray[k][m] = SCENE_CELL_VOID_VALUE;
		}
	}
    // Génération des mines
    srand((unsigned int)time(NULL));
    for(int k=0;k<nbRow;k++){
		for(int m=0;m<nbCol;m++){
			// On génère un nombre entre 1 et 100 inclut
			int randCaseValue = rand()% 100 + 1;
			// Et on transforme toutes les valeurs en dessous du pourcentage demandé en mine
			if (randCaseValue<percent){
				// Mine
				sceneArray[k][m] = 9;
			}
			else{
				sceneArray[k][m] = 0;
			}
		}
	}
	// Completion des cases adjacentes aux mines
	for(int k=0;k<nbRow;k++){
		for(int m=0;m<nbCol;m++){
			if (sceneArray[k][m] == 0){
				// Recherche
				// x : Numéro de colonne relative à la case sélectionnée
				// y : Numéro de ligne relative à la case sélectionnée
				for (int y=-1; y<=1; y++){
					for (int x=-1; x<=1; x++){
						// On vérifie les collisions
						if(  (k+y)>0 && (m+x)>0 || (k+y)<nbRow && (m+x)<nbCol ) {
							if (sceneArray[k+x][m+y] == 9){
								sceneArray[k][m]++;
							}
						}
						else{
							//printf("Collision détectée !\n");
							continue;
						}
						
					}
				}
			}
		}
	}
	// Masquage des cases
	for(int k=0;k<nbRow;k++){
		for(int m=0;m<nbCol;m++){
			sceneArray[k][m] += SCENE_CELL_MASK_OFFSET;
		}
	}
}