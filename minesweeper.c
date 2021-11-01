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
#define SCENE_CELL_MARK_M_OFFSET (20)
#define SCENE_CELL_MARK_I_OFFSET (30) // I for Interrogation ?


void SceneDisplay(int sceneArray[][SCENE_NB_COL_MAX], int nbRow, int nbCol);
int SceneInit(int sceneArray[][SCENE_NB_COL_MAX], int nbRow, int nbCol, int percent);
int GetCommand(int *pIRow, int *pICol);
int SceneDiscoverCell(int sceneArray[][SCENE_NB_COL_MAX], int nbRow, int nbCol, int atRow, int atCol);
void SceneMaskCells(int sceneArray[][SCENE_NB_COL_MAX], int nbRow, int nbCol);
void SceneUnmaskCells(int sceneArray[][SCENE_NB_COL_MAX], int nbRow, int nbCol);
void SceneToggleMarkCell(int sceneArray[][SCENE_NB_COL_MAX], int atRow, int atCol, int markOffset);

int main(){
    int sceneArray[SCENE_NB_ROW_MAX][SCENE_NB_COL_MAX];

	printf("-------------ENTER APP--------------\n");


	int nbRow = 20;
	int nbCol = 25;
    int minePercent = 15;
	int iRow, iCol;
	int resultOfGC, resultOfSDC;
	bool endOfGame = false;
    
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
	while (endOfGame==false){
		SceneDisplay(sceneArray, nbRow, nbCol);
		do{
			printf("\nLimites des coo : [%d-%d][%d-%d]\n", 0, nbRow, 0, nbCol);
			resultOfGC = GetCommand(&iRow, &iCol);
			printf("\nVous avez joué en [%d][%d]", iRow, iCol);
			printf("\n");
		}
		while ( (iRow<0) || (iRow>nbRow) || (iCol<0) || (iCol>nbCol));

		switch(resultOfGC){
			// Cas du P :
			case 0:
				if (sceneArray[iRow][iCol]>=SCENE_CELL_MARK_M_OFFSET) {
					printf("\n/!\\ Merci d'enlever le flag de la case avant de la jouer !\n");
					break;
				}
				resultOfSDC = SceneDiscoverCell(sceneArray, nbRow, nbCol, iRow, iCol);
				if (resultOfSDC == -1){
					SceneUnmaskCells(sceneArray, nbRow, nbCol);
					SceneDisplay(sceneArray, nbRow, nbCol);
					printf("\n /!\\ Dommage d'avoir perdu, après tout il ne te reste plus que tes yeux pour pleurer :)\n");
					return EXIT_SUCCESS;
				}
				else {
					printf("Nombre de cells découvertes : %d\n", resultOfSDC);
				}
				break;
			// Cas du M :
			case 1:
				printf("\nVous avez marqué un M aux coo : [%d][%d]\n", iRow, iCol);
				SceneToggleMarkCell(sceneArray, iRow, iCol, SCENE_CELL_MARK_M_OFFSET);
				break;
			// Cas du ? :
			case 2:
				printf("Vous avez marqué un ? aux coo : [%d][%d]\n", iRow, iCol);
				SceneToggleMarkCell(sceneArray, iRow, iCol, SCENE_CELL_MARK_I_OFFSET);
				break;
			// Cas du Q :
			case 3:
				printf("Merci d'avoir joué ! Au revoir !\n");
				return EXIT_SUCCESS;
				break;
			default:
				printf("Entrée invalide\n");
				break;
		}
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
				if (sceneArray[k][m]>=SCENE_CELL_MASK_OFFSET && sceneArray[k][m]<SCENE_CELL_MARK_M_OFFSET){
					printf(" . ");
				}
				else if (sceneArray[k][m]>=SCENE_CELL_MARK_M_OFFSET && sceneArray[k][m]<SCENE_CELL_MARK_I_OFFSET){
					printf(" M ");
				} 
				else if (sceneArray[k][m]>=SCENE_CELL_MARK_I_OFFSET) {
					printf(" ? ");
				}
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

int SceneInit(int sceneArray[][SCENE_NB_COL_MAX], int nbRow, int nbCol, int percent){
    // On initialise toutes les valeurs du tableau à "SCENE_CELL_VOID_VALUE"
    for(int k=0;k<nbRow;k++){
		for(int m=0;m<nbCol;m++){
			sceneArray[k][m] = SCENE_CELL_VOID_VALUE;
		}
	}
    // Génération des mines
    int nbMines = nbRow * nbCol * percent/100;
	int nbSetMines = 0;
	int x, y;
    while(nbMines > nbSetMines){
        x = rand()%nbRow;
        y = rand()%nbCol;
        if (sceneArray[x][y] != SCENE_MINE_VALUE){
            sceneArray[x][y] = SCENE_MINE_VALUE;
            nbSetMines++;
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
	// Masquage des cellules
	SceneMaskCells(sceneArray, nbRow, nbCol);

	// Calcul du nombre de cellules à découvrir
	int nbCellsToDiscover = 0;
	for(int k=0;k<nbRow;k++){
		for(int m=0;m<nbCol;m++){
			nbCellsToDiscover++;
		}
	}
	nbCellsToDiscover-=nbMines;
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
				case 'Q':
				case 'q':
					iReturnAction=3;
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

int SceneDiscoverCell(int sceneArray[][SCENE_NB_COL_MAX], int nbRow, int nbCol, int atRow, int atCol){
	int nbDiscoversCells = 0;

	if (sceneArray[atRow][atCol] < SCENE_CELL_MASK_OFFSET){
		return 0;
	}
	else {
		sceneArray[atRow][atCol]-=SCENE_CELL_MASK_OFFSET;
		// Si il s'agit d'une mine
		if (sceneArray[atRow][atCol] == 9) {
			return -1;
		}
		if (sceneArray[atRow][atCol] > 0){
			return 1;
		}
		if (sceneArray[atRow][atCol] == 0){
				// Recherche
				// x : Numéro de colonne relative à la case sélectionnée
				// y : Numéro de ligne relative à la case sélectionnée
				for (int y=-1; y<=1; y++){
					for (int x=-1; x<=1; x++){
						// On vérifie les collisions
						if(  (atRow+y)>0 && (atCol+x)>0 || (atRow+y)<nbRow && (atCol+x)<nbCol ) {
							nbDiscoversCells++;
							if (sceneArray[atRow+x][atCol+y] > 9){
								sceneArray[atRow+y][atCol+x] = sceneArray[atRow+y][atCol+x] % 10;
								if (sceneArray[atRow+(y)][atCol+(x)] == 0){
									SceneDiscoverCell(sceneArray, nbRow, nbCol, atRow+(x), atCol+(y));
								}			
							}			
						}
					}
				}
			return nbDiscoversCells;
		}
	}
}

void SceneMaskCells(int sceneArray[][SCENE_NB_COL_MAX], int nbRow, int nbCol){
	for(int k=0;k<nbRow;k++){
		for(int m=0;m<nbCol;m++){
			sceneArray[k][m] += SCENE_CELL_MASK_OFFSET;
		}
	}
}

void SceneUnmaskCells(int sceneArray[][SCENE_NB_COL_MAX], int nbRow, int nbCol){
	for(int k=0;k<nbRow;k++){
		for(int m=0;m<nbCol;m++){
			if (sceneArray[k][m]>=SCENE_CELL_MASK_OFFSET && sceneArray[k][m] < SCENE_CELL_MARK_M_OFFSET) {
				sceneArray[k][m] -= SCENE_CELL_MASK_OFFSET;
			}
			else if (sceneArray[k][m]>=SCENE_CELL_MARK_M_OFFSET && sceneArray[k][m] < SCENE_CELL_MARK_I_OFFSET){
				sceneArray[k][m] -= SCENE_CELL_MARK_M_OFFSET;
				sceneArray[k][m] -= SCENE_CELL_MASK_OFFSET;
			}
			else if (sceneArray[k][m]>=SCENE_CELL_MARK_I_OFFSET){
				sceneArray[k][m] -= SCENE_CELL_MARK_I_OFFSET;
				sceneArray[k][m] -= SCENE_CELL_MASK_OFFSET;
			}
		}
	}
}

void SceneToggleMarkCell(int sceneArray[][SCENE_NB_COL_MAX], int atRow, int atCol, int markOffset){
	if (sceneArray[atRow][atCol] < SCENE_CELL_MASK_OFFSET){
		return;
	}
	else{
		// Si il n'y a pas de marquage
		if (sceneArray[atRow][atCol] < SCENE_CELL_MARK_M_OFFSET){
			// On modifie l'offset pour rendre correcte la valeur de la case lorsqu'on va la modifier
			markOffset-=SCENE_CELL_MASK_OFFSET;
			sceneArray[atRow][atCol]+=markOffset;
			return;
		}
		// Si on demande d'enlever le marquage M
		if (sceneArray[atRow][atCol]>=SCENE_CELL_MARK_M_OFFSET && (sceneArray[atRow][atCol]<SCENE_CELL_MARK_I_OFFSET) && (markOffset==SCENE_CELL_MARK_M_OFFSET) ){
			markOffset-=SCENE_CELL_MASK_OFFSET;
			sceneArray[atRow][atCol]-=markOffset;
			return;
		}
		// Si on demande d'enlever le marquage ?
		if (sceneArray[atRow][atCol]>=SCENE_CELL_MARK_I_OFFSET && (markOffset==SCENE_CELL_MARK_I_OFFSET)){
			markOffset-=SCENE_CELL_MASK_OFFSET;
			sceneArray[atRow][atCol]-=markOffset;
			return;
		}
		// Si on demande de mettre un marquage ? alors qu'il y a déjà un M
		if (sceneArray[atRow][atCol]>=SCENE_CELL_MARK_M_OFFSET && (sceneArray[atRow][atCol]<SCENE_CELL_MARK_I_OFFSET) && (markOffset==SCENE_CELL_MARK_I_OFFSET)){
			sceneArray[atRow][atCol]-=SCENE_CELL_MARK_M_OFFSET;
			sceneArray[atRow][atCol]+=markOffset;
			return;
		}
		// Si on demande de mettre un marquage M alors qu'il y a déjà un ?
		if (sceneArray[atRow][atCol]>=SCENE_CELL_MARK_I_OFFSET && (markOffset==SCENE_CELL_MARK_M_OFFSET)){
			sceneArray[atRow][atCol]-=SCENE_CELL_MARK_I_OFFSET;
			sceneArray[atRow][atCol]+=markOffset;
			return;
		}
	}
}
