# 🔧 CORREÇÕES NECESSÁRIAS NO MENU MENSAGENS

## ❌ ERROS IDENTIFICADOS:

### **1. SHADOWING DE VARIÁVEIS (CRÍTICO)**
```c
// LINHA ~175 - PROBLEMA:
Equipa* e = buscar_equipa(eqs, nome_equipa);  // 'e' já existe
Membro* m = buscar_membro(ht, email);         // 'm' já existe  
int ok = adicionar_membro_equipa(e, email);   // 'ok' já existe
```

### **2. FORMATAÇÃO INCONSISTENTE**
```c
// OPÇÃO 6 - SEM CABEÇALHO:
printf("Nome da equipa: ");  // DEVERIA TER imprimir_cabecalho()

// OPÇÃO 7 - SEM CABEÇALHO:
printf("Nome da equipa: ");  // DEVERIA TER imprimir_cabecalho()
```

### **3. INTERFACE INCONSISTENTE**
```c
// USA PRINTF COM CORES:
printf("\033[1;32m Membro adicionado com sucesso!\033[0m\n");

// DEVERIA USAR:
mensagem_sucesso("Membro adicionado com sucesso!");
```

### **4. ERRO DE SINTAXE**
```c
// LINHA ~207 - Sleep() MAL POSICIONADO:
}
Sleep(3000);  // FORA DO BLOCO!
```

## 🔧 CORREÇÕES MANUAIS NECESSÁRIAS:

### **OPÇÃO 6:**
1. Adicionar `imprimir_cabecalho("ADICIONAR MEMBRO A EQUIPA");`
2. Renomear `Equipa* e` para `Equipa* equipa_temp`
3. Renomear `Membro* m` para `Membro* membro_temp`  
4. Renomear `int ok` para `int resultado`
5. Substituir `printf` por `mensagem_sucesso()/mensagem_erro()`

### **OPÇÃO 7:**
1. Adicionar `imprimir_cabecalho("REMOVER MEMBRO DA EQUIPA");`
2. Renomear `Equipa* e` para `Equipa* equipa_temp`
3. Renomear `int ok` para `int resultado`
4. Substituir `printf` por `mensagem_sucesso()/mensagem_erro()`
5. Corrigir posição do `Sleep(3000);`

### **SHADOWING NO PERFIL:**
- Linha ~218: Renomear `MembroEquipa* me` para `MembroEquipa* membro_perfil`

## ⚠️ STATUS: NECESSITA CORREÇÃO MANUAL

As correções automáticas falharam devido à complexidade do código.
**RECOMENDAÇÃO:** Fazer correções manuais linha por linha.