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

bool 	login(HashTable* ht, Grafo* g, ListaEquipas* eqs, char* email, char* senha);
bool 	registrar(HashTable* ht, const char* email, const char* senha, TipoMembro tipo);
bool 	enviar_mensagem(HashTable* ht, Grafo* g, const char* origem, const char* destino, const char* conteudo);
bool 	enviar_mensagem_para_equipa(HashTable* ht, Grafo* g, ListaEquipas* eqs, const char* origem, const char* nome_equipa, const char* conteudo);
void 	listar_mensagens_comuns(const char* usuario, const char* outro, bool destino_e_equipa);
void 	carregar_membros(HashTable* ht);
void 	verificar_ou_criar_pasta_data();
void 	salvar_membro_em_ficheiro(Membro* m);
void	actualizar_permissao(HashTable *ht, const char *email);
void	listar_permisao(Membro *m, char **permissao);
int	ft_strlen(char **pt);

#endif
