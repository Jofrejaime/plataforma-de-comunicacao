#ifndef APP_H
#define APP_H

#include <stdbool.h>
#include "hash.h"
#include "grafo.h"
#include <ctype.h>
#include "menu.h"
#include "equipa.h"

    ListaEquipas* equipas_globais;
    Grafo *grafo_global;
bool login(HashTable* ht, char* email, char* senha);
bool registrar(HashTable* ht, const char* email, const char* senha, TipoMembro tipo);
bool enviar_mensagem(HashTable* ht, Grafo* g, const char* origem, const char* destino, const char* conteudo);
void listar_mensagens_comuns(const char* origem, const char* destino);

#endif
