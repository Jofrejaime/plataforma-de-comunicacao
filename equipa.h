typedef enum {
    PUBLICA,
    PRIVADA
} tipoEquipa;

typedef struct Equipa {
    char nome[50];
    TipoEquipa tipo;
    Membro *membros; // lista de membros
    int totalMembros;
    struct Equipa *prox; // Para lista hash (colisões)
} equipa;

equipa	criar_equipa(char *nome, int tipo);
