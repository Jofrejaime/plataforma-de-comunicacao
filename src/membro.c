#include "membro.h"
#include "hash.h"
#include "app.h"

// Cria um novo membro. Retorna NULL em caso de erro de alocação.
Membro* criar_membro(const char* email, const char* senha, TipoMembro tipo, int salvar) {
    if (!email || !senha) return NULL;
    Membro* novo = (Membro*)malloc(sizeof(Membro));
    if (!novo) return NULL;
    strcpy(novo->email, email);
    strcpy(novo->senha, senha);
    novo->tipo = tipo;
    novo->ativo = 1;
    novo->permissao[0] = 1;
    novo->permissao[1] = 1;
    novo->permissao[2] = 1;
    novo->documentos = NULL;
    novo->prox = NULL;
    return novo;
}

// Imprime informações do membro. Não altera lógica, apenas exibe.
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
        printf("\t%s: %s\n", permissoes[i], membro->permissao[i] ? "Ativo" : "Desativado");
    printf("Ativo: %s\n", membro->ativo ? "Sim" : "Não");
}

// Libera memória de um membro e seus documentos.
void liberar_membro(Membro* membro) {
    if (!membro) return;
    Documento* doc = membro->documentos;
    while (doc) {
        Documento* prox = doc->prox;
        free(doc);
        doc = prox;
    }
    free(membro);
}
