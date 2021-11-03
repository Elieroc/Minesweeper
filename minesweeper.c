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

int main(int argc, char*argv[]){

    int sceneArray[SCENE_NB_ROW_MAX][SCENE_NB_COL_MAX];

	// On vérifie les paramètres fournis
	while ( (argc<3) || (atoi(argv[1])<SCENE_NB_ROW_MIN) || (atoi(argv[1])>SCENE_NB_ROW_MAX) || (atoi(argv[2])<SCENE_NB_ROW_MIN) || (atoi(argv[2])>SCENE_NB_ROW_MAX) || (atoi(argv[3])<SCENE_MINE_PERCENT_MIN) || (atoi(argv[3])>SCENE_MINE_PERCENT_MAX)){
		printf("Merci de respecter la syntaxe suivante : \n");
		printf("./minesweeper <nbLignes> <nbColones> <PourcentageMines>\r\n");
		printf("\nNB lignes [%d-%d]\r\n", SCENE_NB_ROW_MIN, SCENE_NB_ROW_MAX);
		printf("NB colonnes [%d-%d]\r\n", SCENE_NB_COL_MIN, SCENE_NB_COL_MAX);
		printf("Pourcentage de mines [%d-%d]\r\n", SCENE_MINE_PERCENT_MIN, SCENE_MINE_PERCENT_MAX);
		return -1;
	}

	printf("-------------ENTER APP--------------\n");

	int nbRow = atoi(argv[1]);
	int nbCol = atoi(argv[2]);
    int minePercent = atoi(argv[3]);

	int iRow, iCol;
	int resultOfGC;
	int nbDiscoversCells = 0;

	int nbCellsToDiscover = SceneInit(sceneArray, nbRow, nbCol, minePercent);
	int nbCellsToDiscoverAtStart = nbCellsToDiscover;

	while (nbCellsToDiscover>0){
		printf("\nNombre de cellules découvertes : %d\n", nbDiscoversCells);
		printf("Nombre de cellules à découvrir : %d\n", nbCellsToDiscover);
		SceneDisplay(sceneArray, nbRow, nbCol);
		do{
			printf("\nLimites des coo : [%d-%d][%d-%d]\n", 0, nbRow, 0, nbCol);
			resultOfGC = GetCommand(&iRow, &iCol);
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
				nbDiscoversCells = SceneDiscoverCell(sceneArray, nbRow, nbCol, iRow, iCol);
				nbCellsToDiscover = nbCellsToDiscoverAtStart - nbDiscoversCells;
				if (nbDiscoversCells == -1){
					SceneUnmaskCells(sceneArray, nbRow, nbCol);
					SceneDisplay(sceneArray, nbRow, nbCol);
					printf("\n /!\\ Dommage d'avoir perdu, après tout il ne te reste plus que tes yeux pour pleurer :)\n");
					return EXIT_SUCCESS;
				}
				else {
					printf("\nVous avez joué en [%d][%d]", iRow, iCol);
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
	printf("\nBravo ! Vous méritez un demi bitcoin !\n");
	SceneUnmaskCells(sceneArray, nbRow, nbCol);
	SceneDisplay(sceneArray, nbRow, nbCol);

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
				if (sceneArray[k][m] == SCENE_CELL_VOID_VALUE) {
					printf(" %c ", SCENE_CELL_VOID_CHAR);
				}
				else if (sceneArray[k][m] == SCENE_MINE_VALUE) {
					printf(" %c ", SCENE_MINE_CHAR);
				}
				else {
					printf("%2d ", sceneArray[k][m]);
				}
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
	srand((unsigned int)time(NULL));
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
			if (sceneArray[k][m] == SCENE_CELL_VOID_VALUE){
				// Recherche
				// x : Numéro de colonne relative à la case sélectionnée
				// y : Numéro de ligne relative à la case sélectionnée
				for (int y=-1; y<=1; y++){
					for (int x=-1; x<=1; x++){
						// On vérifie les collisions
						if(  (k+y)>0 && (m+x)>0 || (k+y)<nbRow && (m+x)<nbCol ) {
							if (sceneArray[k+y][m+x] == 9){
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
	return nbCellsToDiscover;
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
	static int nbDiscoversCells = 0;
	
	// Si la case est déjà découverte
	if (sceneArray[atRow][atCol] < SCENE_CELL_MASK_OFFSET){
		return 0;
	}
	else {
		// On démasque la case
		sceneArray[atRow][atCol]-=SCENE_CELL_MASK_OFFSET;
		nbDiscoversCells++;
		// Si il s'agit d'une mine
		if (sceneArray[atRow][atCol] == SCENE_MINE_VALUE) {
			return -1;
		}
		// Si la case a une valeur d'adjacence non nulle
		if (sceneArray[atRow][atCol] > SCENE_CELL_VOID_VALUE){
			return 1;
		}
		// Si la case vaut 0
		if (sceneArray[atRow][atCol] == SCENE_CELL_VOID_VALUE){
			// Recherche
			// x : Coordonnée de colonne relative à la case sélectionnée
			// y : Coordonnée de ligne relative à la case sélectionnée
			for (int y=-1; y<=1; y++){
				for (int x=-1; x<=1; x++){
					// On vérifie les collisions
					if ( (atRow+y>=0) && (atCol+x>=0) && (atRow+y<nbRow) && (atCol+x<nbCol) ) {
						// Si la case est masquée
						if (sceneArray[atRow+y][atCol+x] >= SCENE_CELL_MASK_OFFSET){
							// Si la case vaut 0 (on rajoute l'offset car la case testée n'est pas encore démasquée)
							if (sceneArray[atRow+y][atCol+x] == (SCENE_CELL_VOID_VALUE + SCENE_CELL_MASK_OFFSET) ){
								SceneDiscoverCell(sceneArray, nbRow, nbCol, atRow+(y), atCol+(x));
							}
						}	
					}
				}
			}
		}
	}
	return nbDiscoversCells;
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
			}
			else if (sceneArray[k][m]>=SCENE_CELL_MARK_I_OFFSET){
				sceneArray[k][m] -= SCENE_CELL_MARK_I_OFFSET;
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
