#include "app.h"
#include "membro.h"

bool login(HashTable* ht, char* email, char* senha) {
    Membro* m = buscar_membro(ht, email);
    if (m && strcmp(m->senha, senha) == 0 && m->ativo) {
        printf("Login bem-sucedido!\n");
        return true;
    }
    printf("Login falhou. Verifique email, senha ou estado da conta.\n");
    return false;
}

bool registrar(HashTable* ht, const char* email, const char* senha, TipoMembro tipo) {
    if (buscar_membro(ht, email)) {
        printf("Email já cadastrado!\n");
        return false;
    }

    Membro* novo = criar_membro(email, senha, tipo);
    if (!novo || !inserir_membro(ht, novo)) {
        printf("Erro ao cadastrar!\n");
        return false;
    }

    printf("Membro cadastrado com sucesso!\n");
    return true;
}
