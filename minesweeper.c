/*
	Projet Démineur de Elie Rocamora 821
	Début : 01/11/2021
	Fin : 04/11/2021
	Temps de travail : ~12h
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>

#define SCENE_NB_ROW_MIN (4)
#define SCENE_NB_COL_MIN (4)
#define SCENE_NB_ROW_MAX (30)
#define SCENE_NB_COL_MAX (30)
#define SCENE_MINE_PERCENT_MIN (20)
#define SCENE_MINE_PERCENT_MAX (70)
#define SCENE_MINE_VALUE (9)
#define SCENE_MINE_CHAR ('*')
#define SCENE_CELL_VOID_VALUE (0)
#define SCENE_CELL_VOID_CHAR (' ') //Caractère espace
#define SCENE_CELL_MASK_OFFSET (10)
#define INPUT_BUFFER_LEN (32)
#define SCENE_CELL_MARK_M_OFFSET (20)
#define SCENE_CELL_MARK_I_OFFSET (30) // I for Interrogation ?

int SceneInit(int iSceneArray[][SCENE_NB_COL_MAX], int iNbRow, int iNbCol, int iPercent);
void SceneDisplay(int iSceneArray[][SCENE_NB_COL_MAX], int iNbRow, int iNbCol);
int GetCommand(int *pIRow, int *pICol);
int SceneDiscoverCell(int iSceneArray[][SCENE_NB_COL_MAX], int iNbRow, int iNbCol, int iAtRow, int iAtCol, int *iNbDiscoversCells);
void SceneUnmaskCells(int iSceneArray[][SCENE_NB_COL_MAX], int iNbRow, int iNbCol);
void SceneToggleMarkCell(int iSceneArray[][SCENE_NB_COL_MAX], int iAtRow, int iAtCol, int iMarkOffset);

int main(int argc, char*argv[]){

    int iScene[SCENE_NB_ROW_MAX][SCENE_NB_COL_MAX];

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

	int iSceneNbRow = atoi(argv[1]);
	int iSceneNbCol = atoi(argv[2]);
    int iSceneMinePercent = atoi(argv[3]);

	int iRow, iCol;
	int iResultOfGC;
	int iResultOfSDC;
	int iNbDiscoversCells = 0;

	int iNbCellsToDiscoverAtStart = SceneInit(iScene, iSceneNbRow, iSceneNbCol, iSceneMinePercent);
	int iNbCellsToDiscover = iNbCellsToDiscoverAtStart;

	printf("\r\nTaper 'h' pour obtenir de l'aide sur les commandes\r\n");

	while (iNbCellsToDiscover>0){

		printf("\r\nNombre de cellules découvertes : %d\r\n", iNbDiscoversCells);
		printf("Nombre de cellules à découvrir : %d\r\n", iNbCellsToDiscover);
		SceneDisplay(iScene, iSceneNbRow, iSceneNbCol);

		do{
			printf("\r\nLimites des coo : [%d-%d][%d-%d]\r\n", 0, iSceneNbRow, 0, iSceneNbCol);
			iResultOfGC = GetCommand(&iRow, &iCol);
			printf("\r\n");
		}
		while ( (iRow<0) || (iRow>iSceneNbRow) || (iCol<0) || (iCol>iSceneNbCol));

		switch(iResultOfGC){
			// Cas du P :
			case 0:
				if (iScene[iRow][iCol]>=SCENE_CELL_MARK_M_OFFSET) {
					printf("\r\n/!\\ Merci d'enlever le flag de la case avant de la jouer !\r\n");
					break;
				}
				// J'utilise un pointeur car j'avais des bugs pour compter le nombre de cellules découvertes 
				// Donc là ça marche et je ne me perds pas :)
				iResultOfSDC = SceneDiscoverCell(iScene, iSceneNbRow, iSceneNbCol, iRow, iCol, &iNbDiscoversCells);
				iNbCellsToDiscover = iNbCellsToDiscoverAtStart - iNbDiscoversCells;
				if (iResultOfSDC == -1){
					SceneUnmaskCells(iScene, iSceneNbRow, iSceneNbCol);
					SceneDisplay(iScene, iSceneNbRow, iSceneNbCol);
					printf("\r\n /!\\ Dommage d'avoir perdu, après tout il ne te reste plus que tes yeux pour pleurer :)\r\n");
					return EXIT_SUCCESS;
				}
				else {
					printf("\r\nVous avez joué en [%d][%d]", iRow, iCol);
				}
				break;
			// Cas du M :
			case 1:
				printf("\r\nVous avez marqué un 'M' aux coo : [%d][%d]\r\n", iRow, iCol);
				SceneToggleMarkCell(iScene, iRow, iCol, SCENE_CELL_MARK_M_OFFSET);
				break;
			// Cas du ? :
			case 2:
				printf("Vous avez marqué un '?' aux coo : [%d][%d]\r\n", iRow, iCol);
				SceneToggleMarkCell(iScene, iRow, iCol, SCENE_CELL_MARK_I_OFFSET);
				break;
			// Cas du Q :
			case 3:
				printf("Merci d'avoir joué ! Au revoir !\r\n");
				return EXIT_SUCCESS;
				break;
			// Cas du H :
			case 4:
				printf("Voici la liste des commandes utilisables :\r\n");
				printf("p <ligne> <colonne> --> Pour jouer une case\r\n");
				printf("m <ligne> <colonne> --> Pour marquer une case où il y a une mine\r\n");
				printf("? <ligne> <colonne> --> Pour marquer une case qui vous évoque le doute\r\n");
				printf("q                   --> Pour quitter la partie\r\n");
				break;
			default:
				printf("Entrée invalide\r\n");
				break;
		}
	}

	printf("\r\nBravo ! Vous méritez une récompense ...\r\n");
	printf("\r\nMais vous n'en n'aurez pas (:\r\n");
	SceneUnmaskCells(iScene, iSceneNbRow, iSceneNbCol);
	SceneDisplay(iScene, iSceneNbRow, iSceneNbCol);

    return EXIT_SUCCESS;
}

int SceneInit(int iSceneArray[][SCENE_NB_COL_MAX], int iNbRow, int iNbCol, int iPercent){
    // On initialise toutes les valeurs du tableau à "SCENE_CELL_VOID_VALUE"
    for(int k=0;k<iNbRow;k++){
		for(int m=0;m<iNbCol;m++){
			iSceneArray[k][m] = SCENE_CELL_VOID_VALUE;
		}
	}
    // Génération des mines
	srand((unsigned int)time(NULL));
    int iNbMines = iNbRow * iNbCol * iPercent/100;
	int iNbSetMines = 0;
	int iMinePosX, iMinePosY;
    while(iNbMines > iNbSetMines){
        iMinePosX = rand()%iNbRow;
        iMinePosY = rand()%iNbCol;
        if (iSceneArray[iMinePosX][iMinePosY] != SCENE_MINE_VALUE){
            iSceneArray[iMinePosX][iMinePosY] = SCENE_MINE_VALUE;
            iNbSetMines++;
        }
    }

	// Completion des cases adjacentes aux mines
	for(int k=0;k<iNbRow;k++){
		for(int m=0;m<iNbCol;m++){
			if (iSceneArray[k][m] == SCENE_CELL_VOID_VALUE){
				// Recherche
				// x : Numéro de colonne relative à la case sélectionnée
				// y : Numéro de ligne relative à la case sélectionnée
				for (int y=-1; y<=1; y++){
					for (int x=-1; x<=1; x++){
						// On vérifie les collisions
						if(  (k+y)>0 && (m+x)>0 || (k+y)<iNbRow && (m+x)<iNbCol ) {
							if (iSceneArray[k+y][m+x] == 9){
								iSceneArray[k][m]++;
							}
						}						
					}
				}
			}
		}
	}
	// Masquage des cellules
	for(int k=0;k<iNbRow;k++){
		for(int m=0;m<iNbCol;m++){
			iSceneArray[k][m] += SCENE_CELL_MASK_OFFSET;
		}
	}

	// Calcul du nombre de cellules à découvrir
	int iNbCellsToDiscover = 0;
	for(int k=0;k<iNbRow;k++){
		for(int m=0;m<iNbCol;m++){
			iNbCellsToDiscover++;
		}
	}
	iNbCellsToDiscover-=iNbMines;
	return iNbCellsToDiscover;
}

void SceneDisplay(int iSceneArray[][SCENE_NB_COL_MAX], int iNbRow, int iNbCol){
	int k, m;

	// Dizaines
	printf("\r\n");
	printf("   ");
	for(m=0;m<iNbCol;m++){
		if(m%10==0) printf("%2d ", m/10);
		else printf("   ");
	}
	printf("\r\n");

	// Numéro de colonnes
	printf("   ");
	for(m=0;m<iNbCol;m++){
		printf("%2d ", m%10);
	}
	printf("\r\n");

    // Affichage des "-"
    printf("   ");
	for(m=0;m<iNbCol;m++){
		printf("---");
	}
	printf("\r\n");

	for(k=0;k<iNbRow;k++){
		// Numéro de lignes
		printf("%2d", k);
        // Affichage des "|"
        printf("|");
		// Affichage des éléments
		for(m=0;m<iNbCol;m++){
			if (iSceneArray[k][m]<SCENE_CELL_MASK_OFFSET){
				if (iSceneArray[k][m] == SCENE_CELL_VOID_VALUE) {
					printf(" %c ", SCENE_CELL_VOID_CHAR);
				}
				else if (iSceneArray[k][m] == SCENE_MINE_VALUE) {
					printf(" %c ", SCENE_MINE_CHAR);
				}
				else {
					printf("%2d ", iSceneArray[k][m]);
				}
			}
			else{
				if (iSceneArray[k][m]>=SCENE_CELL_MASK_OFFSET && iSceneArray[k][m]<SCENE_CELL_MARK_M_OFFSET){
					printf(" . ");
				}
				else if (iSceneArray[k][m]>=SCENE_CELL_MARK_M_OFFSET && iSceneArray[k][m]<SCENE_CELL_MARK_I_OFFSET){
					printf(" M ");
				} 
				else if (iSceneArray[k][m]>=SCENE_CELL_MARK_I_OFFSET) {
					printf(" ? ");
				}
			}
		}
        // Affichage des "|"
        printf("|");
        // Numéro de lignes
		printf("%d\r\n", k);
	}

    // Affichage des "-"
    printf("   ");
	for(m=0;m<iNbCol;m++){
		printf("---");
	}

	// Numéro de colonnes
	printf("\r\n");
	printf("   ");
	for(m=0;m<iNbCol;m++){
		printf("%2d ", m%10);
	}
	printf("\r\n");

	printf("   ");
	for(m=0;m<iNbCol;m++){
		if(m%10==0) printf("%2d ", m/10);
		else printf("   ");
	}
	printf("\r\n");
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
				case 'H':
				case 'h':
					iReturnAction=4;
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

int SceneDiscoverCell(int iSceneArray[][SCENE_NB_COL_MAX], int iNbRow, int iNbCol, int iAtRow, int iAtCol, int *iNbDiscoversCells){
	
	// Si la case est déjà découverte
	if (iSceneArray[iAtRow][iAtCol] < SCENE_CELL_MASK_OFFSET){
		return 0;
	}
	else {
		// On démasque la case
		iSceneArray[iAtRow][iAtCol]-=SCENE_CELL_MASK_OFFSET;
		// Chose curieuse : j'ai remarqué que l'on ne pouvait pas faire ++ sur un pointeur, 
		// j'ai donc utilisé cette notation
		*iNbDiscoversCells += 1;
		// Si il s'agit d'une mine
		if (iSceneArray[iAtRow][iAtCol] == SCENE_MINE_VALUE) {
			return -1;
		}
		// Si la case a une valeur d'adjacence non nulle
		if (iSceneArray[iAtRow][iAtCol] > SCENE_CELL_VOID_VALUE){
			return 1;
		}
		// Si la case vaut 0
		if (iSceneArray[iAtRow][iAtCol] == SCENE_CELL_VOID_VALUE){
			// Recherche
			// x : Coordonnée de colonne relative à la case sélectionnée
			// y : Coordonnée de ligne relative à la case sélectionnée
			for (int y=-1; y<=1; y++){
				for (int x=-1; x<=1; x++){
					// On vérifie les collisions
					if ( (iAtRow+y>=0) && (iAtCol+x>=0) && (iAtRow+y<iNbRow) && (iAtCol+x<iNbCol) ) {
						// Si la case est masquée
						if (iSceneArray[iAtRow+y][iAtCol+x] >= SCENE_CELL_MASK_OFFSET){
							// Si la case vaut 0 (on rajoute l'offset car la case testée n'est pas encore démasquée)
							if (iSceneArray[iAtRow+y][iAtCol+x] == (SCENE_CELL_VOID_VALUE + SCENE_CELL_MASK_OFFSET) ){
								SceneDiscoverCell(iSceneArray, iNbRow, iNbCol, iAtRow+(y), iAtCol+(x), iNbDiscoversCells);
							}
							else {
								// On démasque la case
								iSceneArray[iAtRow+y][iAtCol+x]-=SCENE_CELL_MASK_OFFSET;
								*iNbDiscoversCells += 1;
							}
						}	
					}
				}
			}
		}
	}
	return 0;
}

void SceneUnmaskCells(int iSceneArray[][SCENE_NB_COL_MAX], int iNbRow, int iNbCol){
	for(int k=0;k<iNbRow;k++){
		for(int m=0;m<iNbCol;m++){
			if (iSceneArray[k][m]>=SCENE_CELL_MASK_OFFSET && iSceneArray[k][m] < SCENE_CELL_MARK_M_OFFSET) {
				iSceneArray[k][m] -= SCENE_CELL_MASK_OFFSET;
			}
			else if (iSceneArray[k][m]>=SCENE_CELL_MARK_M_OFFSET && iSceneArray[k][m] < SCENE_CELL_MARK_I_OFFSET){
				iSceneArray[k][m] -= SCENE_CELL_MARK_M_OFFSET;
			}
			else if (iSceneArray[k][m]>=SCENE_CELL_MARK_I_OFFSET){
				iSceneArray[k][m] -= SCENE_CELL_MARK_I_OFFSET;
			}
		}
	}
}

void SceneToggleMarkCell(int iSceneArray[][SCENE_NB_COL_MAX], int iAtRow, int iAtCol, int iMarkOffset){
	if (iSceneArray[iAtRow][iAtCol] < SCENE_CELL_MASK_OFFSET){
		return;
	}
	else{
		// Si il n'y a pas de marquage
		if (iSceneArray[iAtRow][iAtCol] < SCENE_CELL_MARK_M_OFFSET){
			// On modifie l'offset pour rendre correcte la valeur de la case lorsqu'on va la modifier
			iMarkOffset-=SCENE_CELL_MASK_OFFSET;
			iSceneArray[iAtRow][iAtCol]+=iMarkOffset;
			return;
		}
		// Si on demande d'enlever le marquage M
		if (iSceneArray[iAtRow][iAtCol]>=SCENE_CELL_MARK_M_OFFSET && (iSceneArray[iAtRow][iAtCol]<SCENE_CELL_MARK_I_OFFSET) && (iMarkOffset==SCENE_CELL_MARK_M_OFFSET) ){
			iMarkOffset-=SCENE_CELL_MASK_OFFSET;
			iSceneArray[iAtRow][iAtCol]-=iMarkOffset;
			return;
		}
		// Si on demande d'enlever le marquage ?
		if (iSceneArray[iAtRow][iAtCol]>=SCENE_CELL_MARK_I_OFFSET && (iMarkOffset==SCENE_CELL_MARK_I_OFFSET)){
			iMarkOffset-=SCENE_CELL_MASK_OFFSET;
			iSceneArray[iAtRow][iAtCol]-=iMarkOffset;
			return;
		}
		// Si on demande de mettre un marquage ? alors qu'il y a déjà un M
		if (iSceneArray[iAtRow][iAtCol]>=SCENE_CELL_MARK_M_OFFSET && (iSceneArray[iAtRow][iAtCol]<SCENE_CELL_MARK_I_OFFSET) && (iMarkOffset==SCENE_CELL_MARK_I_OFFSET)){
			iSceneArray[iAtRow][iAtCol]-=SCENE_CELL_MARK_M_OFFSET;
			iSceneArray[iAtRow][iAtCol]+=iMarkOffset;
			return;
		}
		// Si on demande de mettre un marquage M alors qu'il y a déjà un ?
		if (iSceneArray[iAtRow][iAtCol]>=SCENE_CELL_MARK_I_OFFSET && (iMarkOffset==SCENE_CELL_MARK_M_OFFSET)){
			iSceneArray[iAtRow][iAtCol]-=SCENE_CELL_MARK_I_OFFSET;
			iSceneArray[iAtRow][iAtCol]+=iMarkOffset;
			return;
		}
	}
}
