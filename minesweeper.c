/*
Projet de Démineur de Elie Rocamora 821
Début : 01/11/2021
Fin :
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

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
#define INPUT_BUFFER_LEN (32)


void SceneDisplay(int sceneArray[][SCENE_NB_COL_MAX], int nbRow, int nbCol);
void SceneInit(int sceneArray[][SCENE_NB_COL_MAX], int nbRow, int nbCol, int percent);
int GetCommand(int *pIRow, int *pICol);

int main(){
    int sceneArray[SCENE_NB_ROW_MAX][SCENE_NB_COL_MAX];

	printf("-------------ENTER APP--------------\n");


	int nbRow = 20;
	int nbCol = 25;
    int minePercent = 15;
	int iRow, iCol;
	int result;
    
	/*
	do{
		printf("Enter nbRow [%d ; %d]: ", SCENE_NB_ROW_MIN, SCENE_NB_ROW_MAX); 
		scanf("%i", &nbRow);
	}
	while((nbRow<SCENE_NB_ROW_MIN)||(nbRow>SCENE_NB_ROW_MAX));

	do{
		printf("Enter nbCol [%d ; %d]: ", SCENE_NB_COL_MIN, SCENE_NB_COL_MAX); 
		scanf("%i", &nbCol);
	}
	while((nbCol<SCENE_NB_ROW_MIN)||(nbCol>SCENE_NB_ROW_MAX));

    do{
		printf("Enter percent of mines [%d ; %d]: ", SCENE_MINE_PERCENT_MIN, SCENE_MINE_PERCENT_MAX); 
		scanf("%i", &minePercent);
	}
	while((minePercent<SCENE_MINE_PERCENT_MIN)||(minePercent>SCENE_MINE_PERCENT_MAX));
	*/

    SceneInit(sceneArray, nbRow, nbCol, minePercent);
    SceneDisplay(sceneArray, nbRow, nbCol);
	
	do{
		printf("\nLimites des coo : [%d-%d][%d-%d]\n", 0, nbRow, 0, nbCol);
		result = GetCommand(&iRow, &iCol);
		printf("\n");
	}
	while ( (iRow<0) || (iRow>nbRow) || (iCol<0) || (iCol>nbCol));

	switch(result){
		case 0:
			printf("Vous avez joué aux coo : [%d][%d]\n", iRow, iCol);
			break;
		case 1:
			printf("Vous avez marqué un M aux coo : [%d][%d]\n", iRow, iCol);
			break;
		case 2:
			printf("Vous avez marqué un ? aux coo : [%d][%d]\n", iRow, iCol);
			break;
		default:
			printf("Entrée invalide\n");
			break;
	}

    return EXIT_SUCCESS;
}

void SceneDisplay(int sceneArray[][SCENE_NB_COL_MAX], int nbRow, int nbCol){
	int k, m;

	// Dizaines
	printf("\n");
	printf("   ");
	for(m=0;m<nbCol;m++){
		if(m%10==0) printf("%2d ", m/10);
		else printf("   ");
	}
	printf("\n");

	// Numéro de colonnes
	printf("   ");
	for(m=0;m<nbCol;m++){
		printf("%2d ", m%10);
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

	// Numéro de colonnes
	printf("\n");
	printf("   ");
	for(m=0;m<nbCol;m++){
		printf("%2d ", m%10);
	}
	printf("\n");

	printf("   ");
	for(m=0;m<nbCol;m++){
		if(m%10==0) printf("%2d ", m/10);
		else printf("   ");
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

int GetCommand(int *pIRow, int *pICol){
	const char separators[]="\r\n ";
	char buffer[INPUT_BUFFER_LEN];
	char *pToken;
	int iReturnAction;

	*pIRow=*pICol=0;
	printf("Entrer une commande : ");
	fgets(buffer, INPUT_BUFFER_LEN, stdin);
	pToken=strtok(buffer, separators);
	if (pToken){
		if (isdigit(pToken[0])){
			*pIRow = atoi(pToken);
			pToken=strtok(NULL, separators);
			if (pToken){
				*pICol=atoi(pToken);
			}
			iReturnAction=0;
		}
		else {
			switch (pToken[0]){
				case 'P':
				case 'p':
					iReturnAction=0;
					break;
				case 'M':
				case 'm':
					iReturnAction=1;
					break;
				case '?':
					iReturnAction=2;
					break;
				default:
					iReturnAction=-1;
					break;
			}
			pToken=strtok(NULL, separators);
			if (pToken){
				*pIRow=atoi(pToken);
				pToken=strtok(NULL, separators);
				if (pToken){
					*pICol=atoi(pToken);
				}
			}
		}
	}
	return iReturnAction;
}
