#ifndef MEMBRO_H
#define MEMBRO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    ADMIN,
    CORPORATIVO,
    CONVIDADO
} TipoMembro;

typedef struct Documento {
    char nome[100];
    struct Documento* prox;
} Documento;

typedef struct Membro {
    char email[100];
    char senha[50];
    TipoMembro tipo;
    int ativo;
    int permisao[3];
    Documento* documentos;
    struct Membro* prox; // para encadeamento da hash
} Membro;

// Funções públicas
Membro* criar_membro(const char* email, const char* senha, TipoMembro tipo);
void imprimir_membro(Membro* membro); // opcional para debug

#endif
