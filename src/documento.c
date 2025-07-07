#include "../include/documento.h"
#include "../include/membro.h"

void registrar_documento(const char* email, const char* tipo, const char* participante, const char* data, const char* nome_arquivo) {
    char path[256];
    FILE* f;
    
    if (!email || !tipo || !participante || !data || !nome_arquivo) return;
    sprintf(path, "documentos/%s_docs.txt", email);
    f = fopen(path, "a");
    if (!f) return;
    fprintf(f, "%s;%s;%s;%s\n", tipo, participante, data, nome_arquivo);
    fclose(f);
}

void listar_documentos(const char* email) {
    char path[256];
    FILE* f;
    char linha[512];
    
    if (!email) return;
    sprintf(path, "documentos/%s_docs.txt", email);
    f = fopen(path, "r");
    if (!f) {
        printf("Nenhum documento encontrado para %s.\n", email);
        return;
    }
    
    printf("%-10s | %-30s | %-12s | %s\n", "TIPO", "PARTICIPANTE", "DATA", "ARQUIVO");
    printf("--------------------------------------------------------------------------------\n");
    while (fgets(linha, sizeof(linha), f)) {
        char tipo[16], participante[100], data[32], nome_arquivo[128];
        if (sscanf(linha, "%15[^;];%99[^;];%31[^;];%127[^\n]", tipo, participante, data, nome_arquivo) == 4) {
            printf("%-10s | %-30s | %-12s | %s\n", tipo, participante, data, nome_arquivo);
        }
    }
    fclose(f);
}

void enviar_documento(const char* remetente, const char* destino) {
    char pasta[100];
    time_t agora;
    struct tm* tempo;
    char data[11];
    char nome_ficheiro[200];
    FILE* f;
    char conteudo[512];
    
    if (!remetente || !destino) return;
    sprintf(pasta, "documentos/%s", remetente);
    verificar_ou_criar_pasta(pasta);
    agora = time(NULL);
    tempo = localtime(&agora);
    strftime(data, sizeof(data), "%Y-%m-%d", tempo);
    sprintf(nome_ficheiro, "%s/para_%s_%s.txt", pasta, destino, data);
    f = fopen(nome_ficheiro, "w");
    if (!f) return;
    
    printf("Digite o conteúdo do documento:\n> ");
    fgets(conteudo, sizeof(conteudo), stdin);
    conteudo[strcspn(conteudo, "\n")] = 0;
    /* Salva apenas o conteúdo puro no arquivo do documento */
    fprintf(f, "%s\n", conteudo);
    fclose(f);
    /* Registro nos logs de documentos enviados e recebidos */
    registrar_documento(remetente, "ENVIADO", destino, data, nome_ficheiro);
    registrar_documento(destino, "RECEBIDO", remetente, data, nome_ficheiro);
}

void carregar_documentos_membro(Membro* membro) {
    char path[256];
    FILE* f;
    char linha[256];
    
    if (!membro) return;
    sprintf(path, "documentos/%s_docs.txt", membro->email);
    f = fopen(path, "r");
    if (!f) return;
    
    while (fgets(linha, sizeof(linha), f)) {
        char tipo[16], participante[100], data[32], nome_arquivo[128];
        if (sscanf(linha, "%15[^;];%99[^;];%31[^;];%127[^\n]", tipo, participante, data, nome_arquivo) == 4) {
            Documento* doc = malloc(sizeof(Documento));
            strncpy(doc->nome, nome_arquivo, sizeof(doc->nome));
            doc->prox = membro->documentos;
            membro->documentos = doc;
        }
    }
    fclose(f);
}

/* Lê o conteúdo de um arquivo físico de documento no novo padrão CSV */
void ler_documento_fisico(const char* caminho) {
    FILE* f;
    char linha[1024];
    
    f = fopen(caminho, "r");
    if (!f) {
        printf("Arquivo não encontrado: %s\n", caminho);
        return;
    }
    
    while (fgets(linha, sizeof(linha), f)) {
        char remetente[100], destino[100], data[32], conteudo[512];
        if (sscanf(linha, "%99[^;];%99[^;];%31[^;];%511[^\n]", remetente, destino, data, conteudo) == 4) {
            printf("Remetente: %s\nDestino: %s\nData: %s\nConteúdo: %s\n", remetente, destino, data, conteudo);
        }
    }
    fclose(f);
}
