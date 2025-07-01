#include "menu.h"
#include "hash.h"
#include "grafo.h"
#include "equipa.h"
#include "app.h"

int main() {
    verificar_ou_criar_pasta_data();
    verificar_ou_criar_pasta_mensagens();

    // Alocação das estruturas principais com verificação de erro
    HashTable* ht = criar_hash();
    if (!ht) {
        fprintf(stderr, "Erro ao alocar tabela hash. Encerrando.\n");
        return 1;
    }
    Grafo* g = criar_grafo();
    if (!g) {
        fprintf(stderr, "Erro ao alocar grafo. Encerrando.\n");
        desalocar_hash(ht);
        return 1;
    }
    ListaEquipas* eqs = criar_lista_equipas();
    if (!eqs) {
        fprintf(stderr, "Erro ao alocar lista de equipas. Encerrando.\n");
        desalocar_hash(ht);
        desalocar_grafo(g);
        return 1;
    }

    carregar_mensagens_para_grafo(g, "mensagens");
    carregar_membros(ht);
    carregar_equipas(eqs);
    int opcao =  0;

    do {
        opcao = mostrar_menu_principal();
        switch (opcao) {
            case 1:
                menu_login(ht, g, eqs);
                break;
            case 2:
                menu_registro(ht);
                break;
            case 0:
                printf("Saindo...\n");
                Sleep(3000);
                break;
            default:
                printf("Opção inválida.\n");
                Sleep(3000);
                break;
        }
    } while (opcao != 0);

    // Liberação de memória antes de sair
    desalocar_hash(ht);
    desalocar_grafo(g);
    desalocar_lista_equipas(eqs);

    return 0;
}
