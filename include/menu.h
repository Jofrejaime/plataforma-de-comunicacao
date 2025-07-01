#ifndef MENU_H
#define MENU_H

#include "grafo.h"
#include "equipa.h"
#include "membro.h"
#include "hash.h"
#include "app.h"
#include <conio.h>
#include <windows.h>

// Mostra o menu principal e retorna a opção escolhida
int mostrar_menu_principal();
// Menu de login
void menu_login(HashTable* ht, Grafo* g, ListaEquipas* eqs);
// Menu de registro
void menu_registro(HashTable* ht);
// Menu de mensagens
void menu_mensagens(HashTable* ht, Grafo* g, const char* usuario, ListaEquipas* eqs);
// Menu de administração
void menu_admin(HashTable* ht, ListaEquipas* eqs);
// Menu para equipas de um membro
void menu_equipas_membro(const char* email, TipoMembro tipo, ListaEquipas* eqs);
// Menu iterativo para seleção de opções
int menu_iterativo(char **opcs);

#endif
