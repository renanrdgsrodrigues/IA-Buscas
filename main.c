/* Renan Rodrigues, nUSP: 9278132 */

#include "main.h"

//Struct que representa cada posição da matriz de entrada
struct no_ {
	char charac; //Caracter armazenado no nó (campo livre (*), barreira (-), início (#), fim ($)
	int visited; //0 = nao visitado; 1 = visitado
	int coordX;  //Coordenada X deste nó
	int coordY;  //Coordenada Y deste nó
	double distToFinal; // h(n) -> Distancia do nó ao nó meta
	double distTotal; //g(n) -> distancia do nó ao nó meta somado com a distancia do nó pai dele
	struct no_* father; //Ponteiro para o nó pai para guiar na impressão do caminho resposta
};


struct queue_ {
	int coordX;
	int coordY;
};



int main ( int argc, char** argv ) {
	FILE* fp = NULL;
	NODE** no = NULL;
	int line, col, initialX, finalX, initialY, finalY;

	fp = openFile ( );
	getSides ( fp, &line, &col );

	no = allocMat ( line, col );
	initializeMat ( fp, no );
	getInitialFinalCoord ( no, line, col, &initialX, &initialY, &finalX, &finalY );

	menu ( no, line, col, initialX, initialY, finalX, finalY );
	

	allFree ( fp, no, line );

	return 0;
}


void menu ( NODE** no, int line, int col, int initialX, int initialY, int finalX, int finalY ) {
	int op;
	clock_t time[2];
	printf ( "Escolha uma forma de busca:\n" );
	printf ( "1 - BFS (busca em largura).\n2 - DFS (busca em profundidade).\n3 - Best First.\n4 - A Estrela (A*).\n" );
	scanf("%d", &op );

	switch ( op ) {
		case 1 : time[0] = clock();
			 BFS ( no, line, col, initialX, initialY );		
		 	 time[1] = clock();
			 printf("TEMPO = %gms\n", (time[1] - time[0])*1000.0/CLOCKS_PER_SEC);
			
			break;

		case 2 : time[0] = clock(); 
			 DFS ( no, line, col, initialX, initialY );
			 time[1] = clock();
			 printf("TEMPO = %gms\n", (time[1]-time[0])*1000.0/CLOCKS_PER_SEC);
			
			break;

		case 3 : time[0] = clock(); 
			 if ( !BestFirst ( no, line, col, initialX, initialY, finalX, finalY ) ) printf ( "\nNao encontrado.\n\n" );
			 time[1] = clock();
			 printf("TEMPO = %gms\n", (time[1]-time[0])*1000.0/CLOCKS_PER_SEC);
			
			break;

		case 4 : time[0] = clock(); 
			 if ( !AStar ( no, line, col, initialX, initialY, finalX, finalY ) ) printf ( "\nNao encontrado.\n\n" );
			 time[1]=clock();
			 printf("TEMPO = %gms\n", (time[1]-time[0])*1000.0/CLOCKS_PER_SEC);
			
			break;

		default : printf ( "Opcao nao aceita.\n" );
			break;
	}
}



/************************************************************************************************************************/
/*********************************** FUNÇÕES AUXILIARES *****************************************************************/

//Função que abre o arquivo
FILE* openFile ( ){
	FILE* fp;
	char fileName[20];
	
	printf ( "Insira o nome do arquivo txt (com a extensao) : " ); 
	scanf ( "%s", fileName );

	fp = fopen ( fileName, "r" );
	if ( fp == NULL ) {
		printf ( "Erro ao abrir o arquivo.\n");
		exit( 0 );
	}
	printf ( "Arquivo aberto com sucesso.\n" );

	return fp;
}


//Função que conta os caracteres do arquivo
void getSides ( FILE* fp, int* line, int* col ){
	char gc; //"GetChar"
	
	fscanf ( fp, "%d", &(*line) ); //Le o caracter e transforma em inteiro
	gc = fgetc ( fp ); //Recebe o espaço entre os valores da linha e coluna	
	fscanf ( fp, "%d", &(*col) ); //Le o caracter e transforma em inteiro
	gc = fgetc ( fp ); //Recebe o \n da primeira linha
}


NODE** allocMat ( int line, int col ){
	int i;
	NODE** no = NULL;
	
	no = ( NODE** )malloc( sizeof(NODE*) * line );
	if ( no == NULL ) {
		printf ( "Erro ao alocar o vetor.\n" );
		exit( 0 );
	}

	for ( i = 0; i < line; i++ ) {
		no[i] = ( NODE* )malloc( sizeof(NODE) * col );
		if ( no[i] == NULL ) {
			printf ( "Erro ao alocar o vetor %d.\n", i );
			exit( 0 );
		}
	}

	return no;
}


void initializeMat ( FILE* fp, NODE** no ) {
	int i = 0, j = 0;
	char gc; //"GetChar"
	
	//Le o arquivo enquanto nao encontrar seu final
	while ( (gc = fgetc( fp )) != EOF ) {
		if ( gc != '\n' ) { //Se o caracter lido nao for '\n', é um caracter válido		
			no[i][j].father = NULL;
			no[i][j].charac = gc;
			no[i][j].coordX = 0;
			no[i][j].coordY = 0;
			no[i][j].visited = 0;
			no[i][j].distToFinal = 0;
			no[i][j].distTotal = 0;
			j++;
		}
		else { //Se o caracter lido for um '\n', pula para a próxima linha e voltar para a coluna 0
			i++;
			j = 0;
		}
	}	
}


void getInitialFinalCoord ( NODE** no, int line, int col, int* initialX, int* initialY, int* finalX, int* finalY ) {
	int i, j;

	for ( i = 0; i < line; i++ ) {
		for ( j = 0; j < col; j++ ) {
			if ( no[i][j].charac == '#' ) {
				( *initialX ) = i;
				( *initialY ) = j;
			}

			else if ( no[i][j].charac == '$' ) {
				( *finalX ) = i;
				( *finalY ) = j;
			}
		}
	}
}


void allFree ( FILE* fp, NODE** no, int line ) {
	int i;
	
	//Libera toda a matriz
	for ( i = 0; i < line; i++ ) {
		free( no[i] );
	}
	free ( no );
	
	//Fecha o arquivo
	fclose ( fp );
}


void printMat ( NODE** no, int line, int col ) {
	int i, j;

	for ( i = 0; i < line; i++ ) {
		for ( j = 0; j < col; j++ ) {
			printf ( "POS[%d][%d] = %c <<<>>> VIS = %d\n", i, j, no[i][j].charac, no[i][j].visited );
		}
		printf ( "\n" );
	}
}


void printCoords ( NODE** no, int posX, int posY ) {
	NODE* aux = no[posX][posY].father;
	QUEUE* queue = NULL;
	int i = 0, j;

	queue = ( QUEUE* )malloc( sizeof( QUEUE ) );

	queue[i].coordX = no[posX][posY].coordX;
	queue[i].coordY = no[posX][posY].coordY;
	
	while ( aux->father != NULL ){
		i++;
		queue = ( QUEUE* )realloc( queue, sizeof( QUEUE ) * ( i + 1) );
		queue[i].coordX = aux->coordX;				
		queue[i].coordY = aux->coordY;

		aux = aux->father;
	}

	i++;
	queue = ( QUEUE* )realloc( queue, sizeof( QUEUE ) * (i + 1) );
	queue[i].coordX = aux->coordX;
	queue[i].coordY = aux->coordY;

	printf ( "[");
	for ( j = i; j >= 0; j-- ) printf ( " (%d,%d)", queue[j].coordX, queue[j].coordY );
	printf(" ] \n");

	free( queue );
}


void resetVetAux ( int* vetAux, int size ) {
	int i;

	for ( i = 0; i < size; i++ ) vetAux[i] = -1;
}


void checkPositions ( NODE** no, int* vetAux, int line, int col, int posX, int posY ) {		
	if( upIsOk(posX) ){
		if( !(no[posX - 1][posY].visited) ){
			no[posX - 1][posY].visited = 1;
			no[posX - 1][posY].father = &no[posX][posY];
			vetAux[0] = posX - 1;
			vetAux[1] = posY;				
		}
	}
		
	if( upIsOk(posX) && rightIsOk(col, posY) ){
		if( !(no[posX - 1][posY + 1].visited) ){
			no[posX - 1][posY + 1].visited = 1;
			no[posX - 1][posY + 1].father = &no[posX][posY];	
			vetAux[2] = posX - 1;
			vetAux[3] = posY + 1;
		}
	}

	if( rightIsOk(col, posY) ){
		if( !(no[posX][posY + 1].visited) ){
			no[posX][posY + 1].visited = 1;
			no[posX][posY + 1].father = &no[posX][posY];	
			vetAux[4] = posX;
			vetAux[5]= posY + 1;
		}
	}

	if( rightIsOk(col, posY) && downIsOk(line, posX) ){
		if( !(no[posX + 1][posY + 1].visited) ){
			no[posX + 1][posY + 1].visited = 1;
			no[posX + 1][posY + 1].father = &no[posX][posY];	
			vetAux[6] = posX + 1;
			vetAux[7] = posY + 1;
		}
	}

	if( downIsOk(line, posX) ){
		if( !(no[posX + 1][posY].visited) ) {
			no[posX + 1][posY].visited = 1;
			no[posX + 1][posY].father = &no[posX][posY];	
			vetAux[8] = posX + 1;
			vetAux[9] = posY;
		}
	}

	if( (downIsOk(line, posX)) && (leftIsOk(posY)) ){
		if( !(no[posX + 1][posY - 1].visited) ){
			no[posX + 1][posY - 1].visited = 1;
			no[posX + 1][posY - 1].father = &no[posX][posY];	
			vetAux[10] = posX + 1;
			vetAux[11] = posY - 1;
		}
	}

	if( leftIsOk(posY) ){
		if( !(no[posX][posY - 1].visited) ){
			no[posX][posY - 1].visited = 1;
			no[posX][posY - 1].father = &no[posX][posY];	
			vetAux[12] = posX;
			vetAux[13] = posY - 1;
		}
	}

	if( (leftIsOk(posY)) && (upIsOk(posX)) ){
		if( !(no[posX - 1][posY - 1].visited) ){
			no[posX - 1][posY - 1].visited = 1;
			no[posX - 1][posY - 1].father = &no[posX][posY];	
			vetAux[14] = posX - 1;
			vetAux[15] = posY - 1;
		}
	}
}


int upIsOk(int posX){
	if(posX - 1 >= 0) return 1;

	return 0;
}


int rightIsOk(int col, int posY){
	if(posY + 1 < col) return 1;

	return 0;
}


int downIsOk(int line, int posX){
	if(posX + 1 < line) return 1;

	return 0;
}


int leftIsOk(int posY){
	if(posY - 1 >= 0) return 1;

	return 0;
}


void initializeVet ( int* vetAux, int size ) {
	int i;

	for ( i = 0; i < size; i++ ) vetAux[i] = -1;
}



/*************************************************************************************************************************************************/
/****************************************************************** BFS **************************************************************************/
void BFS ( NODE** no, int line, int col, int posX, int posY ) {
	NODE* queue = NULL;
	int i, next = 1, countInsert = 1;
	int vetAux[VIZ];
	int passo = 0;

	no[posX][posY].visited = 1;
	no[posX][posY].coordX = posX;
	no[posX][posY].coordY = posY;

	queue = allocQueue( );
	insertQueue ( queue, &countInsert, posX, posY );

	initializeVet ( vetAux, VIZ );
	while ( !emptyQueue ( next, countInsert ) ) {
		passo++;	
		if ( no[posX][posY].charac == '*' || no[posX][posY].charac == '#' ) {
			checkPositions ( no, vetAux, line, col, posX, posY ); 
			for ( i = 0; i < VIZ; i += 2 ) {
				
				if ( vetAux[i] > -1 ) {
					no[vetAux[i]][vetAux[i + 1]].coordX = vetAux[i];
					no[vetAux[i]][vetAux[i + 1]].coordY = vetAux[i + 1];
					insertQueue ( queue, &countInsert, vetAux[i], vetAux[i + 1] );
				}
	
			}
			resetVetAux ( vetAux, VIZ );
		}

		else if ( no[posX][posY].charac == '$' ) {
			printf ( "Encontrado.\n" );
			break;
		}

		posX = queue[next].coordX;
		posY = queue[next].coordY;
		next++;
	}
	printf ("\nPASSOS = %d\n\n", passo);
	printCoords ( no, posX, posY );
	freeBFS ( queue );
}


void freeBFS ( NODE* queue ) {
	free ( queue );
}


int emptyQueue ( int next, int countInsert ) {
	if ( next >= countInsert ) return 1;

	return 0;
}


NODE* allocQueue ( ) {
	NODE* queue = NULL;
	
	queue = ( NODE* )malloc( sizeof( NODE ) );
	if( queue == NULL ){
		printf ( "Erro ao alocar a lista.\n" );
		exit ( 1 );
	}

	return queue;
}


void insertQueue ( NODE* queue, int* count, int posX, int posY) { 
	queue[( *count ) - 1].coordX = posX;	
	queue[( *count ) - 1].coordY = posY;
	
	queue = ( NODE* )realloc( queue, sizeof( NODE ) * ( ( *count ) + 1) );
	( *count )++;
}



/*************************************************************************************************************************************************/
/******************************************************************* DFS *************************************************************************/
void DFS ( NODE** no, int line, int col, int posX, int posY ) {
	NODE* stack = NULL;
	int i, next = 1, current = 0, top = -1;
	int vetAux[VIZ];
	int passo = 0;

	no[posX][posY].visited = 1;
	no[posX][posY].coordX = posX;
	no[posX][posY].coordY = posY;
	
	stack = allocStack( line, col );
	insertStack ( stack, &top, posX, posY );

	initializeVet ( vetAux, VIZ );
	do {
		passo++;
		if ( no[posX][posY].charac == '*' || no[posX][posY].charac == '#' ) {
			checkPositions ( no, vetAux, line, col, posX, posY );
			
			for ( i = VIZ - 1; i >= 0; i -= 2 ) {
				if ( vetAux[i] > -1 ) {
					no[vetAux[i - 1]][vetAux[i]].coordX = vetAux[i - 1];
					no[vetAux[i - 1]][vetAux[i]].coordY = vetAux[i];
					insertStack ( stack, &top, vetAux[i - 1], vetAux[i] );
				}	
			}

			resetVetAux ( vetAux, VIZ );
		}

		else if ( no[posX][posY].charac == '$' ) {
			printf ( "Encontrado.\n" );
			break;
		}

		posX = stack[top].coordX;
		posY = stack[top].coordY;
		top--;

	}while ( !emptyStack ( top ) );

	printf ("\nPASSOS = %d\n\n", passo);
	printCoords ( no, posX, posY );

	freeDFS ( stack );
}


void freeDFS ( NODE* stack ) {
	free ( stack );
}


int emptyStack ( int top ) {
	if ( top == 0 ) return 1;
	
	return 0;
}


void insertStack ( NODE* stack, int* top, int posX, int posY ) {
	(*top)++;
	stack[ (*top) ].coordX = posX;
	stack[ (*top) ].coordY = posY;
}


NODE* allocStack ( int line, int col ) {
	NODE* stack = ( NODE* )malloc( sizeof(NODE) * (line*col) );
	if ( stack == NULL ) {
		printf ( "Erro ao alocar pilha.\n" );
		exit(1);
	}
	return stack;
}



/**************************************************************************************************************************************************/
/******************************************************************* BEST FIRST *******************************************************************/
int BestFirst ( NODE** no, int line, int col, int posX, int posY, int finalX, int finalY ) {
	int vetAux[16];
	int menor = INF;
	int i, j;
	int passo = 0;

	no[posX][posY].visited = 1;
	no[posX][posY].coordX = posX;
	no[posX][posY].coordY = posY;

	calcDistToFinal ( no, line, col, finalX, finalY );

	initializeVet ( vetAux, VIZ );
	while ( no[posX][posY].distToFinal != 0 ) {
		passo++;	
		menor = INF;
		checkPositions ( no, vetAux, line, col, posX, posY );	

		for ( i = 0; i < VIZ; i += 2 ) {
			if ( vetAux[i] > -1 ) {
	
				no[vetAux[i]][vetAux[i + 1]].coordX = vetAux[i];
				no[vetAux[i]][vetAux[i + 1]].coordY = vetAux[i + 1];

				if ( no[vetAux[i]][vetAux[i + 1]].charac != '-' ) {
					if ( menor > no[vetAux[i]][vetAux[i + 1]].distToFinal ) {
						menor = no[vetAux[i]][vetAux[i + 1]].distToFinal;
						
						posX = vetAux[i];
						posY = vetAux[i + 1];
					}
				}

			}
		}

		resetVetAux ( vetAux, VIZ );

		if ( no[posX][posY].charac == '$') {
			printf ( "\nEncontrado.\n");
			printf ("\nPASSOS = %d\n\n", passo);
			printCoords ( no, posX, posY );
			
			return 1;
		}
	}

	return 0;
}


void calcDistToFinal ( NODE** no, int line, int col, int finalX, int finalY ) {
	int i, j;
	int distX = 0, distY = 0;

	for ( i = 0; i < line; i++ ) {
		for ( j = 0; j < col; j++ ) {
			/* USO DA DISTANCIA DE MANHATTAN */
			distX = finalX - i;
			if ( distX < 0 ) distX *= -1; //norma da diferença da coordenada X
			distY = finalY - j;	
			if ( distY < 0 ) distY *= -1; //norma da diferença da coordenada y			
			no[i][j].distToFinal = (double) (distX + distY);
		}
	}
}



/**************************************************************************************************************************************************/
/*********************************************************** A ESTRELA (A*) ***********************************************************************/
int AStar ( NODE** no, int line, int col, int posX, int posY, int finalX, int finalY ) {
	int vetAux[VIZ];
	int i, auxPosX, auxPosY;
	double menor = INF, menor2 = INF;
	double coordMenor2[2], coordAux[3] = { INF, 0.0, 0.0 }; //Ambos vetores possuem as coordenadas de um nó. CoordAux[0] é a distância
	int passo = 0;

	initializeVet ( vetAux, VIZ );
	calcDistToFinal ( no, line, col, finalX, finalY );

	no[posX][posY].distTotal = no[posX][posY].distToFinal; //primeiro no tem distancia 0 dele mesmo
	no[posX][posY].visited = 1;

	while ( no[posX][posY].distToFinal != 0 ) {
		passo++;
		if ( no[posX][posY].charac == '*' || no[posX][posY].charac == '#' ) {
			menor = INF;
			menor2 = INF;

			checkPositions ( no, vetAux, line, col, posX, posY );	
			for ( i = 0; i < VIZ; i += 2 ) {
				if ( vetAux[i] > -1 ) {

					if ( no[vetAux[i]][vetAux[i + 1]].charac != '-' ) {
						no[vetAux[i]][vetAux[i + 1]].coordX = vetAux[i];
						no[vetAux[i]][vetAux[i + 1]].coordY = vetAux[i + 1];
						calcTotalDist ( no, posX, posY, vetAux[i], vetAux[i + 1] );	

						if ( no[vetAux[i]][vetAux[i + 1]].distTotal < menor ) {
							menor2 = menor;
							menor = no[vetAux[i]][vetAux[i + 1]].distTotal;
						
							auxPosX = vetAux[i];
							auxPosY = vetAux[i + 1];
						}

						else if ( no[posX][posY].distTotal < menor2 ) {
							menor2 = no[posX][posY].distTotal;
							coordMenor2[0] = vetAux[i];
							coordMenor2[1] = vetAux[i + 1];
						}
					}

				}
			}

			posX = auxPosX;
			posY = auxPosY;

			function ( coordAux, coordMenor2, menor, menor2, posX, posY );
		
		}
		resetVetAux ( vetAux, VIZ );
		if ( no[posX][posY].distToFinal == 0 ) {
			printf ( "Encontrado.\n" );
			printf ("\n%d\n\n", passo);
			printCoords ( no, posX, posY );

			return 1;
		}
	}

	return 0;
}


void function ( double* coordAux, double* coordMenor2, double menor, double menor2, int posX, int posY ){
	double aux;

	if ( menor > coordAux[0]) {
		coordAux[0] = menor;
				
		aux = posX;
		posX = coordAux[1];
		coordAux[1] = aux;

		aux = posY;
		posY = coordAux[2];
		coordAux[2] = aux;
		}	
	else {
		coordAux[0] = menor2;
		coordAux[1] = coordMenor2[0];
		coordAux[2] = coordMenor2[1];
	}
}


void calcTotalDist ( NODE** no, int posX, int posY, int coordXSon, int coordYSon ) {
	if ( (coordXSon == posX - 1) && (coordYSon == posY) ) {
		no[coordXSon][coordYSon].distTotal = no[coordXSon][coordYSon].distToFinal + 1; //distancia para cima
	}

	else if ( (coordXSon == posX - 1) && (coordYSon == posY + 1) ) {
		no[coordXSon][coordYSon].distTotal = no[coordXSon][coordYSon].distToFinal + sqrt(2); //distancia para diagonal superior direita
	}

	else if ( (coordXSon == posX) && (coordYSon == posY + 1) ) {
		no[coordXSon][coordYSon].distTotal = no[coordXSon][coordYSon].distToFinal + 1; //distancia para direita
	}

	else if ( (coordXSon == posX + 1) && (coordYSon == posY + 1) ) {
		no[coordXSon][coordYSon].distTotal = no[coordXSon][coordYSon].distToFinal + sqrt(2);//distancia para diagonal inferior direita
	}

	else if ( (coordXSon == posX + 1) && (coordYSon == posY) ) {
		no[coordXSon][coordYSon].distTotal = no[coordXSon][coordYSon].distToFinal + 1;//distancia para baixo
	}

	else if ( (coordXSon == posX + 1) && (coordYSon == posY - 1) ) {
		no[coordXSon][coordYSon].distTotal = no[coordXSon][coordYSon].distToFinal + sqrt(2);//distancia para diagonal inferior esquerda
	}

	else if ( (coordXSon == posX) && (coordYSon == posY - 1) ) {
		no[coordXSon][coordYSon].distTotal = no[coordXSon][coordYSon].distToFinal + 1; //distancia para esquerda
	}

	else if ( (coordXSon == posX - 1) && (coordYSon == posY - 1) ) {
		no[coordXSon][coordYSon].distTotal = no[coordXSon][coordYSon].distToFinal + sqrt(2);//disntacia para diagonal superior esquerda
	}
}
