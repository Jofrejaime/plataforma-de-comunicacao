#include "../include/menu.h"

/* Funções auxiliares para interface */
void imprimir_cabecalho(const char* titulo) {
    system("cls");
    printf("+==============================================================+\n");
    printf("|                    PLATAFORMA DE COMUNICACAO                 |\n");
    printf("+==============================================================+\n");
    printf("| %-60s |\n", titulo);
    printf("+==============================================================+\n");
}

void imprimir_separador(void) {
    printf("\n--------------------------------------------------------------\n");
}

void mensagem_sucesso(const char* msg) {
    printf("\n[SUCESSO] %s\n", msg);
    imprimir_separador();
    printf("Pressione qualquer tecla para continuar...");
    getch();
}

void mensagem_erro(const char* msg) {
    printf("\n[ERRO] %s\n", msg);
    imprimir_separador();
    printf("Pressione qualquer tecla para continuar...");
    getch();
}

void mensagem_info(const char* msg) {
    printf("\n[INFO] %s\n", msg);
}

void menu_mensagens(HashTable* ht, Grafo* g, const char* usuario, ListaEquipas* eqs) {
    int opcao;
    char destino[100], conteudo[256];
    char nome_equipa[100], email[100];
    char titulo[100];
    char *opcs[] = {
        "1. Enviar mensagem",
        "2. Ver equipas",
        "3. Enviar documento",
        "4. Convidar membro",
        "5. Listar meus documentos",
        "6. Adicionar Membro",
        "7. Remover Membro",
        "8. Visualizar meu perfil",
        "9. Bloquear/Desbloquear membro",
        "0. Sair",
        NULL
    };
    Membro* m;
    Equipa* eq;
    int membro;
    MembroEquipa* membro_equipa;
    int ok;
    int is_equipa;
    int encontrou;
    
    m = buscar_membro(ht, usuario);
    do {
        sprintf(titulo, "MENU PRINCIPAL - Usuário: %s (%s)", usuario, 
                m->tipo == 0 ? "ADMIN" : (m->tipo == 1 ? "CORPORATIVO" : "CONVIDADO"));
        imprimir_cabecalho(titulo);
        opcao = menu_iterativo(opcs);
        if (opcao == 1) {
            imprimir_cabecalho("ENVIAR MENSAGEM");
            printf("Digite o destinatario (email ou nome da equipa):\n> ");
            fgets(destino, sizeof(destino), stdin);
            destino[strcspn(destino, "\n")] = 0;
            
            eq = buscar_equipa(eqs, destino);
            if (eq) {
                /* Verifica se é membro ou adiciona automaticamente se corporativo em equipa pública */
                membro = 0;
                membro_equipa = eq->membros;
                while (membro_equipa) {
                    if (strcmp(membro_equipa->email, usuario) == 0) {
                        membro = 1;
                        break;
                    }
                    membro_equipa = membro_equipa->prox;
                }
                
                /* Se não é membro mas é corporativo em equipa pública, adiciona automaticamente */
                if (!membro && eq->tipo == PUBLICA && m->tipo == CORPORATIVO) {
                    if (adicionar_membro_equipa(eq, usuario)) {
                        salvar_todas_equipas(eqs);
                        membro = 1;
                        printf("\n[INFO] Voce foi automaticamente adicionado a equipa publica '%s'\n", eq->nome);
                    }
                }
                if (membro) {
                    printf("\nHistorico de mensagens da equipa '%s':\n", destino);
                    imprimir_separador();
                    listar_mensagens_comuns(usuario, destino, 1);
                    imprimir_separador();
                    printf("Digite sua mensagem para a equipa:\n> ");
                    fgets(conteudo, sizeof(conteudo), stdin);
                    conteudo[strcspn(conteudo, "\n")] = 0;
                    ok = enviar_mensagem_para_equipa(ht, g, eqs, usuario, destino, conteudo);
                    if (ok)
                        mensagem_sucesso("Mensagem enviada para a equipa com sucesso!");
                    else
                        mensagem_erro("Falha ao enviar mensagem. Verifique suas permissões.");
                } else {
                    if (eq->tipo == PRIVADA) {
                        mensagem_erro("Acesso negado! Esta equipa é privada e você não foi convidado.");
                    } else {
                        mensagem_erro("Acesso negado! Apenas membros corporativos podem acessar equipas públicas.");
                    }
                }
            } else if (buscar_membro(ht, destino)) {
                printf("\nConversa com '%s':\n", destino);
                imprimir_separador();
                listar_mensagens_comuns(usuario, destino, 0);
                imprimir_separador();
                printf("Digite sua mensagem:\n> ");
                fgets(conteudo, sizeof(conteudo), stdin);
                conteudo[strcspn(conteudo, "\n")] = 0;
                ok = enviar_mensagem(ht, g, usuario, destino, conteudo);
                if (ok)
                    mensagem_sucesso("Mensagem enviada com sucesso!");
                else
                    mensagem_erro("Falha ao enviar. Destinatario inativo ou voce foi bloqueado.");
            } else {
                mensagem_erro("Destinatário não encontrado! Verifique o email ou nome da equipa.");
                continue;
            }
        } else if (opcao == 2) {
            menu_equipas_membro(usuario, m->tipo, eqs);
        } else if (opcao == 3) {
            imprimir_cabecalho("ENVIAR DOCUMENTO");
            printf("Digite o destinatario (email ou equipa):\n> ");
            fgets(destino, sizeof(destino), stdin);
            destino[strcspn(destino, "\n")] = 0;
            is_equipa = (buscar_equipa(eqs, destino) != NULL) ? 1 : 0;
            if (is_equipa || buscar_membro(ht, destino)) {
                enviar_documento(usuario, destino);
                mensagem_sucesso("Documento enviado e registrado com sucesso!");
            } else {
                mensagem_erro("Destinatário não encontrado! Verifique o email ou nome da equipa.");
            }
        } else if (opcao == 4) {
            imprimir_cabecalho("CONVIDAR MEMBRO");
            printf("Digite o email do convidado:\n> ");
            fgets(destino, sizeof(destino), stdin);
            destino[strcspn(destino, "\n")] = 0;
            
            sprintf(conteudo, "123456");
            ok = registrar(ht, destino, conteudo, CONVIDADO);
            if (ok) {
                printf("\n[INFO] Senha gerada automaticamente: 123456\n");
                mensagem_sucesso("Membro convidado criado! Senha: 123456");
            } else {
                mensagem_erro("Falha ao criar convite! Email pode ja existir.");
            }
        } else if (opcao == 9) {
            imprimir_cabecalho("BLOQUEAR/DESBLOQUEAR MEMBRO");
            printf("1. Bloquear membro\n");
            printf("2. Desbloquear membro\n");
            printf("Escolha uma opcao:\n> ");
            scanf("%d", &ok);
            getchar();
            
            if (ok == 1) {
                printf("Digite o email do membro a bloquear:\n> ");
                fgets(destino, sizeof(destino), stdin);
                destino[strcspn(destino, "\n")] = 0;
                
                if (bloquear_membro(m, destino)) {
                    mensagem_sucesso("Membro bloqueado com sucesso!");
                } else {
                    mensagem_erro("Falha ao bloquear! Membro pode ja estar bloqueado.");
                }
            } else if (ok == 2) {
                printf("Digite o email do membro a desbloquear:\n> ");
                fgets(destino, sizeof(destino), stdin);
                destino[strcspn(destino, "\n")] = 0;
                
                if (desbloquear_membro(m, destino)) {
                    mensagem_sucesso("Membro desbloqueado com sucesso!");
                } else {
                    mensagem_erro("Falha ao desbloquear! Membro pode nao estar bloqueado.");
                }
            }
        } else if (opcao == 5) {
            imprimir_cabecalho("MEUS DOCUMENTOS");
            listar_documentos(usuario);
            imprimir_separador();
            printf("Pressione qualquer tecla para voltar ao menu...");
            getch();
        } else if(opcao == 6)
        {
        	if (m->permissao[0] == 1)
        	{
        		 printf("Nome da equipa: ");
                fgets(nome_equipa, sizeof(nome_equipa), stdin);
                nome_equipa[strcspn(nome_equipa, "\n")] = 0;
                printf("Email do membro: ");
                fgets(email, sizeof(email), stdin);
                email[strcspn(email, "\n")] = 0;
                Equipa* equipa_temp = buscar_equipa(eqs, nome_equipa);
                Membro* membro_temp = buscar_membro(ht, email);
                if (equipa_temp && membro_temp && membro_temp->ativo) {
                    int resultado = adicionar_membro_equipa(equipa_temp, email);
                    if (ok) {
                        printf("\033[1;32m Membro adicionado com sucesso!\033[0m\n");
                        salvar_todas_equipas(eqs);
                    } else {
                        printf("\033[1;31m Membro ja esta na equipa ou erro ao adicionar.\033[0m\n");
                    }
                } else {
                    printf("\033[1;31m Equipa ou membro inv�lido.\033[0m\n");
                }
                Sleep(3000);
			}else{
				printf("\033[1;31m Nao possui perminssao para esta accao!\033[0m\n");
				Sleep(3000);
			}
		}else if(opcao == 7)
		{
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
                        printf("\033[1;31m Membro nao encontrado na equipa ou erro ao remover.\033[0m\n");
                    }
                } else {
                    printf("\033[1;31m Equipa nao encontrada.\033[0m\n");
                }
			}else
			{
				printf("\033[1;31m Nao possui permissao para esta accao\033[0m\n");
			}
			 Sleep(3000);
		}else if (opcao == 8) {
            imprimir_cabecalho("MEU PERFIL");
            printf("Email: %s\n", m->email);
            printf("Tipo: %s\n", m->tipo == 0 ? "ADMINISTRADOR" : (m->tipo == 1 ? "CORPORATIVO" : "CONVIDADO"));
            printf("Equipas: ");
            encontrou = 0;
            eq = eqs->lista;
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
            if (!encontrou) printf("(Nenhuma equipa)\n");
            else printf("\n");
            printf("\nDocumentos:\n");
            imprimir_separador();
            listar_documentos(m->email);
            imprimir_separador();
            printf("Pressione qualquer tecla para voltar ao menu...");
            getch();
        } else if (opcao == 0) {
            mensagem_info("Voltando ao menu principal...");
            Sleep(1000);
        } else if (opcao != 0) {
            mensagem_erro("Opcao invalida! Selecione uma opcao valida.");
        }
    } while (opcao != 0);
    printf("\nObrigado por usar a plataforma! Ate logo...\n");
    Sleep(2000);
}

int mostrar_menu_principal(void) {
    char *opcs[] = {"1. Fazer Login", "2. Registrar-se", "0. Sair", NULL};
    imprimir_cabecalho("BEM-VINDO A PLATAFORMA DE COMUNICACAO");
    return (menu_iterativo(opcs));
}

void menu_login(HashTable* ht, Grafo* g, ListaEquipas* eqs) {
    char email[100], senha[50];
    int ok;
    
    imprimir_cabecalho("AUTENTICACAO DE USUARIO");
    printf("Digite seu email:\n> ");
    scanf("%s", email);
    printf("Digite sua senha:\n> ");
    scanf("%s", senha);
    getchar(); /* limpa ENTER do buffer */

    printf("\nVerificando credenciais...");
    Sleep(1000);
    
    ok = login(ht, g, eqs, email, senha);
    if (ok) {
        mensagem_sucesso("Login realizado com sucesso! Bem-vindo a plataforma!");
    } else {
        mensagem_erro("Falha na autenticacao! Verifique email, senha ou status da conta.");
    }
}

int contar_membros(HashTable* ht) {
    int count = 0;
    int i;
    for (i = 0; i < TAM_HASH; i++) {
        Membro* atual = ht->tabela[i];
        while (atual) {
            count++;
            atual = atual->prox;
        }
    }
    return count;
}

void menu_registro(HashTable* ht) {
    char email[100], senha[50];
    int tipo;
    int ok;
    int total_membros;

    imprimir_cabecalho("REGISTRO DE NOVO MEMBRO");
    printf("Digite seu email:\n> ");
    scanf("%s", email);
    printf("Crie uma senha:\n> ");
    scanf("%s", senha);
    
    total_membros = contar_membros(ht);
    if (total_membros == 0) {
        tipo = ADMIN;
        printf("\n[INFO] Primeiro usuario - Tipo: ADMINISTRADOR\n");
    } else {
        tipo = CORPORATIVO;
        printf("\n[INFO] Tipo de conta: CORPORATIVO\n");
    }

    printf("\nProcessando registro...");
    Sleep(1000);
    
    ok = registrar(ht, email, senha, (TipoMembro)tipo);
    if (ok)
        mensagem_sucesso("Conta criada com sucesso! Voce ja pode fazer login.");
    else
        mensagem_erro("Falha no registro! Email pode ja existir ou dados invalidos.");
}

void menu_admin(HashTable* ht, ListaEquipas* eqs) {
    int opcao, tipo;
    char nome_equipa[100], email[100];
    Equipa* e;
    Membro* m;
    int ok;
    char *menu[] = {
    "1. Enviar mensagem",
    "2. Criar nova equipa", 
    "3. Adicionar membro a equipa", 
    "4. Remover membro da equipa",
    "5. Convidar membro para plataforma",
    "6. Atualizar permissoes",
    "7. Listar todas as equipas",
    "0. Voltar",
     NULL
    };
    
    do{
        imprimir_cabecalho("PAINEL DE ADMINISTRACAO");
        opcao = menu_iterativo(menu);

        switch (opcao) {
            case 1:
                imprimir_cabecalho("ENVIAR MENSAGEM");
                printf("Digite o destinatario (email ou nome da equipa):\n> ");
                fgets(email, sizeof(email), stdin);
                email[strcspn(email, "\n")] = 0;
                
                e = buscar_equipa(eqs, email);
                if (e) {
                    printf("\nHistorico de mensagens da equipa '%s':\n", email);
                    imprimir_separador();
                    listar_mensagens_comuns("admin", email, 1);
                    imprimir_separador();
                    printf("Digite sua mensagem para a equipa:\n> ");
                    fgets(nome_equipa, sizeof(nome_equipa), stdin);
                    nome_equipa[strcspn(nome_equipa, "\n")] = 0;
                    /* Admin sempre pode enviar para qualquer equipa */
                    if (strlen(nome_equipa) > 0) {
                        if (registrar_comunicacao(NULL, "admin", email, nome_equipa)) {
                            mensagem_sucesso("Mensagem enviada para a equipa com sucesso!");
                        } else {
                            mensagem_erro("Falha ao salvar mensagem!");
                        }
                    } else {
                        mensagem_erro("Mensagem vazia nao foi enviada!");
                    }
                } else if (buscar_membro(ht, email)) {
                    printf("\nConversa com '%s':\n", email);
                    imprimir_separador();
                    listar_mensagens_comuns("admin", email, 0);
                    imprimir_separador();
                    printf("Digite sua mensagem:\n> ");
                    fgets(nome_equipa, sizeof(nome_equipa), stdin);
                    nome_equipa[strcspn(nome_equipa, "\n")] = 0;
                    if (strlen(nome_equipa) > 0) {
                        if (registrar_comunicacao(NULL, "admin", email, nome_equipa)) {
                            mensagem_sucesso("Mensagem enviada com sucesso!");
                        } else {
                            mensagem_erro("Falha ao salvar mensagem!");
                        }
                    } else {
                        mensagem_erro("Mensagem vazia nao foi enviada!");
                    }
                } else {
                    mensagem_erro("Destinatario nao encontrado!");
                }
                break;
                
            case 2:
                imprimir_cabecalho("CRIAR NOVA EQUIPA");
                printf("Digite o nome da equipa:\n> ");
                fgets(nome_equipa, sizeof(nome_equipa), stdin);
                nome_equipa[strcspn(nome_equipa, "\n")] = 0;

                printf("\nSelecione o tipo de equipa:\n");
                printf("   0 - PRIVADA (apenas membros convidados)\n");
                printf("   1 - PUBLICA (todos os corporativos podem entrar)\n");
                printf("> ");
                scanf("%d", &tipo);
                getchar();

                if (criar_equipa(eqs, nome_equipa, tipo, 1)) {
                    mensagem_sucesso("Equipa criada e salva com sucesso!");
                }else{
                    mensagem_erro("Falha na criação! Equipa pode já existir.");
                }
                break;

            case 3:
                imprimir_cabecalho("ADICIONAR MEMBRO A EQUIPA");
                printf("Digite o nome da equipa:\n> ");
                fgets(nome_equipa, sizeof(nome_equipa), stdin);
                nome_equipa[strcspn(nome_equipa, "\n")] = 0;
                printf("Digite o email do membro:\n> ");
                fgets(email, sizeof(email), stdin);
                email[strcspn(email, "\n")] = 0;
                
                e = buscar_equipa(eqs, nome_equipa);
                m = buscar_membro(ht, email);
                if (e && m && m->ativo) {
                    ok = adicionar_membro_equipa(e, email);
                    if (ok) {
                        salvar_todas_equipas(eqs);
                        mensagem_sucesso("Membro adicionado à equipa com sucesso!");
                    } else {
                        mensagem_erro("Membro já pertence a esta equipa!");
                    }
                } else {
                    mensagem_erro("Equipa não encontrada ou membro inválido/inativo!");
                }
                break;
            case 4:
                imprimir_cabecalho("REMOVER MEMBRO DA EQUIPA");
                printf("Digite o nome da equipa:\n> ");
                fgets(nome_equipa, sizeof(nome_equipa), stdin);
                nome_equipa[strcspn(nome_equipa, "\n")] = 0;
                printf("Digite o email do membro:\n> ");
                fgets(email, sizeof(email), stdin);
                email[strcspn(email, "\n")] = 0;
                
                e = buscar_equipa(eqs, nome_equipa);
                if (e) {
                    ok = remover_membro_equipa(e, email);
                    if (ok) {
                        salvar_todas_equipas(eqs);
                        mensagem_sucesso("Membro removido da equipa com sucesso!");
                    } else {
                        mensagem_erro("Membro não encontrado nesta equipa!");
                    }
                } else {
                    mensagem_erro("Equipa não encontrada!");
                }
                break;

            case 5:
                imprimir_cabecalho("CONVIDAR MEMBRO PARA PLATAFORMA");
                printf("Digite o email do convidado:\n> ");
                scanf("%s", email);
                getchar();
                
                sprintf(nome_equipa, "123456");
                ok = registrar(ht, email, nome_equipa, CONVIDADO);
                if (ok) {
                    printf("\n[INFO] Senha gerada automaticamente: 123456\n");
                    mensagem_sucesso("Membro convidado criado! Senha: 123456");
                } else {
                    mensagem_erro("Falha ao criar convite! Email pode ja existir.");
                }
                break;
                
            case 6:
                imprimir_cabecalho("ATUALIZAR PERMISSOES");
                printf("Digite o email do usuario:\n> ");
                scanf("%s",email);
                getchar();
                actualizar_permissao(ht, email);
                break;
                
            case 7:
                imprimir_cabecalho("LISTA DE EQUIPAS");
                imprimir_equipas(eqs);
                imprimir_separador();
                printf("Pressione qualquer tecla para voltar...");
                getch();
                break;
                
            case 0:
                mensagem_info("Voltando ao menu anterior...");
                Sleep(1000);
                break;

            default:
                mensagem_erro("Opção inválida! Selecione uma opção válida.");
        }

    } while (opcao != 0);
}

void menu_convidado(HashTable* ht, Grafo* g, const char* usuario, ListaEquipas* eqs) {
    int opcao;
    char titulo[100];
    char *opcs[] = {
        "1. Enviar mensagem",
        "2. Ver minhas equipas",
        "3. Visualizar meu perfil",
        "0. Sair",
        NULL
    };
    Membro* m;
    Equipa* eq;
    int encontrou, membro, ok;
    char destino[100], conteudo[256];
    MembroEquipa* membro_equipa;
    
    (void)g; /* Suprime warning de parametro nao usado */
    
    m = buscar_membro(ht, usuario);
    do {
        sprintf(titulo, "MENU CONVIDADO - Usuario: %s", usuario);
        imprimir_cabecalho(titulo);
        opcao = menu_iterativo(opcs);
        
        if (opcao == 1) {
            imprimir_cabecalho("ENVIAR MENSAGEM");
            printf("Digite o destinatario (email ou nome da equipa):\n> ");
            fgets(destino, sizeof(destino), stdin);
            destino[strcspn(destino, "\n")] = 0;
            
            eq = buscar_equipa(eqs, destino);
            if (eq) {
                /* Verifica se convidado faz parte da equipa */
                membro = 0;
                membro_equipa = eq->membros;
                while (membro_equipa) {
                    if (strcmp(membro_equipa->email, usuario) == 0) {
                        membro = 1;
                        break;
                    }
                    membro_equipa = membro_equipa->prox;
                }
                
                if (membro) {
                    printf("\nHistorico de mensagens da equipa '%s':\n", destino);
                    imprimir_separador();
                    listar_mensagens_comuns(usuario, destino, 1);
                    imprimir_separador();
                    printf("Digite sua mensagem para a equipa:\n> ");
                    fgets(conteudo, sizeof(conteudo), stdin);
                    conteudo[strcspn(conteudo, "\n")] = 0;
                    ok = enviar_mensagem_para_equipa(ht, g, eqs, usuario, destino, conteudo);
                    if (ok)
                        mensagem_sucesso("Mensagem enviada para a equipa com sucesso!");
                    else
                        mensagem_erro("Falha ao enviar mensagem.");
                } else {
                    mensagem_erro("Acesso negado! Voce nao e membro desta equipa.");
                }
            } else if (buscar_membro(ht, destino)) {
                printf("\nConversa com '%s':\n", destino);
                imprimir_separador();
                listar_mensagens_comuns(usuario, destino, 0);
                imprimir_separador();
                printf("Digite sua mensagem:\n> ");
                fgets(conteudo, sizeof(conteudo), stdin);
                conteudo[strcspn(conteudo, "\n")] = 0;
                ok = enviar_mensagem(ht, g, usuario, destino, conteudo);
                if (ok)
                    mensagem_sucesso("Mensagem enviada com sucesso!");
                else
                    mensagem_erro("Falha ao enviar. Destinatario inativo ou voce foi bloqueado.");
            } else {
                mensagem_erro("Destinatario nao encontrado!");
            }
        } else if (opcao == 2) {
            menu_equipas_membro(usuario, m->tipo, eqs);
        } else if (opcao == 3) {
            imprimir_cabecalho("MEU PERFIL");
            printf("Email: %s\n", m->email);
            printf("Tipo: CONVIDADO\n");
            printf("Equipas: ");
            encontrou = 0;
            eq = eqs->lista;
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
            if (!encontrou) printf("(Nenhuma equipa)\n");
            else printf("\n");
            imprimir_separador();
            printf("Pressione qualquer tecla para voltar ao menu...");
            getch();
        } else if (opcao == 0) {
            mensagem_info("Voltando ao menu principal...");
            Sleep(1000);
        } else if (opcao != 0) {
            mensagem_erro("Opcao invalida! Selecione uma opcao valida.");
        }
    } while (opcao != 0);
    printf("\nObrigado por usar a plataforma! Ate logo...\n");
    Sleep(2000);
}

void menu_equipas_membro(const char* email, TipoMembro tipo, ListaEquipas* eqs) {
    int opcao;
    Equipa* atual;
    int contador = 0;
    Equipa* lista_visivel[100]; /* limite prático */
    char titulo[100];

    do {
        contador = 0;
        sprintf(titulo, "EQUIPAS DISPONIVEIS PARA %s", email);
        imprimir_cabecalho(titulo);
        
        atual = eqs->lista;

        while (atual) {
            int mostrar = 0;

            if (tipo == CORPORATIVO && atual->tipo == PUBLICA) {
                mostrar = 1;
            } else if (tipo == CONVIDADO || tipo == CORPORATIVO) {
                /* Verifica se está na lista de membros */
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
                       atual->tipo == PUBLICA ? "Publica" : "Privada");
                lista_visivel[contador++] = atual;
            }

            atual = atual->prox;
        }

        if (contador == 0) {
            mensagem_info("Nenhuma equipa disponível para você.");
            Sleep(2000);
            return;
        }

        printf("\n0. Voltar\n");
        imprimir_separador();
        printf("Escolha uma equipa para ver os membros:\n> ");
        scanf("%d", &opcao);
        getchar();

        if (opcao > 0 && opcao <= contador) {
            Equipa* equipa_selecionada = lista_visivel[opcao - 1];
            
            /* Se e corporativo e equipa publica, verifica se precisa entrar */
            if (tipo == CORPORATIVO && equipa_selecionada->tipo == PUBLICA) {
                int ja_membro = 0;
                MembroEquipa* verificar = equipa_selecionada->membros;
                while (verificar) {
                    if (strcmp(verificar->email, email) == 0) {
                        ja_membro = 1;
                        break;
                    }
                    verificar = verificar->prox;
                }
                
                if (!ja_membro) {
                    printf("\n[INFO] Entrando automaticamente na equipa publica '%s'...\n", equipa_selecionada->nome);
                    if (adicionar_membro_equipa(equipa_selecionada, email)) {
                        salvar_todas_equipas(eqs);
                        printf("[SUCESSO] Voce agora e membro da equipa!\n");
                    }
                    Sleep(2000);
                }
            }
            
            sprintf(titulo, "MEMBROS DA EQUIPA: %s", equipa_selecionada->nome);
            imprimir_cabecalho(titulo);
            listar_membros_equipa(equipa_selecionada);
            imprimir_separador();
            printf("Pressione qualquer tecla para continuar...");
            getch();
        } else if (opcao != 0) {
            mensagem_erro("Opção inválida! Selecione um número válido.");
        }

    } while (opcao != 0);
}

int	menu_iterativo(char **opcs)
{
    int select = 0, teclas, i;
    int size = ft_strlen(opcs);
    
    while (1){
        printf("\n");
        printf("+--------------------------------------------------------------+\n");
        printf("|                     SELECIONE UMA OPCAO                     |\n");
        printf("+--------------------------------------------------------------+\n");
        
        for (i = 0; i < size; i++){
            if (i == select) {
                printf("| > \033[1;36m%-58s\033[0m |\n", opcs[i]);
            } else {
                printf("|   %-58s |\n", opcs[i]);
            }
        }
        
        printf("+--------------------------------------------------------------+\n");
        printf("\nUse as setas para navegar e ENTER para selecionar\n");
        
        teclas = _getch(); /* lê um primeiro valor ( 0 ou 224) só depois lê o valor da tecla */
        
        if(teclas == 224 || teclas == 0){
            teclas = _getch();
            if(teclas == 72)select = (select - 1) % size;
            if(teclas == 80)select = (select + 1) % size;
            if(select == -1)select = size - 1;
        }else if(teclas == 13) /* Enter */
        {
            if (select == size - 1) select = 0;
            else select += 1;
            return (select);
        }
        
        system("cls");
    }	
}
