#include "../include/menu.h"

void menu_mensagens(HashTable* ht, Grafo* g, const char* usuario, ListaEquipas* eqs) {
    int opcao;
    char destino[100], conteudo[256];
    char nome_equipa[100], email[100];
    char *opcs[] = {
        "1. Enviar mensagem",
        "2. Ver equipas",
        "3. Enviar documento",
        "4. Listar meus documentos",
        "5. Adicionar membros a equipa",
        "6. Remover Membro da equipa",
        "7. Visualizar meu perfil",
        "0. Sair",
        NULL
    };
    Membro* m = buscar_membro(ht, usuario);
    do {
        printf("\n--- Plataforma (%s) ---\n", usuario);
        opcao = menu_iterativo(opcs);
        if (opcao == 1) {
            printf("Destino (email ou equipa): ");
            fgets(destino, sizeof(destino), stdin);
            destino[strcspn(destino, "\n")] = 0;
            Equipa* eq = buscar_equipa(eqs, destino);
            if (eq) {
                // Verifica se o usuário faz parte da equipa
                int membro = 0;
                MembroEquipa* me = eq->membros;
                while (me) {
                    if (strcmp(me->email, usuario) == 0) {
                        membro = 1;
                        break;
                    }
                    me = me->prox;
                }
                if (membro) {
                    listar_mensagens_comuns(usuario, destino, 1);
                    printf("\nNova mensagem para equipa: ");
                    fgets(conteudo, sizeof(conteudo), stdin);
                    conteudo[strcspn(conteudo, "\n")] = 0;
                    int ok = enviar_mensagem_para_equipa(ht, g, eqs, usuario, destino, conteudo);
                    if (ok)
                        printf("\033[1;32m Mensagem enviada para a equipa com sucesso!\033[0m\n");
                    else
                        printf("\033[1;31m Erro ao enviar mensagem para a equipa. Verifique permissões ou existência.\033[0m\n");
                    Sleep(3000);
                } else {
                    printf("Você não faz parte desta equipa.\n");
                    Sleep(3000);
                }
            } else if (buscar_membro(ht, destino)) {
                listar_mensagens_comuns(usuario, destino, 0);
                printf("\nNova mensagem: ");
                fgets(conteudo, sizeof(conteudo), stdin);
                conteudo[strcspn(conteudo, "\n")] = 0;
                int ok = enviar_mensagem(ht, g, usuario, destino, conteudo);
                if (ok)
                    printf("\033[1;32m Mensagem enviada com sucesso!\033[0m\n");
                else
                    printf("\033[1;31m Erro ao enviar mensagem. Verifique se o destinatário existe e está ativo.\033[0m\n");
                Sleep(3000);
            } else {
                printf("\033[1;31m Erro: destinatário não encontrado.\033[0m\n");
                Sleep(3000);
                continue;
            }
        } else if (opcao == 2) {
            menu_equipas_membro(usuario, m->tipo, eqs);
        } else if (opcao == 3) {
            printf("Destino (email ou equipa): ");
            fgets(destino, sizeof(destino), stdin);
            destino[strcspn(destino, "\n")] = 0;
            int is_equipa = buscar_equipa(eqs, destino) != NULL;
            if (is_equipa || buscar_membro(ht, destino)) {
                enviar_documento(usuario, destino);
                printf("\033[1;32m Documento enviado!\033[0m\n");
            } else {
                printf("\033[1;31m Erro: destinatário não encontrado.\033[0m\n");
                Sleep(3000);
                getch();
            }
        } else if (opcao == 4) {
            listar_documentos(usuario);
            printf("Pressione qualquer tecla para voltar ao menu...");
            getch();
        }else if (opcao == 5){
        	if (m->permissao[0] == 1)
        	{
        		 printf("Nome da equipa: ");
                fgets(nome_equipa, sizeof(nome_equipa), stdin);
                nome_equipa[strcspn(nome_equipa, "\n")] = 0;
                printf("Email do membro: ");
                fgets(email, sizeof(email), stdin);
                email[strcspn(email, "\n")] = 0;
                Equipa* e = buscar_equipa(eqs, nome_equipa);
                Membro* m = buscar_membro(ht, email);
                if (e && m && m->ativo) {
                    int ok = adicionar_membro_equipa(e, email);
                    if (ok) {
                        printf("\033[1;32m Membro adicionado com sucesso!\033[0m\n");
                        salvar_todas_equipas(eqs);
                    } else {
                        printf("\033[1;31m Membro já está na equipa ou erro ao adicionar.\033[0m\n");
                    }
                } else {
                    printf("\033[1;31m Equipa ou membro inválido.\033[0m\n");
                }
                Sleep(3000);
			}else{
				printf("\033[1;31m Não possui perminssao para está acção!\033[0m\n");
				Sleep(3000);
			}
		}else if(opcao == 6){
			if(m->permissao[1] == 1)
			{
				 printf("Nome da equipa: ");
                fgets(nome_equipa, sizeof(nome_equipa), stdin);
                nome_equipa[strcspn(nome_equipa, "\n")] = 0;
                printf("Email do membro: ");
                fgets(email, sizeof(email), stdin);
                email[strcspn(email, "\n")] = 0;
                Equipa* e = buscar_equipa(eqs, nome_equipa);
                if (e) {
                    int ok = remover_membro_equipa(e, email);
                    if (ok) {
                        salvar_todas_equipas(eqs);
                        printf("\033[1;32m Membro removido com sucesso!\033[0m\n");
                    } else {
                        printf("\033[1;31m Membro não encontrado na equipa ou erro ao remover.\033[0m\n");
                    }
                } else {
                    printf("\033[1;31m Equipa não encontrada.\033[0m\n");
                }
			}else
			{
				printf("\033[1;31m Nao possui permissao para esta accao\033[0m\n");
			}
			 Sleep(3000);
		}else if (opcao == 7) {
            printf("\n--- Meu Perfil ---\n");
            printf("Email: %s\n", m->email);
            printf("Tipo: %s\n", m->tipo == 0 ? "ADMIN" : (m->tipo == 1 ? "CORPORATIVO" : "CONVIDADO"));
            printf("Equipas: ");
            int encontrou = 0;
            Equipa* eq = eqs->lista;
            while (eq) {
                MembroEquipa* me = eq->membros;
                while (me) {
                    if (strcmp(me->email, m->email) == 0) {
                        printf("%s ", eq->nome);
                        encontrou = 1;
                        break;
                    }
                    me = me->prox;
                }
                eq = eq->prox;
            }
            if (!encontrou) printf("(Nenhuma)\n");
            else printf("\n");
            printf("Documentos:\n");
            listar_documentos(m->email);
            printf("Pressione qualquer tecla para voltar ao menu...");
            getch();
        } else if (opcao == 0) {
            printf("Voltando ao menu principal...\n");
        } else if (opcao != 0) {
            printf("Opção inválida.\n");
            Sleep(3000);
        }
    } while (opcao != 0);
    printf("Saindo da plataforma...\n");
}

int mostrar_menu_principal() {
    char *opcs[] = {"1. Login", "2. Registrar", "0. Sair", NULL};
    return (menu_iterativo(opcs));
}

void menu_login(HashTable* ht, Grafo* g, ListaEquipas* eqs) {
    char email[100], senha[50];
    printf("\n--- Login ---\n");
    printf("Email: ");
    scanf("%s", email);
    printf("Senha: ");
    scanf("%s", senha);
    getchar(); // limpa ENTER do buffer

    int ok = login(ht, g, eqs, email, senha);
    if (ok) {
        printf("Login bem-sucedido!\n");
        Sleep(3000);
    } else {
        printf("Login falhou. Verifique email, senha ou estado da conta.\n");
        Sleep(3000);
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

    int ok = registrar(ht, email, senha, (TipoMembro)tipo);
    if (ok)
        printf("Membro cadastrado com sucesso!\n");
    else
        printf("Erro ao cadastrar membro. Verifique se o email jÃ¡ existe ou se os dados sÃ£o vÃ¡lidos.\n");
    Sleep(3000);
}

void menu_admin(HashTable* ht, ListaEquipas* eqs) {
    int opcao, tipo;
    char nome_equipa[100], email[100];
    char *menu[] = {
    "1. Criar equipa", 
    "2. Adicionar membro a equipa", 
    "3. Remover membro de equipa",
    "4. Actualizar permissoes",
    "5. Listar equipas",
    "0. Voltar",
     NULL
    };
    
    do{
          opcao = menu_iterativo(menu);

        switch (opcao) {
            case 1:
                printf("Nome da equipa: ");
                fgets(nome_equipa, sizeof(nome_equipa), stdin);
                nome_equipa[strcspn(nome_equipa, "\n")] = 0;

                printf("Tipo (0 = PRIVADA, 1 = PUBLICA): ");
                scanf("%d", &tipo);
                getchar();

                if (criar_equipa(eqs, nome_equipa, tipo, 1)) {
                    printf("Equipa criada com sucesso!\n");
                    Sleep(3000);
                    
                }else{
                    printf("Erro: equipa jÃ¡ existe ou falha de criaÃ§Ã£o.\n");
                    Sleep(3000);
                }
                break;

            case 2:
                printf("Nome da equipa: ");
                fgets(nome_equipa, sizeof(nome_equipa), stdin);
                nome_equipa[strcspn(nome_equipa, "\n")] = 0;
                printf("Email do membro: ");
                fgets(email, sizeof(email), stdin);
                email[strcspn(email, "\n")] = 0;
                Equipa* e = buscar_equipa(eqs, nome_equipa);
                Membro* m = buscar_membro(ht, email);
                if (e && m && m->ativo) {
                    int ok = adicionar_membro_equipa(e, email);
                    if (ok) {
                        printf("Membro adicionado com sucesso!\n");
                        salvar_todas_equipas(eqs);
                    } else {
                        printf("Membro jÃ¡ estÃ¡ na equipa ou erro ao adicionar.\n");
                    }
                } else {
                    printf("Equipa ou membro invÃ¡lido.\n");
                }
                Sleep(3000);
                break;
            case 3:
                printf("Nome da equipa: ");
                fgets(nome_equipa, sizeof(nome_equipa), stdin);
                nome_equipa[strcspn(nome_equipa, "\n")] = 0;
                printf("Email do membro: ");
                fgets(email, sizeof(email), stdin);
                email[strcspn(email, "\n")] = 0;
                e = buscar_equipa(eqs, nome_equipa);
                if (e) {
                    int ok = remover_membro_equipa(e, email);
                    if (ok) {
                        salvar_todas_equipas(eqs);
                        printf("Membro removido com sucesso!\n");
                    } else {
                        printf("Membro nÃ£o encontrado na equipa ou erro ao remover.\n");
                    }
                } else {
                    printf("Equipa nÃ£o encontrada.\n");
                }
                Sleep(3000);
                break;

            case 4:
                printf("Informe o email do user : ");
                scanf("%s",email);
                actualizar_permissao(ht, email);
                break;
            case 5:
                imprimir_equipas(eqs);
                getch();
                break;
            case 0:
                printf("Voltando ao menu anterior...\n");
                break;

            default:
                printf("OpÃ§Ã£o invÃ¡lida.\n");
                  Sleep(3000);
        }

    } while (opcao != 0);
}

void menu_equipas_membro(const char* email, TipoMembro tipo, ListaEquipas* eqs) {
    int opcao;
    Equipa* atual;
    int contador = 0;
    Equipa* lista_visivel[100]; // limite prÃ¡tico

    do {
        contador = 0;
        printf("\n=== Equipas disponÃ­veis para %s ===\n", email);
        atual = eqs->lista;

        while (atual) {
            int mostrar = 0;

            if (tipo == CORPORATIVO && atual->tipo == PUBLICA) {
                mostrar = 1;
            } else if (tipo == CONVIDADO || tipo == CORPORATIVO) {
                // Verifica se estÃ¡ na lista de membros
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
                       atual->tipo == PUBLICA ? "PÃºblica" : "Privada");
                lista_visivel[contador++] = atual;
            }

            atual = atual->prox;
        }

        if (contador == 0) {
            printf("Nenhuma equipa disponÃ­vel para vocÃª.\n");
              Sleep(3000);
            return;
        }

        printf("0. Voltar\n");
        printf("Escolha uma equipa para ver os membros: ");
        scanf("%d", &opcao);
        getchar();

        if (opcao > 0 && opcao <= contador) {
            listar_membros_equipa(lista_visivel[opcao - 1]);
        } else if (opcao != 0) {
            printf("OpÃ§Ã£o invÃ¡lida.\n");
              Sleep(3000);
        }

    } while (opcao != 0);
}

int	menu_iterativo(char **opcs)
{
    int select = 0,  teclas;
    int size = ft_strlen(opcs);
    int i;
    while (1){
        system("cls");
        for (i = 0; i < size; i++){
            if (i == select) printf(">> \033[1;36m %s \033[0m\n", opcs[i]);
            else printf(" %s\n", opcs[i]);
        }
        
        teclas = _getch();// lÃª um primeiro valor ( 0 ou 224) sÃ³ depois lÃª o valor da tecla
        
        if(teclas == 224 || teclas == 0){
            teclas = _getch();
            if(teclas == 72)select = (select - 1) % size;
            if(teclas == 80)select = (select + 1) % size;
            if(select == -1)select = size - 1;
        }else if(teclas == 13)// Enter
        {
          if (select == size - 1) select = 0;
            else select += 1;
            return (select);
        }
        
    }	
}
