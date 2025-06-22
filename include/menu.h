#ifndef MENU_H
#define MENU_H

#include "hash.h"
#include "grafo.h"

void mostrar_menu_principal();
void menu_login(HashTable* ht, Grafo* g);
void menu_registro(HashTable* ht);
void menu_mensagens(HashTable* ht, Grafo* g, const char* usuario);

#endif
