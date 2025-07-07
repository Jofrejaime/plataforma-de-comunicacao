#include "../include/hash.h"

// Calcula o índice de hash para um email. Retorna 0 se email for NULL.
int hash(const char* email) {
	int i;
    if (!email) return 0;
    int soma = 0;
    for (i = 0; email[i] != '\0'; i++)
        soma += email[i];
    return soma % TAM_HASH;
}

// Cria uma nova tabela hash. Retorna NULL em caso de erro de alocação.
HashTable* criar_hash(void) {
	int i;
    HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
    if (!ht) return NULL;
    for (i = 0; i < TAM_HASH; i++)
        ht->tabela[i] = NULL;
    return ht;
}

// Insere um membro na tabela hash. Retorna 1 em caso de sucesso, 0 em caso de erro.
int inserir_membro(HashTable* ht, Membro* novo) {
    if (!ht || !novo) return 0;
    int indice = hash(novo->email);
    novo->prox = ht->tabela[indice];
    ht->tabela[indice] = novo;
    return 1;
}

// Busca um membro pelo email. Retorna ponteiro para o membro ou NULL se não encontrado.
Membro* buscar_membro(HashTable* ht, const char* email) {
    if (!ht || !email) return NULL;
    int indice = hash(email);
    Membro* atual = ht->tabela[indice];
    while (atual) {
        if (strcmp(atual->email, email) == 0)
            return atual;
        atual = atual->prox;
    }
    return NULL;
}

// Remove um membro da tabela hash. Retorna 1 em caso de sucesso, 0 se não encontrado ou erro.
int remover_membro(HashTable* ht, const char* email) {
    if (!ht || !email) return 0;
    int indice = hash(email);
    Membro* atual = ht->tabela[indice];
    Membro* anterior = NULL;
    while (atual) {
        if (strcmp(atual->email, email) == 0) {
            if (anterior)
                anterior->prox = atual->prox;
            else
                ht->tabela[indice] = atual->prox;
            liberar_membro(atual);
            return 1;
        }
        anterior = atual;
        atual = atual->prox;
    }
    return 0;
}

// Libera toda a tabela hash e seus membros.
void desalocar_hash(HashTable* ht) {
	int i;
    if (!ht) return;
    for (i = 0; i < TAM_HASH; i++) {
        Membro* atual = ht->tabela[i];
        while (atual) {
            Membro* prox = atual->prox;
            liberar_membro(atual);
            atual = prox;
        }
    }
    free(ht);
}
