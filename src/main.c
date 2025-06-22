#include "grafo.h"

int main() {
    Grafo* g = criar_grafo();

    adicionar_aresta(g, "admin@email.com", "user@email.com", "Olá, tudo bem?");
    adicionar_aresta(g, "user@email.com", "Equipa_Dev", "Enviei o relatório.");
    adicionar_aresta(g, "Equipa_Dev", "user@email.com", "Recebido!");

    imprimir_grafo(g);
    return 0;
}
