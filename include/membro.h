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

// Estrutura para lista de bloqueados
typedef struct Bloqueado {
    char email[100];
    struct Bloqueado* prox;
} Bloqueado;

// Estrutura para membro
typedef struct Membro {
    char email[100];
    char senha[50];
    TipoMembro tipo;
    int ativo;
    int permissao[3];
    Documento* documentos;
    Bloqueado* bloqueados; // lista de membros bloqueados
    struct Membro* prox; // para encadeamento da hash
} Membro;

// Cria um novo membro. Retorna NULL em caso de erro de alocação.
Membro* criar_membro(const char* email, const char* senha, TipoMembro tipo, int salvar);
// Imprime informações do membro (opcional para debug)
void imprimir_membro(Membro* membro);
// Libera memória de um membro e seus documentos
void liberar_membro(Membro* membro);
// Funções de bloqueio
int bloquear_membro(Membro* bloqueador, const char* email_bloqueado);
int desbloquear_membro(Membro* bloqueador, const char* email_desbloqueado);
int esta_bloqueado(Membro* membro, const char* email_verificar);

#endif
