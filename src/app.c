#include "app.h"
#include "documento.h"

// --- Funções de registro e login ---
// Registra um novo membro. Retorna 1 em caso de sucesso, 0 em caso de erro.
bool registrar(HashTable* ht, const char* email, const char* senha, TipoMembro tipo) {
    if (!ht || !email || !senha || !strchr(email, '@')) return 0;
    if (buscar_membro(ht, email)) return 0;
    Membro* novo = criar_membro(email, senha, tipo, 1);
    if (!novo || !inserir_membro(ht, novo)) return 0;
    salvar_membro_em_ficheiro(novo);
    return 1;
}

// Realiza login de um membro. Retorna 1 em caso de sucesso, 0 em caso de erro.
bool login(HashTable* ht, Grafo* g, ListaEquipas* eqs, char* email, char* senha) {
    if (!ht || !email || !senha) return 0;
    Membro* m = buscar_membro(ht, email);
    if (m && strcmp(m->senha, senha) == 0 && m->ativo) {
        if (m->tipo == ADMIN) {
            menu_admin(ht, eqs);
        } else {
            menu_mensagens(ht, g, email, eqs);
        }
        return 1;
    }
    return 0;
}

// --- Funções de mensagens ---
// Lista mensagens comuns entre dois usuários (apenas exibe)
void listar_mensagens_comuns(const char* usuario, const char* outro, bool destino_e_equipa) {
    char path[150];
    FILE* f;
    if (destino_e_equipa) {
        snprintf(path, sizeof(path), "mensagens/%s.txt", outro);
    } else {
        snprintf(path, sizeof(path), "mensagens/%s.txt", usuario);
    }
    f = fopen(path, "r");
    if (!f) {
        printf("    (sem mensagens anteriores com %s)\n", outro);
        return;
    }
    printf("\nHistórico com %s:\n", outro);
    char linha[512];
    char ultima_data[6] = "";
    int encontrou = 0;
    while (fgets(linha, sizeof(linha), f)) {
        char data[11], hora[6], ori[100], des[100], msg[256];
        int lidos = sscanf(linha, "%10s %5s - ori: %99[^ ] des: %99[^ ] - %[^]", data, hora, ori, des, msg);
        if (lidos == 5) {
            int deve_mostrar = 0;
            if (destino_e_equipa) {
                deve_mostrar = 1;
            } else {
                if ((strcmp(ori, outro) == 0 && strcmp(des, usuario) == 0) || (strcmp(des, outro) == 0 && strcmp(ori, usuario) == 0)) {
                    deve_mostrar = 1;
                }
            }
            if (deve_mostrar) {
                encontrou = 1;
                char data_md[6];
                strncpy(data_md, data + 5, 5);
                data_md[5] = '\0';
                if (strcmp(data_md, ultima_data) != 0) {
                    printf("\n\t %s\n", data_md);
                    strcpy(ultima_data, data_md);
                }
                if (destino_e_equipa) {
                    printf("\t   %s: %s\n", ori, msg);
                } else if (strcmp(ori, usuario) == 0) {
                    printf("\t   %s\n", msg);
                } else {
                    printf("\t   %s: %s\n", ori, msg);
                }
            }
        }
    }
    if (!encontrou) {
        printf("    (sem mensagens trocadas)\n");
    }
    fclose(f);
}

// Envia mensagem entre membros. Retorna 1 em caso de sucesso, 0 em caso de erro.
bool enviar_mensagem(HashTable* ht, Grafo* g, const char* origem, const char* destino, const char* conteudo) {
    if (!ht || !g || !origem || !destino || !conteudo) return 0;
    Membro* remetente = buscar_membro(ht, origem);
    if (!remetente || !remetente->ativo) return 0;
    Membro* receptor = buscar_membro(ht, destino);
    if (!receptor && !buscar_vertice(g, destino)) return 0;
    if (!registrar_comunicacao(g, origem, destino, conteudo)) return 0;
    return 1;
}

// Envia mensagem para uma equipa. Retorna 1 em caso de sucesso, 0 em caso de erro.
bool enviar_mensagem_para_equipa(HashTable* ht, Grafo* g, ListaEquipas* eqs, const char* origem, const char* nome_equipa, const char* conteudo) {
    if (!ht || !g || !eqs || !origem || !nome_equipa || !conteudo) return 0;
    Equipa* e = buscar_equipa(eqs, nome_equipa);
    if (!e) return 0;
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
    if (!permitido) return 0;
    if (!registrar_comunicacao(g, origem, nome_equipa, conteudo)) return 0;
    return 1;
}

// --- Funções de membros ---
// Salva membro em ficheiro
void salvar_membro_em_ficheiro(Membro* m) {
    if (!m) return;
    FILE* f = fopen("data/membros.txt", "a");
    if (f) {
        // Novo formato: email;senha;tipo;ativo\n
        fprintf(f, "%s;%s;%d;%d\n", m->email, m->senha, m->tipo, m->ativo);
        fclose(f);
    }
}

// Carrega membros do ficheiro
void carregar_membros(HashTable* ht) {
    if (!ht) return;
    FILE* f = fopen("data/membros.txt", "r");
    if (!f) return;
    char email[100], senha[50];
    int tipo, ativo;
    while (fscanf(f, "%99[^;];%49[^;];%d;%d\n", email, senha, &tipo, &ativo) == 4) {
        Membro* m = criar_membro(email, senha, tipo, 0);
        if (!m) continue;
        m->ativo = ativo;
        inserir_membro(ht, m);
        carregar_documentos_membro(m);
    }
    fclose(f);
}

// Lista permissões de um membro (apenas exibe)
void listar_permissao(Membro *m, char **permissao) {
    if (!m || !permissao) return;
    printf("Permissões do colaborador %s\n", m->email);
    for (int i = 0; i < 3; i++) {
        if (m->permissao[i] == 1)
            printf("%s : ativo!\n", permissao[i]);
        else
            printf("%s : desativado\n", permissao[i]);
    }
}

// Atualiza permissão de um membro
void actualizar_permissao(HashTable *ht, const char *email) {
    if (!ht || !email) return;
    char *permissoes[] = {"1. Adicionar", "2. Excluir", "3. Convidar", "4. Voltar", NULL};
    int select = 0;
    Membro *m = buscar_membro(ht, email);
    if (!m) return;
    select = menu_iterativo(permissoes);
    if (m->permissao[select] == 0) {
        m->permissao[select] = 1;
    } else if (m->permissao[select] == 1) {
        m->permissao[select] = 0;
    }
}

// --- Utilitários ---
// Retorna o número de strings em um array de strings terminado por NULL
int ft_strlen(char *pt[]) {
    if (!pt) return 0;
    int i = 0;
    while (pt[i])
        i++;
    return (i);
}


// Verifica se a pasta existe, e a cria se não existir
void verificar_ou_criar_pasta(const char* nome_pasta) {
    char path[256];
    snprintf(path, sizeof(path), "%s/.verifica", nome_pasta);
    FILE* teste = fopen(path, "r");
    if (teste) {
        fclose(teste); // pasta já existe
    } else {
        MKDIR(nome_pasta);
    }
}
