#ifndef EQUIPA_H
#define EQUIPA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Enum para o tipo de equipa
typedef enum {
    PRIVADA,
    PUBLICA
} TipoEquipa;

// Estrutura para membro de equipa
typedef struct MembroEquipa {
    char email[100];
    struct MembroEquipa* prox;
} MembroEquipa;

// Estrutura para equipa
typedef struct Equipa {
    char nome[100];
    TipoEquipa tipo;
    MembroEquipa* membros;
    struct Equipa* prox;
} Equipa;

// Estrutura para lista de equipas
typedef struct {
    Equipa* lista;
} ListaEquipas;

// Cria uma nova lista de equipas
ListaEquipas* criar_lista_equipas();
// Cria uma nova equipa e adiciona à lista
Equipa* criar_equipa(ListaEquipas* lista, const char* nome, TipoEquipa tipo, int salvar);
// Busca uma equipa pelo nome
Equipa* buscar_equipa(ListaEquipas* lista, const char* nome);
// Adiciona um membro à equipa
int adicionar_membro_equipa(Equipa* e, const char* email);
// Remove um membro da equipa
int remover_membro_equipa(Equipa* e, const char* email);
// Lista os membros de uma equipa
void listar_membros_equipa(Equipa* e);
// Imprime todas as equipas da lista
void imprimir_equipas(ListaEquipas* lista);
// Carrega equipas do ficheiro
void carregar_equipas(ListaEquipas* lista);
// Salva todas as equipas no ficheiro
void salvar_todas_equipas(ListaEquipas* lista);

#endif
