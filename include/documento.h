#ifndef DOCUMENTO_H
#define DOCUMENTO_H

#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
// Registra um documento enviado ou recebido por um membro
// tipo: "ENVIADO" ou "RECEBIDO"
// participante: email do outro membro
// data: data do envio/recebimento
// nome_arquivo: nome do arquivo do documento
void registrar_documento(const char* email, const char* tipo, const char* participante, const char* data, const char* nome_arquivo);

// Lista todos os documentos enviados e recebidos por um membro
void listar_documentos(const char* email);

// Envia documento para um membro ou equipa
void enviar_documento(const char* remetente, const char* destino, bool destino_e_equipa);

// Função utilitária para criar/verificar pasta
void verificar_ou_criar_pasta(const char* nome_pasta);

// Carrega a lista de documentos do membro a partir do arquivo de log
void carregar_documentos_membro(Membro* membro);

// Lê o conteúdo de um arquivo físico de documento no novo padrão CSV
void ler_documento_fisico(const char* caminho);

#endif
