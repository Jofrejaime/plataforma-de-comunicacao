#include "grafo.h"

Grafo* criar_grafo() {
    Grafo* g = (Grafo*)malloc(sizeof(Grafo));
    if (!g) return NULL;
    g->lista_vertices = NULL;
    return g;
}

Vertice* buscar_vertice(Grafo* g, const char* id) {
    Vertice* atual = g->lista_vertices;
    while (atual) {
        if (strcmp(atual->id, id) == 0)
            return atual;
        atual = atual->prox;
    }
    return NULL;
}

Vertice* adicionar_vertice(Grafo* g, const char* id) {
    if (buscar_vertice(g, id)) return NULL;

    Vertice* novo = (Vertice*)malloc(sizeof(Vertice));
    strcpy(novo->id, id);
    novo->lista_adj = NULL;
    novo->prox = g->lista_vertices;
    g->lista_vertices = novo;
    return novo;
}

int adicionar_aresta(Grafo* g, const char* origem, const char* destino, const char* conteudo) {
    Vertice* v_origem = buscar_vertice(g, origem);
    if (!v_origem)
        v_origem = adicionar_vertice(g, origem);

    Vertice* v_destino = buscar_vertice(g, destino);
    if (!v_destino)
        v_destino = adicionar_vertice(g, destino);

    Aresta* nova = (Aresta*)malloc(sizeof(Aresta));
    strcpy(nova->destino, destino);
    strcpy(nova->conteudo, conteudo);
    nova->prox = v_origem->lista_adj;
    v_origem->lista_adj = nova;

    return 1;
}

void imprimir_grafo(Grafo* g) {
    Vertice* v = g->lista_vertices;
    while (v) {
        printf("%s -> ", v->id);
        Aresta* a = v->lista_adj;
        while (a) {
            printf("[%s: \"%s\"] -> ", a->destino, a->conteudo);
            a = a->prox;
        }
        printf("NULL\n");
        v = v->prox;
    }
}
