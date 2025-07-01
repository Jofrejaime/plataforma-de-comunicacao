#ifndef MEMBRO_H
#define MEMBRO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Enum para o tipo de membro
typedef enum {
    ADMIN,
    CORPORATIVO,
    CONVIDADO
} TipoMembro;

// Estrutura para documento do membro
typedef struct Documento {
    char nome[100];
    struct Documento* prox;
} Documento;

// Estrutura para membro
typedef struct Membro {
    char email[100];
    char senha[50];
    TipoMembro tipo;
    int ativo;
    int permissao[3]; // Corrigido nome do campo
    Documento* documentos;
    struct Membro* prox; // para encadeamento da hash
} Membro;

// Cria um novo membro
Membro* criar_membro(const char* email, const char* senha, TipoMembro tipo, int salvar);
// Imprime informações do membro (opcional para debug)
void imprimir_membro(Membro* membro);

#endif
