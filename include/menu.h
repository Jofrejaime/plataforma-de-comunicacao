#ifndef MENU_H
#define MENU_H

#include "grafo.h"
#include "equipa.h"
#include "membro.h"
#include "hash.h"

void mostrar_menu_principal();
void menu_login(HashTable* ht, Grafo* g);
void menu_registro(HashTable* ht);
void menu_mensagens(HashTable* ht, Grafo* g, const char* usuario);
void menu_admin(HashTable* ht, ListaEquipas* eqs);
void menu_equipas_membro(const char* email, TipoMembro tipo, ListaEquipas* eqs);

#endif
