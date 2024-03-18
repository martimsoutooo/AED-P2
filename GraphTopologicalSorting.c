//
// Algoritmos e Estruturas de Dados --- 2023/2024
//
// Topological Sorting
//
// João Varela 113780
// Carolina Prata 114246
// Martim Santos 114614

#include "GraphTopologicalSorting.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "IntegersQueue.h"
#include "instrumentation.h"

#define ITERACOES InstrCount[0]
#define ACESSOS InstrCount[1]

struct _GraphTopoSort {
  int* marked;                     // Aux array
  unsigned int* numIncomingEdges;  // Aux array
  unsigned int* vertexSequence;    // The result
  int validResult;                 // 0 or 1
  unsigned int numVertices;        // From the graph
  Graph* graph;
};

// AUXILIARY FUNCTION
// Allocate memory for the struct 
// And for its array fields
// Initialize all struct fields
//
static GraphTopoSort* _create(Graph* g) {
  // Assegurar que o grafo fornecido não é nulo.
  assert(g != NULL);

  // Inicializar a variável para a estrutura GraphTopoSort.
  GraphTopoSort* p = NULL;

  // TO BE COMPLETED
  // Alocar memória para a estrutura GraphTopoSort.
  p = (GraphTopoSort*)malloc(sizeof(struct _GraphTopoSort));

  // Verificar se a memória foi alocada corretamente.
  if (p == NULL) {
    return NULL;
  }

  // Definir o número de vértices do grafo
  p->numVertices = GraphGetNumVertices(g);
  ACESSOS++;

  // Alocar memória para o array de vértices
  p->marked = (int*)malloc(p->numVertices * sizeof(int));
  ACESSOS += 2;

  ACESSOS++;
   // Verificar se a memória foi alocada corretamente.
  if (p->marked == NULL) {
    free(p);
    return NULL;
  }
  
  
  p->numIncomingEdges = (unsigned int*)malloc(p->numVertices * sizeof(unsigned int));
  ACESSOS += 2;

  ACESSOS++;
  // Verificar se a memória foi alocada corretamente
  if (p->numIncomingEdges == NULL) {
    free(p->marked);
    free(p);
    ACESSOS++;
    return NULL;
    
  }

  // Alocar memória para o array de sequência de vértices
  p->vertexSequence = (unsigned int*)malloc(p->numVertices * sizeof(unsigned int));
  ACESSOS += 2;

  ACESSOS++;
  // Verificar se a memória foi alocada corretamente
  if (p->vertexSequence == NULL) {
    free(p->marked);
    free(p->numIncomingEdges);
    ACESSOS +=2 ;
    free(p);
    return NULL;
  }

  // Inicializar os arrays alocados com zeros
  for (unsigned int i = 0; i < p->numVertices; i++) {
    ITERACOES++;
    p->marked[i] = 0;
    p->numIncomingEdges[i] = 0;
    p->vertexSequence[i] = 0;
    ACESSOS +=4;
  }

// Definir o grafo e o resultado da validação 
  p->graph = g;
  p->validResult = 0;
  ACESSOS +=2;

  return p; 
}

//
// Computing the topological sorting, if any, using the 1st algorithm:
// 1 - Create a copy of the graph
// 2 - Successively identify vertices without incoming edges and remove their
//     outgoing edges
// Check if a valid sorting was computed and set the isValid field
// For instance, by checking if the number of elements in the vertexSequence is
// the number of graph vertices
//
// Function to compute the topological sort of a directed graph.
GraphTopoSort* GraphTopoSortComputeV1(Graph* g) {
    assert(g != NULL && GraphIsDigraph(g) == 1);

    // TO BE COMPLETED
    // Inicializar a estrutura de ordenação topológica
    GraphTopoSort* topoSort = _create(g);
    if (topoSort == NULL) {
        return NULL;
    }

    // Fazer uma cópia do grafo 
    Graph* graphCopy = GraphCopy(g);
    if (graphCopy == NULL) {
        GraphTopoSortDestroy(&topoSort);
        return NULL;
    }

    // Inicializar o contador para o número de vértices ordenados
    unsigned int VerticesCount = 0;

    // Iterar sobre os vértices para encontrar e processar vértices sem arestas de entrada
    
    for (unsigned int currentVertex = 0; currentVertex < topoSort->numVertices; currentVertex++) {
        ITERACOES++;
        ACESSOS++;
        // Identificar vértices sem arestas de entrada que ainda não foram marcados
        ACESSOS++;
        if ((GraphGetVertexInDegree(graphCopy, currentVertex) == 0) && (!topoSort->marked[currentVertex])) {

            // Marcar o vértice atual e adicioná-lo à sequência de ordenação topológica
            topoSort->marked[currentVertex] = 1;
            topoSort->vertexSequence[VerticesCount++] = currentVertex;
            ACESSOS+=2;
            // Remover arestas de saída do vértice atual
            unsigned int* adjacentVertices = GraphGetAdjacentsTo(graphCopy, currentVertex);
            for (unsigned int i = 1; i <= adjacentVertices[0]; i++) {
                GraphRemoveEdge(graphCopy, currentVertex, adjacentVertices[i]);
                ITERACOES++;
            }
            
            // Libertar a memória alocada para a lista de vértices adjacentes
            free(adjacentVertices);
            
            // Repor o índice do vértice atual para re-verificar novos vértices sem arestas de entrada
            currentVertex = -1; // Será incrementado para 0 na próxima iteração
        }

    }

    // Verificar se o número de vértices ordenados é igual ao número total de vértices
    topoSort->validResult = (VerticesCount == topoSort->numVertices);
    ACESSOS += 2;

    // Limpar o grafo copiado
    GraphDestroy(&graphCopy);

    // Devolver a estrutura de ordenação topológica
    return topoSort;
}




//
// Computing the topological sorting, if any, using the 2nd algorithm
// Check if a valid sorting was computed and set the isValid field
// For instance, by checking if the number of elements in the vertexSequence is
// the number of graph vertices
//
GraphTopoSort* GraphTopoSortComputeV2(Graph* g) {
  assert(g != NULL && GraphIsDigraph(g) == 1);

  // TO BE COMPLETED
  // Criar e inicializar a estrutura de ordenação topológica
  GraphTopoSort* topoSort = _create(g);

  // Verificar se a estrutura foi inicializada corretamente
  if (topoSort == NULL) {
    return NULL;
  }

  // Construir a ordenação topológica
  unsigned int sortedVerticesCount = 0;
  unsigned int currentVertex;

  // Criar um array para armazenar o grau de entrada (InDegree) de cada vértice
  
  for (currentVertex = 0; currentVertex < topoSort->numVertices; currentVertex++) {
    ACESSOS++;
    topoSort->numIncomingEdges[currentVertex] = GraphGetVertexInDegree(g, currentVertex);
    ACESSOS++;
    ITERACOES++;
  }
  
  // Processar os vértices para determinar a ordenação topológica
  
  for (currentVertex = 0; currentVertex < topoSort->numVertices; currentVertex++) {
    ACESSOS++;
    // Procurar por vértices sem arestas de entrada e que ainda não foram marcados
    ACESSOS++;
    ITERACOES++;
    if (topoSort->numIncomingEdges[currentVertex] == 0 && !topoSort->marked[currentVertex]) {

      // Marcar o vértice e adicioná-lo à sequência de ordenação
      topoSort->marked[currentVertex] = 1;
      topoSort->vertexSequence[sortedVerticesCount++] = currentVertex;
      ACESSOS+=2;
      // Obter os vértices adjacentes e decrementar o grau de entrada dos mesmos
      unsigned int *adjVertices = GraphGetAdjacentsTo(g, currentVertex);
      for (unsigned int i = 1; i <= adjVertices[0]; i++) {
        unsigned int adjacentVertex = adjVertices[i];
        topoSort->numIncomingEdges[adjacentVertex]--;
        ITERACOES++;
        ACESSOS++;
      }
      
      // Libertar a memória alocada para a lista de vértices adjacentes
      free(adjVertices);
      
      // Reiniciar o índice do vértice para verificar novos vértices sem arestas de entrada
      currentVertex = -1; // Vai ser incrementado para 0 na próxima iteração do loop for
    }
  }

  // Verificar se todos os vértices foram processados e determinar a validade da ordenação
  topoSort->validResult = (sortedVerticesCount == topoSort->numVertices);
  ACESSOS+=2;
  // Retornar a estrutura de ordenação topológica
  return topoSort;
}


//
// Computing the topological sorting, if any, using the 3rd algorithm
// Check if a valid sorting was computed and set the isValid field
// For instance, by checking if the number of elements in the vertexSequence is
// the number of graph vertices
//
GraphTopoSort* GraphTopoSortComputeV3(Graph* g) {
  assert(g != NULL && GraphIsDigraph(g) == 1);

  // Criar e inicializar a estrutura de ordenação topológica
  GraphTopoSort* topoSortStructure = _create(g);

  // Construir a ordenação topológica
  
  for (unsigned int vertexIndex = 0; vertexIndex < topoSortStructure->numVertices; vertexIndex++) {
      ACESSOS++;
      topoSortStructure->numIncomingEdges[vertexIndex] = GraphGetVertexInDegree(g, vertexIndex);
      ITERACOES++;
      ACESSOS++;
  }

  // Criar uma fila vazia
  ACESSOS++;
  Queue* vertexQueue = QueueCreate(topoSortStructure->numVertices);
  
  // Inserir na fila todos os vértices com numIncomingEdges[vertexIndex] == 0
  
  for (unsigned int vertexIndex = 0; vertexIndex < topoSortStructure->numVertices; vertexIndex++) {
      ACESSOS++;
      ACESSOS++;
      ITERACOES++;
      if (topoSortStructure->numIncomingEdges[vertexIndex] == 0) {
          QueueEnqueue(vertexQueue, vertexIndex);
      }
  }

  // Processar os vértices enquanto a fila não estiver vazia
  unsigned int sortedVertexCount = 0;
  while (!QueueIsEmpty(vertexQueue)) {
      // Retirar um vértice da fila
      unsigned int currentVertex = QueueDequeue(vertexQueue);
      
      // Adicionar o vértice à sequência de ordenação
      topoSortStructure->vertexSequence[sortedVertexCount] = currentVertex;
      sortedVertexCount++;
      ACESSOS++;
      // Obter e processar todos os vértices adjacentes ao vértice atual
      unsigned int* adjacentVertices = GraphGetAdjacentsTo(g, currentVertex);
      for (unsigned int i = 1; i <= adjacentVertices[0]; i++) {
          ACESSOS++;
          ITERACOES++;
          unsigned int adjacentVertex = adjacentVertices[i];
          topoSortStructure->numIncomingEdges[adjacentVertex]--;
          
          // Se um vértice adjacente não tem mais arestas de entrada, adicioná-lo à fila
          ACESSOS++;
          if (topoSortStructure->numIncomingEdges[adjacentVertex] == 0) {
              QueueEnqueue(vertexQueue, adjacentVertex);
          }
      }
      
      // Libertar a memória alocada para a lista de vértices adjacentes
      free(adjacentVertices);
  }

  // Destruir a fila
  QueueDestroy(&vertexQueue);
  
  // Verificar se todos os vértices foram ordenados
  ACESSOS++;
  if (sortedVertexCount == topoSortStructure->numVertices) {
    ACESSOS++;
      topoSortStructure->validResult = 1;
  }

  // Retornar a estrutura de ordenação topológica
  return topoSortStructure;
}



void GraphTopoSortDestroy(GraphTopoSort** p) {
  assert(*p != NULL);

  GraphTopoSort* aux = *p;

  free(aux->marked);
  free(aux->numIncomingEdges);
  free(aux->vertexSequence);

  ACESSOS +=3;

  free(*p);
  *p = NULL;
}

//
// A valid sorting was computed?
//
int GraphTopoSortIsValid(const GraphTopoSort* p) {
  ACESSOS++;
  return p->validResult;
}

//
// Getting an array containing the topological sequence of vertex indices
// Or NULL, if no sequence could be computed
// MEMORY IS ALLOCATED FOR THE RESULTING ARRAY
//
unsigned int* GraphTopoSortGetSequence(const GraphTopoSort* p) {
  assert(p != NULL);
  // TO BE COMPLETED
  ACESSOS++;

  // Verificar se o resultado da ordenação topológica é válido
  if (p->validResult == 1) {
    // Incrementar o contador de acessos.
    ACESSOS++;

    // Alocar memória para a sequência de vértices da ordenação topológica.
    unsigned int* sequenciaVertices = (unsigned int*)malloc(p->numVertices * sizeof(unsigned int));

    if (sequenciaVertices == NULL) {
      // Retornar NULL se a alocação de memória falhar
      return NULL;
    }
    
    // Copiar a sequência de vértices da ordenação topológica para o array alocado
    for (unsigned int i = 0; i < p->numVertices; i++) {
      // Incrementar os contadores de acessos e iterações
      ACESSOS++;
      ITERACOES++;
      ACESSOS++;

      // Copiar cada vértice para a sequência
      sequenciaVertices[i] = p->vertexSequence[i];
    }

    // Retornar a sequência de vértices da ordenação topológica
    return sequenciaVertices;
  }

  // Se o resultado da ordenação não for válido, retornar NULL
  return NULL;
}

// DISPLAYING on the console

//
// The toplogical sequence of vertex indices, if it could be computed
//
void GraphTopoSortDisplaySequence(const GraphTopoSort* p) {
  assert(p != NULL);

  ACESSOS++;
  if (p->validResult == 0) {
    printf(" *** The topological sorting could not be computed!! *** \n");
    return;
  }

  printf("Topological Sorting - Vertex indices:\n");
  
  for (unsigned int i = 0; i < GraphGetNumVertices(p->graph); i++) {
    ACESSOS++;
    ITERACOES++;
    printf("%d ", p->vertexSequence[i]);
  }
  printf("\n");
}

//
// The toplogical sequence of vertex indices, if it could be computed
// Followed by the digraph displayed using the adjecency lists
// Adjacency lists are presented in topologic sorted order
//
void GraphTopoSortDisplay(const GraphTopoSort* p) {
  assert(p != NULL);

  // The topological order
  GraphTopoSortDisplaySequence(p);

  ACESSOS++;
  if (p->validResult == 0) {
    return;
  }

  // The Digraph
 
  for (unsigned int i = 0; i < GraphGetNumVertices(p->graph); i++) {
    ACESSOS++;
    GraphListAdjacents(p->graph, p->vertexSequence[i]);
    ACESSOS++;
    ITERACOES++;
  }
  printf("\n");
}

