#ifndef EQUIPA_H
#define EQUIPA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    PRIVADA,
    PUBLICA
} TipoEquipa;

typedef struct MembroEquipa {
    char email[100];
    struct MembroEquipa* prox;
} MembroEquipa;

typedef struct Equipa {
    char nome[100];
    TipoEquipa tipo;
    MembroEquipa* membros;
    struct Equipa* prox;
} Equipa;

typedef struct {
    Equipa* lista;
} ListaEquipas;

// Funções públicas
ListaEquipas* criar_lista_equipas();
Equipa* criar_equipa(ListaEquipas* lista, const char* nome, TipoEquipa tipo);
Equipa* buscar_equipa(ListaEquipas* lista, const char* nome);
int adicionar_membro_equipa(Equipa* e, const char* email);
int remover_membro_equipa(Equipa* e, const char* email);
void listar_membros_equipa(Equipa* e);
void imprimir_equipas(ListaEquipas* lista);
void carregar_equipas(ListaEquipas* lista);
void salvar_todas_equipas(ListaEquipas* lista);
#endif
