/* Renan Rodrigues, nUSP: 9278132 */

#ifndef main_h
#define main_h

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define INF 32999.0
#define VIZ 16 //número máximo de coordenadas para todos os vizinhos (8 vizinhos = 16 coordenadas)



/************** ESTRUTURAS USADAS ****************/
typedef struct no_ NODE;

typedef struct queue_ QUEUE;




/***************** FUNÇÕES BÁSICAS PARA LEITURA DO ARQUIVO E INICIALIZAÇÃO DA MATRIZ **********************/

//Abre o arquivo para leitura
FILE* openFile ( );

//Le as dimensões da matriz
void getSides ( FILE*, int*, int* );

//Aloca a matriz
NODE** allocMat ( int, int );

//Inicia os valores da matriz
void initializeMat ( FILE*, NODE** );

//Acha as coordenadas iniciais e finais
void getInitialFinalCoord ( NODE**, int, int, int*, int*, int*, int* );

//Libera as alocações
void allFree ( FILE*, NODE**, int );




/******************************** FUNÇÕES USADAS PARA A BUSCA EM LARGURA *********************************/

//Executa a Busca em Largura
void BFS ( NODE**, int, int, int, int );

//Aloca uma lista
NODE* allocQueue ( );//QUEUE* allocQueue ( );

//Verifica se a lista está vazia
int emptyQueue ( int, int );

//Insere elemento na lista
void insertQueue ( NODE*, int*, int, int );//void insertQueue ( QUEUE*, int*, int, int );

//Libera alocações
void freeBFS ( NODE* );//void freeBFS ( QUEUE* );




/************************ FUNÇÕES USADA PARA A BUSCA EM PROFUNDIDADE ************************************/

//Executa a busca em profundidade
void DFS ( NODE**, int, int, int, int);

//Libera alocações
void freeDFS ( NODE* );

//Verifica se a pilha esta vazia
int emptyStack ( int );

//Insere na pilha
void insertStack ( NODE*, int*, int, int );

//Aloca pilha
NODE* allocStack ( int, int );




/*********************** FUNÇÕES USADAS PARA A BUSCA POR BEST FIRST **************************************/

//Execura a busca Best-First
int BestFirst ( NODE**, int, int, int, int, int, int );

//Calcula a distância de cada nó ao nó destino
void calcDistToFinal ( NODE**, int, int, int, int );




/***************************** FUNÇÕES USADAS PARA A BUSCA PELO "A ESTRELA" (A*) ***********************/

//Executa a busca A*
int AStar ( NODE**, int, int, int, int, int, int );

//Calcula a distância de cada nó ao nó destino somando a distância do nó ao seu nó pai
void calcTotalDist ( NODE**, int, int, int, int );

void function ( double*, double*, double, double, int, int );


/***************************** FUNÇÕES AUXILIARES ***********************************/

//Chama o menu para o usuário
void menu ( NODE**, int, int, int, int, int, int );

//Imprime a matriz inicial
void printMat ( NODE**, int, int );

//Inicializa o vetor de coordenadas de vizinhos livres
void initializeVet ( int*, int );

//Verifica se os vizinhos do nó estão livres
void checkPositions ( NODE**, int*, int, int, int, int );

//Verifica se existe matriz à cima
int upIsOk ( int );

//Verifica se existe matriz a direita
int rightIsOk ( int, int );

//Verifica se existe matriz a baixo
int downIsOk ( int, int );

//Verifica se existe matriz a esquerda
int leftIsOk ( int );

//Reseta o vetor de vizinhos livres
void resetVetAux ( int*, int );

void printCoords ( NODE**, int, int );

#endif
