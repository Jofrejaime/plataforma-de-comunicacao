#include "menu.h"
#include "hash.h"
#include "grafo.h"
#include "equipa.h"
#include "app.h"

int main() {
    verificar_ou_criar_pasta_data();
    HashTable* ht = criar_hash();
    Grafo* g = criar_grafo();
    carregar_mensagens_para_grafo(g, "mensagens");
    ListaEquipas* eqs = criar_lista_equipas();
    carregar_membros(ht);
    carregar_equipas(eqs);
    int opcao;

    do {
        mostrar_menu_principal();
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                menu_login(ht, g, eqs);
                break;
            case 2:
                menu_registro(ht);
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }

    } while (opcao != 0);

    return 0;
}