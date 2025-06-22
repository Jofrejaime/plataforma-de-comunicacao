#ifndef GRAFO_H
#define GRAFO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Aresta {
    char destino[100]; // Pode ser email ou nome da equipa
    char conteudo[256];
    struct Aresta* prox;
} Aresta;

typedef struct Vertice {
    char id[100]; // email ou nome da equipa
    Aresta* lista_adj;
    struct Vertice* prox;
} Vertice;

typedef struct {
    Vertice* lista_vertices;
} Grafo;

// Funções públicas
Grafo* criar_grafo();
Vertice* adicionar_vertice(Grafo* g, const char* id);
int adicionar_aresta(Grafo* g, const char* origem, const char* destino, const char* conteudo);
void imprimir_grafo(Grafo* g);

#endif
