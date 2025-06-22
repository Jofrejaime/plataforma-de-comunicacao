#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "membro.h"

#define TAM_HASH 101

typedef struct {
    Membro* tabela[TAM_HASH];
} HashTable;

// Funções públicas
HashTable* criar_hash();
int hash(const char* email);
int inserir_membro(HashTable* ht, Membro* novo);
Membro* buscar_membro(HashTable* ht, const char* email);
int remover_membro(HashTable* ht, const char* email);

#endif
