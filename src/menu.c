#include "menu.h"

void menu_mensagens(HashTable* ht, Grafo* g, const char* usuario, ListaEquipas* eqs) {
    int opcao;
    char destino[100], conteudo[256];
    char *opcs[] = {
        "1. Enviar mensagem",
        "2. Ver equipas",
        "0. Sair",
        "3. Enviar documento",
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
            if (buscar_equipa(eqs, destino)) {
                listar_mensagens_comuns(usuario, destino, true);
                printf("\nNova mensagem para equipa: ");
                fgets(conteudo, sizeof(conteudo), stdin);
                conteudo[strcspn(conteudo, "\n")] = 0;
                int ok = enviar_mensagem_para_equipa(ht, g, eqs, usuario, destino, conteudo);
                if (ok)
                    printf("Mensagem enviada para a equipa com sucesso!\n");
                else
                    printf("Erro ao enviar mensagem para a equipa. Verifique permissões ou existência.\n");
                Sleep(3000);
            } else if (buscar_membro(ht, destino)) {
                listar_mensagens_comuns(usuario, destino, false);
                printf("\nNova mensagem: ");
                fgets(conteudo, sizeof(conteudo), stdin);
                conteudo[strcspn(conteudo, "\n")] = 0;
                int ok = enviar_mensagem(ht, g, usuario, destino, conteudo);
                if (ok)
                    printf("Mensagem enviada com sucesso!\n");
                else
                    printf("Erro ao enviar mensagem. Verifique se o destinatário existe e está ativo.\n");
                Sleep(3000);
            } else {
                printf("Erro: destinatário não encontrado.\n");
                Sleep(3000);
                continue;
            }
        } else if (opcao == 2) {
            menu_equipas_membro(usuario, m->tipo, eqs);
        } else if (opcao == 3) {
            printf("Destino (email ou equipa): ");
            fgets(destino, sizeof(destino), stdin);
            destino[strcspn(destino, "\n")] = 0;
            bool is_equipa = buscar_equipa(eqs, destino) != NULL;
            if (is_equipa || buscar_membro(ht, destino)) {
                enviar_documento(usuario, destino, is_equipa);
                printf("Documento enviado!\n");
            } else {
                printf("Erro: destinatário não encontrado.\n");
                Sleep(3000);
                getch();
            }
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
	char *opcs[] = {"1. -Login", "2. Registrar", " 0. Sair", NULL};
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
        printf("Erro ao cadastrar membro. Verifique se o email já existe ou se os dados são válidos.\n");
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
                    printf("Erro: equipa já existe ou falha de criação.\n");
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
                        printf("Membro já está na equipa ou erro ao adicionar.\n");
                    }
                } else {
                    printf("Equipa ou membro inválido.\n");
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
                        printf("Membro não encontrado na equipa ou erro ao remover.\n");
                    }
                } else {
                    printf("Equipa não encontrada.\n");
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
                printf("Opção inválida.\n");
                  Sleep(3000);
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
            printf("Opção inválida.\n");
              Sleep(3000);
        }

    } while (opcao != 0);
}

int	menu_iterativo(char **opcs)
{
	int select = 0,  teclas;
	int size = ft_strlen(opcs);
	
	while (1){
		system("cls");
		for (int i = 0; i < size; i++){
			if (i == select) printf(">> \033[1;36m %s \033[0m\n", opcs[i]);
			else printf(" %s\n", opcs[i]);
		}
		
		teclas = _getch();// lê um primeiro valor ( 0 ou 224) só depois lê o valor da tecla
		
		if(teclas == 224 || teclas == 0){
			teclas = _getch();
			if(teclas == 72)select = (select - 1) % size;
			if(teclas == 80)select = (select + 1) % size;
			if(select == -1)select = size - 1;
		}else if(teclas == 13)// Enter
			return (select);
		
	}	
}