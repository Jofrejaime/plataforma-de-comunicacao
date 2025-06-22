#include "membro.h"

Membro* criar_membro(const char* email, const char* senha, TipoMembro tipo) {
    Membro* novo = (Membro*)malloc(sizeof(Membro));
    if (!novo) return NULL;
    strcpy(novo->email, email);
    strcpy(novo->senha, senha);
    novo->tipo = tipo;
    novo->ativo = 1;
    novo->documentos = NULL;
    novo->prox = NULL;
    return novo;
}

void imprimir_membro(Membro* membro) {
    if (!membro) return;
    printf("Email: %s\n", membro->email);
    printf("Tipo: %d\n", membro->tipo);
    printf("Ativo: %d\n", membro->ativo);
}
