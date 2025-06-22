#include "menu.h"
#include "app.h"

void mostrar_menu_principal() {
    printf("\n=== Plataforma de Comunicação ===\n");
    printf("1. Login\n");
    printf("2. Registrar\n");
    printf("0. Sair\n");
    printf("Escolha uma opção: ");
}

void menu_login(HashTable* ht) {
    char email[100], senha[50];
    printf("\n--- Login ---\n");
    printf("Email: ");
    scanf("%s", email);
    printf("Senha: ");
    scanf("%s", senha);
    login(ht, email, senha);
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
