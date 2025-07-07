# Melhorias Críticas para o Projeto

## 1. Sistema de Bloqueio (CRÍTICO)
- Adicionar campo `bloqueados[]` na estrutura Membro
- Implementar funções `bloquear_membro()` e `desbloquear_membro()`
- Verificar bloqueios antes de enviar mensagens

## 2. Restrições de Membro Convidado (CRÍTICO)
- Convidados só podem responder mensagens, não iniciar
- Convidados não podem partilhar documentos
- Convidados só acedem equipas onde foram convidados

## 3. Validação de Permissões (IMPORTANTE)
- Integrar sistema de permissões em todas as operações
- Verificar permissões antes de executar ações
- Mensagens claras quando permissão negada

## 4. Melhorar Feedback ao Utilizador (IMPORTANTE)
- Todas as operações devem retornar mensagens claras
- Distinguir entre diferentes tipos de erro
- Mensagens de sucesso mais informativas

## 5. Validações de Segurança (IMPORTANTE)
- Validar formato de email mais rigorosamente
- Verificar força da senha
- Sanitizar inputs do utilizador

## 6. Funcionalidade de Perfil (MENOR)
- Mostrar equipas do membro no perfil
- Histórico de atividades
- Estatísticas de uso

## Prioridade de Implementação:
1. Sistema de bloqueio
2. Restrições de convidado
3. Validação de permissões
4. Melhorar feedback