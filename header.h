
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    ADMIN,
    CORPORATIVO,
    CONVIDADO
} tipoMembro;

typedef struct Documento {
    char nome[100];
    struct Documento *prox;
} documento;

typedef struct Membro {
    char email[100];
    char senha[50];
    char nome[50];
    TipoMembro tipo;
    int ativo; // 1 = ativo, 0 = desativado
    Documento *documentos; // Lista ligada de documentos
    struct Equipa *equipas; // lista de equipas
    int totalEquipas;
    struct Membro *prox; // Para lista hash (colisões)
} membro;

typedef struct Equipa {
    char nome[50];
    TipoEquipa tipo;
    Membro *membros; // lista de membros
    int totalMembros;
    struct Equipa *prox; // Para lista hash (colisões)
} equipa;

typedef struct Mensagem {
    char origem[100]; // email de origem
    char destino[100]; // email ou nome da equipa
    char conteudo[256];
    int isDocumento;
    struct Mensagem *prox;
} mensagem;
