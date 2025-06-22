#include "menu.h"
#include "app.h"

void menu_mensagens(HashTable* ht, Grafo* g, const char* usuario) {
    int opcao;
    char destino[100], conteudo[256];

    Membro* m = buscar_membro(ht, usuario);

    do {
        printf("\n--- Plataforma (%s) ---\n", usuario);
        printf("1. Enviar mensagem\n");
        printf("2. Ver equipas\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        getchar();

        if (opcao == 1) {
            printf("Destino (email ou equipa): ");
            fgets(destino, sizeof(destino), stdin);
          if (buscar_equipa(equipas_globais, destino)) {
            listar_mensagens_comuns(usuario, destino); // histórico da equipa

            printf("\nNova mensagem para equipa: ");
            fgets(conteudo, sizeof(conteudo), stdin);
            conteudo[strcspn(conteudo, "\n")] = 0;

            enviar_mensagem_para_equipa(ht, g, equipas_globais, usuario, destino, conteudo);
        } else {
            listar_mensagens_comuns(usuario, destino); // histórico com pessoa

            printf("\nNova mensagem: ");
            fgets(conteudo, sizeof(conteudo), stdin);
            conteudo[strcspn(conteudo, "\n")] = 0;

            enviar_mensagem(ht, g, usuario, destino, conteudo);
        }

        } else if (opcao == 2) {
            menu_equipas_membro(usuario, m->tipo, equipas_globais);
        } else if (opcao != 0) {
            printf("Opção inválida.\n");
        }

    } while (opcao != 0);

    printf("Saindo da plataforma...\n");
}

void mostrar_menu_principal() {
    printf("\n=== Plataforma de Comunicação ===\n");
    printf("1. Login\n");
    printf("2. Registrar\n");
    printf("0. Sair\n");
    printf("Escolha uma opção: ");
}

void menu_login(HashTable* ht, Grafo* g) {
    char email[100], senha[50];
    printf("\n--- Login ---\n");
    printf("Email: ");
    scanf("%s", email);
    printf("Senha: ");
    scanf("%s", senha);

    if (login(ht, email, senha)) {
        menu_mensagens(ht, g, email); // entra na plataforma
    }
}

void menu_registro(HashTable* ht) {
    char email[100], senha[50];
    int tipo;

    printf("\n--- Registrar Membro ---\n");
    printf("Email: ");
    scanf("%s", email);
    printf("Senha: ");
    scanf("%s", senha);
    printf("Tipo (0=ADMIN, 1=CORPORATIVO, 2=CONVIDADO): ");
    scanf("%d", &tipo);

    registrar(ht, email, senha, (TipoMembro)tipo);
}

void menu_admin(HashTable* ht, ListaEquipas* eqs) {
    int opcao;
    char nome_equipa[100], email[100];
    int tipo;

    do {
        printf("\n=== Menu Administrador ===\n");
        printf("1. Criar equipa\n");
        printf("2. Adicionar membro a equipa\n");
        printf("3. Remover membro de equipa\n");
        printf("4. Listar equipas\n");
        printf("0. Voltar\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        getchar(); // limpa ENTER

        switch (opcao) {
            case 1:
                printf("Nome da equipa: ");
                fgets(nome_equipa, sizeof(nome_equipa), stdin);
                nome_equipa[strcspn(nome_equipa, "\n")] = 0;

                printf("Tipo (0 = PRIVADA, 1 = PUBLICA): ");
                scanf("%d", &tipo);
                getchar();

                if (criar_equipa(eqs, nome_equipa, tipo))
                    printf("Equipa criada com sucesso!\n");
                else
                    printf("Erro: equipa já existe ou falha de criação.\n");
                break;

            case 2:
                printf("Nome da equipa: ");
                fgets(nome_equipa, sizeof(nome_equipa), stdin);
                nome_equipa[strcspn(nome_equipa, "\n")] = 0;

                printf("Email do membro: ");
                fgets(email, sizeof(email), stdin);
                email[strcspn(email, "\n")] = 0;

                {
                    Equipa* e = buscar_equipa(eqs, nome_equipa);
                    Membro* m = buscar_membro(ht, email);

                    if (e && m && m->ativo) {
                        if (adicionar_membro_equipa(e, email))
                            printf("Membro adicionado com sucesso!\n");
                        else
                            printf("Membro já está na equipa.\n");
                    } else {
                        printf("Equipa ou membro inválido.\n");
                    }
                }
                break;

            case 3:
                printf("Nome da equipa: ");
                fgets(nome_equipa, sizeof(nome_equipa), stdin);
                nome_equipa[strcspn(nome_equipa, "\n")] = 0;

                printf("Email do membro: ");
                fgets(email, sizeof(email), stdin);
                email[strcspn(email, "\n")] = 0;

                {
                    Equipa* e = buscar_equipa(eqs, nome_equipa);
                    if (e) {
                        if (remover_membro_equipa(e, email))
                            printf("Membro removido com sucesso!\n");
                        else
                            printf("Membro não encontrado na equipa.\n");
                    } else {
                        printf("Equipa não encontrada.\n");
                    }
                }
                break;

            case 4:
                imprimir_equipas(eqs);
                break;

            case 0:
                printf("Voltando ao menu anterior...\n");
                break;

            default:
                printf("Opção inválida.\n");
        }

    } while (opcao != 0);
}

void menu_equipas_membro(const char* email, TipoMembro tipo, ListaEquipas* eqs) {
    int opcao;
    Equipa* atual;
    int contador = 0;
    Equipa* lista_visivel[100]; // limite prático

    do {
        contador = 0;
        printf("\n=== Equipas disponíveis para %s ===\n", email);
        atual = eqs->lista;

        while (atual) {
            int mostrar = 0;

            if (tipo == CORPORATIVO && atual->tipo == PUBLICA) {
                mostrar = 1;
            } else if (tipo == CONVIDADO || tipo == CORPORATIVO) {
                // Verifica se está na lista de membros
                MembroEquipa* m = atual->membros;
                while (m) {
                    if (strcmp(m->email, email) == 0) {
                        mostrar = 1;
                        break;
                    }
                    m = m->prox;
                }
            }

            if (mostrar) {
                printf("%d. %s (%s)\n", contador + 1, atual->nome,
                       atual->tipo == PUBLICA ? "Pública" : "Privada");
                lista_visivel[contador++] = atual;
            }

            atual = atual->prox;
        }

        if (contador == 0) {
            printf("Nenhuma equipa disponível para você.\n");
            return;
        }

        printf("0. Voltar\n");
        printf("Escolha uma equipa para ver os membros: ");
        scanf("%d", &opcao);
        getchar();

        if (opcao > 0 && opcao <= contador) {
            listar_membros_equipa(lista_visivel[opcao - 1]);
        } else if (opcao != 0) {
            printf("Opção inválida.\n");
        }

    } while (opcao != 0);
}
