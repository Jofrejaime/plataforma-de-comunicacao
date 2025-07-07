#ifndef MENU_H
#define MENU_H

#include "../include/grafo.h"
#include "../include/equipa.h"
#include "../include/membro.h"
#include "../include/hash.h"
#include "../include/app.h"
#include "../include/documento.h"
#include <conio.h>
#include <windows.h>

// Mostra o menu principal e retorna a opção escolhida (int)
int mostrar_menu_principal(void);
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
// Menu especifico para membros convidados
void menu_convidado(HashTable* ht, Grafo* g, const char* usuario, ListaEquipas* eqs);
// Menu iterativo para seleção de opções, retorna índice da opção escolhida
int menu_iterativo(char **opcs);
// Funções auxiliares de interface
void imprimir_cabecalho(const char* titulo);
void imprimir_separador(void);
void mensagem_sucesso(const char* msg);
void mensagem_erro(const char* msg);
void mensagem_info(const char* msg);
// Função para contar membros
int contar_membros(HashTable* ht);
// Funções auxiliares de interface
void imprimir_cabecalho(const char* titulo);
void imprimir_separador(void);
void mensagem_sucesso(const char* msg);
void mensagem_erro(const char* msg);
void mensagem_info(const char* msg);
// Função para contar membros
int contar_membros(HashTable* ht);

#endif
