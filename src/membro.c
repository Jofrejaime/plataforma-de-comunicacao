#include "../include/membro.h"
#include "../include/hash.h"
#include "../include/app.h"

// Cria um novo membro. Retorna NULL em caso de erro de alocação.
Membro* criar_membro(const char* email, const char* senha, TipoMembro tipo, int salvar) {
    Membro* novo;
    (void)salvar; /* Suprime warning de parametro nao usado */
    if (!email || !senha) return NULL;
    novo = (Membro*)malloc(sizeof(Membro));
    if (!novo) return NULL;
    strcpy(novo->email, email);
    strcpy(novo->senha, senha);
    novo->tipo = tipo;
    novo->ativo = 1;
    novo->permissao[0] = 0;
    novo->permissao[1] = 0;
    novo->permissao[2] = 0;
    novo->documentos = NULL;
    novo->bloqueados = NULL;
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
    int i;
    printf("Email: %s\n", membro->email);
    printf("Tipo: %s\n", tipo_str[membro->tipo]);
    printf("\tPermissões:\n");
    for (i = 0; i < 3; i++)
        printf("\t%s: %s\n", permissoes[i], membro->permissao[i] ? "Ativo" : "Desativado");
    printf("Ativo: %s\n", membro->ativo ? "Sim" : "Não");
}

// Libera memória de um membro e seus documentos.
void liberar_membro(Membro* membro) {
    Documento* doc;
    Bloqueado* bloq;
    if (!membro) return;
    
    doc = membro->documentos;
    while (doc) {
        Documento* prox = doc->prox;
        free(doc);
        doc = prox;
    }
    
    bloq = membro->bloqueados;
    while (bloq) {
        Bloqueado* prox = bloq->prox;
        free(bloq);
        bloq = prox;
    }
    
    free(membro);
}

// Bloqueia um membro. Retorna 1 em caso de sucesso, 0 em caso de erro.
int bloquear_membro(Membro* bloqueador, const char* email_bloqueado) {
    Bloqueado* novo;
    Bloqueado* atual;
    
    if (!bloqueador || !email_bloqueado) return 0;
    
    /* Verifica se já está bloqueado */
    atual = bloqueador->bloqueados;
    while (atual) {
        if (strcmp(atual->email, email_bloqueado) == 0) return 0;
        atual = atual->prox;
    }
    
    /* Adiciona à lista de bloqueados */
    novo = (Bloqueado*)malloc(sizeof(Bloqueado));
    if (!novo) return 0;
    
    strcpy(novo->email, email_bloqueado);
    novo->prox = bloqueador->bloqueados;
    bloqueador->bloqueados = novo;
    
    return 1;
}

// Desbloqueia um membro. Retorna 1 em caso de sucesso, 0 se não encontrado.
int desbloquear_membro(Membro* bloqueador, const char* email_desbloqueado) {
    Bloqueado* atual;
    Bloqueado* anterior = NULL;
    
    if (!bloqueador || !email_desbloqueado) return 0;
    
    atual = bloqueador->bloqueados;
    while (atual) {
        if (strcmp(atual->email, email_desbloqueado) == 0) {
            if (anterior) {
                anterior->prox = atual->prox;
            } else {
                bloqueador->bloqueados = atual->prox;
            }
            free(atual);
            return 1;
        }
        anterior = atual;
        atual = atual->prox;
    }
    
    return 0;
}

// Verifica se um membro está bloqueado. Retorna 1 se bloqueado, 0 caso contrário.
int esta_bloqueado(Membro* membro, const char* email_verificar) {
    Bloqueado* atual;
    
    if (!membro || !email_verificar) return 0;
    
    atual = membro->bloqueados;
    while (atual) {
        if (strcmp(atual->email, email_verificar) == 0) return 1;
        atual = atual->prox;
    }
    
    return 0;
}
