# Testes automáticos

O projeto possui 12 testes escritos com `assert`. Cada teste é um executável independente e cobre caminhos normais, estruturas vazias, argumentos inválidos, limites, falhas esperadas e liberação final.

| Arquivo | Cobertura principal |
|---|---|
| `teste_atendimento.c` | criação, prioridade, classificação e transições de status |
| `teste_entrada.c` | texto, inteiros, positivos e formatos inválidos |
| `teste_repositorio.c` | inserção, duplicidade, busca e expansão dinâmica |
| `teste_pilha.c` | comportamento LIFO, topo, sequência e destruição |
| `teste_fila.c` | comportamento FIFO, consulta, remoção e fila vazia |
| `teste_bst.c` | inserção, busca, altura, degeneração e percursos |
| `teste_avl.c` | rotações LL, RR, LR e RL, alturas e balanceamento |
| `teste_heap.c` | prioridade, desempate, subida, descida e expansão |
| `teste_arvore_decisao.c` | quatro caminhos e quatro classificações |
| `teste_validacao.c` | invariantes e corrupções controladas |
| `teste_experimentos.c` | sequências crescente, decrescente e pseudoaleatória |
| `teste_sistema.c` | cadastro, classificação, chamada e conclusão integrados |

Os arquivos `dados_*.txt` alimentam automaticamente os testes que precisam simular entrada. Eles redirecionam `stdin` com `freopen`, portanto não exigem digitação manual.

## Execução no GitHub

O workflow `testes.yml` compila cada teste com:

```text
-std=c11 -Wall -Wextra -pedantic
```

Depois, executa todos os binários a partir da raiz do repositório. Qualquer falha de compilação, aviso tratado como erro pelo compilador ou `assert` interrompe o workflow.
