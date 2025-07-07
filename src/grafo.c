#include "../include/grafo.h"
#include "../include/app.h"

// Cria e inicializa um novo grafo. Retorna NULL em caso de erro de alocação.
Grafo* criar_grafo() {
    Grafo* g = (Grafo*)malloc(sizeof(Grafo));
    if (!g) return NULL;
    g->lista_vertices = NULL;
    return g;
}



// Busca um vértice pelo id. Retorna NULL se não encontrado ou erro.
Vertice* buscar_vertice(Grafo* g, const char* id) {
    if (!g || !id) return NULL;
    Vertice* atual = g->lista_vertices;
    while (atual) {
        if (strcmp(atual->id, id) == 0)
            return atual;
        atual = atual->prox;
    }
    return NULL;
}

// Adiciona um novo vértice se não existir. Retorna NULL em caso de erro.
Vertice* adicionar_vertice(Grafo* g, const char* id) {
    if (!g || !id || buscar_vertice(g, id)) return NULL;
    Vertice* novo = (Vertice*)malloc(sizeof(Vertice));
    if (!novo) return NULL;
    strcpy(novo->id, id);
    novo->lista_adj = NULL;
    novo->prox = g->lista_vertices;
    g->lista_vertices = novo;
    return novo;
}

// Adiciona uma aresta entre dois vértices. Retorna 1 em caso de sucesso, 0 em caso de erro.
int adicionar_aresta(Grafo* g, const char* origem, const char* destino, const char* conteudo) {
    if (!g || !origem || !destino || !conteudo) return 0;
    Vertice* v_origem = buscar_vertice(g, origem);
    if (!v_origem) v_origem = adicionar_vertice(g, origem);
    if (!v_origem) return 0;
    Aresta* a = v_origem->lista_adj;
    while (a) {
        if (strcmp(a->destino, destino) == 0) return 0; // já existe
        a = a->prox;
    }
    Aresta* nova = (Aresta*)malloc(sizeof(Aresta));
    if (!nova) return 0;
    strcpy(nova->destino, destino);
    strcpy(nova->conteudo, conteudo);
    nova->prox = v_origem->lista_adj;
    v_origem->lista_adj = nova;
    return 1;
}

// Imprime o grafo (apenas exibe)
void imprimir_grafo(Grafo* g) {
    if (!g) return;
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

// Registra uma comunicação e salva nos arquivos. Retorna 1 em caso de sucesso, 0 em caso de erro.
int registrar_comunicacao(Grafo* g, const char* origem, const char* destino, const char* conteudo) {
    if (!g || !origem || !destino || !conteudo) return 0;
    Vertice* v_origem = buscar_vertice(g, origem);
    if (!v_origem) v_origem = adicionar_vertice(g, origem);
    Vertice* v_destino = buscar_vertice(g, destino);
    if (!v_destino) v_destino = adicionar_vertice(g, destino);
    // Verifica se a aresta já existe
    Aresta* atual = v_origem->lista_adj;
    while (atual) {
        if (strcmp(atual->destino, destino) == 0) break;
        atual = atual->prox;
    }
    // Se ainda não existe, adiciona a aresta
    if (!atual) {
        if (!adicionar_aresta(g, origem, destino, conteudo)) return 0;
    }
    // Gera linha com data e hora
    char linha[512];
    time_t agora = time(NULL);
    struct tm* tempo = localtime(&agora);
    char datahora[32];
    strftime(datahora, sizeof(datahora), "%Y-%m-%d %H:%M", tempo);
    snprintf(linha, sizeof(linha), "%s;%s;%s;%s\n", datahora, origem, destino, conteudo);
    // Grava em ficheiros
    if (strchr(destino, '@') == NULL) {
        char path[150];
        snprintf(path, sizeof(path), "mensagens/%s.txt", destino);
        FILE* f = fopen(path, "a");
        if (f) {
            fputs(linha, f);
            fclose(f);
        }
    } else {
        char path_origem[150], path_destino[150];
        snprintf(path_origem, sizeof(path_origem), "mensagens/%s.txt", origem);
        snprintf(path_destino, sizeof(path_destino), "mensagens/%s.txt", destino);
        FILE* f1 = fopen(path_origem, "a");
        if (f1) { fputs(linha, f1); fclose(f1); }
        FILE* f2 = fopen(path_destino, "a");
        if (f2) { fputs(linha, f2); fclose(f2); }
    }
    return 1;
}

// Carrega mensagens dos arquivos para o grafo. Não imprime mensagens de erro, apenas retorna se falhar.
void carregar_mensagens_para_grafo(Grafo* g, const char* pasta_mensagens) {
    if (!g || !pasta_mensagens) return;
    DIR* dir = opendir(pasta_mensagens);
    if (!dir) return;
    struct dirent* ent;
    char path[256];
    char linha[512];
    while ((ent = readdir(dir)) != NULL) {
        if (strstr(ent->d_name, ".txt")) {
            snprintf(path, sizeof(path), "%s/%s", pasta_mensagens, ent->d_name);
            FILE* f = fopen(path, "r");
            if (!f) continue;
            while (fgets(linha, sizeof(linha), f)) {
                char datahora[32], ori[100], des[100], msg[256];
                int lidos = sscanf(linha, "%31[^;];%99[^;];%99[^;];%255[^\n]", datahora, ori, des, msg);
                if (lidos == 4) {
                    adicionar_aresta(g, ori, des, msg);
                }
            }
            fclose(f);
        }
    }
    closedir(dir);
}

// Função para liberar memória de uma aresta
void liberar_aresta(Aresta* a) {
    while (a) {
        Aresta* prox = a->prox;
        free(a);
        a = prox;
    }
}

// Função para liberar memória de um vértice e suas arestas
void liberar_vertice(Vertice* v) {
    if (!v) return;
    liberar_aresta(v->lista_adj);
    free(v);
}

// Função para liberar todo o grafo
void desalocar_grafo(Grafo* g) {
    if (!g) return;
    Vertice* v = g->lista_vertices;
    while (v) {
        Vertice* prox = v->prox;
        liberar_vertice(v);
        v = prox;
    }
    free(g);
}

