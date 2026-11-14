# Especificacao - Central de Atendimento e Priorizacao

## 1. Objetivo

Desenvolver, em C11, um sistema que cadastre, classifique, localize e priorize
atendimentos, integrando arvore binaria de decisao, fila, pilha, BST, AVL e
heap maximo.

## 2. Regras definidas

### Codigo

- Tipo: `unsigned long`.
- Deve ser positivo.
- Gerado automaticamente pelo sistema.
- Contador `proximo_codigo` iniciado em 1.
- Avanca somente depois de um cadastro confirmado.

### Ordem de chegada

- Tipo: `unsigned long`.
- Contador independente `proxima_ordem`, iniciado em 1.
- Usada para desempate no heap.

### Prioridade

- Informada no cadastro para todos os chamados.
- Deve estar entre 1 e 10.
- A fila ignora esse campo.
- O heap utiliza esse campo para ordenar urgentes e criticos.

### Classificacao

- `CLASSIFICACAO_NAO_DEFINIDA`
- `CLASSIFICACAO_COMUM`
- `CLASSIFICACAO_MODERADA`
- `CLASSIFICACAO_URGENTE`
- `CLASSIFICACAO_CRITICA`

Encaminhamento:

- Comum ou moderado: fila.
- Urgente ou critico: heap maximo.

### Status

- `STATUS_ABERTO`
- `STATUS_CLASSIFICADO`
- `STATUS_EM_ATENDIMENTO`
- `STATUS_CONCLUIDO`

Transicoes permitidas:

```text
ABERTO -> CLASSIFICADO -> EM_ATENDIMENTO -> CONCLUIDO
```

### Arvore de decisao

```text
O servico esta completamente indisponivel?
|-- Sim: O problema afeta varias pessoas?
|   |-- Sim: CRITICO
|   `-- Nao: URGENTE
`-- Nao: O problema impede o trabalho?
    |-- Sim: MODERADO
    `-- Nao: COMUM
```

A arvore sera implementada em `arvore_decisao.c/.h`. A leitura de respostas
sera responsabilidade de `entrada.c/.h`.

### Historico

- Pilha dinamica.
- Sequencia numerica iniciada em 1.
- Eventos de cadastro, classificacao, inicio do atendimento, conclusao e
  mudanca de status.
- A pilha guarda uma copia do `RegistroHistorico`, nao um `Atendimento *`.

### Repositorio

- Vetor dinamico de `Atendimento *`.
- Capacidade inicial 8.
- Crescimento por duplicacao.
- Proprietario dos atendimentos e responsavel por libera-los.

### Fila

- Dinamica e encadeada.
- Guarda `Atendimento *`.
- Possui inicio, fim e tamanho.
- Libera apenas seus nos.

### Heap

- Vetor dinamico de `Atendimento *`.
- Capacidade inicial 8 e crescimento por duplicacao.
- Maior prioridade possui precedencia.
- Em empate, menor ordem de chegada possui precedencia.
- Libera apenas seu vetor.

### BST e AVL

- Indexadas pelo codigo do atendimento.
- Guardam `Atendimento *`.
- Arvore vazia possui altura -1.
- Folha possui altura 0.
- A AVL conta cada rotacao primitiva: LL/RR = 1 e LR/RL = 2.
- Liberam apenas seus nos.

## 3. Propriedade da memoria

Cada `Atendimento` sera criado uma unica vez. Repositorio, fila, heap, BST e
AVL guardam ponteiros para o mesmo objeto. Somente o repositorio libera os
atendimentos.

Ordem de destruicao:

1. Fila.
2. Heap.
3. BST.
4. AVL.
5. Pilha.
6. Arvore de decisao.
7. Repositorio.

## 4. Comportamento transacional

Ao classificar:

1. Validar atendimento e status.
2. Obter classificacao.
3. Confirmar classificacao e atualizar o status.
4. Inserir na fila ou no heap.
5. Se a insercao falhar, restaurar classificacao e status.
6. Registrar o evento.

Falha de memoria durante uma integracao parcial sera tratada como erro critico:
o sistema informa o problema, encerra e libera as estruturas com seguranca.

## 5. Resultados das operacoes do sistema

As operacoes de integracao usarao `ResultadoSistema`, com valores especificos
para sucesso, argumento invalido, memoria, prioridade, nao encontrado, status,
classificacao duplicada, fila vazia, heap vazio e estrutura invalida.

## 6. Menu planejado

```text
1. Cadastrar chamado
2. Classificar chamado
3. Consultar pela BST
4. Consultar pela AVL
5. Exibir BST
6. Exibir AVL
7. Exibir fila
8. Chamar proximo comum
9. Exibir heap
10. Chamar prioritario
11. Concluir atendimento
12. Exibir historico
13. Comparar BST e AVL
14. Validar estruturas
0. Encerrar
```

## 7. Ordem de implementacao

1. `atendimento.h/.c` e testes.
2. `entrada.h/.c` e testes.
3. `repositorio.h/.c` e testes.
4. `pilha.h/.c` e testes.
5. `fila.h/.c` e testes.
6. `bst.h/.c` e testes.
7. `avl.h/.c` e testes.
8. `heap.h/.c` e testes.
9. `arvore_decisao.h/.c` e testes.
10. `validacao.h/.c`.
11. `sistema.h/.c` e teste integrado.
12. `entrada` integrada ao `main.c`.
13. Experimentos BST e AVL.
14. README, relatorio e revisao final.
