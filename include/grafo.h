#ifndef GRAFO_H
#define GRAFO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

// Estrutura para aresta do grafo
typedef struct Aresta {
    char destino[100]; // Pode ser email ou nome da equipa
    char conteudo[256];
    struct Aresta* prox;
} Aresta;

// Estrutura para vértice do grafo
typedef struct Vertice {
    char id[100]; // email ou nome da equipa
    Aresta* lista_adj;
    struct Vertice* prox;
} Vertice;

// Estrutura principal do grafo
typedef struct {
    Vertice* lista_vertices;
} Grafo;

// Cria um novo grafo. Retorna NULL em caso de erro de alocação.
Grafo* criar_grafo();
// Busca um vértice pelo id. Retorna NULL se não encontrado ou erro.
Vertice* buscar_vertice(Grafo* g, const char* id);
// Adiciona um vértice ao grafo. Retorna NULL em caso de erro.
Vertice* adicionar_vertice(Grafo* g, const char* id);
// Adiciona uma aresta entre dois vértices. Retorna 1 em caso de sucesso, 0 em caso de erro.
int adicionar_aresta(Grafo* g, const char* origem, const char* destino, const char* conteudo);
// Imprime o grafo
void imprimir_grafo(Grafo* g);
// Registra uma comunicação no grafo e nos arquivos
int registrar_comunicacao(Grafo* g, const char* origem, const char* destino, const char* conteudo);
// Carrega mensagens dos arquivos para o grafo
void carregar_mensagens_para_grafo(Grafo* g, const char* pasta_mensagens);
// Libera memória de uma aresta
void liberar_aresta(Aresta* a);
// Libera memória de um vértice
void liberar_vertice(Vertice* v);
// Libera todo o grafo
void desalocar_grafo(Grafo* g);

#endif
