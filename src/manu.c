#include "menu.h"
#include "app.h"

void menu_mensagens(HashTable* ht, Grafo* g, const char* usuario) {
    int opcao;
    char destino[100], conteudo[256];

    do {
        printf("\n--- Enviar Mensagens (%s) ---\n", usuario);
        printf("1. Enviar nova mensagem\n");
        printf("0. Sair da plataforma\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        getchar(); // limpa ENTER

        if (opcao == 1) {
            printf("Destino (email ou equipa): ");
            fgets(destino, sizeof(destino), stdin);
            destino[strcspn(destino, "\n")] = 0;

            // NOVO: mostrar mensagens anteriores
            listar_mensagens_comuns(usuario, destino);

            printf("\nNova mensagem: ");
            fgets(conteudo, sizeof(conteudo), stdin);
            conteudo[strcspn(conteudo, "\n")] = 0;

            enviar_mensagem(ht, g, usuario, destino, conteudo);
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
