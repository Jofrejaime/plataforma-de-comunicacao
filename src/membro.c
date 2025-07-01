#include "membro.h"
#include "hash.h"
#include "app.h"
Membro* criar_membro(const char* email, const char* senha, TipoMembro tipo, int salvar) {
    Membro* novo = (Membro*)malloc(sizeof(Membro));
    if (!novo) return NULL;
    strcpy(novo->email, email);
    strcpy(novo->senha, senha);
    novo->tipo = tipo;
    novo->ativo = 1;
    novo->permisao[0] = 1;
    novo->permisao[1] = 1;
    novo->permisao[2] = 1;
    novo->documentos = NULL;
    novo->prox = NULL;
    if (salvar) {
        salvar_membro_em_ficheiro(novo);
    }
    return novo;
}

void imprimir_membro(Membro* membro) {
    const char* tipo_str[] = {"ADMIN", "CORPORATIVO", "CONVIDADO"};
    const char* permissoes[] = {"Adicionar", "Excluir", "Convidar"};
    if (!membro) {
        printf("Membro não encontrado.\n");
        return;
    }
    printf("Email: %s\n", membro->email);
    printf("Tipo: %s\n", tipo_str[membro->tipo]);
    printf("\tPermissões:\n");
    for (int i = 0; i < 3; i++)
        printf("\t%s: %s\n", permissoes[i], membro->permisao[i] ? "Ativo" : "Desativado");
    printf("Ativo: %s\n", membro->ativo ? "Sim" : "Não");
}
