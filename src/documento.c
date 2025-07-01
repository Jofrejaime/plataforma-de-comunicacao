#include "documento.h"

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
    printf("Documentos de %s:\n", email);
    while (fgets(linha, sizeof(linha), f)) {
        char tipo[16], participante[100], data[32], nome_arquivo[128];
        if (sscanf(linha, "%15[^;];%99[^;];%31[^;];%127[^\n]", tipo, participante, data, nome_arquivo) == 4) {
            printf("%s | %s | %s | %s\n", tipo, participante, data, nome_arquivo);
        }
    }
    fclose(f);
}

void enviar_documento(const char* remetente, const char* destino, bool destino_e_equipa) {
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
    fprintf(f, "remetente: %s\n", remetente);
    fprintf(f, "destino: %s\n", destino);
    fprintf(f, "data: %s\n", data);
    fprintf(f, "----------------------\n");
    printf("Digite o conteúdo do documento:\n> ");
    char conteudo[512];
    fgets(conteudo, sizeof(conteudo), stdin);
    conteudo[strcspn(conteudo, "\n")] = 0;
    fprintf(f, "%s\n", conteudo);
    fclose(f);
    // Registro nos logs de documentos enviados e recebidos
    registrar_documento(remetente, "ENVIADO", destino, data, nome_ficheiro);
    registrar_documento(destino, "RECEBIDO", remetente, data, nome_ficheiro);
}
