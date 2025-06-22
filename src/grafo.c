#include "grafo.h"

Grafo* criar_grafo() {
    Grafo* g = (Grafo*)malloc(sizeof(Grafo));
    if (!g) return NULL;
    g->lista_vertices = NULL;
    return g;
}
void verificar_ou_criar_pasta_mensagens() {
    FILE* teste = fopen("mensagens/.verifica", "r");
    if (teste) {
        fclose(teste); // pasta já existe
    } else {
        MKDIR("mensagens");
    }
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

int registrar_comunicacao(Grafo* g, const char* origem, const char* destino, const char* conteudo) {
    verificar_ou_criar_pasta_mensagens();
    Vertice* v_origem = buscar_vertice(g, origem);
    if (!v_origem) v_origem = adicionar_vertice(g, origem);

    Vertice* v_destino = buscar_vertice(g, destino);
    if (!v_destino) v_destino = adicionar_vertice(g, destino);

    // Verifica se já existe ligação entre origem → destino
    Aresta* atual = v_origem->lista_adj;
    while (atual) {
        if (strcmp(atual->destino, destino) == 0)
            break;
        atual = atual->prox;
    }

    // Só adiciona aresta se ainda não existir
    if (!atual) {
        Aresta* nova = (Aresta*)malloc(sizeof(Aresta));
        strcpy(nova->destino, destino);
        strcpy(nova->conteudo, conteudo); // opcional
        nova->prox = v_origem->lista_adj;
        v_origem->lista_adj = nova;
    }

    // Gera data/hora atual
    char linha[512];
    time_t agora = time(NULL);
    struct tm* tempo = localtime(&agora);
    char datahora[64];
    strftime(datahora, sizeof(datahora), "%Y-%m-%d %H:%M", tempo);

    snprintf(linha, sizeof(linha), "%s - ori: %s des: %s - %s\n", datahora, origem, destino, conteudo);

    // Criar caminho do ficheiro: mensagens/origem.txt
    char path_origem[150];
    char path_destino[150];

    snprintf(path_origem, sizeof(path_origem), "mensagens/%s.txt", origem);
    snprintf(path_destino, sizeof(path_destino), "mensagens/%s.txt", destino);

    FILE* f_origem = fopen(path_origem, "a");
    if (f_origem) {
        fputs(linha, f_origem);
        fclose(f_origem);
    }

    FILE* f_destino = fopen(path_destino, "a");
    if (f_destino) {
        fputs(linha, f_destino);
        fclose(f_destino);
    }

    return 1;
}
