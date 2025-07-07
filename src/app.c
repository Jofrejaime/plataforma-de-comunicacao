#include "../include/app.h"
#include "../include/documento.h"
#include "../include/membro.h"
#include "../include/hash.h"
#include "../include/grafo.h"
#include "../include/menu.h"
// --- Funções de registro e login ---
// Registra um novo membro. Retorna 1 em caso de sucesso, 0 em caso de erro.
int registrar(HashTable* ht, const char* email, const char* senha, TipoMembro tipo) {
    Membro* novo;
    if (!ht || !email || !senha || !strchr(email, '@')) return 0;
    if (buscar_membro(ht, email)) return 0;
    novo = criar_membro(email, senha, tipo, 1);
    if (!novo || !inserir_membro(ht, novo)) return 0;
    salvar_membro_em_ficheiro(novo);
    return 1;
}

// Realiza login de um membro. Retorna 1 em caso de sucesso, 0 em caso de erro.
int login(HashTable* ht, Grafo* g, ListaEquipas* eqs, char* email, char* senha) {
    Membro* m;
    if (!ht || !email || !senha) return 0;
    m = buscar_membro(ht, email);
    if (m && strcmp(m->senha, senha) == 0 && m->ativo) {
        if (m->tipo == ADMIN) {
            menu_admin(ht, eqs);
        } else if (m->tipo == CONVIDADO) {
            menu_convidado(ht, g, email, eqs);
        } else {
            menu_mensagens(ht, g, email, eqs);
        }
        return 1;
    }
    return 0;
}

// --- Funções de mensagens ---
// Lista mensagens comuns entre dois usuários (apenas exibe)
void listar_mensagens_comuns(const char* usuario, const char* outro, int destino_e_equipa) {
    char path[150];
    FILE* f;
    char linha[512];
    char last_date[11] = ""; /* yyyy-mm-dd\0 */
    char last_year[5] = "";
    int encontrou = 0;
    
    if (destino_e_equipa) {
        sprintf(path, "mensagens/%s.txt", outro);
    } else {
        sprintf(path, "mensagens/%s.txt", usuario);
    }
    f = fopen(path, "r");
    if (!f) {
        printf("    (sem mensagens anteriores com %s)\n", outro);
        return;
    }
    
    printf(" ");
    while (fgets(linha, sizeof(linha), f)) {
        char datahora[32], ori[100], des[100], msg[256];
        int lidos = sscanf(linha, "%31[^;];%99[^;];%99[^;];%255[^\n]", datahora, ori, des, msg);
        if (lidos == 4) {
            int deve_mostrar = 0;
            if (destino_e_equipa) {
                deve_mostrar = 1;
            } else {
                if ((strcmp(ori, outro) == 0 && strcmp(des, usuario) == 0) || (strcmp(des, outro) == 0 && strcmp(ori, usuario) == 0)) {
                    deve_mostrar = 1;
                }
            }
            if (deve_mostrar) {
                char ano[5], mes[3], dia[3];
                char data_atual[11];
                encontrou = 1;
                
                /* Extrai ano, mês, dia */
                strncpy(ano, datahora, 4); ano[4] = '\0';
                strncpy(mes, datahora+5, 2); mes[2] = '\0';
                strncpy(dia, datahora+8, 2); dia[2] = '\0';
                sprintf(data_atual, "%s-%s-%s", ano, mes, dia);
                
                /* Mostra ano se mudou */
                if (strcmp(last_year, ano) != 0) {
                    printf("%s-", ano);
                    strcpy(last_year, ano);
                    last_date[0] = '\0'; /* força mostrar data */
                }
                /* Mostra data (mm-dd) se mudou */
                if (strcmp(last_date, data_atual) != 0) {
                    printf("%s-%s\n", mes, dia);
                    strcpy(last_date, data_atual);
                }
                /* Mostra nome do oposto apenas se for mensagem dele */
                if (strcmp(ori, usuario) == 0) {
                    printf("    %s\n", msg);
                } else {
                    printf("%s: %s\n", ori, msg);
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
int enviar_mensagem(HashTable* ht, Grafo* g, const char* origem, const char* destino, const char* conteudo) {
    Membro* remetente;
    Membro* receptor;
    if (!ht || !g || !origem || !destino || !conteudo) return 0;
    remetente = buscar_membro(ht, origem);
    if (!remetente || !remetente->ativo) return 0;
    receptor = buscar_membro(ht, destino);
    if (!receptor && !buscar_vertice(g, destino)) return 0;
    
    /* Verifica se o destinatário bloqueou o remetente */
    if (receptor && esta_bloqueado(receptor, origem)) return 0;
    
    if (!registrar_comunicacao(g, origem, destino, conteudo)) return 0;
    return 1;
}

// Envia mensagem para uma equipa. Retorna 1 em caso de sucesso, 0 em caso de erro.
int enviar_mensagem_para_equipa(HashTable* ht, Grafo* g, ListaEquipas* eqs, const char* origem, const char* nome_equipa, const char* conteudo) {
    Equipa* e;
    Membro* m;
    int permitido = 0;
    MembroEquipa* me;
    
    if (!ht || !g || !eqs || !origem || !nome_equipa || !conteudo) return 0;
    e = buscar_equipa(eqs, nome_equipa);
    if (!e) return 0;
    m = buscar_membro(ht, origem);
    
    if (e->tipo == PUBLICA && m->tipo == CORPORATIVO) {
        permitido = 1;
    } else {
        me = e->membros;
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
    FILE* f;
    char email[100], senha[50];
    int tipo, ativo;
    Membro* m;
    
    if (!ht) return;
    f = fopen("data/membros.txt", "r");
    if (!f) return;
    
    while (fscanf(f, "%99[^;];%49[^;];%d;%d\n", email, senha, &tipo, &ativo) == 4) {
        m = criar_membro(email, senha, tipo, 0);
        if (!m) continue;
        m->ativo = ativo;
        inserir_membro(ht, m);
        carregar_documentos_membro(m);
    }
    fclose(f);
}

// Lista permissões de um membro (apenas exibe)
void listar_permissao(Membro *m, char **permissao) {
	int i;
    if (!m || !permissao) return;
    printf("Permissões do colaborador %s\n", m->email);
    for (i = 0; i < 3; i++) {
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
    select -= 1;
    if (m->permissao[select] == 0) {
        m->permissao[select] = 1;
        printf("\033[1;32m permins�o alterada com sucesso ja pode executar!\033[0m\n");
    } else if (m->permissao[select] == 1) {
        m->permissao[select] = 0;
        printf("\033[1;32m permins�o alterada com sucesso ja nao pode executar!\033[0m\n");
    }
    Sleep(3000);
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

void verificar_ou_criar_pasta(const char* nome_pasta) {
    char path[256];
    FILE* teste;
    
    sprintf(path, "%s/.verifica", nome_pasta);
    teste = fopen(path, "r");
    if (teste) {
        fclose(teste);
    } else {
        MKDIR(nome_pasta);
    }
}
