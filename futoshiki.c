#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

enum operateur {
	DIF, SUP, INF
};

typedef struct CASE {
	size_t x;
	size_t y;
	int * domaine;
	// indices de toutes les contraintes de la case dans le tableau contraintes
	size_t * conts;
	// indice du dernier indice ajouter au tableau precedant
	size_t indLastConst;
} CASE;

typedef struct CONTRAINTE {
	enum operateur op;
	CASE * droite;
	CASE * gauche;
} CONTRAINTE;

size_t nbContraintes;
CONTRAINTE * contraintes;

CASE * grid;

size_t lineSize;

FILE * openFile(char * fileName) {
	FILE * grille = fopen(fileName, "r");

	return grille;
}

void closeFile(FILE * file) {
	fclose(file);
}

char getOpChar(enum operateur op) {
	switch(op) {
		case DIF :
			return '!';
		case SUP :
			return '<';
		case INF :
			return '>';
		default :
			return 'E';
	}
}

void displayConstraint(size_t numConst, CONTRAINTE * constr) {
	printf("CONSTRAINT num %zu\n", numConst);
	char op = getOpChar(constr->op);
	printf("    (%zu,%zu)", constr->gauche->x, constr->gauche->y);
	printf(" %c ", op);
	printf("(%zu,%zu)\n", constr->droite->x, constr->droite->y);	
}

void displayCase (size_t numCase, CASE *caseToDisplay) {
	printf("CASE num %zu\n", numCase);
	printf("    x = %zu\n", caseToDisplay->x);
	printf("    y = %zu\n", caseToDisplay->y);
	printf("    domaine = {");
	int i;
	for(i = 0; i < lineSize; ++i) {
		printf("%d, ", caseToDisplay->domaine[i]);
	}
	printf("}\n");
}

void displayGrid() {
	size_t i;
	for (i = 0; i < lineSize * lineSize; ++i) {
		CASE * currentCase = &grid[i];
		displayCase(i, currentCase);
		printf("\n");
	}
}

void initAllCases(CASE * grid) {
	int comptLine = -1;
	size_t i;
	for (i = 0; i < lineSize * lineSize; ++i) {
	
		if (i % lineSize == 0)
			comptLine++;
		
		CASE currentCase;
		currentCase.y = i % lineSize;
		currentCase.x = comptLine;

		// Initialisation du domaine
		currentCase.domaine = malloc(lineSize * sizeof (int));
		size_t m;
		for (m = 0; m < lineSize; ++m) {
			currentCase.domaine[m] = m + 1;
		}

		currentCase.conts = malloc((2 * (lineSize - 1)) * sizeof (int));
		currentCase.indLastConst = 0;
		
		grid[i] = currentCase;
	}
}

void initBasicConstraints(CASE *grid) {
	
	size_t numCurrentCase = 0;
	size_t indDebLigne = 0;
	size_t lastConstInd = 0;
	
	for (numCurrentCase = 0; numCurrentCase < nbContraintes; ++numCurrentCase) {
		CASE * currentCase = &grid[numCurrentCase];
		
		displayCase(numCurrentCase, currentCase);
		printf("\n");

		// Recherche de l'indice de la case de debut de la ligne
		while (!(indDebLigne == currentCase->x * lineSize)) {

			indDebLigne += lineSize;
		}		
		
		printf("indDebLigne : %zu\n", indDebLigne);

		// Ajout de toutes les contraintes de la ligne courante a 
		// partir de la case courrante car les cases precedentes ont deja mis
		// les contraintes sur celle-ci
		size_t indCaseDroite = indDebLigne + currentCase->y;
		for (; indCaseDroite < indDebLigne + lineSize; ++indCaseDroite) {

			if (numCurrentCase != indCaseDroite) {
				CASE * caseDroite = &grid[indCaseDroite];
				
				CONTRAINTE cont;
				cont.gauche = currentCase;
				cont.droite = caseDroite;
				cont.op = DIF;
				
				contraintes[lastConstInd] = cont;
				currentCase->conts[currentCase->indLastConst] = lastConstInd;
				caseDroite->conts[currentCase->indLastConst] = lastConstInd;
				
				displayConstraint(lastConstInd, &cont);
				lastConstInd++;
			}
		}
		
		// Ajout de toutes les contraintes de la colonne courante
		size_t colNumber = 1;
		for (; colNumber < lineSize; ++colNumber) {

			CASE * caseDroite = &grid[(colNumber + currentCase->y) * lineSize - 1];

			if (caseDroite != currentCase) {
				CONTRAINTE cont;
				cont.gauche = currentCase;
				cont.droite = caseDroite;
				cont.op = DIF;
				
				contraintes[lastConstInd] = cont;
				currentCase->conts[currentCase->indLastConst] = lastConstInd;
				caseDroite->conts[currentCase->indLastConst] = lastConstInd;
				//lastConstInd++;
			}
		}
	}
	
	printf("nbConstr : %zu, lastConst : %zu\n", nbContraintes, lastConstInd);
}

int initGrid(FILE *file) {
	char * line = NULL;
	size_t len = 0;
	ssize_t read;

	read = getline(&line, &len, file);
	if (read <= 0) {
		fprintf(stderr, "Grid file empty.\n");
		return -1;
	}

	if(sscanf(line, "%zu", &lineSize) != 1) {
		fprintf(stderr, "Grid size must be a number.\n");
		return -1;
	}
	
	printf("Grid size : %zu\n", lineSize);

	grid = malloc((lineSize * lineSize) * sizeof(CASE));

	nbContraintes = ((2 * (lineSize - 1)) * lineSize * lineSize);
	contraintes = malloc(nbContraintes * sizeof (CONTRAINTE));

	// initialisation des cases
	initAllCases(grid);

	printf("Cases initialized\n");
	displayGrid();
	printf("\n");
	// initialisation des contraintes
	initBasicConstraints(grid);

	int isLineWithNumber = 1;
	/*numCurrentCase = 0;
	while ((read = getline(&line, &len, file)) != -1) {
		printf("Retrieved line of length %zu :\n", read);
		printf("%s\n", line);
 
		//int isNumDecide = 0;
		int j = 0;
		for (; j < len; j++){
			if (line[j] != ' '){
				if (line[j] == '>' || line[j] == '<'){
	    			
					/*int k = 0;
					for (; k < gridSize * gridSize; k++){
						if (cases[k].x == j - 1){
	    				
						}
							//contraintes
					}/*
				}
				else {
					if (isLineWithNumber) {
						/*CASE thisCase;
						thisCase.x = compt;
						thisCase.y = j / 2;

						if (!isNumDecide){
							int dom[gridSize];
							int i;
							for (i = 0; i < gridSize; i++)
								dom[i] = i + 1;
							thisCase.domaine = dom;
						}
						else {
							int dom[1];
							dom = current;
							thisCase.domaine = dom;
						}/*
							    	
						isLineWithNumber = 0;
						numCurrentCase++;
					}
					else {
				
						isLineWithNumber = 1;
					}
				}
			}
		}
        
		compt++;
	}*/

	if (line)
		free(line);
	
	return 1;
}

// Penser a tout free parce que j'ai tout compris

int main(int argc, char * argv[]) {
	if (argc - 1 < 1) {
		fprintf(stderr, "Error on argument. Usage :\nfutoshiki <GridfileName>\n");
		exit(EXIT_FAILURE);
	}

	FILE *gridFile = openFile(argv[1]);
	if (gridFile == NULL) {
		fprintf(stderr, "Impossible d'ouvrir le fichier\n");
		exit(EXIT_FAILURE);
	}

	int res = initGrid(gridFile);
	if (res < 0) {
		fprintf(stderr, "Erreur de creation de la grille\n");
		exit(EXIT_FAILURE);
	}

	return (0);
}
