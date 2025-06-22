#include "app.h"

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
    salvar_membro_em_ficheiro(novo);
    return true;
}

void listar_mensagens_comuns(const char* usuario, const char* outro, bool destino_e_equipa) {
    char path[150];
    FILE* f;

    // Decide qual ficheiro abrir
    if (destino_e_equipa) {
        snprintf(path, sizeof(path), "mensagens/%s.txt", outro); // equipa
    } else {
        snprintf(path, sizeof(path), "mensagens/%s.txt", usuario); // pessoal
    }

    f = fopen(path, "r");
    if (!f) {
        printf("    (sem mensagens anteriores com %s)\n", outro);
        return;
    }

    printf("\n📨 Histórico com %s:\n", outro);

    char linha[512];
    char ultima_data[6] = "";
    int encontrou = 0;

    while (fgets(linha, sizeof(linha), f)) {
        char data[11], hora[6], ori[100], des[100], msg[256];

        int lidos = sscanf(linha, "%10s %5s - ori: %99[^ ] des: %99[^ ] - %[^\n]",
                           data, hora, ori, des, msg);

        if (lidos == 5) {
            int deve_mostrar = 0;

            if (destino_e_equipa) {
                deve_mostrar = 1;
            } else {
                // mostra só se for uma troca entre usuario e outro
                if ((strcmp(ori, outro) == 0 && strcmp(des, usuario) == 0) ||
                    (strcmp(des, outro) == 0 && strcmp(ori, usuario) == 0)) {
                    deve_mostrar = 1;
                }
            }

            if (deve_mostrar) {
                encontrou = 1;

                // Pega MM/DD
                char data_md[6];
                strncpy(data_md, data + 5, 5);
                data_md[5] = '\0';

                // Cabeçalho de data (📅)
                if (strcmp(data_md, ultima_data) != 0) {
                    printf("\n\t📅 %s\n", data_md);
                    strcpy(ultima_data, data_md);
                }

                // Exibição da mensagem
                if (destino_e_equipa) {
                    printf("\t   %s: %s\n", ori, msg);
                } else if (strcmp(ori, usuario) == 0) {
                    printf("\t   %s\n", msg); // enviada por mim
                } else {
                    printf("\t   %s: %s\n", ori, msg); // recebida
                }
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

bool login(HashTable* ht, Grafo* g, ListaEquipas* eqs, char* email, char* senha) {
    Membro* m = buscar_membro(ht, email);
    if (m && strcmp(m->senha, senha) == 0 && m->ativo) {
        printf("Login bem-sucedido!\n");

        if (m->tipo == ADMIN) {
            menu_admin(ht, eqs);
        } else {
            menu_mensagens(ht, g, email, eqs);
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

    // ✅ Esta função já grava no ficheiro e adiciona a ligação no grafo
    if (registrar_comunicacao(g, origem, nome_equipa, conteudo)) {
        printf("Mensagem enviada para a equipa %s.\n", nome_equipa);
        return true;
    }

    printf("Erro ao registrar a comunicação.\n");
    return false;
}


void salvar_membro_em_ficheiro(Membro* m) {
    FILE* f = fopen("data/membros.txt", "a");
    if (f) {
        fprintf(f, "%s %s %d %d\n", m->email, m->senha, m->tipo, m->ativo);
        fclose(f);
    }
}

void carregar_membros(HashTable* ht) {
    FILE* f = fopen("data/membros.txt", "r");
    if (!f) return;

    char email[100], senha[50];
    int tipo, ativo;

    while (fscanf(f, "%s %s %d %d", email, senha, &tipo, &ativo) == 4) {
        Membro* m = criar_membro(email, senha, tipo);
        m->ativo = ativo;
        inserir_membro(ht, m);
    }

    fclose(f);
}

void verificar_ou_criar_pasta_data() {
#ifdef _WIN32
    _mkdir("data");
#else
    mkdir("data", 0777);
#endif
}
