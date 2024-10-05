/**
 * @Authors: Guilherme Cardoso, Anael Porto, Julia Amadio, Thabata Santos, Renan Alves
 * @Date: 2024-08-20
 * @Description: Implemente os algoritmos de Djikstra e Floyd.
 **/
 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <locale.h>


// Constantes
#define PRINT_SPACES 3
#define SHOW_MATRIX_HEADER true
#define SHOW_ARRAY_HEADER true
#define INFINITY 999
#define INFINITY_SYMBOL "x"


// Renomeando ponteiros para facilitar a vida...
typedef int* Array;
typedef int** Matrix;


// Funções

void printLine(int times) {
    int i;
    printf("\n");
    for(i = 0; i < times; i++) printf("-");
}

void printMatrix(Matrix matrix, int rows, int cols) {
    int i, j;
    
    if(SHOW_MATRIX_HEADER) {
        printf(" %*c |", PRINT_SPACES - 1, ' ');
        for(i = 0; i < cols; i++)
           printf("%*d ", PRINT_SPACES, i);
        
        printLine((PRINT_SPACES + 1) * (cols + 1));
    }
    
    printf("\n");
    for(i = 0; i < rows; i++) {
        if(SHOW_MATRIX_HEADER) printf("%*d |", PRINT_SPACES, i);
        
        for(j = 0; j < cols; j++) {
            if(matrix[i][j] == INFINITY)
                printf("%*s ", PRINT_SPACES, INFINITY_SYMBOL);
            else
                printf("%*d ", PRINT_SPACES, matrix[i][j]);
        }
        printf("\n");
    }
}


void printArray(Array array, int length) {
    int i;
     
    if(SHOW_ARRAY_HEADER) {
        printf(" %*c |", PRINT_SPACES - 1, 'i');
        for(i = 0; i < length; i++)
            printf("%*d ", PRINT_SPACES, i);
    
        printLine((PRINT_SPACES + 1) * (length + 1));
        
        printf("\n");
        printf(" %*c |", PRINT_SPACES - 1, 'v');
    }
    
    for(i = 0; i < length; i++) {
        if(array[i] == INFINITY)
            printf("%*s ", PRINT_SPACES, INFINITY_SYMBOL);
        else
            printf("%*d ", PRINT_SPACES, array[i]);
    }
    printf("\n");
}


Array newArray(int length) {
    Array array = (Array) malloc(length * sizeof(int));
    return array;
}


Matrix newMatrix(int rows, int cols) {
	Matrix matrix = (Matrix) malloc(rows * sizeof(Array));
	
	int i;
    for (i = 0; i < rows; i++)
        matrix[i] = newArray(cols);
        
    return matrix;
}

Matrix copyMatrix(Matrix matrix, int rows, int cols) {
    Matrix copied_matrix = newMatrix(rows, cols);
    
    int i, j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            copied_matrix[i][j] = matrix[i][j];
        }
    }
    
    return copied_matrix;
}


void freeMatrix(Matrix matrix, int rows) {
     int i;
     for (i = 0; i < rows; i++)
         free(matrix[i]);
     free(matrix);
}


int getRandom(int min, int max) {
    return rand() % (max - min + 1) + min;
}


bool connectNodes(float probability) {
	int random = getRandom(0, 99);
	return random < (probability * 100);
}


Matrix getAdjMatrixByErdosRenyi(int nodes, float probability, bool isOriented) {
	Matrix matrix = newMatrix(nodes, nodes);
	
	int i, j;
	for(i = 0; i < nodes; i++) {
		
    	for(j = 0; j < nodes; j++) {
            
            // "O grafo não possui ligações de um vértice para si mesmo"
    		if(i == j) {
    			matrix[i][j] = 0;
    			
   			// Se é grafo não-orientado e aresta m[j][i] já determinada
			} else if(!isOriented && i > j) {
                matrix[i][j] = matrix[j][i];
                
            // Senão, a aresta m[i][j] existe com probabilidade p
			} else {
                matrix[i][j] = connectNodes(probability);
            }
		}
    }
    
    return matrix;
}


Matrix transformAdjMatrixToWeightedMatrix(Matrix adj_matrix, int nodes) {
    Matrix weighted_matrix = newMatrix(nodes, nodes);
    
    int i, j;
	for(i = 0; i < nodes; i++) {
        for(j = 0; j < nodes; j++) {
            if(i == j)
                weighted_matrix[i][j] = 0;
            else if(adj_matrix[i][j] == 0)
                weighted_matrix[i][j] = INFINITY;
            else
                weighted_matrix[i][j] = adj_matrix[i][j];
        }
    }
        
    return weighted_matrix;
}


int getNodeByMinDistance(Array distances, bool visited[], int nodes) {
    int min = INFINITY, min_node_index = -1, i;

    for(i = 0; i < nodes; i++) {
        if(!visited[i] && distances[i] <= min) {
            min = distances[i];
            min_node_index = i;
        }
    }
    
    return min_node_index;
}


Array getMinDistancesFromNodeByDijkstra(Matrix weighted_matrix, int nodes, int node_id) {
    Array distances = newArray(nodes);
    bool visited[nodes];
    
    int i, count;
    
    // init
    for(i = 0; i < nodes; i++) {
        distances[i] = INFINITY;
        visited[i] = false;
    }
    
    // A distância do nó node_id para ele mesmo é 0
    distances[node_id] = 0;
    
    for(count = 0; count < nodes - 1; count++) {
        // Seleciona o vértice com a menor distância ainda não processado
        int u = getNodeByMinDistance(distances, visited, nodes);
        visited[u] = true;

        // Atualiza o valor das distâncias dos vértices adjacentes ao vértice selecionado
        for(int v = 0; v < nodes; v++) {
            
            // Atualiza distances[v] se não estiver no caminho mais curto e existir um caminho mais curto passando por u
            if(!visited[v] && weighted_matrix[u][v] && distances[u] != INFINITY && distances[u] + weighted_matrix[u][v] < distances[v]) {
                distances[v] = distances[u] + weighted_matrix[u][v];
            }
        }
    }
     
    return distances;     
}


Matrix getAllMinDistancesByFloydWarshall(Matrix weighted_matrix, int nodes) {
    Matrix distances = copyMatrix(weighted_matrix, nodes, nodes);
    
    int k, i, j;
    for(k = 0; k < nodes; k++){
        for(i = 0; i < nodes; i++){
            for(j = 0; j < nodes; j++){
                if((distances[i][k] + distances[k][j]) < distances[i][j]){
                    distances[i][j] = distances[i][k] + distances[k][j];
                }
            }
        }
    }
    
    return distances;
}


int main() {
	// inits
    srand(time(NULL));
    setlocale(LC_ALL, "");
    
    // config
    int nodes = 6;            // Quantidade de vértices a serem criados
    float probability = 0.42; // Chance entre [0,1] de conectar vértices
    bool isOriented = false;  // true: grafo orientado | false: grafo não-orientado
    int node_id = 0;          // ID do nó a ser analisado no algoritmo de Dijkstra (valor entre 0 e nodes-1)
    
    // pre-process
    Matrix adj_matrix = getAdjMatrixByErdosRenyi(nodes, probability, isOriented);
    Matrix weighted_matrix = transformAdjMatrixToWeightedMatrix(adj_matrix, nodes);
    
    // process
    Array node_distances = getMinDistancesFromNodeByDijkstra(weighted_matrix, nodes, node_id);
    Matrix graph_distances = getAllMinDistancesByFloydWarshall(weighted_matrix, nodes);
    
	// outputs
	printf("\n\nMatriz de adjascências:\n");
    printMatrix(adj_matrix, nodes, nodes);
    
    printf("\n\nDistâncias do nó %d até outros nós [Dijkstra]:\n", node_id);
    printArray(node_distances, nodes);
    
    printf("\n\nMatriz de distâncias para todo o grafo [Floyd-Warshall]:\n");
    printMatrix(graph_distances, nodes, nodes);
    
    printf("\n");
    
	// frees
	freeMatrix(adj_matrix, nodes);
	freeMatrix(weighted_matrix, nodes);
	free(node_distances);
	freeMatrix(graph_distances, nodes);
	
	system("pause");
	return 0;
}