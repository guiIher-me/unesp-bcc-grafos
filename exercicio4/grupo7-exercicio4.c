/*
@Authors: Guilherme Cardoso, Anael Porto, Julia Amadio, Thabata Santos, Renan Alves
@Date: 2024-09-09
@Description: Implemente o Algoritmo de Kruskal visando grafos ponderados. 
Para fazer os testes, você pode modificar o código do exercício 1 de forma que 
cada aresta do grafo tenha um peso gerado por uma distribuição uniforme em (0,1). 
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <locale.h>
#include <unistd.h>

typedef struct {
    int src, dest;
    double weight;
} Edge;

int getNodesInput() {
    int nodes;
    
    do {
       printf("Informe o numero de nos N:\n");
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

int compareEdges(const void *a, const void *b) {
    Edge *edgeA = (Edge *)a;
    Edge *edgeB = (Edge *)b;
    return (edgeA->weight > edgeB->weight) - (edgeA->weight < edgeB->weight);
}

int find(int parent[], int i) {
    if (parent[i] == i)
        return i;
    return find(parent, parent[i]);
}

void unionSets(int parent[], int rank[], int x, int y) {
    int xroot = find(parent, x);
    int yroot = find(parent, y);

    if (xroot != yroot) {
        if (rank[xroot] < rank[yroot])
            parent[xroot] = yroot;
        else if (rank[xroot] > rank[yroot])
            parent[yroot] = xroot;
        else {
            parent[yroot] = xroot;
            rank[xroot]++;
        }
    }
}

void writeToFile(const char *filename, int nodes, int m[][nodes], Edge edgeTable[], int edges) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("\nErro ao abrir/criar o arquivo.\n");
        return;
    }
    
    //Escreve matriz de adjacencias no arquivo
    fprintf(file, "Matriz de Adjacencias:\n");
    for(int i = 0; i < nodes; i++) {
        for(int j = 0; j < nodes; j++) {
            fprintf(file, "%d ", m[i][j]);
        }
        fprintf(file, "\n");
    }
    
    //Escreve tabela de arestas no arquivo
    fprintf(file, "\nTabela de Arestas:\n");
    fprintf(file, "Num de arestas: %d. Colunas 1-2: vertices. Coluna 3: peso da aresta.\n", edges);
    for(int i = 0; i < edges; i++) {
        fprintf(file, "%d - %d | %.2f\n", edgeTable[i].src, edgeTable[i].dest, edgeTable[i].weight);
    }
    
    fclose(file);
    printf("\nDados gerados escritos no arquivo '%s' com sucesso.\n", filename);
}

int main() {
    //inits
    srand(time(NULL));
    setlocale(LC_ALL, "");
    const bool SHOW_HEADER = true;
    const char *FILENAME = "output-adj-matrix-edgetable.txt";
    
    //inputs
    int nodes = getNodesInput();
    int probability = getProbabilityInput();
    
    //process
    int i, j, m[nodes][nodes];
    
	//contador de arestas
	int edges = 0;

    for(i = 0; i < nodes; i++) {
        for(j = 0; j < nodes; j++) {
            if(i == j) {
                m[i][j] = 0;
            } else if(i > j) {
                m[i][j] = m[j][i];
            } else {
                m[i][j] = connectVertex(probability);
                if(m[i][j] == 1) edges++;
            }
        }
    }

	printf("\nMatriz de adjacencias gerada:\n");
    printAdjMatrix(nodes, m, SHOW_HEADER);

    //Geracao da tabela de arestas com os pesos
    Edge edgeTable[edges];
    int row = 0; //contador para linhas da tabela ja preenchidas

    for(i = 0; i < nodes; i++) {
        for(j = 0; j < nodes; j++) {
            if(i < j && m[i][j] == 1) {
                edgeTable[row].src = i;
                edgeTable[row].dest = j;
                edgeTable[row].weight = (double)rand() / RAND_MAX;
                row++;
            }
        }
    }
    
	printf("\n----------------------------[TABELA DE ARESTAS]----------------------------\n");
    printf("Num de arestas: %d. Colunas 1-2: vertices. Coluna 3: peso da aresta.\n", edges);
    for(i = 0; i < edges; i++) {
        printf("%d - %d | %.2f\n", (int)edgeTable[i].src, (int)edgeTable[i].dest, edgeTable[i].weight);
    }

    writeToFile(FILENAME, nodes, m, edgeTable, edges);

    //Algoritmo de Kruskal
    //1. Ordenar arestas por peso de forma crescente
    qsort(edgeTable, edges, sizeof(Edge), compareEdges);

    //2. Inicializar a estrutura para aplicar a uniao
    int parent[nodes];
    int rank[nodes];
    for(i = 0; i < nodes; i++) {
        parent[i] = i;
        rank[i] = 0;
    }
    
    //3. Processar arestas e construir a arvore geradora minima
    printf("\n--------------------[Resultado do Algoritmo de Kruskal]--------------------\n");
    printf("Arestas da arvore geradora minima:\n");

    double mstWeight = 0;
    for(i = 0; i < edges; i++) {
        int x = find(parent, edgeTable[i].src);
        int y = find(parent, edgeTable[i].dest);
        
        if(x != y) {
            printf("%d - %d: %.2f\n", edgeTable[i].src, edgeTable[i].dest, edgeTable[i].weight);
            mstWeight += edgeTable[i].weight;
            unionSets(parent, rank, x, y);
        }
    }
    
    printf("Peso total da arvore geradora minima: %.3f\n", mstWeight);

	system("pause");
    return 0;
}
