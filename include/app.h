#ifndef APP_H
#define APP_H

#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include "hash.h"
#include "grafo.h"
#include "menu.h"
#include "equipa.h"
#include "membro.h"

#ifdef _WIN32
#include <direct.h>
#define MKDIR(dir) _mkdir(dir)
#else
#include <sys/stat.h>
#include <sys/types.h>
#define MKDIR(dir) mkdir(dir, 0777)
#endif

// --- Autenticação ---
// Realiza login de um membro. Retorna 1 em caso de sucesso, 0 em caso de erro.
bool login(HashTable* ht, Grafo* g, ListaEquipas* eqs, char* email, char* senha);
// Registra um novo membro. Retorna 1 em caso de sucesso, 0 em caso de erro.
bool registrar(HashTable* ht, const char* email, const char* senha, TipoMembro tipo);

// --- Mensagens ---
// Envia mensagem entre membros. Retorna 1 em caso de sucesso, 0 em caso de erro.
bool enviar_mensagem(HashTable* ht, Grafo* g, const char* origem, const char* destino, const char* conteudo);
// Envia mensagem para uma equipa. Retorna 1 em caso de sucesso, 0 em caso de erro.
bool enviar_mensagem_para_equipa(HashTable* ht, Grafo* g, ListaEquipas* eqs, const char* origem, const char* nome_equipa, const char* conteudo);
// Lista mensagens comuns entre dois usuários
void listar_mensagens_comuns(const char* usuario, const char* outro, bool destino_e_equipa);
// Envia documento (parâmetro destino_e_equipa pode ser ignorado)
void enviar_documento(const char* remetente, const char* destino, bool destino_e_equipa);

// --- Membros ---
// Carrega membros do ficheiro
void carregar_membros(HashTable* ht);
// Salva membro em ficheiro
void salvar_membro_em_ficheiro(Membro* m);
// Atualiza permissão de um membro
void actualizar_permissao(HashTable *ht, const char *email);
// Lista permissões de um membro
void listar_permissao(Membro *m, char **permissao);

// --- Utilitários ---
// Verifica ou cria a pasta de dados
void verificar_ou_criar_pasta_data();
// Retorna o número de strings em um array de strings terminado por NULL
int ft_strlen(char **pt);

#endif
