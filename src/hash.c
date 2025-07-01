#include "hash.h"

int hash(const char* email) {
    if (!email) return 0;
    int soma = 0;
    for (int i = 0; email[i] != '\0'; i++)
        soma += email[i];
    return soma % TAM_HASH;
}

HashTable* criar_hash() {
    HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
    if (!ht) return NULL;
    for (int i = 0; i < TAM_HASH; i++)
        ht->tabela[i] = NULL;
    return ht;
}

int inserir_membro(HashTable* ht, Membro* novo) {
    if (!ht || !novo) return 0;
    int indice = hash(novo->email);
    novo->prox = ht->tabela[indice];
    ht->tabela[indice] = novo;
    return 1;
}

Membro* buscar_membro(HashTable* ht, const char* email) {
    if (!ht) return NULL;
    int indice = hash(email);
    Membro* atual = ht->tabela[indice];
    while (atual) {
        if (strcmp(atual->email, email) == 0)
            return atual;
        atual = atual->prox;
    }
    return NULL;
}

int remover_membro(HashTable* ht, const char* email) {
    if (!ht) return 0;
    int indice = hash(email);
    Membro* atual = ht->tabela[indice];
    Membro* anterior = NULL;
    while (atual) {
        if (strcmp(atual->email, email) == 0) {
            if (anterior)
                anterior->prox = atual->prox;
            else
                ht->tabela[indice] = atual->prox;
            free(atual);
            return 1;
        }
        anterior = atual;
        atual = atual->prox;
    }
    return 0;
}
