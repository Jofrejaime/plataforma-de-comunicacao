#ifndef MENU_H
#define MENU_H

#include "grafo.h"
#include "equipa.h"
#include "membro.h"
#include "hash.h"
#include "app.h"
#include "documento.h"
#include <conio.h>
#include <windows.h>

// Mostra o menu principal e retorna a opção escolhida (int)
int mostrar_menu_principal();
// Menu de login: exibe mensagens apropriadas ao usuário
void menu_login(HashTable* ht, Grafo* g, ListaEquipas* eqs);
// Menu de registro: exibe mensagens apropriadas ao usuário
void menu_registro(HashTable* ht);
// Menu de mensagens: exibe mensagens apropriadas ao usuário
void menu_mensagens(HashTable* ht, Grafo* g, const char* usuario, ListaEquipas* eqs);
// Menu de administração: exibe mensagens apropriadas ao usuário
void menu_admin(HashTable* ht, ListaEquipas* eqs);
// Menu para equipas de um membro: exibe mensagens apropriadas ao usuário
void menu_equipas_membro(const char* email, TipoMembro tipo, ListaEquipas* eqs);
// Menu iterativo para seleção de opções, retorna índice da opção escolhida
int menu_iterativo(char **opcs);

#endif
