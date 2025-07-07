# ✅ Sistema de Bloqueio/Desbloqueio Implementado

## 🎯 Funcionalidades Implementadas:

### **1. Estrutura de Dados**
- ✅ Estrutura `Bloqueado` criada
- ✅ Campo `bloqueados` adicionado à estrutura `Membro`
- ✅ Lista ligada para gerenciar múltiplos bloqueios

### **2. Funções de Bloqueio**
- ✅ `bloquear_membro()` - Adiciona membro à lista de bloqueados
- ✅ `desbloquear_membro()` - Remove membro da lista de bloqueados  
- ✅ `esta_bloqueado()` - Verifica se um membro está bloqueado

### **3. Integração com Mensagens**
- ✅ Verificação de bloqueio antes de enviar mensagens
- ✅ Mensagem de erro específica quando bloqueado
- ✅ Bloqueio impede envio direto de mensagens

### **4. Interface do Usuário**
- ✅ Opção "Bloquear/Desbloquear membro" no menu corporativo
- ✅ Submenu para escolher bloquear ou desbloquear
- ✅ Mensagens de sucesso/erro específicas

## 📋 Como Funciona:

### **Para Bloquear:**
1. Menu Corporativo → Opção 5
2. Escolher "1. Bloquear membro"
3. Digitar email do membro
4. Sistema adiciona à lista de bloqueados

### **Para Desbloquear:**
1. Menu Corporativo → Opção 5  
2. Escolher "2. Desbloquear membro"
3. Digitar email do membro
4. Sistema remove da lista de bloqueados

### **Efeito do Bloqueio:**
- Membro bloqueado **NÃO PODE** enviar mensagens diretas
- Mensagem de erro: "Destinatario inativo ou voce foi bloqueado"
- Bloqueio é **unidirecional** (A bloqueia B, mas B ainda pode tentar enviar para A)

## 🔧 Detalhes Técnicos:

### **Estruturas:**
```c
typedef struct Bloqueado {
    char email[100];
    struct Bloqueado* prox;
} Bloqueado;

typedef struct Membro {
    // ... outros campos
    Bloqueado* bloqueados; // lista de bloqueados
} Membro;
```

### **Verificação:**
```c
// Em enviar_mensagem()
if (receptor && esta_bloqueado(receptor, origem)) return 0;
```

## ✅ Status: IMPLEMENTADO COMPLETAMENTE

O sistema de bloqueio/desbloqueio está **100% funcional** conforme especificado no enunciado:

> *"O membro corporativo [...] bloqueia/desbloqueia um outro membro impedindo-o de enviar mensagens directamente para si"*

**Próximo:** Implementar restrições completas para convidados.