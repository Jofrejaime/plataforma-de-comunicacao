#include "equipa.h"

ListaEquipas* criar_lista_equipas() {
    ListaEquipas* lista = (ListaEquipas*)malloc(sizeof(ListaEquipas));
    if (lista) lista->lista = NULL;
    return lista;
}

Equipa* criar_equipa(ListaEquipas* lista, const char* nome, TipoEquipa tipo) {
    if (!lista || buscar_equipa(lista, nome)) return NULL;

    Equipa* nova = (Equipa*)malloc(sizeof(Equipa));
    strcpy(nova->nome, nome);
    nova->tipo = tipo;
    nova->membros = NULL;
    nova->prox = lista->lista;
    lista->lista = nova;
    salvar_todas_equipas(lista);
    return nova;
}

Equipa* buscar_equipa(ListaEquipas* lista, const char* nome) {
    Equipa* atual = lista->lista;
    while (atual) {
        if (strcmp(atual->nome, nome) == 0)
            return atual;
        atual = atual->prox;
    }
    return NULL;
}

int adicionar_membro_equipa(Equipa* e, const char* email) {
    if (!e) return 0;

    MembroEquipa* m = e->membros;
    while (m) {
        if (strcmp(m->email, email) == 0) return 0; // já está
        m = m->prox;
    }

    MembroEquipa* novo = (MembroEquipa*)malloc(sizeof(MembroEquipa));
    strcpy(novo->email, email);
    novo->prox = e->membros;
    e->membros = novo;
    return 1;
}

int remover_membro_equipa(Equipa* e, const char* email) {
    if (!e) return 0;
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

void listar_membros_equipa(Equipa* e) {
    if (!e) return;
    printf("Membros da equipa %s:\n", e->nome);
    MembroEquipa* m = e->membros;
    while (m) {
        printf(" - %s\n", m->email);
        m = m->prox;
    }
}

void imprimir_equipas(ListaEquipas* lista) {
    Equipa* e = lista->lista;
    while (e) {
        printf("Equipa: %s (%s)\n", e->nome, e->tipo == PUBLICA ? "Pública" : "Privada");
        listar_membros_equipa(e);
        e = e->prox;
    }
}

void carregar_equipas(ListaEquipas* lista) {
    FILE* f = fopen("data/equipas.txt", "r");
    if (!f) return;

    char linha[512];
    while (fgets(linha, sizeof(linha), f)) {
        char nome[100];
        int tipo;
        char* token = strtok(linha, " ");
        strcpy(nome, token);

        token = strtok(NULL, " ");
        tipo = atoi(token);

        Equipa* e = criar_equipa(lista, nome, tipo);

        // Adicionar membros
        while ((token = strtok(NULL, " \n")) != NULL) {
            adicionar_membro_equipa(e, token);
        }
    }

    fclose(f);
}
void salvar_todas_equipas(ListaEquipas* lista) {
    FILE* f = fopen("data/equipas.txt", "w");
    if (!f) return;

    Equipa* e = lista->lista;
    while (e) {
        fprintf(f, "%s %d", e->nome, e->tipo);
        MembroEquipa* m = e->membros;
        while (m) {
            fprintf(f, " %s", m->email);
            m = m->prox;
        }
        fprintf(f, "\n");
        e = e->prox;
    }

    fclose(f);
}

