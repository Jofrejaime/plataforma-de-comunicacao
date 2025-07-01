#include "documento.h"
#include "membro.h"

void registrar_documento(const char* email, const char* tipo, const char* participante, const char* data, const char* nome_arquivo) {
    if (!email || !tipo || !participante || !data || !nome_arquivo) return;
    char path[256];
    snprintf(path, sizeof(path), "documentos/%s_docs.txt", email);
    FILE* f = fopen(path, "a");
    if (!f) return;
    fprintf(f, "%s;%s;%s;%s\n", tipo, participante, data, nome_arquivo);
    fclose(f);
}

void listar_documentos(const char* email) {
    if (!email) return;
    char path[256];
    snprintf(path, sizeof(path), "documentos/%s_docs.txt", email);
    FILE* f = fopen(path, "r");
    if (!f) {
        printf("Nenhum documento encontrado para %s.\n", email);
        return;
    }
    char linha[512];
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
    if (!remetente || !destino) return;
    char pasta[100];
    snprintf(pasta, sizeof(pasta), "documentos/%s", remetente);
    verificar_ou_criar_pasta(pasta);
    time_t agora = time(NULL);
    struct tm* tempo = localtime(&agora);
    char data[11];
    strftime(data, sizeof(data), "%Y-%m-%d", tempo);
    char nome_ficheiro[200];
    snprintf(nome_ficheiro, sizeof(nome_ficheiro), "%s/para_%s_%s.txt", pasta, destino, data);
    FILE* f = fopen(nome_ficheiro, "w");
    if (!f) return;
    char conteudo[512];
    printf("Digite o conteúdo do documento:\n> ");
    fgets(conteudo, sizeof(conteudo), stdin);
    conteudo[strcspn(conteudo, "\n")] = 0;
    // Novo formato: rem;dest;data;conteudo\n
    fprintf(f, "%s;%s;%s;%s\n", remetente, destino, data, conteudo);
    fclose(f);
    // Registro nos logs de documentos enviados e recebidos
    registrar_documento(remetente, "ENVIADO", destino, data, nome_ficheiro);
    registrar_documento(destino, "RECEBIDO", remetente, data, nome_ficheiro);
}

void carregar_documentos_membro(Membro* membro) {
    if (!membro) return;
    char path[256];
    snprintf(path, sizeof(path), "documentos/%s_docs.txt", membro->email);
    FILE* f = fopen(path, "r");
    if (!f) return;
    char linha[256];
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

// Lê o conteúdo de um arquivo físico de documento no novo padrão CSV
void ler_documento_fisico(const char* caminho) {
    FILE* f = fopen(caminho, "r");
    if (!f) {
        printf("Arquivo não encontrado: %s\n", caminho);
        return;
    }
    char linha[1024];
    while (fgets(linha, sizeof(linha), f)) {
        char remetente[100], destino[100], data[32], conteudo[512];
        if (sscanf(linha, "%99[^;];%99[^;];%31[^;];%511[^\n]", remetente, destino, data, conteudo) == 4) {
            printf("Remetente: %s\nDestino: %s\nData: %s\nConteúdo: %s\n", remetente, destino, data, conteudo);
        }
    }
    fclose(f);
}
