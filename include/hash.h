#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/membro.h"

#define TAM_HASH 101

// Estrutura da tabela hash
typedef struct {
    Membro* tabela[TAM_HASH];
} HashTable;

// Cria uma nova tabela hash. Retorna NULL em caso de erro de alocação.
HashTable* criar_hash(void);
// Função de hash para emails. Retorna 0 se email for NULL.
int hash(const char* email);
// Insere um membro na tabela hash. Retorna 1 em caso de sucesso, 0 em caso de erro.
int inserir_membro(HashTable* ht, Membro* novo);
// Busca um membro pelo email. Retorna ponteiro para o membro ou NULL se não encontrado.
Membro* buscar_membro(HashTable* ht, const char* email);
// Remove um membro da tabela hash. Retorna 1 em caso de sucesso, 0 se não encontrado ou erro.
int remover_membro(HashTable* ht, const char* email);
// Libera toda a tabela hash
void desalocar_hash(HashTable* ht);

#endif
