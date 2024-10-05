/**
 * @Authors: Guilherme Cardoso, Anael Porto, Julia Amadio, Thabata Santos, Renan Alves
 * @Date: 2024-08-19
 * @Description: Implemente um programa que imprime em um arquivo a matriz de
 *  adjacência de um grafo de Erdos-Renyi. Considere que o grafo não possui
 *  ligações de um vértice para si mesmo. Como entrada, o usuário deve
 *  fornecer o parâmetro p e se deseja um grafo orientado ou não-orientado.
 **/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <locale.h>

int getNodesInput() {
    int nodes;
    
    do {
       printf("Informe o número de nós N:\n");
       scanf("%d", &nodes);
    } while(nodes <= 0);
    
    return nodes;
}


int getProbabilityInput() {
    int probability;
    
    do {
       printf("Informe a probabilidade percentual P: (0 <= P <= 100)\n");
       scanf("%d", &probability);
    } while(probability < 0 || probability > 100);
    
    return probability; 
}


bool getIsOrientedInput() {
   char isOriented;
   
   do {
       printf("O grafo é orientado? (S/N)\n"); 
       scanf(" %c", &isOriented);
   } while(isOriented != 'S' && isOriented != 'N');
   
   return isOriented == 'S';
}


int getRandom(int min, int max) {
    return rand() % (max - min + 1) + min;
}


bool connectVertex(int probability) {
   int random = getRandom(0, 99);
   return random < probability;
}


void printAdjMatrix(int nodes, int m[][nodes], const bool show_header) {
    int i, j;
    int spaces = 3;
    
    if(show_header) {
        printf(" %*c", spaces, ' ');
        for(i = 0; i < nodes; i++)
           printf("%*d ", spaces, i);
    }
    
    printf("\n");
    for(i = 0; i < nodes; i++) {
        if(show_header) printf("%*d ", spaces, i);
        
        for(j = 0; j < nodes; j++)
            printf("%*d ", spaces, m[i][j]);
        printf("\n");
    }
}


void fileAdjMatrix(int nodes, int m[][nodes], const char *filename) {
    FILE *file = fopen(filename, "w");
    
    if(file == NULL) {
        printf("\n\nErro ao abrir/criar o arquivo.\n");
        return;
    }
    
    for(int i = 0; i < nodes; i++) {
        for(int j = 0; j < nodes; j++)
            fprintf(file, "%d ", m[i][j]);
        fprintf(file, "\n");
    }
    
    fclose(file);
    printf("\n\nMatriz escrita no arquivo '%s' com sucesso.\n", filename);
}


int main() {
    // inits
    srand(time(NULL));
    setlocale(LC_ALL, "");
    const bool SHOW_HEADER = true;
    const char *FILENAME = "output-adj-matrix.txt";
    
    // inputs
    int  nodes = getNodesInput();
    int  probability = getProbabilityInput();
    bool isOriented = getIsOrientedInput();
    
    // process
    int i, j, m[nodes][nodes];
    
    for(i = 0; i < nodes; i++) {
    	for(j = 0; j < nodes; j++) {
              
            // "O grafo não possui ligações de um vértice para si mesmo"
    		if(i == j) {
    			m[i][j] = 0;
    			
   			// Se é grafo não-orientado e aresta m[j][i] já determinada
			} else if(!isOriented && i > j) {
                m[i][j] = m[j][i];
                
            // Senão, a aresta m[i][j] existe com probabilidade p
			} else {
                m[i][j] = connectVertex(probability);
            }
		}
    }
    
    // outputs
    printAdjMatrix(nodes, m, SHOW_HEADER);
    fileAdjMatrix (nodes, m, FILENAME);
    
    system("pause");
    return 0;
}