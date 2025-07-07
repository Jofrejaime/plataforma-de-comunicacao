# Instruções para Compilação no DEV-C++

## Alterações Realizadas para Compatibilidade C89/C90:

### 1. Remoção de `stdbool.h`
- Substituído `bool` por `int`
- Substituído `true` por `1` e `false` por `0`

### 2. Declaração de Variáveis
- Todas as variáveis movidas para o início das funções
- Removidas declarações no meio de loops e blocos

### 3. Substituição de Funções
- `snprintf()` substituído por `sprintf()` (cuidado com buffer overflow)

## Como Compilar no DEV-C++:

### Opção 1: Usando o Makefile
```bash
# No terminal do DEV-C++:
make -f Makefile_DEVC
```

### Opção 2: Compilação Manual
```bash
gcc -std=c89 -Wall -Iinclude src/*.c -o plataforma
```

### Opção 3: Projeto no DEV-C++
1. Criar novo projeto
2. Adicionar todos os arquivos .c da pasta src/
3. Configurar include path para pasta include/
4. Definir padrão C89 nas configurações do compilador

## Configurações Importantes no DEV-C++:
- **Padrão C**: C89/C90
- **Include Directories**: ./include
- **Warnings**: Ativar todos (-Wall -Wextra)

## Estrutura de Pastas Necessária:
```
plataforma-de-comunicacao/
├── src/           # Arquivos .c
├── include/       # Arquivos .h  
├── data/          # Criada automaticamente
├── mensagens/     # Criada automaticamente
└── documentos/    # Criada automaticamente
```

## Teste de Compilação:
Após compilar, execute o programa para verificar se não há erros de runtime relacionados às mudanças de compatibilidade.