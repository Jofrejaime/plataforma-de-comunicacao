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

void listar_mensagens_comuns(const char* origem, const char* destino) {
    char path_origem[150];
    snprintf(path_origem, sizeof(path_origem), "mensagens/%s.txt", origem);

    FILE* f = fopen(path_origem, "r");
    if (!f) {
        printf("Sem mensagens anteriores com %s.\n", destino);
        return;
    }

    printf("");
    char linha[512];
    int encontrou = 0;
    while (fgets(linha, sizeof(linha), f)) {
        if (strstr(linha, destino)) {
            printf("%s", linha);
            encontrou = 1;
        }
    }
    fclose(f);

    if (!encontrou) {
        printf("Nenhuma mensagem encontrada com %s.\n", destino);
    }
}

bool enviar_mensagem(HashTable* ht, Grafo* g, const char* origem, const char* destino, const char* conteudo) {
    Membro* remetente = buscar_membro(ht, origem);
    if (!remetente || !remetente->ativo) {
        printf("Erro: remetente inválido ou desativado.\n");
        return false;
    }

    Membro* receptor = buscar_membro(ht, destino);
    if (!receptor && !buscar_vertice(g, destino)) {
        printf("Erro: destino inexistente.\n");
        return false;
    }

    if (registrar_comunicacao(g, origem, destino, conteudo)) {
        printf("Mensagem enviada e registrada com sucesso.\n");
        return true;
    }

    printf("Erro ao registrar mensagem.\n");
    return false;
}
