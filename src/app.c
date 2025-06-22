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

void listar_mensagens_comuns(const char* usuario, const char* outro) {
    char path_origem[150];
    snprintf(path_origem, sizeof(path_origem), "mensagens/%s.txt", usuario);

    FILE* f = fopen(path_origem, "r");
    if (!f) {
        printf("Sem mensagens anteriores com %s.\n", outro);
        return;
    }

    printf("\n📨 Histórico com %s:\n", outro);
    char linha[512];
    int encontrou = 0;
    char ultima_data[6] = "";

    while (fgets(linha, sizeof(linha), f)) {
        char data[11], hora[6], ori[100], des[100], msg[256];

        int lidos = sscanf(linha, "%10s %5s - ori: %99[^ ] des: %99[^ ] - %[^\n]",
                           data, hora, ori, des, msg);

        if (lidos == 5 && (strcmp(ori, outro) == 0 || strcmp(des, outro) == 0)) {
            encontrou = 1;

            // MM/DD
            char data_md[6];
            strncpy(data_md, data + 5, 5);
            data_md[5] = '\0';

            // Cabeçalho de data
            if (strcmp(data_md, ultima_data) != 0) {
                printf("\n\t📅 %s\n", data_md);
                strcpy(ultima_data, data_md);
            }

            // Exibição para mensagens com equipas
            if (strcmp(des, outro) == 0) {
                printf("\t   %s: %s\n", ori, msg); // mostra remetente
            } else if (strcmp(ori, usuario) == 0) {
                printf("\t   %s\n", msg); // enviado por mim para outro
            } else {
                printf("\t   %s: %s\n", ori, msg); // recebido de outro
            }
        }
    }

    if (!encontrou) {
        printf("    (sem mensagens trocadas)\n");
    }

    fclose(f);
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

bool login(HashTable* ht, char* email, char* senha) {
    Membro* m = buscar_membro(ht, email);
    if (m && strcmp(m->senha, senha) == 0 && m->ativo) {
        printf("Login bem-sucedido!\n");

        if (m->tipo == ADMIN) {
            menu_admin(ht, equipas_globais);
        } else {
            menu_mensagens(ht, grafo_global, email); // ou como estiver
        }

        return true;
    }
    printf("Login falhou. Verifique email, senha ou estado da conta.\n");
    return false;
}

bool enviar_mensagem_para_equipa(HashTable* ht, Grafo* g, ListaEquipas* eqs, const char* origem, const char* nome_equipa, const char* conteudo) {
    Equipa* e = buscar_equipa(eqs, nome_equipa);
    if (!e) {
        printf("Erro: equipa não existe.\n");
        return false;
    }

    // Verifica se o remetente tem permissão
    Membro* m = buscar_membro(ht, origem);
    int permitido = 0;

    if (e->tipo == PUBLICA && m->tipo == CORPORATIVO) {
        permitido = 1;
    } else {
        MembroEquipa* me = e->membros;
        while (me) {
            if (strcmp(me->email, origem) == 0) {
                permitido = 1;
                break;
            }
            me = me->prox;
        }
    }

    if (!permitido) {
        printf("Acesso negado: você não pertence à equipa.\n");
        return false;
    }

    // Adiciona no grafo (origem → equipa)
    registrar_comunicacao(g, origem, nome_equipa, conteudo);

    // Gravar no ficheiro da equipa
    char path[150];
    snprintf(path, sizeof(path), "mensagens/%s.txt", nome_equipa);

    time_t agora = time(NULL);
    struct tm* tempo = localtime(&agora);
    char datahora[64];
    strftime(datahora, sizeof(datahora), "%Y-%m-%d %H:%M", tempo);

    FILE* f = fopen(path, "a");
    if (f) {
        fprintf(f, "%s - ori: %s des: %s - %s\n", datahora, origem, nome_equipa, conteudo);
        fclose(f);
        printf("Mensagem enviada para a equipa %s.\n", nome_equipa);
        return true;
    }

    printf("Erro ao gravar mensagem no ficheiro da equipa.\n");
    return false;
}
