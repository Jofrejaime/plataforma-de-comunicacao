#include "../include/menu.h"
#include "../include/hash.h"
#include "../include/grafo.h"
#include "../include/equipa.h"
#include "../include/app.h"
#include <locale.h>

int main() {
    HashTable* ht;
    Grafo* g;
    ListaEquipas* eqs;
    int opcao = 0;
    
    setlocale(LC_ALL, "pt_PT.UTF-8");
    verificar_ou_criar_pasta("data");
    verificar_ou_criar_pasta("mensagens");
    verificar_ou_criar_pasta("documentos");

    /* Alocação das estruturas principais com verificação de erro */
    ht = criar_hash();
    if (!ht) {
        fprintf(stderr, "Erro ao alocar tabela hash. Encerrando.\n");
        return 1;
    }
    g = criar_grafo();
    if (!g) {
        fprintf(stderr, "Erro ao alocar grafo. Encerrando.\n");
        desalocar_hash(ht);
        return 1;
    }
    eqs = criar_lista_equipas();
    if (!eqs) {
        fprintf(stderr, "Erro ao alocar lista de equipas. Encerrando.\n");
        desalocar_hash(ht);
        desalocar_grafo(g);
        return 1;
    }

    carregar_mensagens_para_grafo(g, "mensagens");
    carregar_membros(ht);
    carregar_equipas(eqs);

    do {
        opcao = mostrar_menu_principal();
        // Limpa a tela antes de mostrar o menu novamente
        system("cls");
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
