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
    // Verifica ou adiciona vértices
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
        Aresta* nova = (Aresta*)malloc(sizeof(Aresta));
        strcpy(nova->destino, destino);
        strcpy(nova->conteudo, conteudo); // opcional
        nova->prox = v_origem->lista_adj;
        v_origem->lista_adj = nova;
    }

    // Gera linha com data e hora
    char linha[512];
    time_t agora = time(NULL);
    struct tm* tempo = localtime(&agora);
    char datahora[64];
    strftime(datahora, sizeof(datahora), "%Y-%m-%d %H:%M", tempo);
    snprintf(linha, sizeof(linha), "%s - ori: %s des: %s - %s\n", datahora, origem, destino, conteudo);

    // Grava em ficheiros
    if (strchr(destino, '@') == NULL) {
        //destino é uma equipa (não contém '@')
        char path[150];
        snprintf(path, sizeof(path), "mensagens/%s.txt", destino);
        FILE* f = fopen(path, "a");
        if (f) {
            fputs(linha, f);
            fclose(f);
        }
    } else {
        // 📌 destino é um membro
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
void carregar_mensagens_para_grafo(Grafo* g, const char* pasta_mensagens) {
    DIR* dir = opendir(pasta_mensagens);
    if (!dir) {
        printf("Erro: pasta %s não encontrada.\n", pasta_mensagens);
        return;
    }

    struct dirent* ent;
    char path[256];
    char linha[512];

    while ((ent = readdir(dir)) != NULL) {
        if (strstr(ent->d_name, ".txt")) {
            snprintf(path, sizeof(path), "%s/%s", pasta_mensagens, ent->d_name);

            FILE* f = fopen(path, "r");
            if (!f) continue;

            while (fgets(linha, sizeof(linha), f)) {
                char data[11], hora[6], ori[100], des[100], msg[256];

                int lidos = sscanf(linha, "%10s %5s - ori: %99[^ ] des: %99[^ ] - %[^\n]",
                                   data, hora, ori, des, msg);

                if (lidos == 5) {
                    adicionar_aresta(g, ori, des, msg); // só cria ligação se não existir
                }
            }

            fclose(f);
        }
    }

    closedir(dir);
}

