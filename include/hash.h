#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "membro.h"

#define TAM_HASH 101

// Estrutura da tabela hash
typedef struct {
    Membro* tabela[TAM_HASH];
} HashTable;

// Cria uma nova tabela hash
HashTable* criar_hash();
// Função de hash para emails
int hash(const char* email);
// Insere um membro na tabela hash
int inserir_membro(HashTable* ht, Membro* novo);
// Busca um membro pelo email
Membro* buscar_membro(HashTable* ht, const char* email);
// Remove um membro da tabela hash
int remover_membro(HashTable* ht, const char* email);

#endif
