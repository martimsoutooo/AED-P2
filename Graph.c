//
// Algoritmos e Estruturas de Dados --- 2023/2024
//
// Joaquim Madeira, Joao Manuel Rodrigues - June 2021, Nov 2023
//
// Graph - Using a list of adjacency lists representation
// João Varela 113780
// Carolina Prata 114246
// Martim Santos 114614

#include "Graph.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "SortedList.h"
#include "instrumentation.h"

static int _addEdge(Graph* g, unsigned int v, unsigned int w, double weight);
struct _Vertex {
  unsigned int id;
  unsigned int inDegree;
  unsigned int outDegree;
  List* edgesList;
};

struct _Edge {
  unsigned int adjVertex;
  double weight;
};

struct _GraphHeader {
  int isDigraph;
  int isComplete;
  int isWeighted;
  unsigned int numVertices;
  unsigned int numEdges;
  List* verticesList;
};

// The comparator for the VERTICES LIST

int graphVerticesComparator(const void* p1, const void* p2) {
  unsigned int v1 = ((struct _Vertex*)p1)->id;
  unsigned int v2 = ((struct _Vertex*)p2)->id;
  int d = v1 - v2;
  return (d > 0) - (d < 0);
}

// The comparator for the EDGES LISTS

int graphEdgesComparator(const void* p1, const void* p2) {
  unsigned int v1 = ((struct _Edge*)p1)->adjVertex;
  unsigned int v2 = ((struct _Edge*)p2)->adjVertex;
  int d = v1 - v2;
  return (d > 0) - (d < 0);
}

Graph* GraphCreate(unsigned int numVertices, int isDigraph, int isWeighted) {
  Graph* g = (Graph*)malloc(sizeof(struct _GraphHeader));
  if (g == NULL) abort();

  g->isDigraph = isDigraph;
  g->isComplete = 0;
  g->isWeighted = isWeighted;

  g->numVertices = numVertices;
  g->numEdges = 0;

  g->verticesList = ListCreate(graphVerticesComparator);

  for (unsigned int i = 0; i < numVertices; i++) {
    struct _Vertex* v = (struct _Vertex*)malloc(sizeof(struct _Vertex));
    if (v == NULL) abort();

    v->id = i;
    v->inDegree = 0;
    v->outDegree = 0;

    v->edgesList = ListCreate(graphEdgesComparator);

    ListInsert(g->verticesList, v);
  }

  assert(g->numVertices == ListGetSize(g->verticesList));

  return g;
}

Graph* GraphCreateComplete(unsigned int numVertices, int isDigraph) {
  Graph* g = GraphCreate(numVertices, isDigraph, 0);

  g->isComplete = 1;

  List* vertices = g->verticesList;
  ListMoveToHead(vertices);
  unsigned int i = 0;
  for (; i < g->numVertices; ListMoveToNext(vertices), i++) {
    struct _Vertex* v = ListGetCurrentItem(vertices);
    List* edges = v->edgesList;
    for (unsigned int j = 0; j < g->numVertices; j++) {
      if (i == j) {
        continue;
      }
      struct _Edge* new = (struct _Edge*)malloc(sizeof(struct _Edge));
      if (new == NULL) abort();
      new->adjVertex = j;
      new->weight = 1;

      ListInsert(edges, new);
    }
    if (g->isDigraph) {
      v->inDegree = g->numVertices - 1;
      v->outDegree = g->numVertices - 1;
    } else {
      v->outDegree = g->numVertices - 1;
    }
  }
  if (g->isDigraph) {
    g->numEdges = numVertices * (numVertices - 1);
  } else {
    g->numEdges = numVertices * (numVertices - 1) / 2;
  }

  return g;
}

void GraphDestroy(Graph** p) {
  assert(*p != NULL);
  Graph* g = *p;

  List* vertices = g->verticesList;
  if (ListIsEmpty(vertices) == 0) {
    ListMoveToHead(vertices);
    unsigned int i = 0;
    for (; i < g->numVertices; ListMoveToNext(vertices), i++) {
      struct _Vertex* v = ListGetCurrentItem(vertices);

      List* edges = v->edgesList;
      if (ListIsEmpty(edges) == 0) {
        unsigned int i = 0;
        ListMoveToHead(edges);
        for (; i < ListGetSize(edges); ListMoveToNext(edges), i++) {
          struct _Edge* e = ListGetCurrentItem(edges);
          free(e);
        }
      }
      ListDestroy(&(v->edgesList));
      free(v);
    }
  }

  ListDestroy(&(g->verticesList));
  free(g);

  *p = NULL;
}



Graph* GraphCopy(const Graph *g) {
    assert(g != NULL);

    // TO BE COMPLETED!!
    Graph* copiaGrafo = GraphCreate(g->numVertices, g->isDigraph, g->isWeighted);

    // Verificar se a cópia foi criada com sucesso
    if (copiaGrafo == NULL) {
        free(copiaGrafo);
        return NULL;
    }

    // Se o grafo original não tiver vértices, retornar a cópia vazia
    if (g->numVertices == 0) {
        return copiaGrafo;
    }

    // Obter a lista de vértices do grafo original
    List* verticesOriginais = g->verticesList;
    ListMoveToHead(verticesOriginais);
    ListMoveToHead(copiaGrafo->verticesList);

    // Copiar cada vértice e suas arestas do grafo original para a cópia
    for (unsigned int i = 0; i < g->numVertices; i++) {
        struct _Vertex* verticeOriginal = ListGetCurrentItem(verticesOriginais);
        struct _Vertex* verticeCopia = ListGetCurrentItem(copiaGrafo->verticesList);

        // Copiar informações do vértice original para o vértice cópia
        verticeCopia->id = verticeOriginal->id;
        verticeCopia->inDegree = verticeOriginal->inDegree;
        verticeCopia->outDegree = verticeOriginal->outDegree;

        // Copiar as arestas associadas a cada vértice
        List* arestasOriginais = verticeOriginal->edgesList;
        ListMoveToHead(arestasOriginais);

        for (unsigned int j = 0; j < verticeOriginal->outDegree; j++) {
            struct _Edge* arestaOriginal = ListGetCurrentItem(arestasOriginais);
            struct _Edge* arestaCopia = (struct _Edge*)malloc(sizeof(struct _Edge));

            // Verificar se a alocação de memória para a aresta foi bem-sucedida
            if (arestaCopia == NULL) {
                free(arestaCopia);
                GraphDestroy(&copiaGrafo);
                abort();
            }

            // Copiar informações da aresta original para a aresta cópia
            arestaCopia->adjVertex = arestaOriginal->adjVertex;
            arestaCopia->weight = arestaOriginal->weight;

            // Inserir a aresta cópia na lista de arestas do vértice cópia
            ListInsert(verticeCopia->edgesList, arestaCopia);
            copiaGrafo->numEdges++;

            ListMoveToNext(arestasOriginais);
        }
    
        ListMoveToNext(verticesOriginais);
        ListMoveToNext(copiaGrafo->verticesList);
    }

    // Retornar o grafo cópia.
    return copiaGrafo;

    }

    
Graph* GraphFromFile(FILE* f) {
  assert(f != NULL);

  // TO BE COMPLETED !!  
  //le o ficheiro e cria o grafo
  unsigned int numVertices, numEdges, isWeighted, directed;

  // Ler do ficheiro as propriedades do grafo
  if (fscanf(f, "%u %u %u %u", &directed, &isWeighted, &numVertices, &numEdges) != 4) {
    return NULL;
  }

  // Criar o grafo com as propriedades lidas do ficheiro
  Graph *graph = GraphCreate(numVertices, directed, isWeighted);

  // Ler cada aresta do ficheiro e adicionar ao grafo
  for (unsigned int i = 0; i < numEdges; i++) {
    unsigned int v, w;
    double weight = 1.0; // Peso padrão se o grafo não for ponderado

    // Ler as arestas do ficheiro
    if (isWeighted) {
      if (fscanf(f, "%u %u %lf", &v, &w, &weight) != 3) {
        GraphDestroy(&graph);
        return NULL;
      }
    } else {
      if (fscanf(f, "%u %u", &v, &w) != 2) {
        GraphDestroy(&graph);
        return NULL;
      }
    }

    // Ignorar laços (arestas que conectam um vértice a si mesmo)
    if (v == w) {
      continue;
    }

    // Verificar se a aresta já existe no grafo
    int EdgeValido = 1;
    unsigned int *adjacents = GraphGetAdjacentsTo(graph, v);
    for (unsigned int j = 1; j <= adjacents[0]; j++) {
      if (adjacents[j] == w) {
        EdgeValido = 0;
        break;
      }
    }

    // Libertar a memória alocada para a lista de adjacências
    free(adjacents);

    // Se a aresta for válida (não existir), adicionar ao grafo
    if (EdgeValido) {
      _addEdge(graph, v, w, weight);
    }
  }


  return graph;
}

// Graph

int GraphIsDigraph(const Graph* g) { return g->isDigraph; }

int GraphIsComplete(const Graph* g) { return g->isComplete; }

int GraphIsWeighted(const Graph* g) { return g->isWeighted; }

unsigned int GraphGetNumVertices(const Graph* g) { return g->numVertices; }

unsigned int GraphGetNumEdges(const Graph* g) { return g->numEdges; }

//
// For a graph
//
double GraphGetAverageDegree(const Graph* g) {
  assert(g->isDigraph == 0);
  return 2.0 * (double)g->numEdges / (double)g->numVertices;
}

static unsigned int _GetMaxDegree(const Graph* g) {
  List* vertices = g->verticesList;
  if (ListIsEmpty(vertices)) return 0;

  unsigned int maxDegree = 0;
  ListMoveToHead(vertices);
  unsigned int i = 0;
  for (; i < g->numVertices; ListMoveToNext(vertices), i++) {
    struct _Vertex* v = ListGetCurrentItem(vertices);
    if (v->outDegree > maxDegree) {
      maxDegree = v->outDegree;
    }
  }
  return maxDegree;
}

//
// For a graph
//
unsigned int GraphGetMaxDegree(const Graph* g) {
  assert(g->isDigraph == 0);
  return _GetMaxDegree(g);
}

//
// For a digraph
//
unsigned int GraphGetMaxOutDegree(const Graph* g) {
  assert(g->isDigraph == 1);
  return _GetMaxDegree(g);
}

// Vertices

//
// returns an array of size (outDegree + 1)
// element 0, stores the number of adjacent vertices
// and is followed by indices of the adjacent vertices
//
unsigned int* GraphGetAdjacentsTo(const Graph* g, unsigned int v) {
  assert(v < g->numVertices);

  // Node in the list of vertices
  List* vertices = g->verticesList;
  ListMove(vertices, v);
  struct _Vertex* vPointer = ListGetCurrentItem(vertices);
  unsigned int numAdjVertices = vPointer->outDegree;

  unsigned int* adjacent =
      (unsigned int*)calloc(1 + numAdjVertices, sizeof(unsigned int));

  if (numAdjVertices > 0) {
    adjacent[0] = numAdjVertices;
    List* adjList = vPointer->edgesList;
    ListMoveToHead(adjList);
    for (unsigned int i = 0; i < numAdjVertices; ListMoveToNext(adjList), i++) {
      struct _Edge* ePointer = ListGetCurrentItem(adjList);
      adjacent[i + 1] = ePointer->adjVertex;
    }
  }

  return adjacent;
}

//
// returns an array of size (outDegree + 1)
// element 0, stores the number of adjacent vertices
// and is followed by the distances to the adjacent vertices
//
double* GraphGetDistancesToAdjacents(const Graph* g, unsigned int v) {
  assert(v < g->numVertices);

  // Node in the list of vertices
  List* vertices = g->verticesList;
  ListMove(vertices, v);
  struct _Vertex* vPointer = ListGetCurrentItem(vertices);
  unsigned int numAdjVertices = vPointer->outDegree;

  double* distance = (double*)calloc(1 + numAdjVertices, sizeof(double));

  if (numAdjVertices > 0) {
    distance[0] = numAdjVertices;
    List* adjList = vPointer->edgesList;
    ListMoveToHead(adjList);
    for (unsigned int i = 0; i < numAdjVertices; ListMoveToNext(adjList), i++) {
      struct _Edge* ePointer = ListGetCurrentItem(adjList);
      distance[i + 1] = ePointer->weight;
    }
  }

  return distance;
}

//
// For a graph
//
unsigned int GraphGetVertexDegree(Graph* g, unsigned int v) {
  assert(g->isDigraph == 0);
  assert(v < g->numVertices);

  ListMove(g->verticesList, v);
  struct _Vertex* p = ListGetCurrentItem(g->verticesList);

  return p->outDegree;
}

//
// For a digraph
//
unsigned int GraphGetVertexOutDegree(Graph* g, unsigned int v) {
  assert(g->isDigraph == 1);
  assert(v < g->numVertices);

  ListMove(g->verticesList, v);
  struct _Vertex* p = ListGetCurrentItem(g->verticesList);

  return p->outDegree;
}

//
// For a digraph
//
unsigned int GraphGetVertexInDegree(Graph* g, unsigned int v) {
  assert(g->isDigraph == 1);
  assert(v < g->numVertices);

  ListMove(g->verticesList, v);
  struct _Vertex* p = ListGetCurrentItem(g->verticesList);

  return p->inDegree;
}

// Edges

static int _addEdge(Graph* g, unsigned int v, unsigned int w, double weight) {
  struct _Edge* edge = (struct _Edge*)malloc(sizeof(struct _Edge));
  edge->adjVertex = w;
  edge->weight = weight;

  ListMove(g->verticesList, v);
  struct _Vertex* vertex = ListGetCurrentItem(g->verticesList);
  int result = ListInsert(vertex->edgesList, edge);

  if (result == -1) {
    free(edge);
    return 0;
  } else {
    g->numEdges++;
    vertex->outDegree++;

    ListMove(g->verticesList, w);
    struct _Vertex* destVertex = ListGetCurrentItem(g->verticesList);
    destVertex->inDegree++;
  }

  if (g->isDigraph == 0) {
    // Bidirectional edge
    struct _Edge* edge = (struct _Edge*)malloc(sizeof(struct _Edge));
    edge->adjVertex = v;
    edge->weight = weight;

    ListMove(g->verticesList, w);
    struct _Vertex* vertex = ListGetCurrentItem(g->verticesList);
    result = ListInsert(vertex->edgesList, edge);

    if (result == -1) {
      return 0;
    } else {
      // g->numEdges++; // Do not count the same edge twice on a undirected
      // graph !!
      vertex->outDegree++;
    }
  }

  return 1;
}

int GraphAddEdge(Graph* g, unsigned int v, unsigned int w) {
  assert(g->isWeighted == 0);
  assert(v != w);
  assert(v < g->numVertices);
  assert(w < g->numVertices);

  return _addEdge(g, v, w, 1.0);
}

int GraphAddWeightedEdge(Graph* g, unsigned int v, unsigned int w,
                         double weight) {
  assert(g->isWeighted == 1);
  assert(v != w);
  assert(v < g->numVertices);
  assert(w < g->numVertices);

  return _addEdge(g, v, w, weight);
}

int GraphRemoveEdge(Graph *g, unsigned int v, unsigned int w)
{
  // TO BE COMPLETED !!

  assert(g != NULL);
  assert(v < g->numVertices);
  assert(w < g->numVertices);

  // Encontrar o vértice v no grafo
  List *listaVertices = g->verticesList;
  ListMove(listaVertices, v);
  struct _Vertex *verticeV = ListGetCurrentItem(listaVertices);

  // Encontrar o vértice w no grafo
  ListMove(listaVertices, w);
  struct _Vertex *verticeW = ListGetCurrentItem(listaVertices);

  // Encontrar e remover a aresta de v para w
  List *listaArestasV = verticeV->edgesList;
  ListMoveToHead(listaArestasV);
  for (unsigned int i = 0; i < ListGetSize(listaArestasV); i++) {
      struct _Edge *aresta = ListGetCurrentItem(listaArestasV);
      if (aresta->adjVertex == w) {
          // Remover a aresta atual da lista de arestas de v
          ListRemoveCurrent(listaArestasV);
          free(aresta);                    // Libertar a memória da aresta
          g->numEdges--;                   // Diminuir o número de arestas no grafo
          verticeV->outDegree--;           // Diminuir o grau de saída do vértice v
          if (g->isDigraph) {
              verticeW->inDegree--;         // Diminuir o grau de entrada do vértice w
              return 1; // Aresta removida com sucesso
          }
      }
      ListMoveToNext(listaArestasV);

      // Se não for um digrafo, também remover a aresta inversa
      if (!g->isDigraph) {
          List *listaArestasW = verticeW->edgesList;
          ListMoveToHead(listaArestasW);
          for (unsigned int j = 0; j < ListGetSize(listaArestasW); j++) {
              struct _Edge *arestaInversa = ListGetCurrentItem(listaArestasW);
              if (arestaInversa->adjVertex == v) {
                  // Remover a aresta inversa da lista de arestas de w
                  ListRemoveCurrent(listaArestasW);
                  free(arestaInversa);             // Libertar a memória da aresta inversa
                  verticeW->outDegree--;          // Diminuir o grau de saída do vértice w
                  return 1;                       // Sair do loop uma vez que a aresta inversa é encontrada e removida
              }
              ListMoveToNext(listaArestasW);
          }
      }
  }
  return 0;
}

// CHECKING

int GraphCheckInvariants(const Graph* g) {
    assert(g != NULL);
    // TO BE COMPLETED !!

    // Verificar se o tamanho da lista de vértices é igual ao número de vértices no grafo
    if (ListGetSize(g->verticesList) != g->numVertices) return 0;

    unsigned int totalEdges = 0;
    List* vertices = g->verticesList;

    // Iterar sobre cada vértice do grafo
    for (unsigned int i = 0; i < g->numVertices; i++) {
        // Mover para a cabeça da lista de vértices.
        if (ListIsEmpty(vertices)) return 0;
        ListMoveToHead(vertices);

        struct _Vertex* v = ListGetCurrentItem(vertices);
        if (v == NULL) return 0;

        List* edges = v->edgesList;
        totalEdges += ListGetSize(edges);

        // Verificar se o grau de saída corresponde ao tamanho da lista de arestas
        if (v->outDegree != ListGetSize(edges)) return 0;

        // Se for um digrafo, verificar o grau de entrada
        if (g->isDigraph && v->inDegree != ListGetSize(edges)) return 0;

        // Processar vértices alcançáveis se for um digrafo
        if (g->isDigraph) {
            int* reachableVertices = (int*)calloc(g->numVertices, sizeof(int));
            if (reachableVertices == NULL) return 0;

            // Marcar vértices alcançáveis
            ListMoveToHead(edges);
            for (unsigned int j = 0; j < ListGetSize(edges); j++) {
                if (ListIsEmpty(edges)) break;
                struct _Edge* e = ListGetCurrentItem(edges);
                if (e == NULL) break;
                reachableVertices[e->adjVertex] = 1;
            }

            // Verificar se o vértice é alcançável por outros vértices
            for (unsigned int j = 0; j < g->numVertices; j++) {
                if (j != i && reachableVertices[j]) {
                    ListMove(g->verticesList, j);
                    struct _Vertex* otherVertex = ListGetCurrentItem(g->verticesList);
                    if (otherVertex == NULL) break;
                    List* otherEdges = otherVertex->edgesList;
                    ListMoveToHead(otherEdges);
                    for (unsigned int k = 0; k < ListGetSize(otherEdges); k++) {
                        struct _Edge* otherEdge = ListGetCurrentItem(otherEdges);
                        if (otherEdge == NULL) break;
                        if (otherEdge->adjVertex == i) {
                            v->inDegree++;
                            break;
                        }
                    }
                }
            }
            free(reachableVertices);
        }

        // Verificar se cada aresta é válida.
        for (unsigned int j = 0; j < ListGetSize(edges); j++) {
            struct _Edge* e = ListGetCurrentItem(edges);
            if (e == NULL) break;
            if (e->adjVertex >= g->numVertices) return 0;
        }
    }

    // Verificar se o número total de arestas corresponde ao esperado
    if ((g->isDigraph && g->numEdges != totalEdges) ||
        (!g->isDigraph && g->numEdges != totalEdges / 2)) return 0;

    // Se passar em todos os testes, retorna 1.
    return 1;
}


// DISPLAYING on the console

void GraphDisplay(const Graph* g) {
  printf("---\n");
  if (g->isWeighted) {
    printf("Weighted ");
  }
  if (g->isComplete) {
    printf("COMPLETE ");
  }
  if (g->isDigraph) {
    printf("Digraph\n");
    printf("Max Out-Degree = %d\n", GraphGetMaxOutDegree(g));
  } else {
    printf("Graph\n");
    printf("Max Degree = %d\n", GraphGetMaxDegree(g));
  }
  printf("Vertices = %2d | Edges = %2d\n", g->numVertices, g->numEdges);

  List* vertices = g->verticesList;
  ListMoveToHead(vertices);
  unsigned int i = 0;
  for (; i < g->numVertices; ListMoveToNext(vertices), i++) {
    printf("%2d ->", i);
    struct _Vertex* v = ListGetCurrentItem(vertices);
    if (ListIsEmpty(v->edgesList)) {
      printf("\n");
    } else {
      List* edges = v->edgesList;
      unsigned int i = 0;
      ListMoveToHead(edges);
      for (; i < ListGetSize(edges); ListMoveToNext(edges), i++) {
        struct _Edge* e = ListGetCurrentItem(edges);
        if (g->isWeighted) {
          printf("   %2d(%4.2f)", e->adjVertex, e->weight);
        } else {
          printf("   %2d", e->adjVertex);
        }
      }
      printf("\n");
    }
  }
  printf("---\n");
}

void GraphListAdjacents(const Graph* g, unsigned int v) {
  printf("---\n");

  unsigned int* array = GraphGetAdjacentsTo(g, v);

  printf("Vertex %d has %d adjacent vertices -> ", v, array[0]);

  for (unsigned int i = 1; i <= array[0]; i++) {
    printf("%d ", array[i]);
  }

  printf("\n");

  free(array);

  printf("---\n");
}
