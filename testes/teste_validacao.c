/*========================================= INCLUSOES =============================================*/

#include <assert.h>
#include <stdio.h>

#include "validacao.h"

/*========================================= DEFINICOES =============================================*/

#define QUANTIDADE_TESTE 7UL

/*=== DADOS SIMULADOS PARA TESTE ==================================================================*/

static Atendimento *criar_atendimento_teste(unsigned long codigo, int prioridade, unsigned long ordem){

    Atendimento *atendimento;

    atendimento = atendimento_criar( codigo, "Usuario de teste", "Atendimento usado na validacao",
         prioridade, ordem);

    if (atendimento == NULL) {
        return NULL;
    }

    if (!atendimento_confirmar_classificacao(atendimento, CLASSIFICACAO_URGENTE)) {
        atendimento_destruir(atendimento);
        return NULL;
    }

    return atendimento;
}

/*========================================= TESTES =============================================*/

int main(void)
{
    ArvoreBST bst;
    ArvoreAVL avl;
    HeapMaximo heap;

    Atendimento *atendimentos[QUANTIDADE_TESTE];
    unsigned long i;

    const unsigned long codigos[QUANTIDADE_TESTE] = {20UL, 10UL, 30UL, 5UL, 15UL, 25UL, 35UL};
    const int prioridades[QUANTIDADE_TESTE] = {6, 4, 10, 2, 5, 8, 7};

    printf("Iniciando testes de validacao...\n");

/*=== INICIALIZACAO ==================================================================*/

    printf("Testando estruturas vazias...\n");

    bst_inicializar(&bst);
    avl_inicializar(&avl);

    assert(heap_inicializar(&heap) == 1);

    // Estruturas vazias e coerentes devem ser válidas. 
    assert(validacao_bst(&bst) == 1);
    assert(validacao_avl(&avl) == 1);
    assert(validacao_heap(&heap) == 1);

    // Ponteiros nulos não representam estruturas válidas. 
    assert(validacao_bst(NULL) == 0);
    assert(validacao_avl(NULL) == 0);
    assert(validacao_heap(NULL) == 0);

    printf("Estruturas vazias: OK\n");

/*=== CRIAR E INSERIR ==================================================================*/

    printf("Preenchendo as estruturas...\n");

    /*
      Cada Atendimento é criado uma única vez.
      As três estruturas recebem o mesmo ponteiro.
    */
    for (i = 0UL; i < QUANTIDADE_TESTE; i++) {
        atendimentos[i] = criar_atendimento_teste(codigos[i], prioridades[i], i + 1UL);
        assert(atendimentos[i] != NULL);
        assert(bst_inserir(&bst, atendimentos[i]) == 1);
        assert(avl_inserir(&avl, atendimentos[i]) == 1);
        assert(heap_inserir(&heap, atendimentos[i]) == 1);
    }

    assert(bst_obter_tamanho(&bst) == QUANTIDADE_TESTE);
    assert(avl_obter_tamanho(&avl) == QUANTIDADE_TESTE);
    assert(heap_obter_tamanho(&heap) == QUANTIDADE_TESTE);

    /*
      Todas as estruturas foram preenchidas somente por suas operações públicas.
    */
    assert(validacao_bst(&bst) == 1);
    assert(validacao_avl(&avl) == 1);
    assert(validacao_heap(&heap) == 1);

    printf("Estruturas preenchidas: OK\n");

/*=== TESTANDO TAMANHO INCORRETO BST ==============================================================*/

    printf("Testando deteccao de tamanho incorreto na BST...\n");

    /*
      Simula um erro de manutenção do contador:
      existem 7 nós, mas o metadado informa 8.
    */
    bst.tamanho++;

    assert(validacao_bst(&bst) == 0);

    // Restaura o estado antes do próximo teste. 
    bst.tamanho--;

    assert(validacao_bst(&bst) == 1);

    printf("Tamanho incorreto detectado: OK\n");

/*=== TESTANDO VIOLACAO DE ORDENACAO BST ==========================================================*/

    Atendimento *bst_esquerda_original;
    Atendimento *bst_direita_original;

    printf("Testando ordenacao incorreta na BST...\n");

    /*
      A raiz é 20. Seus filhos armazenam 10 e 30.
      Trocamos apenas os ponteiros dos atendimentos:
     
            20
           /  \
         30    10
     
      Essa estrutura viola a propriedade da BST.
    */
    bst_esquerda_original = bst.raiz->esquerda->atendimento;

    bst_direita_original = bst.raiz->direita->atendimento;

    bst.raiz->esquerda->atendimento = bst_direita_original;

    bst.raiz->direita->atendimento = bst_esquerda_original;

    assert(validacao_bst(&bst) == 0);

    // Restaura os ponteiros corretos. 
    bst.raiz->esquerda->atendimento = bst_esquerda_original;

    bst.raiz->direita->atendimento = bst_direita_original;

    assert(validacao_bst(&bst) == 1);

    printf("Ordenacao incorreta detectada: OK\n");

/*=== TESTANDO ALTURA INCORRETA AVL ==========================================================*/

    int altura_original;

    printf("Testando altura incorreta na AVL...\n");

    // Altera somente a altura armazenada na raiz. 
    altura_original = avl.raiz->altura;
    avl.raiz->altura = altura_original + 1;

    assert(validacao_avl(&avl) == 0);

    // Restaura a altura correta. 
    avl.raiz->altura = altura_original;

    assert(validacao_avl(&avl) == 1);

    printf("Altura incorreta detectada: OK\n");

/*=== TESTANDO TAMANHO INCORRETO AVL ==========================================================*/

    printf("Testando tamanho incorreto na AVL...\n");

    avl.tamanho++;

    assert(validacao_avl(&avl) == 0);

    avl.tamanho--;

    assert(validacao_avl(&avl) == 1);

    printf("Tamanho incorreto detectado: OK\n");

/*=== TESTANDO FATOR BALANCEAMENTO INCORRETO AVL =================================================*/

    NoAVL no10;
    NoAVL no20;
    NoAVL no30;
    ArvoreAVL avl_desbalanceada;

    printf("Testando fator invalido na AVL...\n");

    /*
      Monta manualmente:
     
            30
           /
          20
         /
        10
     
      As alturas estão corretas, mas o fator da raiz é 2.
    */
    no10.atendimento = atendimentos[1];
    no10.altura = 0;
    no10.esquerda = NULL;
    no10.direita = NULL;

    no20.atendimento = atendimentos[0];
    no20.altura = 1;
    no20.esquerda = &no10;
    no20.direita = NULL;

    no30.atendimento = atendimentos[2];
    no30.altura = 2;
    no30.esquerda = &no20;
    no30.direita = NULL;

    avl_desbalanceada.raiz = &no30;
    avl_desbalanceada.tamanho = 3UL;
    avl_desbalanceada.rotacoes = 0UL;

    assert(validacao_avl(&avl_desbalanceada) == 0);

    printf("Fator invalido detectado: OK\n");

/*=== TESTANDO PROPRIEDADES DO HEAP =================================================*/

    Atendimento *heap_raiz_original;
    Atendimento *heap_ultimo_original;

    printf("Testando propriedade do heap...\n");

    /*
      A raiz possui a maior prioridade.
      Trocamos com a última posição para colocar um elemento inferior acima de um superior.
    */
    heap_raiz_original = heap.itens[0];

    heap_ultimo_original = heap.itens[heap.quantidade - 1UL];

    heap.itens[0] = heap_ultimo_original;

    heap.itens[heap.quantidade - 1UL] = heap_raiz_original;

    assert(validacao_heap(&heap) == 0);

    // Restaura as posições originais. 
    heap.itens[0] = heap_raiz_original;

    heap.itens[heap.quantidade - 1UL] = heap_ultimo_original;

    assert(validacao_heap(&heap) == 1);

    printf("Propriedade do heap detectada: OK\n");

/*=== TESTANDO STATUS INCOMPATIVEL DO HEAP =================================================*/

    StatusAtendimento status_original;

    printf("Testando status invalido no heap...\n");

    /*
      Todo atendimento ocupado pelo heap deve permanecer marcado como classificado.
    */
    status_original = heap.itens[0]->status;

    heap.itens[0]->status = STATUS_ABERTO;

    assert(validacao_heap(&heap) == 0);

    // Restaura o objeto compartilhado. 
    heap.itens[0]->status = status_original;

    assert(validacao_heap(&heap) == 1);

    printf("Status invalido detectado: OK\n");

/*=== TESTANDO QUANTIDADE INVALIDADE NO HEAP =================================================*/

    unsigned long quantidade_original;

    printf("Testando quantidade invalida no heap...\n");

    quantidade_original = heap.quantidade;

    /*
      Simula uma quantidade maior que a capacidade, sem acessar as posições inexistentes.
    */
    heap.quantidade =
        heap.capacidade + 1UL;

    assert(validacao_heap(&heap) == 0);

    heap.quantidade = quantidade_original;

    assert(validacao_heap(&heap) == 1);

    printf("Quantidade invalida detectada: OK\n");

/*=== DESTRUIR =================================================*/

    printf("Testando validacao apos destruicao...\n");

    /*
      As estruturas auxiliares são destruídas antes dos atendimentos compartilhados.
    */
    heap_destruir(&heap);
    bst_destruir(&bst);
    avl_destruir(&avl);

    /*
      Estados completamente zerados são coerentes e devem continuar sendo considerados válidos.
    */
    assert(validacao_heap(&heap) == 1);
    assert(validacao_bst(&bst) == 1);
    assert(validacao_avl(&avl) == 1);

    assert(heap.itens == NULL);
    assert(heap.quantidade == 0UL);
    assert(heap.capacidade == 0UL);

    assert(bst.raiz == NULL);
    assert(bst.tamanho == 0UL);

    assert(avl.raiz == NULL);
    assert(avl.tamanho == 0UL);
    assert(avl.rotacoes == 0UL);

    printf("Estados destruidos validos: OK\n");

    /*
      BST, AVL e heap liberaram somente sua própria memória estrutural.
    */
    for (i = 0UL; i < QUANTIDADE_TESTE; i++) {
        assert(atendimentos[i] != NULL);
        assert(atendimentos[i]->codigo == codigos[i]);
    }

    /*
      Agora o teste libera os objetos centrais.
      No sistema real, esta etapa será do repositório.
    */
    for (i = 0UL; i < QUANTIDADE_TESTE; i++) {
        atendimento_destruir(atendimentos[i]);
        atendimentos[i] = NULL;
    }

    printf(
        "Todos os testes de validacao passaram!\n"
    );

    return 0;
}    
