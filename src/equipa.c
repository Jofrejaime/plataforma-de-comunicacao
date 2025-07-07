#include "../include/equipa.h"

// Cria uma nova lista de equipas. Retorna NULL em caso de erro de alocação.
ListaEquipas* criar_lista_equipas() {
    ListaEquipas* lista = (ListaEquipas*)malloc(sizeof(ListaEquipas));
    if (!lista) return NULL;
    lista->lista = NULL;
    return lista;
}

// Cria uma nova equipa e adiciona à lista. Retorna NULL em caso de erro ou se já existir.
Equipa* criar_equipa(ListaEquipas* lista, const char* nome, TipoEquipa tipo, int salvar) {
    if (!lista || !nome || strlen(nome) == 0 || buscar_equipa(lista, nome)) return NULL;
    Equipa* nova = (Equipa*)malloc(sizeof(Equipa));
    if (!nova) return NULL;
    strcpy(nova->nome, nome);
    nova->tipo = tipo;
    nova->membros = NULL;
    nova->prox = lista->lista;
    lista->lista = nova;
    if (salvar)
        salvar_todas_equipas(lista);
    return nova;
}

// Busca uma equipa pelo nome. Retorna ponteiro ou NULL se não encontrada.
Equipa* buscar_equipa(ListaEquipas* lista, const char* nome) {
    if (!lista || !nome) return NULL;
    Equipa* atual = lista->lista;
    while (atual) {
        if (strcmp(atual->nome, nome) == 0)
            return atual;
        atual = atual->prox;
    }
    return NULL;
}

// Adiciona um membro à equipa. Retorna 1 em caso de sucesso, 0 em caso de erro.
int adicionar_membro_equipa(Equipa* e, const char* email) {
    if (!e || !email) return 0;
    MembroEquipa* m = e->membros;
    while (m) {
        if (strcmp(m->email, email) == 0) return 0; // já está
        m = m->prox;
    }
    MembroEquipa* novo = (MembroEquipa*)malloc(sizeof(MembroEquipa));
    if (!novo) return 0;
    strcpy(novo->email, email);
    novo->prox = e->membros;
    e->membros = novo;
    return 1;
}

// Remove um membro da equipa. Retorna 1 em caso de sucesso, 0 se não encontrado ou erro.
int remover_membro_equipa(Equipa* e, const char* email) {
    if (!e || !email) return 0;
    MembroEquipa *atual = e->membros, *anterior = NULL;
    while (atual) {
        if (strcmp(atual->email, email) == 0) {
            if (anterior)
                anterior->prox = atual->prox;
            else
                e->membros = atual->prox;
            free(atual);
            return 1;
        }
        anterior = atual;
        atual = atual->prox;
    }
    return 0;
}

// Lista os membros de uma equipa (apenas exibe, não altera lógica)
void listar_membros_equipa(Equipa* e) {
    if (!e) return;
    MembroEquipa* m = e->membros;
    printf("Membros da equipa %s:\n", e->nome);
    while (m) {
        printf(" - %s\n", m->email);
        m = m->prox;
    }
}

// Imprime todas as equipas da lista (apenas exibe)
void imprimir_equipas(ListaEquipas* lista) {
    if (!lista) return;
    Equipa* e = lista->lista;
    while (e) {
        printf("Equipa: %s (%s)\n", e->nome, e->tipo == PRIVADA ? "Privada" : "Publica");
        listar_membros_equipa(e);
        e = e->prox;
    }
}

// Carrega equipas do ficheiro. Não imprime mensagens de erro, apenas retorna se falhar.
void carregar_equipas(ListaEquipas* lista) {
    if (!lista) return;
    FILE* f = fopen("data/equipas.txt", "r");
    if (!f) return;
    char linha[512];
    while (fgets(linha, sizeof(linha), f)) {
        char nome[100], membros[400];
        int tipo;
        // Novo formato: nome_equipa;tipo;membro1,membro2,...\n
        if (sscanf(linha, "%99[^;];%d;%399[^\n]", nome, &tipo, membros) >= 2) {
            Equipa* e = criar_equipa(lista, nome, tipo, 0);
            char* token = strtok(membros, ",");
            while (token) {
                adicionar_membro_equipa(e, token);
                token = strtok(NULL, ",");
            }
        }
    }
    fclose(f);
}

// Salva todas as equipas no ficheiro. Não imprime mensagens de erro, apenas retorna se falhar.
void salvar_todas_equipas(ListaEquipas* lista) {
    if (!lista) return;
    FILE* f = fopen("data/equipas.txt", "w");
    if (!f) return;
    Equipa* e = lista->lista;
    while (e) {
        // Novo formato: nome_equipa;tipo;membro1,membro2,...\n
        fprintf(f, "%s;%d;", e->nome, e->tipo);
        MembroEquipa* m = e->membros;
        int first = 1;
        while (m) {
            if (!first) fprintf(f, ",");
            fprintf(f, "%s", m->email);
            first = 0;
            m = m->prox;
        }
        fprintf(f, "\n");
        e = e->prox;
    }
    fclose(f);
}

// Libera memória de uma equipa e seus membros.
void liberar_equipa(Equipa* e) {
    if (!e) return;
    MembroEquipa* m = e->membros;
    while (m) {
        MembroEquipa* prox = m->prox;
        free(m);
        m = prox;
    }
    free(e);
}

// Libera toda a lista de equipas.
void desalocar_lista_equipas(ListaEquipas* lista) {
    if (!lista) return;
    Equipa* e = lista->lista;
    while (e) {
        Equipa* prox = e->prox;
        liberar_equipa(e);
        e = prox;
    }
    free(lista);
}

