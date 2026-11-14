# Relatório — Comparação entre BST e AVL

**Aluno:** Clovis Campanha Junior  
**RA:** 202510008  
**Disciplina:** Estruturas de Dados II  
**Professor:** Clerivaldo José Roccia  

## 1. Objetivo

Este relatório apresenta a comparação experimental entre uma árvore
binária de busca sem balanceamento automático (BST) e uma árvore AVL.

Foram utilizados três conjuntos com vinte códigos distintos:

- inserção crescente;
- inserção decrescente;
- inserção pseudoaleatória fixa.

Para cada conjunto foram observados quantidade de nós, altura final,
comparações em três buscas, rotações da AVL e percurso em ordem.

## 2. Metodologia

Em cada experimento, os mesmos objetos `Atendimento` foram inseridos na
BST e na AVL, na mesma ordem. As árvores guardaram ponteiros para esses
objetos, evitando cópias diferentes dos registros.

A métrica de busca foi definida como:

> uma comparação corresponde a um nó visitado.

Foram pesquisados os códigos 1, 10 e 20 em todos os conjuntos.

A altura seguiu a convenção:

- árvore vazia: altura -1;
- folha: altura 0.

Na AVL, cada rotação primitiva foi contada separadamente:

- rotação simples: 1;
- rotação dupla: 2.

O percurso em ordem foi utilizado para confirmar que ambas as árvores
preservaram a ordenação crescente dos códigos.

## 3. Resultados gerais

| Ordem de inserção | Nós BST | Nós AVL | Altura BST | Altura AVL | Rotações AVL |
|---|---:|---:|---:|---:|---:|
| Crescente | 20 | 20 | 19 | 4 | 15 |
| Decrescente | 20 | 20 | 19 | 4 | 15 |
| Pseudoaleatória fixa | 20 | 20 | 5 | 4 | 1 |

## 4. Comparações nas buscas

### Experimento A — inserção crescente

| Código | Comparações BST | Comparações AVL |
|---:|---:|---:|
| 1 | 1 | 4 |
| 10 | 10 | 4 |
| 20 | 20 | 5 |

### Experimento B — inserção decrescente

| Código | Comparações BST | Comparações AVL |
|---:|---:|---:|
| 1 | 20 | 5 |
| 10 | 11 | 5 |
| 20 | 1 | 4 |

### Experimento C — inserção pseudoaleatória fixa

| Código | Comparações BST | Comparações AVL |
|---:|---:|---:|
| 1 | 3 | 3 |
| 10 | 3 | 4 |
| 20 | 4 | 4 |

## 5. Análise dos resultados

### Ordem crescente

Na inserção crescente, cada novo código foi maior que o anterior. Como a
BST não realiza balanceamento, todos os nós foram inseridos no lado
direito, formando uma estrutura semelhante a uma lista encadeada.

Com vinte nós, a BST alcançou altura 19.

A AVL realizou 15 rotações e terminou com altura 4. A busca pelo código
20 exigiu vinte visitas na BST e apenas cinco na AVL.

Entretanto, a busca pelo código 1 exigiu somente uma comparação na BST,
pois esse código permaneceu na raiz. Esse resultado demonstra que uma
árvore de maior altura ainda pode favorecer uma busca isolada, dependendo
da posição da chave.

### Ordem decrescente

Na inserção decrescente, ocorreu o comportamento espelhado. Todos os novos
códigos foram inicialmente inseridos à esquerda na BST, que novamente
atingiu altura 19.

A AVL realizou 15 rotações e manteve altura 4.

Nesse cenário, o código 20 permaneceu na raiz da BST e foi localizado com
uma comparação. Já o código 1 ficou na maior profundidade e exigiu vinte
comparações.

### Ordem pseudoaleatória

Na ordem pseudoaleatória fixa, a sequência de inserção favoreceu uma forma
mais equilibrada para a BST. Sua altura final foi 5, próxima da altura 4
da AVL.

A AVL precisou realizar apenas uma rotação.

As quantidades de comparações ficaram próximas. Em uma das buscas, a BST
utilizou três visitas enquanto a AVL utilizou quatro. Isso confirma que o
balanceamento oferece uma garantia estrutural, mas não reduz
obrigatoriamente toda busca individual.

## 6. Verificação da ordenação

Nos três experimentos, o percurso em ordem da BST e da AVL apresentou:

```text
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
```

Isso confirma que as duas estruturas preservaram a propriedade de árvore
de busca após todas as inserções e rotações.

Além da inspeção do percurso, funções de validação verificaram
programaticamente:
- limites mínimo e máximo das subárvores;
- quantidade real de nós;
- alturas armazenadas na AVL;
- fatores de balanceamento entre -1 e 1.

## 7. Conclusão
Os experimentos mostraram que a ordem de inserção influencia fortemente a
BST comum. Sequências crescentes ou decrescentes podem degenerá-la,
fazendo sua altura crescer linearmente.

A AVL utilizou rotações para manter a altura controlada nos três
conjuntos. Isso produziu um comportamento mais estável, especialmente nos
casos ordenados.

A BST pode apresentar menos comparações em buscas específicas quando a
chave está próxima da raiz. Porém, a AVL reduz o pior caso causado por
ordens desfavoráveis e oferece maior previsibilidade para buscas futuras.

Portanto, para um sistema que recebe códigos em ordens desconhecidas e
precisa realizar buscas frequentes, a AVL oferece uma garantia estrutural
mais adequada, ao custo de armazenar alturas e executar rotações durante
as inserções.

## 8. Testes realizados

Além dos experimentos, foram verificados:

- inserção de código duplicado;
- busca por código existente e inexistente;
- fila vazia;
- heap vazio;
- empate de prioridade resolvido pela ordem de chegada;
- rotações LL, RR, LR e RL;
- expansão dinâmica do repositório e do heap;
- propriedades da BST, AVL e heap;
- destruição repetida e ponteiros nulos;
- integração de cadastro, classificação, chamada e conclusão;
- liberação completa das estruturas.

Todos os módulos foram compilados com:

```text
-std=c11 -Wall -Wextra -pedantic
```
A suíte final possui doze testes, todos concluídos com código de saída
zero.

## 9. Participação

| Integrante | RA | Contribuição |
|---|---|---|
| Clovis Campanha Junior | 202510008 | Levantamento dos requisitos, estudo das estruturas, implementação dos módulos, elaboração dos testes, integração, experimentos e documentação. |

## 10. Ferramentas e apoio

O projeto foi desenvolvido a partir do enunciado da disciplina e dos
estudos individuais sobre estruturas de dados.

Foi utilizada orientação didática assistida por IA para discutir
requisitos, modularização, invariantes, gerenciamento de memória e
planejamento dos testes. O código foi digitado, analisado, compilado e
testado módulo por módulo durante o processo de aprendizagem.